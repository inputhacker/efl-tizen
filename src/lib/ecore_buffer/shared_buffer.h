#ifndef _SHARED_BUFFER_H_
# define _SHARED_BUFFER_H_

#include <stdio.h>
#include <Eina.h>
#include <Ecore_Buffer.h>

#include "es-share-client-protocol.h"
#include "ecore_buffer_private.h"

typedef struct _Shared_Buffer Shared_Buffer;

typedef enum _Shared_Buffer_State
{
   // common
   SHARED_BUFFER_STATE_UNKNOWN,
   SHARED_BUFFER_STATE_ENQUEUE,
   // provider side type
   SHARED_BUFFER_STATE_SUBMIT,
   SHARED_BUFFER_STATE_DEQUEUE,
   // consumer side type
   SHARED_BUFFER_STATE_ATTACH,
   SHARED_BUFFER_STATE_IMPORT,
   SHARED_BUFFER_STATE_DETACH,
   SHARED_BUFFER_STATE_ACQUIRE,
   SHARED_BUFFER_STATE_RELEASE,
} Shared_Buffer_State;

Shared_Buffer        *_shared_buffer_new(struct es_buffer *proxy, int w, int h, int format, unsigned int flags);
void                  _shared_buffer_free(Shared_Buffer *sb);
Eina_Bool             _shared_buffer_info_get(Shared_Buffer *sb, int *w, int *h, int *format, unsigned int *flags);
Eina_Bool             _shared_buffer_buffer_set(Shared_Buffer *sb, Ecore_Buffer *buffer);
Ecore_Buffer         *_shared_buffer_buffer_get(Shared_Buffer *sb);
Eina_Bool             _shared_buffer_proxy_set(Shared_Buffer *sb, struct es_buffer *proxy);
struct es_buffer     *_shared_buffer_proxy_get(Shared_Buffer *sb);
void                  _shared_buffer_state_set(Shared_Buffer *sb, Shared_Buffer_State state);
Shared_Buffer_State   _shared_buffer_state_get(Shared_Buffer *sb);
const char           *_shared_buffer_state_string_get(Shared_Buffer *sb);

#endif /* _SHARED_BUFFER_H_ */
