// TIZEN_ONLY(20171206): add dummy functions for ecore_device API
#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdlib.h>

#include "Ecore.h"
#include "ecore_private.h"

/**
 * @struct _Ecore_Device
 * Contains information about a device.
 */
struct _Ecore_Device
  {
     ECORE_MAGIC; /**< ecore magic data */
     const char *name; /**< name of device */
     const char *desc; /**< description of device */
     const char *identifier; /**< identifier of device */
     Ecore_Device_Class clas; /**<Device class */
     Ecore_Device_Subclass subclas; /**< device subclass */
  };

EAPI Ecore_Device *
ecore_device_add()
{
   WRN("ecore_device is deprecated in efl. Please use efl_device APIs instead\n");
   return NULL;
}

EAPI void
ecore_device_del(Ecore_Device *dev EINA_UNUSED)
{
   WRN("ecore_device is deprecated in efl. Please use efl_device APIs instead\n");
   return;
}

EAPI const Eina_List *
ecore_device_list(void)
{
   WRN("ecore_device is deprecated in efl. Please use efl_device APIs instead\n");
   return NULL;
}

EAPI void
ecore_device_name_set(Ecore_Device *dev EINA_UNUSED, const char *name EINA_UNUSED)
{
   WRN("ecore_device is deprecated in efl. Please use efl_device APIs instead\n");
   return;
}

EAPI const char *
ecore_device_name_get(const Ecore_Device *dev EINA_UNUSED)
{
   WRN("ecore_device is deprecated in efl. Please use efl_device APIs instead\n");
   return NULL;
}

EAPI void
ecore_device_description_set(Ecore_Device *dev EINA_UNUSED, const char *desc EINA_UNUSED)
{
   WRN("ecore_device is deprecated in efl. Please use efl_device APIs instead\n");
   return;
}

EAPI const char *
ecore_device_description_get(const Ecore_Device *dev EINA_UNUSED)
{
   WRN("ecore_device is deprecated in efl. Please use efl_device APIs instead\n");
   return NULL;
}

EAPI void
ecore_device_identifier_set(Ecore_Device *dev EINA_UNUSED, const char *identifier EINA_UNUSED)
{
   WRN("ecore_device is deprecated in efl. Please use efl_device APIs instead\n");
   return;
}

EAPI const char *
ecore_device_identifier_get(const Ecore_Device *dev EINA_UNUSED)
{
   WRN("ecore_device is deprecated in efl. Please use efl_device APIs instead\n");
   return NULL;
}

EAPI void
ecore_device_class_set(Ecore_Device *dev EINA_UNUSED, Ecore_Device_Class clas EINA_UNUSED)
{
   WRN("ecore_device is deprecated in efl. Please use efl_device APIs instead\n");
   return;
}

EAPI Ecore_Device_Class
ecore_device_class_get(const Ecore_Device *dev EINA_UNUSED)
{
   WRN("ecore_device is deprecated in efl. Please use efl_device APIs instead\n");
   return 0;
}

EAPI void
ecore_device_subclass_set(Ecore_Device *dev EINA_UNUSED, Ecore_Device_Subclass subclas EINA_UNUSED)
{
   WRN("ecore_device is deprecated in efl. Please use efl_device APIs instead\n");
   return;
}

EAPI Ecore_Device_Subclass
ecore_device_subclass_get(const Ecore_Device *dev EINA_UNUSED)
{
   WRN("ecore_device is deprecated in efl. Please use efl_device APIs instead\n");
   return 0;
}
//
