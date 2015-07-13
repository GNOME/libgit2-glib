/*
 * ggit-submodule-options.h
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2015 - Ignacio Casal Quinteiro
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

#ifndef __GGIT_SUBMODULE_UPDATE_OPTIONS_H__
#define __GGIT_SUBMODULE_UPDATE_OPTIONS_H__

#include <glib-object.h>
#include <git2.h>

#include "ggit-types.h"

G_BEGIN_DECLS

#define GGIT_TYPE_SUBMODULE_UPDATE_OPTIONS		(ggit_submodule_update_options_get_type ())
#define GGIT_SUBMODULE_UPDATE_OPTIONS(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), GGIT_TYPE_SUBMODULE_UPDATE_OPTIONS, GgitSubmoduleUpdateOptions))
#define GGIT_SUBMODULE_UPDATE_OPTIONS_CONST(obj)	(G_TYPE_CHECK_INSTANCE_CAST ((obj), GGIT_TYPE_SUBMODULE_UPDATE_OPTIONS, GgitSubmoduleUpdateOptions const))
#define GGIT_SUBMODULE_UPDATE_OPTIONS_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST ((klass), GGIT_TYPE_SUBMODULE_UPDATE_OPTIONS, GgitSubmoduleUpdateOptionsClass))
#define GGIT_IS_SUBMODULE_UPDATE_OPTIONS(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), GGIT_TYPE_SUBMODULE_UPDATE_OPTIONS))
#define GGIT_IS_SUBMODULE_UPDATE_OPTIONS_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), GGIT_TYPE_SUBMODULE_UPDATE_OPTIONS))
#define GGIT_SUBMODULE_UPDATE_OPTIONS_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS ((obj), GGIT_TYPE_SUBMODULE_UPDATE_OPTIONS, GgitSubmoduleUpdateOptionsClass))

typedef struct _GgitSubmoduleUpdateOptionsClass		GgitSubmoduleUpdateOptionsClass;
typedef struct _GgitSubmoduleUpdateOptionsPrivate	GgitSubmoduleUpdateOptionsPrivate;

struct _GgitSubmoduleUpdateOptions
{
	GObject parent;

	GgitSubmoduleUpdateOptionsPrivate *priv;
};

struct _GgitSubmoduleUpdateOptionsClass
{
	GObjectClass parent_class;
};

GType                  ggit_submodule_update_options_get_type              (void) G_GNUC_CONST;

const git_submodule_update_options *
                      _ggit_submodule_update_options_get_submodule_update_options (GgitSubmoduleUpdateOptions  *options);

GgitSubmoduleUpdateOptions  *ggit_submodule_update_options_new             (void);

GgitCheckoutOptions   *ggit_submodule_update_options_get_checkout_options  (GgitSubmoduleUpdateOptions *options);
void                   ggit_submodule_update_options_set_checkout_options  (GgitSubmoduleUpdateOptions *options,
                                                                            GgitCheckoutOptions        *checkout_options);

GgitFetchOptions      *ggit_submodule_update_options_get_fetch_options     (GgitSubmoduleUpdateOptions *options);
void                   ggit_submodule_update_options_set_fetch_options     (GgitSubmoduleUpdateOptions *options,
                                                                            GgitFetchOptions           *fetch_options);

GgitCheckoutStrategy   ggit_submodule_update_options_get_clone_checkout_strategy (GgitSubmoduleUpdateOptions *options);
void                   ggit_submodule_update_options_set_clone_checkout_strategy (GgitSubmoduleUpdateOptions *options,
                                                                                  GgitCheckoutStrategy        checkout_strategy);

G_END_DECLS

#endif /* __GGIT_SUBMODULE_UPDATE_OPTIONS_H__ */

/* ex:set ts=8 noet: */
