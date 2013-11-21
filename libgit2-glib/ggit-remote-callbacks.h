/*
 * ggit-remote-callbacks.h
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2013 - Ignacio Casal Quinteiro
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


#ifndef __GGIT_REMOTE_CBS_H__
#define __GGIT_REMOTE_CBS_H__

#include <glib-object.h>
#include <git2.h>

#include "ggit-types.h"

G_BEGIN_DECLS

#define GGIT_TYPE_REMOTE_CBS       (ggit_remote_cbs_get_type ())
#define GGIT_REMOTE_CBS(obj)       ((GgitRemoteCbs *)obj)

GType                      ggit_remote_cbs_get_type            (void) G_GNUC_CONST;

const git_remote_callbacks *_ggit_remote_cbs_get_remote_callbacks  (GgitRemoteCbs        *cbs);

GgitRemoteCbs             *ggit_remote_cbs_ref                 (GgitRemoteCbs        *cbs);
void                       ggit_remote_cbs_unref               (GgitRemoteCbs        *cbs);

GgitRemoteCbs             *ggit_remote_cbs_new                 (void);

G_END_DECLS

#endif /* __GGIT_REMOTE_CBS_H__ */

/* ex:set ts=8 noet: */
