#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include "evas_common_private.h"
#include "evas_private.h"
#include "evas_vg_private.h"

static Vg_Cache* vg_cache = NULL;

static const char *loaders_name[] =
{ /* in order of most likely needed */
  "json"
};

static Vg_File_Data *
_vg_load_from_file(const Eina_File *file, const char *key, Eina_Bool mmap)
{
   Evas_Module       *em;
   Evas_Vg_Load_Func *loader;
   int                error = EVAS_LOAD_ERROR_GENERIC;
   Vg_File_Data      *vfd;
   unsigned int i;

   const char *file_name = eina_file_filename_get(file);
   for (i = 0; i < sizeof (loaders_name) / sizeof (char *); i++)
     {
        em = evas_module_find_type(EVAS_MODULE_TYPE_VG_LOADER, loaders_name[i]);
        if (em)
          {
             if (!evas_module_load(em)) return NULL;
             loader = em->functions;
             vfd = loader->file_open((Eina_File *) file, key, mmap, &error);
             if (vfd)
               {
                  vfd->loader = loader;
                  return vfd;
               }
          }
     }
   WRN("Exhausted all means to load vector file = %s", file_name);
   return NULL;
}

static void
_evas_cache_vg_data_free_cb(void *data)
{
   Vg_File_Data *vfd = data;
   vfd->loader->file_close(vfd);
}

static void
_evas_cache_vg_entry_free_cb(void *data)
{
   Vg_Cache_Entry *vg_entry = data;

   if (vg_entry->vfd)
     {
        vg_entry->vfd->ref--;
        if (vg_entry->vfd->ref <= 0 && !vg_entry->vfd->no_share)
          {
             Eina_Strbuf *hash_key = eina_strbuf_new();
             eina_strbuf_append_printf(hash_key, "%s/%s",
                                       eina_file_filename_get(vg_entry->file),
                                       vg_entry->key);
             if (!eina_hash_del(vg_cache->vfd_hash, eina_strbuf_string_get(hash_key), vg_entry->vfd))
               ERR("Failed to delete vfd = (%p) from hash", vg_entry->vfd);
             eina_strbuf_free(hash_key);
          }
     }

   eina_stringshare_del(vg_entry->key);
   free(vg_entry->hash_key);
   eo_unref(vg_entry->root[0]);
   eo_unref(vg_entry->root[1]);
   eo_unref(vg_entry->root[2]);
   free(vg_entry);
}

static Efl_VG*
_cached_root_get(Vg_Cache_Entry *vg_entry, unsigned int frame_num)
{
   Vg_File_Data *vfd = vg_entry->vfd;

   //Case 1: Animatable
   if (vfd->anim_data)
     {
        //Start frame
        if (vg_entry->root[1] && frame_num == 0)
          {
             return vg_entry->root[1];
          }
        //End frame
        else if (vg_entry->root[2] && (frame_num == (vfd->anim_data->frame_cnt - 1)))
          {
             return vg_entry->root[2];
          }
        //Current frame
        else if (vg_entry->root[0] && (frame_num == (vfd->anim_data->frame_num)))
          {
             return vg_entry->root[0];
          }
     }
   //Case 2: Static
   else
     {
        if (vg_entry->root[0])
          return vg_entry->root[0];
     }

   return NULL;
}

