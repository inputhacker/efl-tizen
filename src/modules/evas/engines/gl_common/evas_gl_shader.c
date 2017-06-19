#include "config.h"
#include "shader/evas_gl_shaders.x"
#include "evas_gl_common.h"

#define SHADER_FLAG_SAM_BITSHIFT 3
#define SHADER_FLAG_MASKSAM_BITSHIFT 6
#define SHADER_PROG_NAME_FMT "/shader/%08x"
#define SHADER_BINARY_EET_COMPRESS 1
#define SHADER_EET_CHECKSUM "/shader/checksum"
#define SHADER_EET_CACHENAME "binary_shader"

#define P(i) ((void*)(intptr_t)i)
#define I(p) ((int)(intptr_t)p)

#ifdef WORDS_BIGENDIAN
# define BASEFLAG SHADER_FLAG_BIGENDIAN
#else
# define BASEFLAG 0
#endif

typedef enum {
   SHADER_FLAG_TEX               = (1 << 0),
   SHADER_FLAG_BGRA              = (1 << 1),
   SHADER_FLAG_MASK              = (1 << 2),
   SHADER_FLAG_SAM12             = (1 << (SHADER_FLAG_SAM_BITSHIFT + 0)),
   SHADER_FLAG_SAM21             = (1 << (SHADER_FLAG_SAM_BITSHIFT + 1)),
   SHADER_FLAG_SAM22             = (1 << (SHADER_FLAG_SAM_BITSHIFT + 2)),
   SHADER_FLAG_MASKSAM12         = (1 << (SHADER_FLAG_MASKSAM_BITSHIFT + 0)),
   SHADER_FLAG_MASKSAM21         = (1 << (SHADER_FLAG_MASKSAM_BITSHIFT + 1)),
   SHADER_FLAG_MASKSAM22         = (1 << (SHADER_FLAG_MASKSAM_BITSHIFT + 2)),
   SHADER_FLAG_IMG               = (1 << 9),
   SHADER_FLAG_BIGENDIAN         = (1 << 10),
   SHADER_FLAG_YUV               = (1 << 11),
   SHADER_FLAG_YUY2              = (1 << 12),
   SHADER_FLAG_NV12              = (1 << 13),
   SHADER_FLAG_YUV_709           = (1 << 14),
   SHADER_FLAG_EXTERNAL          = (1 << 15),
   SHADER_FLAG_AFILL             = (1 << 16),
   SHADER_FLAG_NOMUL             = (1 << 17),
   SHADER_FLAG_ALPHA             = (1 << 18),
   SHADER_FLAG_RGB_A_PAIR        = (1 << 19),
} Shader_Flag;
#define SHADER_FLAG_COUNT 20

static const char *_shader_flags[SHADER_FLAG_COUNT] = {
   "TEX",
   "BGRA",
   "MASK",
   "SAM12",
   "SAM21",
   "SAM22",
   "MASKSAM12",
   "MASKSAM21",
   "MASKSAM22",
   "IMG",
   "BIGENDIAN",
   "YUV",
   "YUY2",
   "NV12",
   "YUV_709",
   "EXTERNAL",
   "AFILL",
   "NOMUL",
   "ALPHA",
   "RGB_A_PAIR"
};

static Eina_Bool compiler_released = EINA_FALSE;

static void
gl_compile_link_error(GLuint target, const char *action, Eina_Bool is_shader)
{
   int loglen = 0, chars = 0;
   char *logtxt;

   if (is_shader)
     /* Shader info log */
     GL_TH(glGetShaderiv, target, GL_INFO_LOG_LENGTH, &loglen);
   else
     /* Program info log */
     GL_TH(glGetProgramiv, target, GL_INFO_LOG_LENGTH, &loglen);

   if (loglen > 0)
     {
        logtxt = calloc(loglen, sizeof(char));
        if (logtxt)
          {
             if (is_shader) GL_TH(glGetShaderInfoLog, target, loglen, &chars, logtxt);
             else GL_TH(glGetProgramInfoLog, target, loglen, &chars, logtxt);
             ERR("Failed to %s: %s", action, logtxt);
             free(logtxt);
          }
     }
}

static inline void
_attributes_bind(GLint prg)
{
   GL_TH(glBindAttribLocation, prg, SHAD_VERTEX,  "vertex");
   GL_TH(glBindAttribLocation, prg, SHAD_COLOR,   "color");
   GL_TH(glBindAttribLocation, prg, SHAD_TEXUV,   "tex_coord");
   GL_TH(glBindAttribLocation, prg, SHAD_TEXUV2,  "tex_coord2");
   GL_TH(glBindAttribLocation, prg, SHAD_TEXUV3,  "tex_coord3");
   GL_TH(glBindAttribLocation, prg, SHAD_TEXA,    "tex_coorda");
   GL_TH(glBindAttribLocation, prg, SHAD_TEXSAM,  "tex_sample");
   GL_TH(glBindAttribLocation, prg, SHAD_MASK,    "mask_coord");
   GL_TH(glBindAttribLocation, prg, SHAD_MASKSAM, "tex_masksample");
}

