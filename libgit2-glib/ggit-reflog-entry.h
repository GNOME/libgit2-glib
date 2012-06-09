/*
 * ggit-reflog-entry.h
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

#ifndef __GGIT_REFLOG_ENTRY_H__
#define __GGIT_REFLOG_ENTRY_H__

#include <glib-object.h>
#include <git2/reflog.h>
#include "ggit-types.h"

G_BEGIN_DECLS

#define GGIT_TYPE_REFLOG_ENTRY       (ggit_reflog_entry_get_type ())
#define GGIT_REFLOG_ENTRY(obj)       ((GgitReflogEntry *)obj)

GType             ggit_reflog_entry_get_type       (void) G_GNUC_CONST;

GgitReflogEntry  *_ggit_reflog_entry_new           (const git_reflog_entry *reflog_entry);

GgitReflogEntry  *ggit_reflog_entry_ref            (GgitReflogEntry        *reflog_entry);
void              ggit_reflog_entry_unref          (GgitReflogEntry        *reflog_entry);

GgitOId          *ggit_reflog_entry_get_old_oid    (GgitReflogEntry        *reflog_entry);
GgitOId          *ggit_reflog_entry_get_new_oid    (GgitReflogEntry        *reflog_entry);
GgitSignature    *ggit_reflog_entry_get_committer  (GgitReflogEntry        *reflog_entry);
const gchar      *ggit_reflog_entry_get_message    (GgitReflogEntry        *reflog_entry);


G_END_DECLS

#endif /* __GGIT_REFLOG_ENTRY_H__ */

/* ex:set ts=8 noet: */
