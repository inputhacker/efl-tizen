#ifndef VG_COMMON_H_
#define VG_COMMON_H_

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include "evas_common_private.h"
#include "evas_private.h"

/******************************************************************************************
 * Lottie Compatible feature implementation
 ******************************************************************************************/
#ifdef BUILD_VG_LOADER_JSON
# include <rlottie_capi.h>
#endif

Eina_Bool vg_common_json_create_vg_node(Vg_File_Data *vfd);

#endif //EVAS_VG_COMMON_H_
