/*
 * ggit-submodule.h
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

#ifndef __GGIT_SUBMODULE_H__
#define __GGIT_SUBMODULE_H__

#include <glib-object.h>
#include <git2/submodule.h>

#include "ggit-types.h"

G_BEGIN_DECLS

#define GGIT_TYPE_SUBMODULE             (ggit_submodule_get_type ())
#define GGIT_SUBMODULE(obj)             ((GgitSubmodule *)obj)
#define GGIT_SUBMODULE_CONST(obj)       ((GgitSubmodule const *)obj)

GType                   ggit_submodule_get_type              (void) G_GNUC_CONST;

GgitSubmodule         *_ggit_submodule_new                   (const git_submodule *submodule);

GgitSubmodule          *ggit_submodule_ref                   (GgitSubmodule       *submodule);
void                    ggit_submodule_unref                 (GgitSubmodule       *submodule);

const gchar            *ggit_submodule_get_name              (GgitSubmodule       *submodule);

const gchar            *ggit_submodule_get_path              (GgitSubmodule       *submodule);

const gchar            *ggit_submodule_get_url               (GgitSubmodule       *submodule);

GgitOId                *ggit_submodule_get_oid               (GgitSubmodule       *submodule);

GgitSubmoduleUpdate     ggit_submodule_get_update            (GgitSubmodule       *submodule);

GgitSubmoduleIgnore     ggit_submodule_get_ignore            (GgitSubmodule       *submodule);

gboolean                ggit_submodule_get_fetch_recurse     (GgitSubmodule       *submodule);

G_END_DECLS

#endif /* __GGIT_SUBMODULE_H__ */

/* ex:set ts=8 noet: */
