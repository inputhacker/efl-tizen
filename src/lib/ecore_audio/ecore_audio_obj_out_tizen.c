#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#if defined (__MacOSX__) || (defined (__MACH__) && defined (__APPLE__)) || defined (__FreeBSD__)
# include <libgen.h>
#endif

#ifdef HAVE_FEATURES_H
#include <features.h>
#endif

#include <Eo.h>
#include "ecore_audio_private.h"

#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>

#define MY_CLASS ECORE_AUDIO_OUT_TIZEN_CLASS
#define MY_CLASS_NAME "Ecore_Audio_Out_Tizen"

#if defined(_WIN32) || defined(__CYGWIN__)
# define MODULE_EXT ".dll"
#else
# define MODULE_EXT ".so"
#endif

#define AUDIO_MODULE_NAME "module"MODULE_EXT
#define MODULE_PATH PACKAGE_LIB_DIR"/ecore_audio/modules/tizen/"MODULE_ARCH

struct _Ecore_Audio_Out_Tizen_Module_Func
{
   int (*init)    (int sample_rate, int channel, void **handle);
   int (*deinit)  (void *handle);
   int (*get_buffer_size)  (void *handle, size_t *size);
   int (*write)   (void *handle, void *buf, unsigned int len);
   int (*drain)   (void *handle);
   int (*set_write_cb)  (void *handle, void (*out_cb)(void *handle, size_t nbytes, void *user_data), void *data);
   int (*unset_write_cb)  (void *handle);
};

typedef struct _Ecore_Audio_Out_Tizen_Module_Func Ecore_Audio_Out_Tizen_Module_Func;

static Ecore_Audio_Out_Tizen_Module_Func module_func = { NULL, };
static Eina_Module *module = NULL;

struct _Ecore_Audio_Out_Tizen_Data
{
   Ecore_Audio_Out_Tizen_Module_Func *func;
   Eo *attached_in;
};

typedef struct _Ecore_Audio_Out_Tizen_Data Ecore_Audio_Out_Tizen_Data;

static Eina_Bool _module_load(Ecore_Audio_Out_Tizen_Data *td)
{
   if (td->func)
     return EINA_TRUE;

   if (!module)
     {
        Eina_Module *em = NULL;
        em = eina_module_new(MODULE_PATH "/" AUDIO_MODULE_NAME);
        if (!eina_module_load(em))
          {
             eina_module_free(em);
             return EINA_FALSE;
          }

        module_func.init            = eina_module_symbol_get(em, "ecore_audio_init");
        module_func.deinit          = eina_module_symbol_get(em, "ecore_audio_deinit");
        module_func.get_buffer_size = eina_module_symbol_get(em, "ecore_audio_get_buffer_size");
        module_func.write           = eina_module_symbol_get(em, "ecore_audio_write");
        module_func.drain           = eina_module_symbol_get(em, "ecore_audio_drain");
        module_func.set_write_cb    = eina_module_symbol_get(em, "ecore_audio_set_write_cb");
        module_func.unset_write_cb  = eina_module_symbol_get(em, "ecore_audio_unset_write_cb");

        if ((!module_func.init) || (!module_func.deinit) || (!module_func.get_buffer_size)
            || (!module_func.write) || (!module_func.drain) || (!module_func.set_write_cb))
          {
             module_func.init            = NULL;
             module_func.deinit          = NULL;
             module_func.get_buffer_size = NULL;
             module_func.write           = NULL;
             module_func.drain           = NULL;
             module_func.set_write_cb    = NULL;
             module_func.unset_write_cb  = NULL;

             eina_module_free(em);
             return EINA_FALSE;
          }
        module = em;
     }

   td->func = &module_func;

   return EINA_TRUE;
}

static void _stream_write_cb(void *handle, size_t len, void *data)
{
   Eo *in = data;
   Ecore_Audio_Out_Tizen_Data *_td;
   eo_do(in, _td = eo_key_data_get("mod_data"));

   void *buf;
   ssize_t bread = 0;
   size_t wlen = len;

   buf = malloc(wlen);
   eo_do(in, bread = ecore_audio_obj_in_read(buf, wlen));

   if (bread > 0)
     {
        _td->func->write(handle, buf, (unsigned int)bread);
     }

   free(buf);
}