static void
_caching_root_update(Vg_Cache_Entry *vg_entry)
{
   Vg_File_Data *vfd = vg_entry->vfd;

   /* Optimization: static viewbox may have same root data regardless of size.
      So we can't use the root data directly, but copy it for each vg_entries.
      In the meantime, non-static viewbox root data may have difference instance for each
      size. So it's affordable to share the root data for each vg_entries. */
   if (vfd->static_viewbox)
     {
        /* TODO: Yet trivial but still we may have a better solution to
           avoid this unnecessary copy. If the ector surface key is not
           to this root pointer. */
        vg_entry->root[0] = evas_vg_container_add(NULL);
        evas_vg_node_dup(vg_entry->root[0], vfd->root);
     }
   else if (vg_entry->root[0] != vfd->root)
     {
        if (vg_entry->root[0]) eo_unref(vg_entry->root[0]);
        vg_entry->root[0] = eo_ref(vfd->root);
     }

   //Animatable?
   if (!vfd->anim_data) return;

   //Start frame
   if (vfd->anim_data->frame_num == 0)
     {
        if (vg_entry->root[1] != vfd->root)
          {
             if (vg_entry->root[1]) eo_unref(vg_entry->root[1]);
             vg_entry->root[1] = eo_ref(vfd->root);
          }
     }
   //End frame
   else if (vfd->anim_data->frame_num == (vfd->anim_data->frame_cnt - 1))
     {
        if (vg_entry->root[2] != vfd->root)
          {
             if (vg_entry->root[2]) eo_unref(vg_entry->root[2]);
             vg_entry->root[2] = eo_ref(vfd->root);
          }
     }
}

static void
_local_transform(Efl_VG *root, double w, double h, Vg_File_Data *vfd)
{
   double sx = 0, sy= 0, scale;
   Eina_Matrix3 m;

   if (!vfd->static_viewbox) return;
   if (vfd->view_box.w == w && vfd->view_box.h == h) return;

   sx = w / vfd->view_box.w;
   sy = h / vfd->view_box.h;

   scale = sx < sy ? sx : sy;
   eina_matrix3_identity(&m);

   // align hcenter and vcenter
   if (vfd->preserve_aspect)
     {
        eina_matrix3_translate(&m, (w - vfd->view_box.w * scale)/2.0, (h - vfd->view_box.h * scale)/2.0);
        eina_matrix3_scale(&m, scale, scale);
        eina_matrix3_translate(&m, -vfd->view_box.x, -vfd->view_box.y);
     }
   else
     {
        eina_matrix3_scale(&m, sx, sy);
        eina_matrix3_translate(&m, -vfd->view_box.x, -vfd->view_box.y);
     }
   evas_vg_node_transformation_set(root, &m);
}

void
evas_cache_vg_init(void)
{
   if (vg_cache)
     {
        vg_cache->ref++;
        return;
     }
   vg_cache = calloc(1, sizeof(Vg_Cache));
   if (!vg_cache)
     {
        CRI("Failed to alloc Vg_Cache");
        return;
     }

   vg_cache->vfd_hash = eina_hash_string_superfast_new(_evas_cache_vg_data_free_cb);
   vg_cache->vg_entry_hash = eina_hash_string_superfast_new(_evas_cache_vg_entry_free_cb);
   vg_cache->ref++;
}

void
evas_cache_vg_shutdown(void)
{
   if (!vg_cache) return;
   vg_cache->ref--;
   if (vg_cache->ref > 0) return;
   eina_hash_free(vg_cache->vfd_hash);
   eina_hash_free(vg_cache->vg_entry_hash);
   free(vg_cache);
   vg_cache = NULL;
}

Vg_File_Data *
evas_cache_vg_file_open(const Eina_File *file, const char *key, Eina_Bool mmap)
{
   Vg_File_Data *vfd;
   Eina_Strbuf *hash_key;

   hash_key = eina_strbuf_new();
   eina_strbuf_append_printf(hash_key, "%s/%s", eina_file_filename_get(file), key);
   vfd = eina_hash_find(vg_cache->vfd_hash, eina_strbuf_string_get(hash_key));
   if (!vfd || vfd->no_share)
     {
        vfd = _vg_load_from_file(file, key, mmap);
        //File exists.
        if (vfd && !vfd->no_share)
          eina_hash_add(vg_cache->vfd_hash, eina_strbuf_string_get(hash_key), vfd);
     }
   eina_strbuf_free(hash_key);
   return vfd;
}

Vg_Cache_Entry*
evas_cache_vg_entry_resize(Vg_Cache_Entry *vg_entry, int w, int h)
{
   return evas_cache_vg_entry_create(vg_entry->file, vg_entry->key, w, h, vg_entry->mmap);
}