static Evas_GL_Program *
_evas_gl_common_shader_program_binary_load(Eet_File *ef, unsigned int flags)
{
   int num = 0, length = 0;
   int *formats = NULL;
   void *data = NULL;
   char pname[32];
   GLint ok = 0, prg, vtx = GL_NONE, frg = GL_NONE;
   Evas_GL_Program *p = NULL;
   Eina_Bool direct = 1;

   if (!ef || !glsym_glProgramBinary) return NULL;

   snprintf(pname, sizeof(pname), SHADER_PROG_NAME_FMT, flags);
   data = (void *) eet_read_direct(ef, pname, &length);
   if (!data)
     {
        data = eet_read(ef, pname, &length);
        direct = 0;
     }
   if ((!data) || (length <= 0)) goto finish;

   GL_TH(glGetIntegerv, GL_NUM_PROGRAM_BINARY_FORMATS, &num);
   if (num <= 0) goto finish;

   formats = calloc(num, sizeof(int));
   if (!formats) goto finish;

   GL_TH(glGetIntegerv, GL_PROGRAM_BINARY_FORMATS, formats);
   if (!formats[0]) goto finish;

   prg = GL_TH(glCreateProgram);
#if 1
   // TODO: invalid rendering error occurs when attempting to use a
   // glProgramBinary. in order to render correctly we should create a dummy
   // vertex shader.
   vtx = GL_TH(glCreateShader, GL_VERTEX_SHADER);
   GL_TH(glAttachShader, prg, vtx);
   frg = GL_TH(glCreateShader, GL_FRAGMENT_SHADER);
   GL_TH(glAttachShader, prg, frg);
#endif
   GL_TH_CALL(glProgramBinaryOES, glsym_glProgramBinary, prg, formats[0], data, length);

   _attributes_bind(prg);

   GL_TH(glGetProgramiv, prg, GL_LINK_STATUS, &ok);
   if (!ok)
     {
        gl_compile_link_error(prg, "load a program object", EINA_FALSE);
        ERR("Abort load of program (%s)", pname);
        GL_TH(glDeleteProgram, prg);
        goto finish;
     }

   p = calloc(1, sizeof(*p));
   if (!p) goto finish;
   p->flags = flags;
   p->prog = prg;
   p->reset = EINA_TRUE;
   p->bin_saved = EINA_TRUE;
   GL_TH(glUseProgram, prg);
   p->uniform.mvp = GL_TH(glGetUniformLocation, prg, "mvp");
   p->uniform.rotation_id = GL_TH(glGetUniformLocation, prg, "rotation_id");
   evas_gl_common_shader_textures_bind(p);

finish:
   if (vtx) GL_TH(glDeleteShader, vtx);
   if (frg) GL_TH(glDeleteShader, frg);
   free(formats);
   if (!direct) free(data);
   return p;
}

static int
_evas_gl_common_shader_program_binary_save(Evas_GL_Program *p, Eet_File *ef)
{
   void* data = NULL;
   GLenum format;
   int length = 0, size = 0;
   char pname[32];

   if (!glsym_glGetProgramBinary) return 0;

   GL_TH(glGetProgramiv, p->prog, GL_PROGRAM_BINARY_LENGTH, &length);
   if (length <= 0) return 0;

   data = malloc(length);
   if (!data) return 0;

   GL_TH_CALL(glGetProgramBinaryOES, glsym_glGetProgramBinary, p->prog, length, &size, &format, data);

   if (length != size)
     {
        free(data);
        return 0;
     }

   snprintf(pname, sizeof(pname), SHADER_PROG_NAME_FMT, p->flags);
   if (eet_write(ef, pname, data, length, SHADER_BINARY_EET_COMPRESS) < 0)
     {
        free(data);
        return 0;
     }

   free(data);
   p->bin_saved = 1;
   return 1;
}

static void
_evas_gl_common_shader_binary_hash(Evas_GL_Shared *shared)
{
   if (shared->shaders_checksum)
     return;

   /* This hash makes it sure that if the shaders code changes, then we
    * will not reuse the old binaries. */
   shared->shaders_checksum = eina_stringshare_printf
         ("%#x:%#x",
          eina_hash_superfast(fragment_glsl, strlen(fragment_glsl)),
          eina_hash_superfast(vertex_glsl, strlen(vertex_glsl)));
}

static Eina_Bool
_evas_gl_common_shader_binary_checksum_check(Evas_GL_Shared *shared, Eet_File *ef)
{
   Eina_Bool ret = EINA_FALSE;
   const char *old_hash;
   int len = 0;

   if (!ef) return EINA_FALSE;
   _evas_gl_common_shader_binary_hash(shared);
   old_hash = eet_read_direct(ef, SHADER_EET_CHECKSUM, &len);
   if (old_hash &&
       (len == (eina_stringshare_strlen(shared->shaders_checksum) + 1)) &&
       (!strcmp(shared->shaders_checksum, old_hash)))
     ret = EINA_TRUE;

   return ret;
}

