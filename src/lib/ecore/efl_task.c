#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <Ecore.h>

#include "ecore_private.h"

#define MY_CLASS EFL_TASK_CLASS

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

static void
_clear_args(Efl_Task_Data *pd)
{
   unsigned int count, i;

   if (!pd->args) return;
   count = eina_array_count(pd->args);
   for (i = 0; i < count; i++)
     eina_stringshare_del(eina_array_data_get(pd->args, i));
   eina_array_free(pd->args);
   pd->args = NULL;
}

static Eina_Array *
_unescape(const char *s)
{
   Eina_Array *args;
   const char *p;
   char *tmp = NULL, *d = NULL;
   if (!s) return NULL;

   Eina_Bool in_quote_dbl = EINA_FALSE;
   Eina_Bool in_quote = EINA_FALSE;

   args = eina_array_new(16);
   if (!args) return NULL;
   for (p = s; *p; p++)
     {
        if (!tmp) tmp = d = strdup(p);
        if (tmp)
          {
             if (in_quote_dbl)
               {
                  switch (*p)
                    {
                     case '\"':
                       in_quote_dbl = EINA_FALSE;
                       *d = 0;
                       eina_array_push(args, eina_stringshare_add(tmp));
                       free(tmp);
                       tmp = d = NULL;
                       break;
                     case '\\':
                       p++;
                       EINA_FALLTHROUGH
                     default:
                       *d = *p;
                       d++;
                       break;
                    }
               }
             else if (in_quote)
               {
                  switch (*p)
                    {
                     case '\'':
                       in_quote = EINA_FALSE;
                       *d = 0;
                       eina_array_push(args, eina_stringshare_add(tmp));
                       free(tmp);
                       tmp = d = NULL;
                       break;
                     case '\\':
                       p++;
                       EINA_FALLTHROUGH
                     default:
                       *d = *p;
                       d++;
                       break;
                    }
               }
             else
               {
                  switch (*p)
                    {
                     case ' ':
                     case '\t':
                     case '\r':
                     case '\n':
                       *d = 0;
                       eina_array_push(args, eina_stringshare_add(tmp));
                       free(tmp);
                       tmp = d = NULL;
                       break;
                     case '\"':
                       in_quote_dbl = EINA_TRUE;
                       break;
                     case '\'':
                       in_quote = EINA_TRUE;
                       break;
                     case '\\':
                       p++;
                       EINA_FALLTHROUGH
                     default:
                       *d = *p;
                       d++;
                       break;
                    }
               }
          }
     }
   if (tmp)
     {
        *d = 0;
        eina_array_push(args, eina_stringshare_add(tmp));
        free(tmp);
     }
   return args;
}

static char *
_escape(const char *s)
{
   Eina_Bool need_quote = EINA_FALSE;
   const char *p;
   char *s2 = malloc((strlen(s) * 2) + 1 + 2), *d;

   if (!s2) return NULL;

   for (p = s; *p; p++)
     {
        switch (*p)
          {
           case '\'':
           case '\"':
           case '$':
           case '#':
           case ';':
           case '&':
           case '`':
           case '|':
           case '(':
           case ')':
           case '[':
           case ']':
           case '{':
           case '}':
           case '>':
           case '<':
           case '\n':
           case '\r':
           case '\t':
             need_quote = EINA_TRUE;
           default:
             break;
          }
     }

   d = s2;
   if (need_quote)
     {
        *d = '\"';
        d++;
     }
   for (p = s; *p; p++, d++)
     {
        switch (*p)
          {
           case ' ':
           case '\\':
           case '\'':
           case '\"':
             *d = '\\';
             d++;
             EINA_FALLTHROUGH
           default:
             *d = *p;
             break;
          }
     }
   if (need_quote)
     {
        *d = '\"';
        d++;
        *d = 0;
     }
   return s2;
}

static void
_rebuild_command(Efl_Task_Data *pd)
{
   unsigned int count, i;
   Eina_Strbuf *sb;
   const char *arg, *cmd;
   Eina_Bool have_args = EINA_FALSE;

   if (!pd->command_dirty) return;
   pd->command_dirty = EINA_FALSE;
   eina_stringshare_del(pd->command);
   pd->command = NULL;
   if (!pd->args) return;
   sb = eina_strbuf_new();
   if (!sb) return;
   count = eina_array_count(pd->args);
   for (i = 0; i < count; i++)
     {
        arg = eina_array_data_get(pd->args, i);
        if (arg)
          {
             char *str = _escape(arg);
             if (str)
               {
                  if (!have_args) eina_strbuf_append(sb, " ");
                  eina_strbuf_append(sb, str);
                  free(str);
                  have_args = EINA_TRUE;
               }
          }
     }
   cmd = eina_strbuf_string_get(sb);
   if (cmd) pd->command = eina_stringshare_add(cmd);
   eina_strbuf_free(sb);
}


static Eina_Bool
_foreach_env_copy(const Eina_Hash *hash EINA_UNUSED, const void *key, void *data, void *fdata)
{
   if (data)
     {
        // only copy env vars not already set
        if (!eina_hash_find(fdata, key))
          eina_hash_add(fdata, key, eina_stringshare_add(data));
     }
   return EINA_TRUE;
}

//////////////////////////////////////////////////////////////////////////

EOLIAN static void
_efl_task_command_set(Eo *obj EINA_UNUSED, Efl_Task_Data *pd, const char *command)
{
   eina_stringshare_replace(&pd->command, command);
   _clear_args(pd);
   pd->args = _unescape(pd->command);
}

