#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef WIN32_LEAN_AND_MEAN

#include <Eina.h>

#include "Ecore_Win32.h"
#include "ecore_win32_private.h"

/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
static int init_count = 0;
static Ecore_Win32_Clipboard_Converter_Cb  sel_converter_cb = NULL;
static void *sel_converter_udata = NULL;
static UINT REGISTERED_MARKUP = 0;
static UINT REGISTERED_VCARD = 0;
static UINT REGISTERED_URL_LIST = 0;
static UINT REGISTERED_HTML = 0;

/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/

/**
 * @cond LOCAL
 */

 static void
 _clipboard_init(void)
 {
     init_count++;
     if (init_count > 1) return;
     REGISTERED_MARKUP = RegisterClipboardFormat("application/x-elementary-markup");
     REGISTERED_VCARD = RegisterClipboardFormat("text/x-vcard");
     REGISTERED_URL_LIST = RegisterClipboardFormat("text/uri-list");
     REGISTERED_HTML = RegisterClipboardFormat("text/html");
 }

//FIXME: Group formats into category, since we have system converstion
//Ex: TEXT, UNICODETEXT, OEMTEXT -> TEXT
static UINT
_convert_to_windows_format(Ecore_Win32_Selection_Format format)
{
   if (format == ECORE_WIN32_SELECTION_FORMAT_TEXT)
     {
        DBG("CF: TEXT");
        return CF_TEXT;
     }
   else if (format == ECORE_WIN32_SELECTION_FORMAT_UNICODE_TEXT)
     {
        DBG("CF: UNICODETEXT");
        return CF_UNICODETEXT;
     }
   else if (format == ECORE_WIN32_SELECTION_FORMAT_OEM_TEXT)
     {
        DBG("CF: OEMTEXT");
        return CF_OEMTEXT;
     }
   else if (format == ECORE_WIN32_SELECTION_FORMAT_DIB)
     {
        DBG("CF: DIB");
        return CF_DIB;
     }
   else if (format == ECORE_WIN32_SELECTION_FORMAT_BITMAP)
     {
        DBG("CF: BITMAP");
        return CF_BITMAP;
     }
   else if (format == ECORE_WIN32_SELECTION_FORMAT_MARKUP)
     {
        DBG("CF: MARKUP");
        return REGISTERED_MARKUP;
     }
   else if (format == ECORE_WIN32_SELECTION_FORMAT_VCARD)
     {
        DBG("CF: VCARD");
        return REGISTERED_VCARD;
     }
   else if (format == ECORE_WIN32_SELECTION_FORMAT_URL_LIST)
     {
        DBG("CF: URL_LIST");
        return REGISTERED_URL_LIST;
     }
   else if (format == ECORE_WIN32_SELECTION_FORMAT_HTML)
     {
        DBG("CF: HTML");
        return REGISTERED_HTML;
     }

   DBG("CF: TEXT");
   return CF_TEXT;
}

static Ecore_Win32_Selection_Format
_convert_to_ecore_sel_format(UINT window_format)
{
   if (window_format == CF_TEXT)
     {
        DBG("->FORMAT: TEXT");
        return ECORE_WIN32_SELECTION_FORMAT_TEXT;
     }
   else if (window_format == CF_UNICODETEXT)
     {
        DBG("->FORMAT: UNICODETEXT");
        return ECORE_WIN32_SELECTION_FORMAT_UNICODE_TEXT;
     }
   else if (window_format == CF_OEMTEXT)
     {
        DBG("->FORMAT: OEMTEXT");
        return ECORE_WIN32_SELECTION_FORMAT_OEM_TEXT;
     }
   else if (window_format == CF_DIB)
     {
        DBG("->FORMAT: DIB");
        return ECORE_WIN32_SELECTION_FORMAT_DIB;
     }
   else if (window_format == CF_BITMAP)
     {
        DBG("->FORMAT: BITMAP");
        return ECORE_WIN32_SELECTION_FORMAT_BITMAP;
     }
   else if (window_format == REGISTERED_MARKUP)
     {
        DBG("->FORMAT: MARKUP");
        return ECORE_WIN32_SELECTION_FORMAT_MARKUP;
     }
   else if (window_format == REGISTERED_VCARD)
     {
        DBG("->FORMAT: VCARD");
        return ECORE_WIN32_SELECTION_FORMAT_VCARD;
     }
   else if (window_format == REGISTERED_URL_LIST)
     {
        DBG("->FORMAT: URL_LIST");
        return ECORE_WIN32_SELECTION_FORMAT_URL_LIST;
     }
   else if (window_format == REGISTERED_HTML)
     {
        DBG("->FORMAT: HTML");
        return ECORE_WIN32_SELECTION_FORMAT_HTML;
     }
   DBG("->FORMAT: TEXT");
   return ECORE_WIN32_SELECTION_FORMAT_TEXT;
}

