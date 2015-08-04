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

#include <libgit2-glib/ggit-types.h>
#include <libgit2-glib/ggit-fetch-options.h>

G_BEGIN_DECLS

#define GGIT_TYPE_CLONE_OPTIONS (ggit_clone_options_get_type ())
G_DECLARE_DERIVABLE_TYPE (GgitCloneOptions, ggit_clone_options, GGIT, CLONE_OPTIONS, GObject)

struct _GgitCloneOptionsClass
{
	GObjectClass parent_class;

	/* virtual methods */
	GgitRepository *(*create_repository) (GgitCloneOptions  *options,
	                                      const gchar       *path,
	                                      gboolean           is_bare,
	                                      GError            **error);

	GgitRemote     *(*create_remote)     (GgitCloneOptions  *options,
	                                      GgitRepository    *repository,
	                                      const gchar       *name,
	                                      const gchar       *url,
	                                      GError            **error);

};

const git_clone_options  *_ggit_clone_options_get_native          (GgitCloneOptions        *options);

GgitCloneOptions          *ggit_clone_options_new                 (void);

gboolean                   ggit_clone_options_get_is_bare         (GgitCloneOptions        *options);

void                       ggit_clone_options_set_is_bare         (GgitCloneOptions        *options,
                                                                   gboolean                 bare);

const gchar               *ggit_clone_options_get_checkout_branch (GgitCloneOptions        *options);

void                       ggit_clone_options_set_checkout_branch (GgitCloneOptions        *options,
                                                                   const gchar             *checkout_branch);

GgitCloneLocal             ggit_clone_options_get_local           (GgitCloneOptions        *options);
void                       ggit_clone_options_set_local           (GgitCloneOptions        *options,
                                                                   GgitCloneLocal           local);

GgitFetchOptions          *ggit_clone_options_get_fetch_options   (GgitCloneOptions        *options);
void                       ggit_clone_options_set_fetch_options   (GgitCloneOptions        *options,
                                                                   GgitFetchOptions        *fetch_options);

G_END_DECLS

#endif /* __GGIT_CLONE_OPTIONS_H__ */

/* ex:set ts=8 noet: */
