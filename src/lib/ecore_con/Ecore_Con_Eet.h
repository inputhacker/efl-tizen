#ifndef _ECORE_CON_EET
# define _ECORE_CON_EET

#include <Eet.h>
#include <Ecore.h>
#include <Ecore_Con.h>

#ifdef EAPI
# undef EAPI
#endif

#ifdef _WIN32
# ifdef EFL_ECORE_CON_BUILD
#  ifdef DLL_EXPORT
#   define EAPI __declspec(dllexport)
#  else
#   define EAPI
#  endif
# else
#  define EAPI __declspec(dllimport)
# endif
#else
# ifdef __GNUC__
#  if __GNUC__ >= 4
#   define EAPI __attribute__ ((visibility("default")))
#  else
#   define EAPI
#  endif
# else
#  define EAPI
# endif
#endif

/**
 * @defgroup Ecore_Con_Eet_Group Eet connection functions
 * @ingroup Ecore_Con_Group
 *
 * The Ecore Connection Eet library ( @c Ecore_Con_Eet) adds @c Eet data
 * serialization features to Ecore Connection objects. Its main aim is to
 * provide a way to send @c Eet data streams to another program through sockets
 * using @c Ecore_Con objects.
 *
 * @{
 */

typedef Eo Ecore_Con_Eet;
typedef struct _Ecore_Con_Reply Ecore_Con_Reply;

/**
 * @typedef Ecore_Con_Eet_Data_Cb
 * @brief Called when an Ecore_Con_Eet object receives data.
 */
typedef void (*Ecore_Con_Eet_Data_Cb)(void *data, Ecore_Con_Reply *reply, const char *protocol_name, void *value);

/**
 * @typedef Ecore_Con_Eet_Raw_Data_Cb
 * @brief Called when an Ecore_Con_Eet object receives raw data.
 */
typedef void (*Ecore_Con_Eet_Raw_Data_Cb)(void *data, Ecore_Con_Reply *reply, const char *protocol_name, const char *section, void *value, size_t length);

/**
 * @typedef Ecore_Con_Eet_Client_Cb
 * @brief Called when a client connects to the server.
 */
typedef Eina_Bool (*Ecore_Con_Eet_Client_Cb)(void *data, Ecore_Con_Reply *reply, Ecore_Con_Client *conn);

/**
 * @typedef Ecore_Con_Eet_Server_Cb
 * @brief Called when the server has accepted the connection of the client.
 */
typedef Eina_Bool (*Ecore_Con_Eet_Server_Cb)(void *data, Ecore_Con_Reply *reply, Ecore_Con_Server *conn);

#ifndef EFL_NOLEGACY_API_SUPPORT
#include "Ecore_Con_Eet_Legacy.h"
#endif
#ifdef EFL_EO_API_SUPPORT
#include "Ecore_Con_Eet_Eo.h"
#endif

/**
 * Create a Ecore_Con_Eet server.
 *
 * @param server    An existing Ecore_Con_Server that have been previously
 *                  created by the server program with @ref
 *                  ecore_con_server_add.
 *
 * This object gets deleted automatically when the parent Ecore_Con_Server is
 * deleted.
 *
 * @return A new Ecore_Con_Eet server.
 *
 * @if MOBILE @since_tizen 3.0
 * @elseif WEARABLE @since_tizen 3.0
 * @endif
 */
EAPI Ecore_Con_Eet *ecore_con_eet_server_new(Ecore_Con_Server *server);

/**
 * Create a Ecore_Con_Eet client.
 *
 * @param server    An existing Ecore_Con_Server that have been previously
 *                  returned by a call to @ref ecore_con_server_connect in the
 *                  client program.
 *
 * This object gets deleted automatically when the parent Ecore_Con_Server is
 * deleted.
 *
 * @return A new Ecore_Con_Eet client.
 *
 * @if MOBILE @since_tizen 3.0
 * @elseif WEARABLE @since_tizen 3.0
 * @endif
 */
EAPI Ecore_Con_Eet *ecore_con_eet_client_new(Ecore_Con_Server *server);

/**
 * Free an existing Ecore_Con_Eet object.
 *
 * @param server    An existing Ecore_Con_Eet object that have been previously
 *                  allocated by a @ref ecore_con_eet_server_new or @ref
 *                  ecore_con_eet_client_new.
 *
 * @if MOBILE @since_tizen 3.0
 * @elseif WEARABLE @since_tizen 3.0
 * @endif
 */
EAPI void ecore_con_eet_server_free(Ecore_Con_Eet *ece);

/**
 * Register an @c Eet data descriptor on a Ecore_Con_Eet object.
 *
 * @param ece       An Ecore_Con_Eet object.
 * @param name      The name of the Eet stream to connect.
 * @param edd       A Eet data descriptor that describes the data organization
 *                  in the Eet stream.
 *
 * @if MOBILE @since_tizen 3.0
 * @elseif WEARABLE @since_tizen 3.0
 * @endif
 */