static Eina_Bool
_evas_gl_common_shader_binary_checksum_write(Evas_GL_Shared *shared, Eet_File *ef)
{
   int ret, len;

   if (!ef) return EINA_FALSE;
   _evas_gl_common_shader_binary_hash(shared);
   len = eina_stringshare_strlen(shared->shaders_checksum) + 1;
   ret = eet_write(ef, SHADER_EET_CHECKSUM, shared->shaders_checksum, len, 0);

   return (ret == len);
}

static int
_evas_gl_common_shader_binary_init(Evas_GL_Shared *shared)
{
   Eet_File *ef = NULL;
   char bin_dir_path[PATH_MAX];
   char bin_file_path[PATH_MAX];

   if (!shared || !shared->info.bin_program)
     return 1;

   if (shared->shaders_cache)
     return 1;

   if (!evas_gl_common_file_cache_dir_check(bin_dir_path, sizeof(bin_dir_path)))
     return 0;

   if (!evas_gl_common_file_cache_file_check(bin_dir_path, SHADER_EET_CACHENAME,
                                             bin_file_path, sizeof(bin_dir_path), 0))
     return 0;

   if (!eet_init()) return 0;
   ef = eet_open(bin_file_path, EET_FILE_MODE_READ);
   if (!_evas_gl_common_shader_binary_checksum_check(shared, ef))
     goto error;

   shared->shaders_cache = ef;
   return 1;

error:
   if (ef) eet_close(ef);
   eet_shutdown();
   return 0;
}

static int
_evas_gl_common_shader_binary_save(Evas_GL_Shared *shared)
{
   char bin_dir_path[PATH_MAX];
   char bin_file_path[PATH_MAX];
   char tmp_file_name[PATH_MAX];
   int tmpfd = -1, copy;
   Eina_Tmpstr *tmp_file_path = NULL;
   Eet_File *ef = NULL;
   Evas_GL_Program *p;
   Eina_Iterator *it;
   char pname[32];

   /* use eet */
   if (!eet_init()) return 0;

   if (!evas_gl_common_file_cache_dir_check(bin_dir_path, sizeof(bin_dir_path)))
     {
        if (!evas_gl_common_file_cache_mkpath(bin_dir_path))
          return 0; /* we can't make directory */
     }

   copy = evas_gl_common_file_cache_file_check(bin_dir_path, SHADER_EET_CACHENAME,
                                               bin_file_path, sizeof(bin_dir_path), 0);

   /* use mkstemp for writing */
   snprintf(tmp_file_name, sizeof(tmp_file_name), "%s.XXXXXX.cache", bin_file_path);
   tmpfd = eina_file_mkstemp(tmp_file_name, &tmp_file_path);
   if (tmpfd < 0) goto error;

   /* copy old file */
   if (copy)
     {
        if(!eina_file_copy(bin_file_path, tmp_file_path, EINA_FILE_COPY_DATA, NULL, NULL))
          ERR("fail to copy old shader");
     }

   ef = eet_open(tmp_file_path, copy ? EET_FILE_MODE_READ_WRITE : EET_FILE_MODE_WRITE);
   if (!ef) goto error;

   if (!_evas_gl_common_shader_binary_checksum_write(shared, ef))
     goto error;

   it = eina_hash_iterator_data_new(shared->shaders_hash);
   EINA_ITERATOR_FOREACH(it, p)
     {
        if (!p->bin_saved)
          {
             int len = 0;
             snprintf(pname, sizeof(pname), SHADER_PROG_NAME_FMT, p->flags);
             eet_read_direct(ef, pname, &len);
             if (len > 0)
               p->bin_saved = 1; // assume bin data is correct
             else
               _evas_gl_common_shader_program_binary_save(p, ef);
          }
     }
   eina_iterator_free(it);

   if (shared->shaders_cache)
     {
        eet_close(shared->shaders_cache);
        shared->shaders_cache = NULL;
        eet_shutdown();
     }

   if (eet_close(ef) != EET_ERROR_NONE) goto destroyed;
   if (rename(tmp_file_path, bin_file_path) < 0) goto destroyed;
   eina_tmpstr_del(tmp_file_path);
   close(tmpfd);
   eet_shutdown();

   shared->needs_shaders_flush = 0;
   return 1;

 destroyed:
   ef = NULL;

 error:
   if (tmpfd >= 0) close(tmpfd);
   if (ef) eet_close(ef);
   if (evas_gl_common_file_cache_file_exists(tmp_file_path))
     unlink(tmp_file_path);
   eina_tmpstr_del(tmp_file_path);
   eet_shutdown();
   return 0;
}

