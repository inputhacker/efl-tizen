#include "evas_gl_private.h"

#define GL_SURFACE_CACHE_LIMIT 50
static Ector_Surface_Cache *surface_cache = NULL;

static void
evas_gl_common_surface_cache_init(void)
{
   if (!surface_cache)
     {
        surface_cache = calloc(1, sizeof(Ector_Surface_Cache));
        if (surface_cache)
          surface_cache->surface_hash = eina_hash_int32_new(NULL);
     }
}

EAPI void
evas_gl_common_surface_cache_dump(void)
{
   Ector_Surface_Data *data;
   if (surface_cache)
     {
        eina_hash_free(surface_cache->surface_hash);
        EINA_LIST_FREE(surface_cache->lru_list, data)
          {
             evas_gl_common_image_free(data->surface);
             free(data);
          }
        free(surface_cache);
        surface_cache = NULL;
     }
}

void
evas_gl_common_surface_cache_set(void *key, void *surface)
{
   Ector_Surface_Data *surface_data = NULL;
   int count;
   evas_gl_common_surface_cache_init();
   surface_data = calloc(1, sizeof(Ector_Surface_Data));
   if (!surface_data) return;
   surface_data->key = key;
   surface_data->surface = surface;
   eina_hash_add(surface_cache->surface_hash, &key, surface_data);
   surface_cache->lru_list = eina_list_prepend(surface_cache->lru_list, surface_data);
   count = eina_list_count(surface_cache->lru_list);
   if (count > GL_SURFACE_CACHE_LIMIT)
   {
      surface_data = eina_list_data_get(eina_list_last(surface_cache->lru_list));
      eina_hash_del(surface_cache->surface_hash, &surface_data->key, surface_data);
      surface_cache->lru_list = eina_list_remove_list(surface_cache->lru_list, eina_list_last(surface_cache->lru_list));
      evas_gl_common_image_free(surface_data->surface);
      free(surface_data);
   }
}

void *
evas_gl_common_surface_cache_get(void *key)
{
   Ector_Surface_Data *surface_data = NULL, *lru_data;
   Eina_List *l;

   evas_gl_common_surface_cache_init();
   surface_data =  eina_hash_find(surface_cache->surface_hash, &key);
   if (surface_data)
     {
        EINA_LIST_FOREACH(surface_cache->lru_list, l, lru_data)
          {
            if (lru_data == surface_data)
              {
                 surface_cache->lru_list = eina_list_promote_list(surface_cache->lru_list, l);
                 break;
              }
          }
        return surface_data->surface;
     }
   return NULL;
}