Vg_Cache_Entry*
evas_cache_vg_entry_create(const Eina_File *file,
                           const char *key,
                           int w, int h,
                           Eina_Bool mmap)
{
   Vg_Cache_Entry* vg_entry;
   Eina_Strbuf *hash_key;

   if (!vg_cache) return NULL;

   //TODO: zero-sized entry is useless. how to skip it?

   hash_key = eina_strbuf_new();
   eina_strbuf_append_printf(hash_key, "%p/%s/%d/%d", file, key, w, h);
   vg_entry = eina_hash_find(vg_cache->vg_entry_hash, eina_strbuf_string_get(hash_key));
   if (!vg_entry)
     {
        vg_entry = calloc(1, sizeof(Vg_Cache_Entry));
        if (!vg_entry)
          {
             CRI("Failed to alloc Vg_Cache_Entry");
             eina_strbuf_free(hash_key);
             return NULL;
          }
        vg_entry->file = file;
        vg_entry->key = eina_stringshare_add(key);
        vg_entry->w = w;
        vg_entry->h = h;
        vg_entry->hash_key = eina_strbuf_string_steal(hash_key);
        vg_entry->mmap = mmap;
        eina_hash_direct_add(vg_cache->vg_entry_hash, vg_entry->hash_key, vg_entry);
     }
   eina_strbuf_free(hash_key);
   vg_entry->ref++;

   vg_entry->vfd = evas_cache_vg_file_open(file, key, mmap);
   //No File??
   if (!vg_entry->vfd)
     {
        evas_cache_vg_entry_del(vg_entry);
        return NULL;
     }
   vg_entry->vfd->ref++;

   return vg_entry;
}

double
evas_cache_vg_anim_duration_get(const Vg_Cache_Entry* vg_entry)
{
   if (!vg_entry->vfd->anim_data) return 0;
   return vg_entry->vfd->anim_data->duration;
}

unsigned int
evas_cache_vg_anim_frame_count_get(const Vg_Cache_Entry* vg_entry)
{
   if (!vg_entry) return 0;
   Vg_File_Data *vfd = vg_entry->vfd;
   if (!vfd || !vfd->anim_data) return 0;
   return vfd->anim_data->frame_cnt;
}

Efl_VG*
evas_cache_vg_tree_get(Vg_Cache_Entry *vg_entry, unsigned int frame_num)
{
   if (!vg_entry) return NULL;
   if ((vg_entry->w < 1) || (vg_entry->h < 1)) return NULL;

   Vg_File_Data *vfd = vg_entry->vfd;
   if (!vfd) return NULL;

//   Efl_VG *root = _cached_root_get(vg_entry, frame_num);
//   if (root) return root;
   Efl_VG *root = NULL;

   if (!vfd->static_viewbox)
     {
        vfd->view_box.w = vg_entry->w;
        vfd->view_box.h = vg_entry->h;
     }

   if (vfd->anim_data) vfd->anim_data->frame_num = frame_num;

   if (!vfd->loader->file_data(vfd)) return NULL;

   //_caching_root_update(vg_entry);
   if (vg_entry->root[0]) eo_unref(vg_entry->root[0]);
   vg_entry->root[0] = eo_ref(vfd->root);

   _local_transform(vg_entry->root[0], vg_entry->w, vg_entry->h, vfd);

   return vg_entry->root[0];
}

void
evas_cache_vg_entry_del(Vg_Cache_Entry *vg_entry)
{
   if (!vg_cache || !vg_entry) return;
   vg_entry->ref--;
   if (vg_entry->ref > 0) return;
   if (!eina_hash_del(vg_cache->vg_entry_hash, vg_entry->hash_key, vg_entry))
     ERR("Failed to delete vg_entry = (%p) from hash", vg_entry);
}

void
evas_cache_vg_entry_default_size_get(const Vg_Cache_Entry *vg_entry, int *w, int *h)
{
   if (!vg_entry)
     {
        if (w) *w = 0;
        if (h) *h = 0;
        return;
     }
   if (w) *w = vg_entry->vfd->w;
   if (h) *h = vg_entry->vfd->h;
}