static inline void
_program_del(Evas_GL_Program *p)
{
   if (p->prog) GL_TH(glDeleteProgram, p->prog);
   free(p);
}

static void
_shaders_hash_free_cb(void *data)
{
   _program_del(data);
}

static void
_shader_lock(Evas_GL_Shared *shared,const char *cache_dir)
{
   char lock_path[PATH_MAX];
   snprintf(lock_path, sizeof(lock_path), "%s/shader_compile.lock", cache_dir);
   shared->lock_shader = open(lock_path, O_RDWR | O_CREAT , S_IRUSR | S_IWUSR);
   if(shared->lock_shader<0)
     {
        ERR("lock create is failed ");
        return;
     }

   /* lock for wait creating shader*/
   struct flock filelock;
   filelock.l_type = F_WRLCK;
   filelock.l_whence = SEEK_SET;
   filelock.l_start = 0;
   filelock.l_len = 0;
   if (fcntl(shared->lock_shader, F_SETLKW, &filelock) == -1)
     {
        ERR("lock take fail");
        return;
     }
}

static void
_shader_unlock(Evas_GL_Shared *shared)
{
   if(shared->lock_shader < 0) return;

    /* reset lock */
   struct flock filelock;
   filelock.l_type = F_UNLCK;
   filelock.l_whence = SEEK_SET;
   filelock.l_start = 0;
   filelock.l_len = 0;
   if (fcntl(shared->lock_shader, F_SETLKW, &filelock) == -1)
     {
        ERR("lock release fail");
     }

   /* need to close lock file */
   close(shared->lock_shader);
}

static char *
evas_gl_common_shader_glsl_get(unsigned int flags, const char *base)
{
   Eina_Strbuf *s = eina_strbuf_new();
   unsigned int k;
   char *str;

   for (k = 0; k < SHADER_FLAG_COUNT; k++)
     {
        if (flags & (1 << k))
          eina_strbuf_append_printf(s, "#define SHD_%s\n", _shader_flags[k]);
     }

   eina_strbuf_append(s, base);
   str = eina_strbuf_string_steal(s);
   eina_strbuf_free(s);
   return str;
}

static Evas_GL_Program *
evas_gl_common_shader_compile(unsigned int flags, const char *vertex,
                              const char *fragment)
{
   Evas_GL_Program *p;
   GLuint vtx, frg, prg;
   GLint ok = 0;

   compiler_released = EINA_FALSE;
   vtx = GL_TH(glCreateShader, GL_VERTEX_SHADER);
   frg = GL_TH(glCreateShader, GL_FRAGMENT_SHADER);

   GL_TH(glShaderSource, vtx, 1, &vertex, NULL);
   GL_TH(glCompileShader, vtx);
   GL_TH(glGetShaderiv, vtx, GL_COMPILE_STATUS, &ok);
   if (!ok)
     {
        gl_compile_link_error(vtx, "compile vertex shader", EINA_TRUE);
        ERR("Abort compile of vertex shader:\n%s", vertex);
        GL_TH(glDeleteShader, vtx);
        return NULL;
     }
   ok = 0;

   GL_TH(glShaderSource, frg, 1, &fragment, NULL);
   GL_TH(glCompileShader, frg);
   GL_TH(glGetShaderiv, frg, GL_COMPILE_STATUS, &ok);
   if (!ok)
     {
        gl_compile_link_error(frg, "compile fragment shader", EINA_TRUE);
        ERR("Abort compile of fragment shader:\n%s", fragment);
        GL_TH(glDeleteShader, vtx);
        GL_TH(glDeleteShader, frg);
        return NULL;
     }
   ok = 0;

   prg = GL_TH(glCreateProgram);
#ifndef GL_GLES
   if ((glsym_glGetProgramBinary) && (glsym_glProgramParameteri))
     GL_TH_CALL(glProgramParameteri, glsym_glProgramParameteri, prg, GL_PROGRAM_BINARY_RETRIEVABLE_HINT, GL_TRUE);
#endif
   GL_TH(glAttachShader, prg, vtx);
   GL_TH(glAttachShader, prg, frg);

   _attributes_bind(prg);

   GL_TH(glLinkProgram, prg);
   GL_TH(glGetProgramiv, prg, GL_LINK_STATUS, &ok);
   if (!ok)
     {
        gl_compile_link_error(prg, "link fragment and vertex shaders", EINA_FALSE);
        ERR("Abort compile of shader (flags: %08x)", flags);
        GL_TH(glDeleteShader, vtx);
        GL_TH(glDeleteShader, frg);
        GL_TH(glDeleteProgram, prg);
        return 0;
     }

   p = calloc(1, sizeof(*p));
   if (p)
     {
       p->flags = flags;
       p->prog = prg;
       p->reset = EINA_TRUE;
     }

   GL_TH(glDeleteShader, vtx);
   GL_TH(glDeleteShader, frg);

   return p;
}

