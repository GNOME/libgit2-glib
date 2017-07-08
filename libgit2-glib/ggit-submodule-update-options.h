/*
 * ggit-submodule-update-options.h
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
#include "ggit-checkout-options.h"

G_BEGIN_DECLS

#define GGIT_TYPE_SUBMODULE_UPDATE_OPTIONS (ggit_submodule_update_options_get_type ())
G_DECLARE_DERIVABLE_TYPE (GgitSubmoduleUpdateOptions, ggit_submodule_update_options, GGIT, SUBMODULE_UPDATE_OPTIONS, GObject)

struct _GgitSubmoduleUpdateOptionsClass
{
	GObjectClass parent_class;
};

const git_submodule_update_options *
                      _ggit_submodule_update_options_get_submodule_update_options (GgitSubmoduleUpdateOptions  *options);

GgitSubmoduleUpdateOptions  *ggit_submodule_update_options_new             (void);

GgitCheckoutOptions   *ggit_submodule_update_options_get_checkout_options  (GgitSubmoduleUpdateOptions *options);
void                   ggit_submodule_update_options_set_checkout_options  (GgitSubmoduleUpdateOptions *options,
                                                                            GgitCheckoutOptions        *checkout_options);

GgitFetchOptions      *ggit_submodule_update_options_get_fetch_options     (GgitSubmoduleUpdateOptions *options);
void                   ggit_submodule_update_options_set_fetch_options     (GgitSubmoduleUpdateOptions *options,
                                                                            GgitFetchOptions           *fetch_options);

G_END_DECLS

#endif /* __GGIT_SUBMODULE_UPDATE_OPTIONS_H__ */

/* ex:set ts=8 noet: */