EOLIAN static const char *
_efl_task_command_get(Eo *obj EINA_UNUSED, Efl_Task_Data *pd)
{
   _rebuild_command(pd);
   return pd->command;
}

EOLIAN static unsigned int
_efl_task_arg_count_get(Eo *obj EINA_UNUSED, Efl_Task_Data *pd)
{
   if (!pd->args) return 0;
   return eina_array_count(pd->args);
}

EOLIAN static void
_efl_task_arg_value_set(Eo *obj EINA_UNUSED, Efl_Task_Data *pd, unsigned int num, const char *arg)
{
   const char *parg = NULL;
   unsigned int count;

   if (!pd->args) pd->args = eina_array_new(16);
   count = eina_array_count(pd->args);
   if (count > num)
     parg = eina_array_data_get(pd->args, num);
   else
     {
        unsigned int i;

        for (i = count; i <= num; i++)
          eina_array_push(pd->args, NULL);
     }

   if (arg)
     eina_array_data_set(pd->args, num, eina_stringshare_add(arg));
   else
     eina_array_data_set(pd->args, num, NULL);
   if (parg) eina_stringshare_del(parg);
   pd->command_dirty = EINA_TRUE;
}

EOLIAN static const char *
_efl_task_arg_value_get(Eo *obj EINA_UNUSED, Efl_Task_Data *pd, unsigned int num)
{
   unsigned int count;

   if (!pd->args) return NULL;
   count = eina_array_count(pd->args);
   if (num >= count) return NULL;
   return eina_array_data_get(pd->args, num);
}

EOLIAN static void
_efl_task_arg_append(Eo *obj EINA_UNUSED, Efl_Task_Data *pd, const char *arg)
{
   if (!pd->args) pd->args = eina_array_new(16);
   if (arg)
     eina_array_push(pd->args, eina_stringshare_add(arg));
   else
     eina_array_push(pd->args, NULL);
   pd->command_dirty = EINA_TRUE;
}

EOLIAN static void
_efl_task_arg_reset(Eo *obj EINA_UNUSED, Efl_Task_Data *pd)
{
   _clear_args(pd);
   pd->command_dirty = EINA_TRUE;
}

EOLIAN static void
_efl_task_env_set(Eo *obj EINA_UNUSED, Efl_Task_Data *pd, const char *var, const char *value)
{
   if (!var) return;
   if (!pd->env)
     pd->env = eina_hash_string_superfast_new
       ((Eina_Free_Cb)eina_stringshare_del);
   if (!pd->env) return;
   if ((value) && (*value))
     eina_hash_add(pd->env, var, eina_stringshare_add(value));
   else eina_hash_del(pd->env, var, NULL);
}


EOLIAN static const char *
_efl_task_env_get(Eo *obj EINA_UNUSED, Efl_Task_Data *pd, const char *var)
{
   if ((!var) || (!pd->env)) return NULL;
   return eina_hash_find(pd->env, var);
}

EOLIAN static void
_efl_task_priority_set(Eo *obj EINA_UNUSED, Efl_Task_Data *pd, Efl_Task_Priority priority)
{
   pd->priority = priority;
}

EOLIAN static Efl_Task_Priority
_efl_task_priority_get(Eo *obj EINA_UNUSED, Efl_Task_Data *pd)
{
   return pd->priority;
}

EOLIAN static int
_efl_task_exit_code_get(Eo *obj EINA_UNUSED, Efl_Task_Data *pd)
{
   return pd->exit_code;
}

EOLIAN static Eina_Bool
_efl_task_run(Eo *obj EINA_UNUSED, Efl_Task_Data *pd EINA_UNUSED)
{
   // NOP
   return EINA_FALSE;
}

EOLIAN static void
_efl_task_end(Eo *obj EINA_UNUSED, Efl_Task_Data *pd EINA_UNUSED)
{
   // NOP
}

EOLIAN static Efl_Object *
_efl_task_efl_object_constructor(Eo *obj, Efl_Task_Data *pd EINA_UNUSED)
{
   obj = efl_constructor(efl_super(obj, MY_CLASS));
   return obj;
}

EOLIAN static void
_efl_task_efl_object_destructor(Eo *obj EINA_UNUSED, Efl_Task_Data *pd)
{
   eina_stringshare_del(pd->command);
   pd->command = NULL;
   _clear_args(pd);
   if (pd->env) eina_hash_free(pd->env);
   pd->env = NULL;
   efl_destructor(efl_super(obj, MY_CLASS));
}

EOLIAN static void
_efl_task_efl_object_parent_set(Eo *obj, Efl_Task_Data *pd, Efl_Object *parent)
{
   Eo *loop;

   efl_parent_set(efl_super(obj, MY_CLASS), parent);
   // copy loop env into exe task env, if not already set in env (overridden)
   loop = efl_provider_find(parent, EFL_LOOP_CLASS);
   if (loop)
     {
        Efl_Task_Data *tdl = efl_data_scope_get(loop, EFL_TASK_CLASS);

        if (tdl)
          {
             if (!pd->env)
               pd->env = eina_hash_string_superfast_new
                 ((Eina_Free_Cb)eina_stringshare_del);
             if (tdl->env)
               eina_hash_foreach(tdl->env, _foreach_env_copy, pd->env);
          }
     }
}

//////////////////////////////////////////////////////////////////////////

#include "efl_task.eo.c"