static Eina_List *
evas_gl_common_shader_precompile_list(Evas_GL_Shared *shared)
{
   int bgra, mask, sam, masksam, img, nomul, afill, yuv;
   Eina_List *li = NULL;

   if (!shared) return NULL;

   // if need to compile almost shaders during precompile, enable this enviroment
   if (getenv("EVAS_GL_SHADER_PRECOMPILE_ALL"))
     {
        // rect
        li = eina_list_append(li, P(BASEFLAG));

        // text
        for (mask = 0; mask <= 1; mask++)
          for (masksam = SHD_SAM11; masksam < (mask ? SHD_SAM_LAST : 1); masksam++)
            {
               int           flags  = BASEFLAG | SHADER_FLAG_TEX | SHADER_FLAG_ALPHA;
               if (mask)     flags |= SHADER_FLAG_MASK;
               if (masksam)  flags |= (1 << (SHADER_FLAG_MASKSAM_BITSHIFT + masksam - 1));
               li = eina_list_append(li, P(flags));
            }

        // images
        for (mask = 0; mask <= 1; mask++)
          for (masksam = SHD_SAM11; masksam < (mask ? SHD_SAM_LAST : 1); masksam++)
            for (sam = SHD_SAM11; sam < SHD_SAM_LAST; sam++)
              for (bgra = 0; bgra <= shared->info.bgra; bgra++)
                for (img = 0; img <= 1; img++)
                  for (nomul = 0; nomul <= 1; nomul++)
                    for (afill = 0; afill <= (mask ? 0 : 1); afill++)
                      {
                         int           flags  = BASEFLAG | SHADER_FLAG_TEX;
                         if (mask)     flags |= SHADER_FLAG_MASK;
                         if (masksam)  flags |= (1 << (SHADER_FLAG_MASKSAM_BITSHIFT + masksam - 1));
                         if (sam)      flags |= (1 << (SHADER_FLAG_SAM_BITSHIFT + sam - 1));
                         if (bgra)     flags |= SHADER_FLAG_BGRA;
                         if (img)      flags |= SHADER_FLAG_IMG;
                         if (nomul)    flags |= SHADER_FLAG_NOMUL;
                         if (afill)    flags |= SHADER_FLAG_AFILL;
                         li = eina_list_append(li, P(flags));
                      }

        // yuv
        for (yuv = SHADER_FLAG_YUV; yuv <= SHADER_FLAG_YUV_709; yuv *= 2)
          for (mask = 0; mask <= 1; mask++)
            for (masksam = SHD_SAM11; masksam < (mask ? SHD_SAM_LAST : 1); masksam++)
              for (nomul = 0; nomul <= 1; nomul++)
                {
                   int           flags  = BASEFLAG | SHADER_FLAG_TEX | yuv;
                   if (mask)     flags |= SHADER_FLAG_MASK;
                   if (masksam)  flags |= (1 << (SHADER_FLAG_MASKSAM_BITSHIFT + masksam - 1));
                   if (yuv == SHADER_FLAG_YUV_709) flags |= SHADER_FLAG_YUV;
                   if (nomul)    flags |= SHADER_FLAG_NOMUL;
                   li = eina_list_append(li, P(flags));
                }

         // rgb+a pair, external, and others will not be precompiled.
        DBG("Built list of %d shaders to precompile", eina_list_count(li));
        return li;
     }

   // To optimize first booting time, the precompiling's list are reduced.
   // so base shaders are only included to the list.

   /* most popular shaders */
   const int BGRA = (shared->info.bgra ? SHADER_FLAG_BGRA : 0);
   const int autoload[] = {
      /* rect */ BASEFLAG,
      /* text */ BASEFLAG | SHADER_FLAG_TEX | SHADER_FLAG_ALPHA,
      /* img1 */ BASEFLAG | SHADER_FLAG_TEX | SHADER_FLAG_IMG | BGRA,
      /* img2 */ BASEFLAG | SHADER_FLAG_TEX | SHADER_FLAG_IMG | SHADER_FLAG_NOMUL | BGRA,
   };

   /* add popular shaders in application */
   const int perf_shader_cnt = 9;
   const int perf_load[] = {
      /* SAM12 img1 */ BASEFLAG | SHADER_FLAG_TEX | SHADER_FLAG_IMG | SHADER_FLAG_SAM12 | BGRA,
      /* SAM12 img2 */ BASEFLAG | SHADER_FLAG_TEX | SHADER_FLAG_IMG | SHADER_FLAG_SAM12 | SHADER_FLAG_NOMUL | BGRA,
      /* SAM21 img1 */ BASEFLAG | SHADER_FLAG_TEX | SHADER_FLAG_IMG | SHADER_FLAG_SAM21 | BGRA,
      /* SAM21 img2 */ BASEFLAG | SHADER_FLAG_TEX | SHADER_FLAG_IMG | SHADER_FLAG_SAM21 | SHADER_FLAG_NOMUL | BGRA,
      /* SAM22 img1 */ BASEFLAG | SHADER_FLAG_TEX | SHADER_FLAG_IMG | SHADER_FLAG_SAM22 | BGRA,
      /* SAM22 img2 */ BASEFLAG | SHADER_FLAG_TEX | SHADER_FLAG_IMG | SHADER_FLAG_SAM22 | SHADER_FLAG_NOMUL | BGRA,
      /* TEX BGRA */ BASEFLAG | SHADER_FLAG_TEX | SHADER_FLAG_NOMUL | BGRA,
      /* EXTERNAL */ BASEFLAG | SHADER_FLAG_TEX | SHADER_FLAG_EXTERNAL,
      /* EXTERNAL2 */ BASEFLAG | SHADER_FLAG_TEX | SHADER_FLAG_EXTERNAL | SHADER_FLAG_NOMUL,
   };

   int i;
   // Add to base shaders
   for (i = 0; i < 4; i++)
     li = eina_list_append(li, P(autoload[i]));

   for (i = 0; i < perf_shader_cnt; i++)
     li = eina_list_append(li, P(perf_load[i]));

   DBG("Built list of %d shaders to precompile", eina_list_count(li));
   return li;
}

