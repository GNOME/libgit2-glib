/*
 * ggit-cherry-pick-options.h
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2014 - Jesse van den Kieboom
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

#ifndef __GGIT_CHERRY_PICK_OPTIONS_H__
#define __GGIT_CHERRY_PICK_OPTIONS_H__

#include <glib-object.h>
#include <git2.h>

#include "ggit-types.h"
#include "ggit-checkout-options.h"

G_BEGIN_DECLS

#define GGIT_TYPE_CHERRY_PICK_OPTIONS (ggit_cherry_pick_options_get_type ())
G_DECLARE_DERIVABLE_TYPE (GgitCherryPickOptions, ggit_cherry_pick_options, GGIT, CHERRY_PICK_OPTIONS, GObject)

struct _GgitCherryPickOptionsClass
{
	GObjectClass parent_class;
};

const git_cherrypick_options *
                      _ggit_cherry_pick_options_get_cherry_pick_options (
                                                              GgitCherryPickOptions  *options);

GgitCherryPickOptions *ggit_cherry_pick_options_new          (void);

guint                  ggit_cherry_pick_options_get_mainline (GgitCherryPickOptions *options);
void                   ggit_cherry_pick_options_set_mainline (GgitCherryPickOptions *options,
                                                              guint                  mainline);

GgitCheckoutOptions   *ggit_cherry_pick_options_get_checkout_options (GgitCherryPickOptions *options);
void                   ggit_cherry_pick_options_set_checkout_options (GgitCherryPickOptions *options,
                                                                      GgitCheckoutOptions   *checkout_options);

GgitMergeOptions      *ggit_cherry_pick_options_get_merge_options (GgitCherryPickOptions *options);
void                   ggit_cherry_pick_options_set_merge_options (GgitCherryPickOptions *options,
                                                                   GgitMergeOptions      *merge_options);

G_END_DECLS

#endif /* __GGIT_CHERRY_PICK_OPTIONS_H__ */

/* ex:set ts=8 noet: */
