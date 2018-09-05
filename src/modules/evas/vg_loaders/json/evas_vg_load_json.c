#include <lotplayer_capi.h>
#include "vg_common.h"

#ifdef ERR
# undef ERR
#endif
#define ERR(...) EINA_LOG_DOM_ERR(_evas_vg_loader_json_log_dom, __VA_ARGS__)

#ifdef INF
# undef INF
#endif
#define INF(...) EINA_LOG_DOM_INFO(_evas_vg_loader_json_log_dom, __VA_ARGS__)

static int _evas_vg_loader_json_log_dom = -1;

static Eina_Bool
evas_vg_load_file_close_json(Vg_File_Data *vfd)
{
   if (!vfd) return EINA_FALSE;

   LOTPlayer *player = (LOTPlayer *) vfd->loader_data;

   ERR("close json vfd = %p, lotplayer! = %p", vfd, player);

   lotplayer_destroy(player);
   free(vfd);

   return EINA_TRUE;
}

static Eina_Bool
evas_vg_load_file_data_json(Vg_File_Data *vfd)
{
   ERR("data json vfd = %p", vfd);

   return vg_common_json_create_vg_node(vfd);
}

static Vg_File_Data*
evas_vg_load_file_open_json(const char *file,
                            const char *key EINA_UNUSED,
                            int *error EINA_UNUSED)
{
   Vg_File_Data *vfd = calloc(1, sizeof(Vg_File_Data));
   if (!vfd) return NULL;

   LOTPlayer *player = lotplayer_create();
   if (!player)
     {
        ERR("Failed to create LOTPlayer");
        free(vfd);
        return NULL;
     }

   int ret = lotplayer_set_file(player, file);
   if (LOT_PLAYER_ERROR_NONE != ret)
     {
        ERR("Failed to lotplayer_set_file(), result = %d", ret);
        free(vfd);
        lotplayer_destroy(player);
        return NULL;
     }

   vfd->loader_data = (void *) player;

   ERR("open json vfd(%p) lotplayer! = %p, file = %s", vfd, player, file);

   return vfd;
}

static Evas_Vg_Load_Func evas_vg_load_json_func =
{
   evas_vg_load_file_open_json,
   evas_vg_load_file_close_json,
   evas_vg_load_file_data_json
};

static int
module_open(Evas_Module *em)
{
   if (!em) return 0;
   em->functions = (void *)(&evas_vg_load_json_func);
   _evas_vg_loader_json_log_dom = eina_log_domain_register
     ("vg-load-json", EVAS_DEFAULT_LOG_COLOR);
   if (_evas_vg_loader_json_log_dom < 0)
     {
        EINA_LOG_ERR("Can not create a module log domain.");
        return 0;
     }
   return 1;
}

static void
module_close(Evas_Module *em EINA_UNUSED)
{
   if (_evas_vg_loader_json_log_dom >= 0)
     {
        eina_log_domain_unregister(_evas_vg_loader_json_log_dom);
        _evas_vg_loader_json_log_dom = -1;
     }
}

static Evas_Module_Api evas_modapi =
{
   EVAS_MODULE_API_VERSION,
   "json",
   "none",
   {
     module_open,
     module_close
   }
};

EVAS_MODULE_DEFINE(EVAS_MODULE_TYPE_VG_LOADER, vg_loader, json);

#ifndef EVAS_STATIC_BUILD_VG_JSON
EVAS_EINA_MODULE_DEFINE(vg_loader, json);
#endif