void
_ecore_win32_set_clipboard(Ecore_Win32_Callback_Data *msg)
{
   DBG("In");

   void *data = NULL;
   int length = 0;

   if (sel_converter_cb)
     {
        DBG("Call to sel_converter_cb");
        sel_converter_cb(sel_converter_udata,
                         _convert_to_ecore_sel_format(msg->window_param),
                         &data,
                         &length);
     }
   if (!data)
     {
        ERR("Sel_data does not exist!!!");
        return;
     }
   HGLOBAL hglb;
   LPTSTR lptstr;

   hglb = GlobalAlloc(GMEM_MOVEABLE, length + 1);
   if (hglb == NULL)
     {
        ERR("Cannot allocate memory");
        return;
     }

   DBG("set data to clipboard: %s; %d", (char *)data, length);
   lptstr = GlobalLock(hglb);
   memcpy(lptstr, data, length);
   lptstr[length] = (TCHAR) 0;
   GlobalUnlock(hglb);

   SetClipboardData(msg->window_param, hglb);

   free(data);
}
/**
 * @endcond
 */



/*============================================================================*
 *                                   API                                      *
 *============================================================================*/


EAPI void
ecore_win32_clipboard_converter_add(const Ecore_Win32_Window *window,
                                    Ecore_Win32_Clipboard_Converter_Cb func,
                                    void *udata)
{
   sel_converter_cb = func;
   sel_converter_udata = udata;
}

EAPI void
ecore_win32_clipboard_converter_del()
{
   sel_converter_cb = NULL;
   sel_converter_udata = NULL;
}

EAPI Eina_Bool
ecore_win32_clipboard_set(const Ecore_Win32_Window *window,
                          Ecore_Win32_Selection_Format format,
                          const void *data,
                          int size)
{
   HGLOBAL global;
   char *d;

   /*
    * See: https://msdn.microsoft.com/en-us/library/windows/desktop/ms649016%28v=vs.85%29.aspx#_win32_Copying_Information_to_the_Clipboard
    * 1. Open the clipboard
    * 2. Empty the clipboard
    * 3. Set the data
    * 4. Close the clipboard
    */

   INF("setting data to the clipboard");

   if (!window || !data || (size <= 0))
     {
        ERR("No window or data");
        return EINA_FALSE;
     }

   _clipboard_init();

   if (!OpenClipboard(window->window))
     {
        DBG("Cannot open clipboard");
        return EINA_FALSE;
     }

   if (!EmptyClipboard())
     {
        DBG("Cannot empty clipboard");
        goto close_clipboard;
     }

   UINT win_format = _convert_to_windows_format(format);
   DBG("set format: %d, value: NULL (delayed rendering)", win_format);
   SetClipboardData(win_format, NULL); //Enable delayed rendering
   //for working with other Windows apps (like notepad)
   if ((win_format == REGISTERED_MARKUP) ||
       (win_format == REGISTERED_HTML) ||
       (win_format == REGISTERED_URL_LIST))
     {
        DBG("Also set format as CF_TEXT");
        SetClipboardData(CF_TEXT, NULL);
     }
   CloseClipboard();

   DBG("End");
   return EINA_TRUE;

close_clipboard:
   CloseClipboard();

   return EINA_FALSE;
}

