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

#ifndef __GSM_CLIENT_XSMP_H__
#define __GSM_CLIENT_XSMP_H__

#include "gsm-client.h"

#include <X11/SM/SMlib.h>

G_BEGIN_DECLS

#define GSM_TYPE_XSMP_CLIENT            (gsm_xsmp_client_get_type ())
#define GSM_XSMP_CLIENT(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GSM_TYPE_XSMP_CLIENT, GsmXSMPClient))
#define GSM_XSMP_CLIENT_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GSM_TYPE_XSMP_CLIENT, GsmXSMPClientClass))
#define GSM_IS_XSMP_CLIENT(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GSM_TYPE_XSMP_CLIENT))
#define GSM_IS_XSMP_CLIENT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GSM_TYPE_XSMP_CLIENT))
#define GSM_XSMP_CLIENT_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GSM_TYPE_XSMP_CLIENT, GsmXSMPClientClass))

typedef struct _GsmXSMPClient        GsmXSMPClient;
typedef struct _GsmXSMPClientClass   GsmXSMPClientClass;

struct _GsmXSMPClient
{
        GsmClient  parent;

        SmsConn    conn;
        IceConn    ice_conn;

        guint      watch_id;
        guint      protocol_timeout;

        int        current_save_yourself;
        int        next_save_yourself;
        char      *id;
        char      *description;
        GPtrArray *props;
};

struct _GsmXSMPClientClass
{
        GsmClientClass parent_class;

};

GType          gsm_xsmp_client_get_type           (void) G_GNUC_CONST;

GsmXSMPClient *gsm_xsmp_client_new                (IceConn ice_conn);

void           gsm_xsmp_client_connect            (GsmXSMPClient *client,
                                                   SmsConn        conn,
                                                   unsigned long *mask_ret,
                                                   SmsCallbacks  *callbacks_ret);

G_END_DECLS

#endif /* __GSM_XSMP_CLIENT_H__ */