static Evas_GL_Program *
evas_gl_common_shader_generate_and_compile(Evas_GL_Shared *shared, unsigned int flags)
{
   char *vertex, *fragment;
   Evas_GL_Program *p;

   if (eina_hash_find(shared->shaders_hash, &flags))
     return NULL;

   vertex = evas_gl_common_shader_glsl_get(flags, vertex_glsl);
   fragment = evas_gl_common_shader_glsl_get(flags, fragment_glsl);

   p = evas_gl_common_shader_compile(flags, vertex, fragment);
   if (p)
     {
        shared->needs_shaders_flush = 1;
        p->uniform.mvp = GL_TH(glGetUniformLocation, p->prog, "mvp");
        p->uniform.rotation_id = GL_TH(glGetUniformLocation, p->prog, "rotation_id");
        evas_gl_common_shader_textures_bind(p);
        eina_hash_add(shared->shaders_hash, &flags, p);
     }
   else WRN("Failed to compile a shader (flags: %08x)", flags);

   free(vertex);
   free(fragment);

   return p;
}

static int
evas_gl_common_shader_precompile_all(Evas_GL_Shared *shared)
{
   Eina_List *li = evas_gl_common_shader_precompile_list(shared);
   Evas_GL_Program *p;
   int total, cnt = 0;
   void *data;

   total = eina_list_count(li);
   EINA_LIST_FREE(li, data)
     {
        p = evas_gl_common_shader_generate_and_compile(shared, I(data));
        if (p)
          {
             p->delete_me = 1;
             cnt++;
          }
     }

   DBG("Precompiled %d/%d shaders!", cnt, total);
   return cnt;
}

int
evas_gl_common_shader_program_init(Evas_GL_Shared *shared)
{
   /* most popular shaders */
   const int BGRA = (shared->info.bgra ? SHADER_FLAG_BGRA : 0);
   const int autoload[] = {
      /* rect */ BASEFLAG,
      /* text */ BASEFLAG | SHADER_FLAG_TEX | SHADER_FLAG_ALPHA,
      /* img1 */ BASEFLAG | SHADER_FLAG_TEX | SHADER_FLAG_IMG | BGRA,
      /* img2 */ BASEFLAG | SHADER_FLAG_TEX | SHADER_FLAG_IMG | SHADER_FLAG_NOMUL | BGRA,
   };
   Evas_GL_Program *p;
   unsigned i;
   char bin_dir_path[PATH_MAX];

   shared->shaders_hash = eina_hash_int32_new(_shaders_hash_free_cb);
   if (!evas_gl_common_file_cache_dir_check(bin_dir_path, sizeof(bin_dir_path)))
     evas_gl_common_file_cache_mkpath(bin_dir_path);

   if(_evas_gl_common_shader_binary_init(shared))
     goto Load;

   //shader lock for wait other process
   _shader_lock(shared,bin_dir_path);
   if(_evas_gl_common_shader_binary_init(shared))
     {
        _shader_unlock(shared);
        goto Load;
     }

   //precompile shader
   evas_gl_common_shader_precompile_all(shared);
   for (i = 0; i < (sizeof(autoload) / sizeof(autoload[0])); i++)
     {
        p = eina_hash_find(shared->shaders_hash, &autoload[i]);
        if (p) p->delete_me = 0;
     }
   evas_gl_common_shaders_flush(shared);

   //shader un-lock for wait other process
   _shader_unlock(shared);
   return 1;

Load:
   for (i = 0; i < (sizeof(autoload) / sizeof(autoload[0])); i++)
     {
        p = _evas_gl_common_shader_program_binary_load(shared->shaders_cache, autoload[i]);
        if (p)
          {
             evas_gl_common_shader_textures_bind(p);
             eina_hash_add(shared->shaders_hash, &autoload[i], p);
          }
     }
   return 1;
}

