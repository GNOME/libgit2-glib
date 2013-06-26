/*
 * ggit-oid.h
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2011 - Ignacio Casal Quinteiro
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


#ifndef __GGIT_OID_H__
#define __GGIT_OID_H__

#include <glib-object.h>
#include <git2.h>

#include "ggit-types.h"

G_BEGIN_DECLS

#define GGIT_TYPE_OID       (ggit_oid_get_type ())
#define GGIT_OID(obj)       ((GgitOId *)obj)

GType          ggit_oid_get_type        (void) G_GNUC_CONST;

GgitOId       *_ggit_oid_wrap           (const git_oid *oid);

const git_oid *_ggit_oid_get_oid        (GgitOId       *oid);

GgitOId       *ggit_oid_copy            (GgitOId       *oid);
void           ggit_oid_free            (GgitOId       *oid);

GgitOId       *ggit_oid_new_from_string (const gchar   *str);

GgitOId       *ggit_oid_new_from_raw    (const guchar  *raw);

gint           ggit_oid_compare         (GgitOId       *a,
                                         GgitOId       *b);

gchar         *ggit_oid_to_string       (GgitOId       *oid);

guint          ggit_oid_hash            (GgitOId const *oid);
gboolean       ggit_oid_equal           (GgitOId const *a,
                                         GgitOId const *b);

G_END_DECLS

#endif /* __GGIT_OID_H__ */

/* ex:set ts=8 noet: */
