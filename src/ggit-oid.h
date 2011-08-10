/*
 * ggit-oid.h
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2011 - Ignacio Casal Quinteiro
 *
 * libgit2-glib is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * libgit2-glib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libgit2-glib; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA  02110-1301  USA
 */


#ifndef __GGIT_OID_H__
#define __GGIT_OID_H__

#include <glib-object.h>
#include <git2/oid.h>

G_BEGIN_DECLS

#define GGIT_TYPE_OID       (ggit_oid_get_type ())
#define GGIT_OID(obj)       ((GgitOId *)obj)
#define GGIT_OID_CONST(obj) ((GgitOId const *)obj)

typedef struct _GgitOId GgitOId;

GType               ggit_oid_get_type            (void) G_GNUC_CONST;

GgitOId           *_ggit_oid_new                 (git_oid *oid);

git_oid           *_ggit_oid_get_oid             (GgitOId *oid);

GgitOId            *ggit_oid_copy                (GgitOId *oid);
void                ggit_oid_free                (GgitOId *oid);

GgitOId            *ggit_oid_fromstr             (const gchar *str);

GgitOId            *ggit_oid_fromraw             (const guchar *raw);

gint                ggit_oid_cmp                 (GgitOId *a,
                                                  GgitOId *b);

gchar              *ggit_oid_to_string           (GgitOId *oid);

G_END_DECLS

#endif /* __GGIT_OID_H__ */