EAPI void
evas_gl_common_shaders_flush(Evas_GL_Shared *shared)
{

   if (!shared) return;
   if (!compiler_released)
     {
        compiler_released = EINA_TRUE;
#ifdef GL_GLES
        GL_TH(glReleaseShaderCompiler);
#else
        if (glsym_glReleaseShaderCompiler)
          GL_TH_CALL(glReleaseShaderCompiler, glsym_glReleaseShaderCompiler);
#endif
     }
   if (shared->needs_shaders_flush)
     {
        Eina_List *to_delete = NULL;
        Eina_Iterator *it;
        Evas_GL_Program *p;

        _evas_gl_common_shader_binary_save(shared);

        it = eina_hash_iterator_data_new(shared->shaders_hash);
        EINA_ITERATOR_FOREACH(it, p)
          {
             if (p->delete_me)
               to_delete = eina_list_append(to_delete, p);
          }

        EINA_LIST_FREE(to_delete, p)
          eina_hash_del(shared->shaders_hash, &p->flags, p);
     }
}

void
evas_gl_common_shader_program_shutdown(Evas_GL_Shared *shared)
{
   if (!shared) return;

   if (shared->needs_shaders_flush)
     evas_gl_common_shaders_flush(shared);

   if (shared->shaders_cache)
     {
        eet_close(shared->shaders_cache);
        shared->shaders_cache = NULL;
        eet_shutdown();
     }

   eina_hash_free(shared->shaders_hash);
   shared->shaders_hash = NULL;
}

static inline unsigned int
evas_gl_common_shader_flags_get(Evas_GL_Shared *shared, Shader_Type type,
                                RGBA_Map_Point *map_points, int npoints,
                                int r, int g, int b, int a,
                                int sw, int sh, int w, int h, Eina_Bool smooth,
                                Evas_GL_Texture *tex, Eina_Bool tex_only,
                                Evas_GL_Texture *mtex, Eina_Bool mask_smooth,
                                int mw, int mh,
                                Shader_Sampling *psam, int *pnomul, Shader_Sampling *pmasksam)
{
   Shader_Sampling sam = SHD_SAM11, masksam = SHD_SAM11;
   int nomul = 1, bgra = 0, k;
   unsigned int flags = BASEFLAG;

   // image downscale sampling
   if (smooth && ((type == SHD_IMAGE) || (type == SHD_IMAGENATIVE)))
     {
        if ((sw >= (w * 2)) && (sh >= (h * 2)))
          sam = SHD_SAM22;
        else if (sw >= (w * 2))
          sam = SHD_SAM21;
        else if (sh >= (h * 2))
          sam = SHD_SAM12;
        if (sam)
          flags |= (1 << (SHADER_FLAG_SAM_BITSHIFT + sam - 1));
     }

   // mask downscale sampling
   if (mtex && mask_smooth)
     {
        if ((mtex->w >= (mw * 2)) && (mtex->h >= (mh * 2)))
          masksam = SHD_SAM22;
        else if (mtex->w >= (mw * 2))
          masksam = SHD_SAM21;
        else if (mtex->h >= (mh * 2))
          masksam = SHD_SAM12;
        if (masksam)
          flags |= (1 << (SHADER_FLAG_MASKSAM_BITSHIFT + masksam - 1));
     }

   switch (type)
     {
      case SHD_RECT:
      case SHD_LINE:
        goto end;
      case SHD_FONT:
        flags |= (SHADER_FLAG_ALPHA | SHADER_FLAG_TEX);
        goto end;
      case SHD_IMAGE:
        flags |= SHADER_FLAG_IMG;
        break;
      case SHD_IMAGENATIVE:
        break;
      case SHD_YUV:
        flags |= SHADER_FLAG_YUV;
        break;
      case SHD_YUY2:
        flags |= SHADER_FLAG_YUY2;
        break;
      case SHD_NV12:
        flags |= SHADER_FLAG_NV12;
        break;
      case SHD_YUV_709:
        flags |= (SHADER_FLAG_YUV_709 | SHADER_FLAG_YUV);
        break;
      case SHD_RGB_A_PAIR:
      case SHD_MAP:
        break;
      default:
        CRI("Impossible shader type.");
        return 0;
     }

   // color mul
   if ((a == 255) && (r == 255) && (g == 255) && (b == 255))
     {
        if (map_points)
          {
             for (k = 0; k < npoints; k++)
               if (map_points[k].col != 0xffffffff)
                 {
                    nomul = 0;
                    break;
                 }
          }
     }
   else
     nomul = 0;

   if (nomul)
     flags |= SHADER_FLAG_NOMUL;

   // bgra
   if (tex_only)
     {
        if (tex->im && tex->im->native.target == GL_TEXTURE_EXTERNAL_OES)
          flags |= SHADER_FLAG_EXTERNAL;
        else
          bgra = 1;
     }
   else
     bgra = shared->info.bgra;

// This is only bug and fixed by upstream. AFILL flag is SET only for external images with No alpha
    if (tex)
     {
        flags |= SHADER_FLAG_TEX;
        if (!tex->alpha && tex_only)
          {
             // TIZEN_ONLY: the AFILL's policy for tizen
             if ((flags & SHADER_FLAG_EXTERNAL) || (tex->pt->dyn.img))
               {
                 flags |= SHADER_FLAG_AFILL;
               }
          }
     }
//

   if (bgra)
     flags |= SHADER_FLAG_BGRA;

end:
   if (mtex)
     flags |= SHADER_FLAG_MASK;

   if (psam) *psam = sam;
   if (pnomul) *pnomul = nomul;
   if (pmasksam) *pmasksam = masksam;
   return flags;
}

