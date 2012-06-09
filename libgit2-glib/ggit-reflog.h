/*
 * ggit-reflog.h
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2012 - Garrett Regier
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

#ifndef __GGIT_REFLOG_H__
#define __GGIT_REFLOG_H__

#include <glib-object.h>
#include <git2/reflog.h>
#include "ggit-types.h"

G_BEGIN_DECLS

#define GGIT_TYPE_REFLOG       (ggit_reflog_get_type ())
#define GGIT_REFLOG(obj)       ((GgitReflog *)obj)

GType             ggit_reflog_get_type             (void) G_GNUC_CONST;

GgitReflog       *_ggit_reflog_new                 (git_reflog *reflog);

GgitReflog       *ggit_reflog_ref                  (GgitReflog *reflog);
void              ggit_reflog_unref                (GgitReflog *reflog);

guint             ggit_reflog_get_entry_count      (GgitReflog *reflog);
GgitReflogEntry  *ggit_reflog_get_entry_from_index (GgitReflog *reflog,
                                                    guint       idx);

G_END_DECLS

#endif /* __GGIT_REFLOG_H__ */

/* ex:set ts=8 noet: */
