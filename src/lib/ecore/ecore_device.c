// TIZEN_ONLY(20171206): add dummy functions for ecore_device API
#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdlib.h>

#include "Ecore.h"
#include "ecore_private.h"

#define EFL_INTERNAL_UNSTABLE
#include "interfaces/efl_common_internal.h"

/* WARNING: This API is not used across EFL, hard to test! */

#ifdef DEBUG_UNTESTED_
// booh
#define SAFETY_CHECK(obj, klass, ...) \
   do { MAGIC_CHECK(obj, klass, 1); \
        return __VA_ARGS__; \
        MAGIC_CHECK_END(); \
   } while (0)

#else
#define SAFETY_CHECK(obj, klass, ...) \
   do { if (!obj) return __VA_ARGS__; } while (0)
#endif

#if 0
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
#endif
static Eina_List *_ecore_devices = NULL;
static int devices_num;

EAPI Ecore_Device *
ecore_device_add()
{
   Ecore_Device *dev;
   Efl_Input_Device_Data *d;

   dev = efl_add_ref(EFL_INPUT_DEVICE_CLASS, NULL,
                     efl_name_set(efl_added, NULL),
                     efl_comment_set(efl_added, NULL),
                     efl_input_device_type_set(efl_added, EFL_INPUT_DEVICE_TYPE_NONE),
                     efl_input_device_source_set(efl_added, NULL));

   d = efl_data_scope_get(dev, EFL_INPUT_DEVICE_CLASS);
   d->subclass = EFL_INPUT_DEVICE_SUBTYPE_NONE;

   _ecore_devices = eina_list_append(_ecore_devices, dev);
   devices_num++;

   return dev;
}

EAPI void
ecore_device_del(Ecore_Device *dev)
{
   Eina_List *l, *l_next;
   Ecore_Device *data;

   SAFETY_CHECK(dev, EFL_INPUT_DEVICE_CLASS);

   EINA_LIST_FOREACH_SAFE(_ecore_devices, l, l_next, data)
     {
        if (dev == data)
          _ecore_devices = eina_list_remove_list(_ecore_devices, l);
     }
   devices_num--;

   efl_unref(dev);
   return;
}

EAPI Ecore_Device *
ecore_device_ref(Ecore_Device *dev)
{
   SAFETY_CHECK(dev, EFL_INPUT_DEVICE_CLASS, NULL);

   return efl_ref(dev);
}

EAPI void
ecore_device_unref(Ecore_Device *dev)
{
   SAFETY_CHECK(dev, EFL_INPUT_DEVICE_CLASS);

   efl_unref(dev);
}

EAPI const Eina_List *
ecore_device_list(void)
{
   return _ecore_devices;
}

EAPI void
ecore_device_name_set(Ecore_Device *dev, const char *name)
{
   SAFETY_CHECK(dev, EFL_INPUT_DEVICE_CLASS);

   efl_name_set(dev, name);
}

EAPI const char *
ecore_device_name_get(const Ecore_Device *dev)
{
   SAFETY_CHECK(dev, EFL_INPUT_DEVICE_CLASS, NULL);

   return efl_name_get(dev);
}

EAPI void
ecore_device_description_set(Ecore_Device *dev, const char *desc)
{
   SAFETY_CHECK(dev, EFL_INPUT_DEVICE_CLASS);

   efl_comment_set(dev, desc);
}

EAPI const char *
ecore_device_description_get(const Ecore_Device *dev)
{
   SAFETY_CHECK(dev, EFL_INPUT_DEVICE_CLASS, NULL);

   return efl_comment_get(dev);
}

EAPI void
ecore_device_identifier_set(Ecore_Device *dev, const char *identifier)
{
   SAFETY_CHECK(dev, EFL_INPUT_DEVICE_CLASS);

   efl_comment_set(dev, identifier);
}

EAPI const char *
ecore_device_identifier_get(const Ecore_Device *dev)
{
   SAFETY_CHECK(dev, EFL_INPUT_DEVICE_CLASS, NULL);

   return efl_comment_get(dev);
}

EAPI void
ecore_device_class_set(Ecore_Device *dev, Ecore_Device_Class clas)
{
   SAFETY_CHECK(dev, EFL_INPUT_DEVICE_CLASS);

   efl_input_device_type_set(dev, clas);
}

EAPI Ecore_Device_Class
ecore_device_class_get(const Ecore_Device *dev)
{
   SAFETY_CHECK(dev, EFL_INPUT_DEVICE_CLASS, EFL_INPUT_DEVICE_TYPE_NONE);

   return efl_input_device_type_get(dev);
}

EAPI void
ecore_device_subclass_set(Ecore_Device *dev, Ecore_Device_Subclass subclas)
{
   Efl_Input_Device_Data *d;
   SAFETY_CHECK(dev, EFL_INPUT_DEVICE_CLASS);

   d = efl_data_scope_get(dev, EFL_INPUT_DEVICE_CLASS);
   if (!d) return;

   d->subclass = subclas;
}

EAPI Ecore_Device_Subclass
ecore_device_subclass_get(const Ecore_Device *dev)
{
   Efl_Input_Device_Data *d;
   SAFETY_CHECK(dev, EFL_INPUT_DEVICE_CLASS, EFL_INPUT_DEVICE_SUBTYPE_NONE);

   d = efl_data_scope_get(dev, EFL_INPUT_DEVICE_CLASS);
   if (!d) return 0;

   return d->subclass;
}
//
