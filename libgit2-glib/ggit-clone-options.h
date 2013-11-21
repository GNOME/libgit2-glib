/*
 * ggit-clone-options.h
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


#ifndef __GGIT_CLONE_OPTIONS_H__
#define __GGIT_CLONE_OPTIONS_H__

#include <glib-object.h>
#include <git2.h>

#include "ggit-types.h"

G_BEGIN_DECLS

#define GGIT_TYPE_CLONE_OPTIONS       (ggit_clone_options_get_type ())
#define GGIT_CLONE_OPTIONS(obj)       ((GgitCloneOptions *)obj)

GType                      ggit_clone_options_get_type            (void) G_GNUC_CONST;

const git_clone_options  *_ggit_clone_options_get_clone_options   (GgitCloneOptions        *clone_options);

GgitCloneOptions          *ggit_clone_options_copy                (GgitCloneOptions        *clone_options);
void                       ggit_clone_options_free                (GgitCloneOptions        *clone_options);

GgitCloneOptions          *ggit_clone_options_new                 (void);

gboolean                   ggit_clone_options_get_is_bare         (GgitCloneOptions        *options);

void                       ggit_clone_options_set_is_bare         (GgitCloneOptions        *options,
                                                                   gboolean                 bare);

const gchar               *ggit_clone_options_get_remote_name     (GgitCloneOptions        *options);

void                       ggit_clone_options_set_remote_name     (GgitCloneOptions        *options,
                                                                   const gchar             *remote_name);

const gchar               *ggit_clone_options_get_checkout_branch (GgitCloneOptions        *options);

void                       ggit_clone_options_set_checkout_branch (GgitCloneOptions        *options,
                                                                   const gchar             *checkout_branch);

G_END_DECLS

#endif /* __GGIT_CLONE_OPTIONS_H__ */

/* ex:set ts=8 noet: */
