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

#ifndef __GSM_AUTOSTART_APP_H__
#define __GSM_AUTOSTART_APP_H__

#include "gsm-app.h"

#include <X11/SM/SMlib.h>

G_BEGIN_DECLS

#define GSM_TYPE_AUTOSTART_APP            (gsm_autostart_app_get_type ())
#define GSM_AUTOSTART_APP(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GSM_TYPE_AUTOSTART_APP, GsmAutostartApp))
#define GSM_AUTOSTART_APP_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GSM_TYPE_AUTOSTART_APP, GsmAutostartAppClass))
#define GSM_IS_AUTOSTART_APP(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GSM_TYPE_AUTOSTART_APP))
#define GSM_IS_AUTOSTART_APP_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GSM_TYPE_AUTOSTART_APP))
#define GSM_AUTOSTART_APP_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GSM_TYPE_AUTOSTART_APP, GsmAutostartAppClass))

typedef struct _GsmAutostartApp        GsmAutostartApp;
typedef struct _GsmAutostartAppClass   GsmAutostartAppClass;
typedef struct _GsmAutostartAppPrivate GsmAutostartAppPrivate;

struct _GsmAutostartApp
{
        GsmApp parent;

        GsmAutostartAppPrivate *priv;
};

struct _GsmAutostartAppClass
{
        GsmAppClass parent_class;

        /* signals */
        void     (*condition_changed)  (GsmApp  *app,
                                        gboolean condition);
};

GType   gsm_autostart_app_get_type           (void) G_GNUC_CONST;

GsmApp *gsm_autostart_app_new                (const char *desktop_file,
                                              const char *client_id);

G_END_DECLS

#endif /* __GSM_AUTOSTART_APP_H__ */