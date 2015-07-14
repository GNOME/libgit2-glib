/*
 * ggit-revert-options.h
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


#ifndef __GGIT_REVERT_OPTIONS_H__
#define __GGIT_REVERT_OPTIONS_H__

#include <glib-object.h>
#include <git2.h>

#include "ggit-types.h"
#include "ggit-checkout-options.h"

G_BEGIN_DECLS

#define GGIT_TYPE_REVERT_OPTIONS       (ggit_revert_options_get_type ())
#define GGIT_REVERT_OPTIONS(obj)       ((GgitRevertOptions *)obj)

GType                  ggit_revert_options_get_type                (void) G_GNUC_CONST;

const git_revert_options *
                      _ggit_revert_options_get_revert_options  (
                                                   GgitRevertOptions   *revert_options);

GgitRevertOptions      *ggit_revert_options_copy  (GgitRevertOptions   *revert_options);
void                   ggit_revert_options_free   (GgitRevertOptions   *revert_options);

GgitRevertOptions      *ggit_revert_options_new   (guint                mainline,
                                                   GgitMergeOptions    *merge_options,
                                                   GgitCheckoutOptions *checkout_options);

G_END_DECLS

#endif /* __GGIT_REVERT_OPTIONS_H__ */

/* ex:set ts=8 noet: */
