/*
 * ggit-utils.h
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

#ifndef __GGIT_UTILS_H__
#define __GGIT_UTILS_H__

#include <glib-object.h>
#include <git2/strarray.h>
#include <git2/object.h>
#include <git2/types.h>

#include "ggit-object.h"

G_BEGIN_DECLS

git_otype       ggit_utils_get_otype_from_gtype       (GType gtype);
GType           ggit_utils_get_gtype_from_otype       (git_otype type);

GgitObject     *ggit_utils_create_real_object         (git_object *obj,
                                                       gboolean    owned);

gchar         **ggit_utils_get_str_array_from_git_strarray
                                                      (git_strarray *gitarray);


void            ggit_utils_get_git_strarray_from_str_array
                                                      (const gchar  **array,
                                                       git_strarray  *gitarray);

G_END_DECLS

#endif

/* ex:set ts=8 noet: */
