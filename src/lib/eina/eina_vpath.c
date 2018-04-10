#include <Eina.h>

#include "eina_private.h"

static Eina_Hash *vpath_data = NULL;

#ifdef CRI
#undef CRI
#endif
#define CRI(...) EINA_LOG_DOM_CRIT(_eina_vpath_log_dom, __VA_ARGS__)

#ifdef ERR
#undef ERR
#endif
#define ERR(...) EINA_LOG_DOM_ERR(_eina_vpath_log_dom, __VA_ARGS__)

#ifdef DBG
#undef DBG
#endif
#define DBG(...) EINA_LOG_DOM_DBG(_eina_vpath_log_dom, __VA_ARGS__)

static int _eina_vpath_log_dom = -1;

static inline void
_eina_vpath_data_add(const char *key, const char *value)
{
   eina_hash_add(vpath_data, key, eina_stringshare_add(value));
}

static inline Eina_Stringshare*
_eina_vpath_data_get(const char *key)
{
   return eina_hash_find(vpath_data, key);
}


static char*
_fallback_runtime_dir(const char *home)
{
   char buf[PATH_MAX];
   struct stat st;

#if defined(HAVE_GETUID) && defined(HAVE_GETEUID)
   if (setuid(geteuid()) != 0)
      {
         fprintf(stderr,
                 "FATAL: Cannot setuid - errno=%i\n",
                 errno);
         abort();
      }
#endif
   // fallback - make ~/.run
   snprintf(buf, sizeof(buf), "%s/.run", home);
   if (!!mkdir(buf,  S_IRUSR | S_IWUSR | S_IXUSR))
     {
        if (errno == EEXIST)
          {
             if (stat(buf, &st) == 0)
               {
                  // some sanity checks - but not for security
                  if (!(st.st_mode & S_IFDIR))
                    {
                       // fatal - exists but is not a dir
                       fprintf(stderr,
                               "FATAL: run dir '%s' exists but not a dir\n",
                               buf);
                       abort();
                    }
#if defined(HAVE_GETUID) && defined(HAVE_GETEUID)
                  if (st.st_uid != geteuid())
                    {
                       // fatal - run dir doesn't belong to user
                       fprintf(stderr,
                               "FATAL: run dir '%s' not owned by uid %i\n",
                               buf, (int)geteuid());
                       abort();
                    }
#endif
               }
             else
               {
                  // fatal - we cant create our run dir in ~/
                  fprintf(stderr,
                          "FATAL: Cannot verify run dir '%s' errno=%i\n",
                          buf, errno);
                  abort();
               }
          }
        else
          {
             // fatal - we cant create our run dir in ~/
             fprintf(stderr,
                     "FATAL: Cannot create run dir '%s' - errno=%i\n",
                     buf, errno);
             abort();
          }
     }
#if defined(HAVE_GETUID) && defined(HAVE_GETEUID)
  if (setreuid(uid, geteuid()) != 0)
     {
        fprintf(stderr,
                "FATAL: Cannot setreuid - errno=%i\n",
                errno);
        abort();
     }
#endif

   return strdup(buf);
}

static char*
_fallback_home_dir()
{
   char buf[PATH_MAX];
   /* Windows does not have getuid(), but home can't be NULL */
#ifdef HAVE_GETEUID
   uid_t uid = geteuid();
   struct stat st;

   snprintf(buf, sizeof(buf), "/tmp/%i", (int)uid);
   if (mkdir(buf,  S_IRUSR | S_IWUSR | S_IXUSR) < 0)
     {
        if (errno != EEXIST)
          {
             if (stat("/tmp", &st) == 0)
               snprintf(buf, sizeof(buf), "/tmp");
             else
               snprintf(buf, sizeof(buf), "/");
          }
     }
   if (stat(buf, &st) != 0)
     {
        if (stat("/tmp", &st) == 0)
          snprintf(buf, sizeof(buf), "/tmp");
        else
          snprintf(buf, sizeof(buf), "/");
     }
#else
   snprintf(buf, sizeof(buf), "/");
#endif
   return strdup(buf);
}

static void
_eina_vpath_interface_sys_init(void)
{
   const char *home, *tmp;

   // $HOME / ~/ etc.
   home = eina_environment_home_get();
   if (!home)
     home = _fallback_home_dir();

   // tmp dir - system wide
   tmp = eina_environment_tmp_get();

   _eina_vpath_data_add("home", home);
   _eina_vpath_data_add("tmp", tmp);
}

Eina_Bool
eina_vpath_init(void)
{
   vpath_data = eina_hash_string_superfast_new((Eina_Free_Cb)eina_stringshare_del);

   _eina_vpath_interface_sys_init();

   _eina_vpath_log_dom = eina_log_domain_register("vpath", "cyan");
   return EINA_TRUE;
}

Eina_Bool
eina_vpath_shutdown(void)
{
   eina_log_domain_unregister(_eina_vpath_log_dom);
   _eina_vpath_log_dom = -1;
   return EINA_TRUE;
}