EAPI void ecore_con_eet_register(Ecore_Con_Eet *ece, const char *name, Eet_Data_Descriptor *edd);

/**
 * Register a data callback on a Ecore_Con_Eet object.
 * When the Ecore_Con_Eet object is deleted, this automatically gets removed.
 *
 * @param ece       An Ecore_Con_Eet object.
 * @param name      The name of the Eet stream to connect.
 * @param func      The function to call as a callback.
 * @param data      The data to pass to the callback.
 *
 * @if MOBILE @since_tizen 3.0
 * @elseif WEARABLE @since_tizen 3.0
 * @endif
 */
EAPI void ecore_con_eet_data_callback_add(Ecore_Con_Eet *ece, const char *name, Ecore_Con_Eet_Data_Cb func, const void *data);

/**
 * Remove a data callback on a Ecore_Con_Eet object.
 *
 * @param ece       An Ecore_Con_Eet object.
 * @param name      The name of the Eet stream to remove callback on.
 *
 * @if MOBILE @since_tizen 3.0
 * @elseif WEARABLE @since_tizen 3.0
 * @endif
 */
EAPI void ecore_con_eet_data_callback_del(Ecore_Con_Eet *ece, const char *name);

/**
 * Register a raw data callback on a Ecore_Con_Eet object.
 * When the Ecore_Con_Eet object is deleted, this automatically gets removed.
 *
 * @param ece       An Ecore_Con_Eet object.
 * @param name      The name of the raw Eet stream to connect.
 * @param func      The function to call as a callback.
 * @param data      The data to pass to the callback.
 *
 * @if MOBILE @since_tizen 3.0
 * @elseif WEARABLE @since_tizen 3.0
 * @endif
 */
EAPI void ecore_con_eet_raw_data_callback_add(Ecore_Con_Eet *ece, const char *name, Ecore_Con_Eet_Raw_Data_Cb func, const void *data);

/**
 * Remove a raw data callback on a Ecore_Con_Eet object.
 *
 * @param ece       An Ecore_Con_Eet object.
 * @param name      The name of the raw Eet stream to remove callback on.
 *
 * @if MOBILE @since_tizen 3.0
 * @elseif WEARABLE @since_tizen 3.0
 * @endif
 */
EAPI void ecore_con_eet_raw_data_callback_del(Ecore_Con_Eet *ece, const char *name);

/**
 * Register a client connect callback on a Ecore_Con_Eet object.
 * @brief This callback can be registered on the server program to know when a
 * client connects.
 * When the Ecore_Con_Eet object is deleted, this automatically gets removed.
 *
 * @param ece       An Ecore_Con_Eet object.
 * @param func      The function to call as a callback.
 * @param data      The data to pass to the callback.
 *
 * @if MOBILE @since_tizen 3.0
 * @elseif WEARABLE @since_tizen 3.0
 * @endif
 */
EAPI void ecore_con_eet_client_connect_callback_add(Ecore_Con_Eet *ece, Ecore_Con_Eet_Client_Cb func, const void *data);

/**
 * Remove a client connect callback on a Ecore_Con_Eet object.
 *
 * @param ece       An Ecore_Con_Eet object.
 * @param func      The callback to remove.
 * @param data      The data passed to this function at the callback registration.
 *
 * @if MOBILE @since_tizen 3.0
 * @elseif WEARABLE @since_tizen 3.0
 * @endif
 */
EAPI void ecore_con_eet_client_connect_callback_del(Ecore_Con_Eet *ece, Ecore_Con_Eet_Client_Cb func, const void *data);

/**
 * Register a client disconnect callback on a Ecore_Con_Eet object.
 * @brief This callback can be registered on the server program to know when a
 * client disconnects.
 * When the Ecore_Con_Eet object is deleted, this automatically gets removed.
 *
 * @param ece       An Ecore_Con_Eet object.
 * @param func      The function to call as a callback.
 * @param data      The data to pass to the callback.
 *
 * @if MOBILE @since_tizen 3.0
 * @elseif WEARABLE @since_tizen 3.0
 * @endif
 */
EAPI void ecore_con_eet_client_disconnect_callback_add(Ecore_Con_Eet *ece, Ecore_Con_Eet_Client_Cb func, const void *data);

/**
 * Remove a client disconnect callback on a Ecore_Con_Eet object.
 *
 * @param ece       An Ecore_Con_Eet object.
 * @param func      The callback to remove.
 * @param data      The data passed to this function at the callback registration.
 *
 * @if MOBILE @since_tizen 3.0
 * @elseif WEARABLE @since_tizen 3.0
 * @endif
 */
EAPI void ecore_con_eet_client_disconnect_callback_del(Ecore_Con_Eet *ece, Ecore_Con_Eet_Client_Cb func, const void *data);