EOLIAN static Eina_Bool
_ecore_audio_out_tizen_ecore_audio_out_input_attach(Eo *eo_obj, Ecore_Audio_Out_Tizen_Data *_td, Eo *in)
{
   int ret;
   Eina_Bool ret2;
   void *handle, *data;
   int samplerate = 0;
   int channels = 0;

   eo_do_super(eo_obj, MY_CLASS, ret2 = ecore_audio_obj_out_input_attach(in));
   if (!ret2)
      return EINA_FALSE;

   eo_do(in, channels = ecore_audio_obj_in_channels_get());
   eo_do(in, samplerate = ecore_audio_obj_in_samplerate_get());

   ret = _td->func->init(samplerate, channels, &handle);
   if (ret)
     {
        eo_do_super(eo_obj, MY_CLASS, ecore_audio_obj_out_input_detach(in));
        ERR("init error : %d", ret);
        return EINA_FALSE;
     }
   eo_do(in, eo_key_data_set("mod_handle", handle));
   eo_do(in, eo_key_data_set("mod_data", _td));
   eo_do(in, eo_key_data_set("pcm_fmt", strdup("S16")));

   eo_do(in, data = eo_key_data_get("mod_handle"));
   if (!data) goto err;
   eo_do(in, data = eo_key_data_get("mod_data"));
   if (!data) goto err;
   eo_do(in, data = eo_key_data_get("pcm_fmt"));
   if (!data) goto err;

   _td->attached_in = in;
   ret = _td->func->set_write_cb(handle, _stream_write_cb, in);

   if (ret) goto err;

   return EINA_TRUE;

err:
   eo_do_super(eo_obj, MY_CLASS, ecore_audio_obj_out_input_detach(in));
   _td->attached_in = NULL;
   ERR("set_write_callback error : %d", ret);
   _td->func->deinit(handle);

   eo_do(in, eo_key_data_del("mod_handle"));
   eo_do(in, eo_key_data_del("mod_data"));
   eo_do(in, data = eo_key_data_get("pcm_fmt"));
   if (data)
     {
        free(data);
        eo_do(in, eo_key_data_del("pcm_fmt"));
     }

   return EINA_FALSE;
}

EOLIAN static Eina_Bool
_ecore_audio_out_tizen_ecore_audio_out_input_detach(Eo *eo_obj, Ecore_Audio_Out_Tizen_Data *_td, Eo *in)
{
   Eina_Bool ret2 = EINA_FALSE;
   void *handle;

   eo_do_super(eo_obj, MY_CLASS, ret2 = ecore_audio_obj_out_input_detach(in));
   if (!ret2)
     return EINA_FALSE;

   eo_do(in, handle = eo_key_data_get("mod_handle"));
   _td->func->unset_write_cb(handle);
   _td->func->drain(handle);
   _td->func->deinit(handle);

   return EINA_TRUE;
}

EOLIAN static Eo *
_ecore_audio_out_tizen_eo_base_constructor(Eo *eo_obj, Ecore_Audio_Out_Tizen_Data *_td)
{

   Ecore_Audio_Output *out_obj = eo_data_scope_get(eo_obj, ECORE_AUDIO_OUT_CLASS);

   eo_obj = eo_do_super_ret(eo_obj, MY_CLASS, eo_obj, eo_constructor());

   if (!_module_load(_td))
     {
        // Need to destructing
        ERR("Faild to load ecore audio module");
        return NULL;
     }

   return eo_obj;
}

EOLIAN static void
_ecore_audio_out_tizen_eo_base_destructor(Eo *eo_obj, Ecore_Audio_Out_Tizen_Data *_td EINA_UNUSED)
{
   eo_do_super(eo_obj, MY_CLASS, eo_destructor());
}

#include "ecore_audio_out_tizen.eo.c"