EAPI char*
eina_vpath_resolve(const char* path)
{
   // XXX: implement parse of path then look up in hash if not just create
   // object where path and result are the same and return that with
   // path set and result set to resolved path - return obj handler calls
   // "do" on object to get the result inside fetched or failed callback.
   // if it's a url then we need a new classs that overrides the do and
   // begins a fetch and on finish calls the event cb or when wait is called
   /* FIXME: not working for WIndows */
   // /* <- full path

   if (!path) return NULL;

   if (path[0] == '~')
     {
        // ~/ <- home directory
        if (path[1] == '/')
          {
             char buf[PATH_MAX];
             const char *home = eina_hash_find(vpath_data, "home");

             if (home)
               {
                  snprintf(buf, sizeof(buf), "%s%s", home, path + 1);
                  return strdup(buf);
               }
          }
#ifdef HAVE_GETPWENT
        // ~username/ <- homedir of user "username"
        else
          {
             const char *p;
             struct passwd pwent, *pwent2 = NULL;
             char *name, buf[PATH_MAX], pwbuf[8129];

             for (p = path + 1; *p; p++)
               {
                  if (*p =='/') break;
               }
             name = alloca(p - path);
             strncpy(name, path + 1, p - path - 1);
             name[p - path - 1] = 0;
             if (!getpwnam_r(name, &pwent, pwbuf, sizeof(pwbuf), &pwent2))
               {
                  if ((pwent2) && (pwent.pw_dir))
                    {
                       return strdup(buf);
                    }
               }
           }
#endif /* HAVE_GETPWENT */
   return NULL;
    }
  // (:xxx:)/* ... <- meta hash table
  else if ((path[0] == '(') && (path[1] == ':'))
    {
       const char *p, *end, *meta;
       char *name, buf[PATH_MAX];
       int max_len = strlen(path);
       Eina_Bool found = EINA_FALSE;

       for (p = path + 2; p <= path + max_len - 2; p++)
         {
            if ((p[0] ==':') && (p[1] == ')'))
              {
                 end = p;
                 found = EINA_TRUE;
                 break;
              }
         }
       p += 2;

       if (!found)
         {
            ERR("(: Needs to have a matching ':)'\nThe string was: %s", path);
            return NULL;
         }

       if (*p != '/')
         {
            ERR("A / is expected after :)\nThe string was: %s", path);
            return NULL;
         }

       if (found)
         {
            name = alloca(end - path);
            strncpy(name, path + 2, end - path - 2);
            name[end - path - 2] = 0;
            meta = _eina_vpath_data_get(name);
            if (meta)
              {
                 snprintf(buf, sizeof(buf), "%s%s", meta, end + 2);
                 return strdup(buf);
              }
            else
              {
                 ERR("Meta key '%s' was not registered!\nThe string was: %s", name, path);
                 return NULL;
              }
         }
    }
   //just return the path, since we assume that this is a normal path
   else
    {
       return strdup(path);
    }

   ERR("The path has to start with either '~/' or '(:NAME:)/' or be a normal path \nThe string was: %s", path);

   return NULL;
}

EAPI void
eina_vpath_interface_app_set(const char *app_domain, Eina_Prefix *app_pfx)
{
   char buf[PATH_MAX];

   EINA_SAFETY_ON_NULL_RETURN(app_domain);
   EINA_SAFETY_ON_NULL_RETURN(app_pfx);

   _eina_vpath_data_add("app.dir", eina_prefix_get(app_pfx));
   _eina_vpath_data_add("app.bin", eina_prefix_bin_get(app_pfx));
   _eina_vpath_data_add("app.lib", eina_prefix_lib_get(app_pfx));
   _eina_vpath_data_add("app.data", eina_prefix_data_get(app_pfx));
   _eina_vpath_data_add("app.locale", eina_prefix_locale_get(app_pfx));
   snprintf(buf, sizeof(buf), "%s/%s",
            _eina_vpath_data_get("config"), app_domain);
   _eina_vpath_data_add("app.config", buf);
   snprintf(buf, sizeof(buf), "%s/%s",
            _eina_vpath_data_get("cache"), app_domain);
   _eina_vpath_data_add("app.cache", buf);
   snprintf(buf, sizeof(buf), "%s/%s",
            _eina_vpath_data_get("data"), app_domain);
   _eina_vpath_data_add("app.local", buf);
}

EAPI void
eina_vpath_interface_user_set(Eina_Vpath_Interface_User *user)
{
   char buf[PATH_MAX];
   Eina_Bool free_run = EINA_FALSE;

   EINA_SAFETY_ON_NULL_RETURN(user);

   if (!user->run)
     {
        user->run = _fallback_runtime_dir(_eina_vpath_data_get("home"));
        free_run = EINA_TRUE;
     }

#define ADD(a) _eina_vpath_data_add("usr." #a , user->a)
   ADD(desktop);
   ADD(documents);
   ADD(downloads);
   ADD(music);
   ADD(pictures);
   ADD(pub);
   ADD(templates);
   ADD(videos);
   ADD(data);
   ADD(config);
   ADD(cache);
   ADD(run);
#undef ADD

   if (free_run)
     free((char*)user->run);
}
