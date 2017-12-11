#ifdef HAVE_CONFIG_H
# include "elementary_config.h"
#endif

#include <Elementary.h>
#include "elm_priv.h"

char _tizen_profile_wearable = -1; /* Unknown */

char tizen_util_probe_profile_wearable()
{
   const char *profile_name;

   if (_tizen_profile_wearable == 1 ||
       _tizen_profile_wearable == 0)
     return _tizen_profile_wearable;

   profile_name = elm_config_profile_get();
   if (!profile_name)
     {
        WRN("Failed to get tizen profile name");
        return _tizen_profile_wearable;
     }

   INF("tizen profile name:%s", profile_name);

   switch (*profile_name)
     {
       case 'w':
       case 'W':
         _tizen_profile_wearable = 1;
         break;
       default:
         _tizen_profile_wearable = 0;
         break;
     }

   return _tizen_profile_wearable;
}