/**
 * Register a server connect callback on a Ecore_Con_Eet object.
 * @brief This callback can be registered on the client program to be called
 * when it has been connected to the server.
 * When the Ecore_Con_Eet object is deleted, this automatically gets removed.
 *
 * @param ece       An Ecore_Con_Eet object.
 * @param func      The function to call as a callback.
 * @param data      The data to pass to the callback.
 *
 * @if MOBILE @since_tizen 3.0
 * @elseif WEARABLE @since_tizen 3.0
 * @endif
 */
EAPI void ecore_con_eet_server_connect_callback_add(Ecore_Con_Eet *ece, Ecore_Con_Eet_Server_Cb func, const void *data);

/**
 * Remove a server connect callback on a Ecore_Con_Eet object.
 *
 * @param ece       An Ecore_Con_Eet object.
 * @param func      The callback to remove.
 * @param data      The data passed to this function at the callback registration.
 *
 * @if MOBILE @since_tizen 3.0
 * @elseif WEARABLE @since_tizen 3.0
 * @endif
 */
EAPI void ecore_con_eet_server_connect_callback_del(Ecore_Con_Eet *ece, Ecore_Con_Eet_Server_Cb func, const void *data);

/**
 * Register a server disconnect callback on a Ecore_Con_Eet object.
 * @brief This callback can be registered on the client program to be called
 * when it has been disconnected from the server.
 * When the Ecore_Con_Eet object is deleted, this automatically gets removed.
 *
 * @param ece       An Ecore_Con_Eet object.
 * @param func      The function to call as a callback.
 * @param data      The data to pass to the callback.
 *
 * @if MOBILE @since_tizen 3.0
 * @elseif WEARABLE @since_tizen 3.0
 * @endif
 */
EAPI void ecore_con_eet_server_disconnect_callback_add(Ecore_Con_Eet *ece, Ecore_Con_Eet_Server_Cb func, const void *data);

/**
 * Remove a server disconnect callback on a Ecore_Con_Eet object.
 *
 * @param ece       An Ecore_Con_Eet object.
 * @param func      The callback to remove.
 * @param data      The data passed to this function at the callback registration.
 *
 * @if MOBILE @since_tizen 3.0
 * @elseif WEARABLE @since_tizen 3.0
 * @endif
 */
EAPI void ecore_con_eet_server_disconnect_callback_del(Ecore_Con_Eet *ece, Ecore_Con_Eet_Server_Cb func, const void *data);

/**
 * Attach data to an Ecore_Con_Eet object.
 *
 * @param ece       An Ecore_Con_Eet object.
 * @param data      The data to attach to the Ecore_Con_Eet object.
 *
 * @if MOBILE @since_tizen 3.0
 * @elseif WEARABLE @since_tizen 3.0
 * @endif
 */
EAPI void ecore_con_eet_data_set(Ecore_Con_Eet *ece, const void *data);

/**
 * Get the data attached to an Ecore_Con_Eet object.
 *
 * @param ece       An Ecore_Con_Eet object.
 * @return The data attached to the Ecore_Con_Eet object.
 *
 * @if MOBILE @since_tizen 3.0
 * @elseif WEARABLE @since_tizen 3.0
 * @endif
 */
EAPI const void *ecore_con_eet_data_get(Ecore_Con_Eet *ece);

/**
 * Get the Ecore_Con_Eet object corresponding to the Ecore_Con_Reply object.
 *
 * @param reply       An Ecore_Con_Reply object.
 * @return The corresponding Ecore_Con_Eet object.
 *
 * @if MOBILE @since_tizen 3.0
 * @elseif WEARABLE @since_tizen 3.0
 * @endif
 */
EAPI Ecore_Con_Eet *ecore_con_eet_reply(Ecore_Con_Reply *reply);

/**
 * Send some data using a protocol type.
 *
 * @param reply         An Ecore_Con_Reply object.
 * @param protocol_name The protocol type to use.
 * @param value         The data to send.
 *
 * @if MOBILE @since_tizen 3.0
 * @elseif WEARABLE @since_tizen 3.0
 * @endif
 */
EAPI void ecore_con_eet_send(Ecore_Con_Reply *reply, const char *protocol_name, void *value);

/**
 * Send some raw data using a protocol type.
 *
 * @param reply         An Ecore_Con_Reply object.
 * @param protocol_name The protocol type to use.
 * @param section       The section to add to the protocol.
 * @param value         The data to send.
 * @param length        The data length.
 *
 * @if MOBILE @since_tizen 3.0
 * @elseif WEARABLE @since_tizen 3.0
 * @endif
 */
EAPI void ecore_con_eet_raw_send(Ecore_Con_Reply *reply, const char *protocol_name, const char *section, void *value, unsigned int length);

/**
 * @}
 */

#endif