/*EAPI Eina_Bool
ecore_win32_clipboard_set(const Ecore_Win32_Window *window,
                          const void *data,
                          int size)
{
   HGLOBAL global;
   char *d;

   /*
    * See: https://msdn.microsoft.com/en-us/library/windows/desktop/ms649016%28v=vs.85%29.aspx#_win32_Copying_Information_to_the_Clipboard
    * 1. Open the clipboard
    * 2. Empty the clipboard
    * 3. Set the data
    * 4. Close the clipboard
    *

   INF("setting data to the clipboard");

   if (!window || !data || (size <= 0))
     return EINA_FALSE;

   if (!OpenClipboard(window->window))
     return EINA_FALSE;

   if (!EmptyClipboard())
     goto close_clipboard;

   /*global = GlobalAlloc(GMEM_MOVEABLE, size + 1);
   if (!global)
     goto close_clipboard;

   d = (char *)GlobalLock(global);
   if (!d)
     goto unlock_global;

   memcpy(d, data, size);
   d[size] = '\0';
   GlobalUnlock(global);*
   //SetClipboardData(CF_TEXT, global);
   SetClipboardData(CF_TEXT, NULL); //Enable delayed rendering
   CloseClipboard();

   ERR("End");
   return EINA_TRUE;

 /*unlock_global:
   GlobalUnlock(global);*
 close_clipboard:
   CloseClipboard();

   return EINA_FALSE;
}*/

EAPI Eina_Bool
ecore_win32_clipboard_get(const Ecore_Win32_Window *window,
                          Ecore_Win32_Selection_Format format,
                          void **data,
                          int *size)
{
   HGLOBAL global;
   void *d;
   void *p;

   /*
    * See https://msdn.microsoft.com/en-us/library/windows/desktop/ms649016%28v=vs.85%29.aspx#_win32_Pasting_Information_from_the_Clipboard
    * 1. Open Clipboard
    * 2. Determine format
    * 3. Retrieve data
    * 4. Manage data
    * 5. Close clipboard
    */

   INF("getting data from the clipboard");

   DBG("In");
   if (!window)
     return EINA_FALSE;

   _clipboard_init();
   UINT win_format = _convert_to_windows_format(format);
   DBG("get format: %d", win_format);
   if (!IsClipboardFormatAvailable(win_format))
   {
      DBG("Format: %d is NOT available", win_format);
     return EINA_FALSE;
   }

   if (!OpenClipboard(window->window))
     goto set_val;

   /* { */
   /*   UINT fmt = 0; */

   /*   while (1) */
   /*     { */
   /*       fmt = EnumClipboardFormats(fmt); */
   /*       printf(" $ Format : %x\n", fmt); */
   /*       if (!fmt) */
   /*         break; */
   /*     } */
   /* } */

   DBG("Get data from clipboard");
   global = GetClipboardData(win_format);
   //global = GetClipboardData(CF_TEXT);
   if (!global)
     goto close_clipboard;

   d = GlobalLock(global);
   if (!d)
     goto unlock_global;

   *size = strlen(d);
   p = malloc(*size);
   if (!p)
     goto unlock_global;

   memcpy(p, d, *size);
   *data = p;
   GlobalUnlock(global);
   CloseClipboard();
   DBG("End");

   return EINA_TRUE;

 unlock_global:
   GlobalUnlock(global);
 close_clipboard:
   CloseClipboard();
 set_val:
   *data = NULL;
   *size = 0;

   return EINA_FALSE;
}

EAPI Eina_Bool
ecore_win32_clipboard_clear(const Ecore_Win32_Window *window)
{
   INF("clearing the clipboard");

   if (!window)
     return EINA_FALSE;

   if (!OpenClipboard(window->window))
     return EINA_FALSE;

   if (!EmptyClipboard())
     goto close_clipboard;

   CloseClipboard();

   return EINA_TRUE;

 close_clipboard:
   CloseClipboard();

   return EINA_FALSE;
}

/**
 * @}
 */
