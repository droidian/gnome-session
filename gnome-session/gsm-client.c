/* -*- Mode: C; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 8 -*-
 *
 * Copyright (C) 2007 Novell, Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "gsm-marshal.h"
#include "gsm-client.h"

static guint32 client_serial = 1;

#define GSM_CLIENT_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), GSM_TYPE_CLIENT, GsmClientPrivate))

struct GsmClientPrivate
{
        char *id;
};

enum {
        SAVED_STATE,
        REQUEST_PHASE2,
        REQUEST_INTERACTION,
        INTERACTION_DONE,
        SAVE_YOURSELF_DONE,
        DISCONNECTED,
        REGISTER_CLIENT,
        REQUEST_LOGOUT,
        LAST_SIGNAL
};

static guint signals[LAST_SIGNAL] = { 0 };

G_DEFINE_TYPE (GsmClient, gsm_client, G_TYPE_OBJECT)

static guint32
get_next_client_serial (void)
{
        guint32 serial;

        serial = client_serial++;

        if ((gint32)client_serial < 0) {
                client_serial = 1;
        }

        return serial;
}

static GObject *
gsm_client_constructor (GType                  type,
                        guint                  n_construct_properties,
                        GObjectConstructParam *construct_properties)
{
        GsmClient      *client;

        client = GSM_CLIENT (G_OBJECT_CLASS (gsm_client_parent_class)->constructor (type,
                                                                                    n_construct_properties,
                                                                                    construct_properties));

        g_free (client->priv->id);
        client->priv->id = g_strdup_printf ("/org/gnome/SessionManager/Client%u", get_next_client_serial ());

#if 0
        res = register_client (client);
        if (! res) {
                g_warning ("Unable to register client with session bus");
        }
#endif
        return G_OBJECT (client);
}

static void
gsm_client_init (GsmClient *client)
{
        client->priv = GSM_CLIENT_GET_PRIVATE (client);
}

static void
gsm_client_finalize (GObject *object)
{
        GsmClient *client;

        g_return_if_fail (object != NULL);
        g_return_if_fail (GSM_IS_CLIENT (object));

        client = GSM_CLIENT (object);

        g_return_if_fail (client->priv != NULL);

        g_free (client->priv->id);
}

static void
gsm_client_class_init (GsmClientClass *klass)
{
        GObjectClass *object_class = G_OBJECT_CLASS (klass);

        object_class->constructor = gsm_client_constructor;
        object_class->finalize = gsm_client_finalize;

        signals[SAVED_STATE] =
                g_signal_new ("saved_state",
                              G_OBJECT_CLASS_TYPE (object_class),
                              G_SIGNAL_RUN_LAST,
                              G_STRUCT_OFFSET (GsmClientClass, saved_state),
                              NULL, NULL,
                              g_cclosure_marshal_VOID__VOID,
                              G_TYPE_NONE,
                              0);

        signals[REQUEST_PHASE2] =
                g_signal_new ("request_phase2",
                              G_OBJECT_CLASS_TYPE (object_class),
                              G_SIGNAL_RUN_LAST,
                              G_STRUCT_OFFSET (GsmClientClass, request_phase2),
                              NULL, NULL,
                              g_cclosure_marshal_VOID__VOID,
                              G_TYPE_NONE,
                              0);

        signals[REQUEST_INTERACTION] =
                g_signal_new ("request_interaction",
                              G_OBJECT_CLASS_TYPE (object_class),
                              G_SIGNAL_RUN_LAST,
                              G_STRUCT_OFFSET (GsmClientClass, request_interaction),
                              NULL, NULL,
                              g_cclosure_marshal_VOID__VOID,
                              G_TYPE_NONE,
                              0);

        signals[INTERACTION_DONE] =
                g_signal_new ("interaction_done",
                              G_OBJECT_CLASS_TYPE (object_class),
                              G_SIGNAL_RUN_LAST,
                              G_STRUCT_OFFSET (GsmClientClass, interaction_done),
                              NULL, NULL,
                              g_cclosure_marshal_VOID__BOOLEAN,
                              G_TYPE_NONE,
                              1, G_TYPE_BOOLEAN);

        signals[SAVE_YOURSELF_DONE] =
                g_signal_new ("save_yourself_done",
                              G_OBJECT_CLASS_TYPE (object_class),
                              G_SIGNAL_RUN_LAST,
                              G_STRUCT_OFFSET (GsmClientClass, save_yourself_done),
                              NULL, NULL,
                              g_cclosure_marshal_VOID__VOID,
                              G_TYPE_NONE,
                              0);

        signals[DISCONNECTED] =
                g_signal_new ("disconnected",
                              G_OBJECT_CLASS_TYPE (object_class),
                              G_SIGNAL_RUN_LAST,
                              G_STRUCT_OFFSET (GsmClientClass, disconnected),
                              NULL, NULL,
                              g_cclosure_marshal_VOID__VOID,
                              G_TYPE_NONE,
                              0);
        signals[REGISTER_CLIENT] =
                g_signal_new ("register-client",
                              G_OBJECT_CLASS_TYPE (object_class),
                              G_SIGNAL_RUN_LAST,
                              G_STRUCT_OFFSET (GsmClientClass, register_client),
                              NULL, NULL,
                              gsm_marshal_STRING__STRING,
                              G_TYPE_STRING,
                              1, G_TYPE_STRING);
        signals[REQUEST_LOGOUT] =
                g_signal_new ("request-logout",
                              G_OBJECT_CLASS_TYPE (object_class),
                              G_SIGNAL_RUN_LAST,
                              G_STRUCT_OFFSET (GsmClientClass, request_logout),
                              NULL, NULL,
                              g_cclosure_marshal_VOID__BOOLEAN,
                              G_TYPE_NONE,
                              0);

        g_type_class_add_private (klass, sizeof (GsmClientPrivate));
}

const char *
gsm_client_get_id (GsmClient *client)
{
        g_return_val_if_fail (GSM_IS_CLIENT (client), NULL);

        return client->priv->id;
}


const char *
gsm_client_get_client_id (GsmClient *client)
{
        g_return_val_if_fail (GSM_IS_CLIENT (client), NULL);

        return GSM_CLIENT_GET_CLASS (client)->get_client_id (client);
}

pid_t
gsm_client_get_pid (GsmClient *client)
{
        g_return_val_if_fail (GSM_IS_CLIENT (client), -1);

        return GSM_CLIENT_GET_CLASS (client)->get_pid (client);
}

char *
gsm_client_get_desktop_file (GsmClient *client)
{
        g_return_val_if_fail (GSM_IS_CLIENT (client), NULL);

        return GSM_CLIENT_GET_CLASS (client)->get_desktop_file (client);
}

char *
gsm_client_get_restart_command (GsmClient *client)
{
        g_return_val_if_fail (GSM_IS_CLIENT (client), NULL);

        return GSM_CLIENT_GET_CLASS (client)->get_restart_command (client);
}

char *
gsm_client_get_discard_command (GsmClient *client)
{
        g_return_val_if_fail (GSM_IS_CLIENT (client), NULL);

        return GSM_CLIENT_GET_CLASS (client)->get_discard_command (client);
}

gboolean
gsm_client_get_autorestart (GsmClient *client)
{
        g_return_val_if_fail (GSM_IS_CLIENT (client), FALSE);

        return GSM_CLIENT_GET_CLASS (client)->get_autorestart (client);
}

void
gsm_client_save_state (GsmClient *client)
{
        g_return_if_fail (GSM_IS_CLIENT (client));
}

void
gsm_client_restart (GsmClient *client, GError **error)
{
        g_return_if_fail (GSM_IS_CLIENT (client));

        GSM_CLIENT_GET_CLASS (client)->restart (client, error);
}

void
gsm_client_save_yourself (GsmClient *client,
                          gboolean   save_state)
{
        g_return_if_fail (GSM_IS_CLIENT (client));

        GSM_CLIENT_GET_CLASS (client)->save_yourself (client, save_state);
}

void
gsm_client_save_yourself_phase2 (GsmClient *client)
{
        g_return_if_fail (GSM_IS_CLIENT (client));

        GSM_CLIENT_GET_CLASS (client)->save_yourself_phase2 (client);
}

void
gsm_client_interact (GsmClient *client)
{
        g_return_if_fail (GSM_IS_CLIENT (client));

        GSM_CLIENT_GET_CLASS (client)->interact (client);
}

void
gsm_client_shutdown_cancelled (GsmClient *client)
{
        g_return_if_fail (GSM_IS_CLIENT (client));

        GSM_CLIENT_GET_CLASS (client)->shutdown_cancelled (client);
}

void
gsm_client_die (GsmClient *client)
{
        g_return_if_fail (GSM_IS_CLIENT (client));

        GSM_CLIENT_GET_CLASS (client)->die (client);
}

void
gsm_client_saved_state (GsmClient *client)
{
        g_signal_emit (client, signals[SAVED_STATE], 0);
}

void
gsm_client_request_phase2 (GsmClient *client)
{
        g_signal_emit (client, signals[REQUEST_PHASE2], 0);
}

void
gsm_client_request_interaction (GsmClient *client)
{
        g_signal_emit (client, signals[REQUEST_INTERACTION], 0);
}

void
gsm_client_interaction_done (GsmClient *client, gboolean cancel_shutdown)
{
        g_signal_emit (client, signals[INTERACTION_DONE], 0, cancel_shutdown);
}

void
gsm_client_save_yourself_done (GsmClient *client)
{
        g_signal_emit (client, signals[SAVE_YOURSELF_DONE], 0);
}

void
gsm_client_disconnected (GsmClient *client)
{
        g_signal_emit (client, signals[DISCONNECTED], 0);
}

void
gsm_client_register_client (GsmClient  *client,
                            const char *previous_id,
                            char      **id)
{
        g_signal_emit (client, signals[REGISTER_CLIENT], 0, previous_id, id);
}

void
gsm_client_request_logout (GsmClient  *client,
                           gboolean    prompt)
{
        g_signal_emit (client, signals[REQUEST_LOGOUT], 0, prompt);
}