void
evas_gl_common_shader_textures_bind(Evas_GL_Program *p)
{
   struct {
      const char *name;
      int enabled;
   } textures[] = {
      { "tex", 0 },
      { "texm", 0 },
      { "texa", 0 },
      { "texu", 0 },
      { "texv", 0 },
      { "texuv", 0 },
      { NULL, 0 }
   };
   Eina_Bool hastex = 0;
   GLint loc;
   int i;

   if (!p || (p->tex_count > 0)) return;

   if ((p->flags & SHADER_FLAG_TEX) != 0)
     {
        textures[0].enabled = 1;
        hastex = 1;
     }
   if ((p->flags & SHADER_FLAG_MASK) != 0)
     {
        textures[1].enabled = 1;
        hastex = 1;
     }
   if ((p->flags & SHADER_FLAG_RGB_A_PAIR) != 0)
     {
        textures[2].enabled = 1;
        hastex = 1;
     }
   if (p->flags & SHADER_FLAG_YUV)
     {
        textures[3].enabled = 1;
        textures[4].enabled = 1;
        hastex = 1;
     }
   else if ((p->flags & SHADER_FLAG_NV12) || (p->flags & SHADER_FLAG_YUY2))
     {
        textures[5].enabled = 1;
        hastex = 1;
     }

   if (hastex)
     {
        GLuint curr_prog = 0;
        GL_TH(glGetIntegerv, GL_CURRENT_PROGRAM, (GLint *)&curr_prog);

        GL_TH(glUseProgram, p->prog); // is this necessary??
        for (i = 0; textures[i].name; i++)
          {
             if (!textures[i].enabled) continue;
             loc = GL_TH(glGetUniformLocation, p->prog, textures[i].name);
             if (loc < 0)
               {
                  ERR("Couldn't find uniform '%s' (shader: %08x)",
                      textures[i].name, p->flags);
               }
             GL_TH(glUniform1i, loc, p->tex_count++);
          }
        GL_TH(glUseProgram, curr_prog);
     }
}

Evas_GL_Program *
evas_gl_common_shader_program_get(Evas_Engine_GL_Context *gc,
                                  Shader_Type type,
                                  RGBA_Map_Point *map_points, int npoints,
                                  int r, int g, int b, int a,
                                  int sw, int sh, int w, int h, Eina_Bool smooth,
                                  Evas_GL_Texture *tex, Eina_Bool tex_only,
                                  Evas_GL_Texture *mtex, Eina_Bool mask_smooth,
                                  int mw, int mh,
                                  Shader_Sampling *psam, int *pnomul,
                                  Shader_Sampling *pmasksam)
{
   unsigned int flags;
   Evas_GL_Program *p;

   flags = evas_gl_common_shader_flags_get(gc->shared, type, map_points, npoints, r, g, b, a,
                                           sw, sh, w, h, smooth, tex, tex_only,
                                           mtex, mask_smooth, mw, mh,
                                           psam, pnomul, pmasksam);
   p = eina_hash_find(gc->shared->shaders_hash, &flags);
   if (!p)
     {
        _evas_gl_common_shader_binary_init(gc->shared);
        if (gc->shared->shaders_cache)
          {
             char pname[32];
             snprintf(pname, sizeof(pname), SHADER_PROG_NAME_FMT, flags);
             p = _evas_gl_common_shader_program_binary_load(gc->shared->shaders_cache, flags);
             if (p)
               {
                  evas_gl_common_shader_textures_bind(p);
                  eina_hash_add(gc->shared->shaders_hash, &flags, p);
                  goto end;
               }
          }
        p = evas_gl_common_shader_generate_and_compile(gc->shared, flags);

        //save shader after compile
        evas_gl_common_shaders_flush(gc->shared);
        if (!p) return NULL;
     }
end:
   if (p->hitcount < PROGRAM_HITCOUNT_MAX)
     p->hitcount++;
   return p;
}
