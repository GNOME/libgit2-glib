/*
 * ggit-remote.h
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2012 - Ignacio Casal Quinteiro
 *
 * libgit2-glib is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * libgit2-glib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with libgit2-glib. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __GGIT_REMOTE_H__
#define __GGIT_REMOTE_H__

#include <glib-object.h>
#include <git2.h>

#include "ggit-types.h"

G_BEGIN_DECLS

#define GGIT_TYPE_REMOTE        (ggit_remote_get_type ())
#define GGIT_REMOTE(obj)        ((GgitRemote *)obj)

#define GGIT_TYPE_REMOTE_HEAD   (ggit_remote_head_get_type ())
#define GGIT_REMOTE_HEAD(obj)        ((GgitRemoteHead *)obj)

GType              ggit_remote_get_type                 (void) G_GNUC_CONST;
GType              ggit_remote_head_get_type            (void) G_GNUC_CONST;

GgitRemoteHead    *ggit_remote_head_ref                 (GgitRemoteHead *remote_head);
void               ggit_remote_head_unref               (GgitRemoteHead *remote_head);

GgitOId           *ggit_remote_head_get_oid             (GgitRemoteHead *remote_head);
GgitOId           *ggit_remote_head_get_local_oid       (GgitRemoteHead *remote_head);
gboolean           ggit_remote_head_is_local            (GgitRemoteHead *remote_head);
const gchar       *ggit_remote_head_get_name            (GgitRemoteHead *remote_head);

GgitRemote       *_ggit_remote_wrap                     (const git_remote *remote);

GgitRemote        *ggit_remote_new                      (GgitRepository   *repository,
                                                         const gchar      *url,
                                                         const gchar      *fetch_spec,
                                                         GError          **error);

GgitRemote        *ggit_remote_ref                      (GgitRemote       *remote);
void               ggit_remote_unref                    (GgitRemote       *remote);

void               ggit_remote_save                     (GgitRemote       *remote,
                                                         GError          **error);

const gchar       *ggit_remote_get_name                 (GgitRemote       *remote);

const gchar       *ggit_remote_get_url                  (GgitRemote       *remote);

void               ggit_remote_connect                  (GgitRemote       *remote,
                                                         gboolean          direction,
                                                         GError          **error);

gboolean           ggit_remote_get_connected            (GgitRemote       *remote);

void               ggit_remote_disconnect               (GgitRemote       *remote);

void               ggit_remote_add_fetch_spec           (GgitRemote       *remote,
                                                         const gchar      *fetch_spec,
                                                         GError          **error);

void               ggit_remote_add_push_spec            (GgitRemote       *remote,
                                                         const gchar      *push_spec,
                                                         GError          **error);

GgitRemoteHead   **ggit_remote_list                     (GgitRemote       *remote,
                                                         GError          **error);

gboolean           ggit_remote_is_valid_url             (const gchar      *url);
gboolean           ggit_remote_is_supported_url         (const gchar      *url);

G_END_DECLS

#endif /* __GGIT_REMOTE_H__ */

/* ex:set ts=8 noet: */
