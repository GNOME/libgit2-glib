/*
 * git-glib-oid.h
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


#ifndef __GIT_GLIB_OID_H__
#define __GIT_GLIB_OID_H__

#include <glib-object.h>
#include <git2/oid.h>

G_BEGIN_DECLS

#define GIT_TYPE_GLIB_OID       (git_glib_oid_get_type ())
#define GIT_GLIB_OID(obj)       ((GitGlibOId *)obj)
#define GIT_GLIB_OID_CONST(obj) ((GitGlibOId const *)obj)

typedef struct _GitGlibOId GitGlibOId;

GType                  git_glib_oid_get_type            (void) G_GNUC_CONST;

GitGlibOId           *_git_glib_oid_new                 (git_oid *oid);

GitGlibOId            *git_glib_oid_copy                (GitGlibOId *oid);
void                   git_glib_oid_free                (GitGlibOId *oid);

GitGlibOId            *git_glib_oid_fromstr             (const gchar *str);

GitGlibOId            *git_glib_oid_fromraw             (const guchar *raw);

gint                   git_glib_oid_cmp                 (GitGlibOId *a,
                                                         GitGlibOId *b);

gchar                 *git_glib_oid_to_string           (GitGlibOId *oid);

G_END_DECLS

#endif /* __GIT_GLIB_OID_H__ */
