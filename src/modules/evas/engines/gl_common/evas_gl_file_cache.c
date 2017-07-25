#include "evas_gl_private.h"
#define EVAS_GL_NO_GL_H_CHECK 1
#include "Evas_GL.h"

static mode_t default_mode = S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH;

Eina_Bool
evas_gl_common_file_cache_is_dir(const char *file)
{
   struct stat st;

   if (stat(file, &st) < 0) return EINA_FALSE;
   if (S_ISDIR(st.st_mode)) return EINA_TRUE;
   return EINA_FALSE;
}

Eina_Bool
evas_gl_common_file_cache_mkdir(const char *dir)
{
   /* evas gl only call this function when the dir is not exist */
   if (mkdir(dir, default_mode) < 0) return EINA_FALSE;
   return EINA_TRUE;
}

Eina_Bool
evas_gl_common_file_cache_file_exists(const char *file)
{
   struct stat st;
   if (!file) return EINA_FALSE;
   if (stat(file, &st) < 0) return EINA_FALSE;
   return EINA_TRUE;
}

Eina_Bool
evas_gl_common_file_cache_mkpath_if_not_exists(const char *path)
{
   struct stat st;

   if (stat(path, &st) < 0)
      return evas_gl_common_file_cache_mkdir(path);
   else if (!S_ISDIR(st.st_mode))
      return EINA_FALSE;
   else
      return EINA_TRUE;
}

Eina_Bool
evas_gl_common_file_cache_mkpath(const char *path)
{
   char ss[PATH_MAX];
   unsigned int i;

#if defined(HAVE_GETUID) && defined(HAVE_GETEUID)
   if (getuid() != geteuid()) return EINA_FALSE;
#endif
   if (evas_gl_common_file_cache_is_dir(path)) return EINA_TRUE;

   for (i = 0; path[i]; ss[i] = path[i], i++)
     {
        if (i == sizeof(ss) - 1) return EINA_FALSE;
        if ((path[i] == '/') && (i > 0))
          {
             ss[i] = 0;
             if (!evas_gl_common_file_cache_mkpath_if_not_exists(ss))
                return EINA_FALSE;
          }
     }
   ss[i] = 0;
   return evas_gl_common_file_cache_mkpath_if_not_exists(ss);
}

int
evas_gl_common_file_cache_dir_check(char *cache_dir, int num)
{
   char *home = NULL;
   char *subdir = ".cache/evas_gl_common_caches";

#if defined(HAVE_GETUID) && defined(HAVE_GETEUID)
   if (getuid() != geteuid()) return 0;
#endif
   home = getenv("HOME");
   if ((!home) || (!home[0])) return 0;

   snprintf(cache_dir, num, "%s/%s", home, subdir);
   return evas_gl_common_file_cache_file_exists(cache_dir);
}

int
evas_gl_common_file_cache_file_check(const char *cache_dir, const char *cache_name, char *cache_file, int dir_num, Evas_GL_Context_Version gles_version)
{
   char before_name[PATH_MAX];
   char after_name[PATH_MAX];
   int new_path_len = 0;
   int i = 0, j = 0;

   char *vendor = NULL;
   char *driver = NULL;
   char *version = NULL;

   vendor = (char *)GL_TH(glGetString, GL_VENDOR);
   driver = (char *)GL_TH(glGetString, GL_RENDERER);
   version = (char *)GL_TH(glGetString, GL_VERSION);

   if (!vendor)  vendor  = "-UNKNOWN-";
   if (!driver)  driver  = "-UNKNOWN-";
   if (!version) version = "-UNKNOWN-";

   if (!strcmp(cache_name, "surface_cap"))
     new_path_len = snprintf(before_name, sizeof(before_name), "%s::%s::%s::%s.%d::%s::%d.eet",
                           vendor, version, driver, MODULE_ARCH, evas_version->micro, cache_name, gles_version);
   else
     new_path_len = snprintf(before_name, sizeof(before_name), "%s::%s::%s::%s.%d::%s.eet",
                           vendor, version, driver, MODULE_ARCH, evas_version->micro, cache_name);

   /* remove '/' from file name */
   for (i = 0; i < new_path_len; i++)
     {
        if (before_name[i] != '/')
          {
             after_name[j] = before_name[i];
             j++;
          }
     }
   after_name[j] = 0;

   snprintf(cache_file, dir_num, "%s/%s", cache_dir, after_name);

   return evas_gl_common_file_cache_file_exists(cache_file);
}

