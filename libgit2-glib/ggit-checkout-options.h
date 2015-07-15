/*
 * ggit-checkout-options.h
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

#ifndef __GGIT_CHECKOUT_OPTIONS_H__
#define __GGIT_CHECKOUT_OPTIONS_H__

#include <glib-object.h>
#include <git2.h>

#include "ggit-types.h"
#include "ggit-tree.h"

G_BEGIN_DECLS

#define GGIT_TYPE_CHECKOUT_OPTIONS (ggit_checkout_options_get_type ())
G_DECLARE_DERIVABLE_TYPE (GgitCheckoutOptions, ggit_checkout_options, GGIT, CHECKOUT_OPTIONS, GObject)

struct _GgitCheckoutOptionsClass
{
	GObjectClass parent_class;

	gint (*notify)   (GgitCheckoutOptions     *options,
	                  GgitCheckoutNotifyFlags  why,
	                  const gchar             *path,
	                  GgitDiffFile            *baseline,
	                  GgitDiffFile            *target,
	                  GgitDiffFile            *workdir);

	void (*progress) (GgitCheckoutOptions     *options,
	                  const gchar             *path,
	                  gsize                    completed_steps,
	                  gsize                    total_steps);
};

const git_checkout_options *
                      _ggit_checkout_options_get_checkout_options (
                                                          GgitCheckoutOptions *options);

GgitCheckoutOptions  *ggit_checkout_options_new          (void);

GgitCheckoutStrategy  ggit_checkout_options_get_strategy  (GgitCheckoutOptions  *options);
void                  ggit_checkout_options_set_strategy  (GgitCheckoutOptions  *options,
                                                           GgitCheckoutStrategy  strategy);

gboolean              ggit_checkout_options_get_disable_filters (
                                                           GgitCheckoutOptions  *options);

void                  ggit_checkout_options_set_disable_filters (
                                                           GgitCheckoutOptions  *options,
                                                           gboolean              disable);

guint                 ggit_checkout_options_get_dir_mode  (GgitCheckoutOptions  *options);
void                  ggit_checkout_options_set_dir_mode  (GgitCheckoutOptions  *options,
                                                           guint                 dir_mode);

guint                 ggit_checkout_options_get_file_mode (GgitCheckoutOptions  *options);
void                  ggit_checkout_options_set_file_mode (GgitCheckoutOptions  *options,
                                                           guint                 file_mode);

gint                  ggit_checkout_options_get_file_open_flags (
                                                           GgitCheckoutOptions  *options);
void                  ggit_checkout_options_set_file_open_flags (
                                                           GgitCheckoutOptions  *options,
                                                           gint                 flags);

GgitCheckoutNotifyFlags
                      ggit_checkout_options_get_notify_flags (
                                                           GgitCheckoutOptions  *options);
void                  ggit_checkout_options_set_notify_flags (
                                                           GgitCheckoutOptions  *options,
                                                           GgitCheckoutNotifyFlags flags);

const gchar * const  *ggit_checkout_options_get_paths     (GgitCheckoutOptions  *options);
void                  ggit_checkout_options_set_paths     (GgitCheckoutOptions  *options,
                                                           const gchar * const  *paths);

GgitTree             *ggit_checkout_options_get_baseline  (GgitCheckoutOptions  *options);
void                  ggit_checkout_options_set_baseline  (GgitCheckoutOptions  *options,
                                                           GgitTree             *tree);

const gchar          *ggit_checkout_options_get_target_directory (
                                                           GgitCheckoutOptions  *options);
void                  ggit_checkout_options_set_target_directory (
                                                           GgitCheckoutOptions  *options,
                                                           const gchar          *directory);

const gchar          *ggit_checkout_options_get_ancestor_label (
                                                           GgitCheckoutOptions  *options);
void                  ggit_checkout_options_set_ancestor_label (
                                                           GgitCheckoutOptions  *options,
                                                           const gchar          *label);

const gchar          *ggit_checkout_options_get_our_label (GgitCheckoutOptions  *options);
void                  ggit_checkout_options_set_our_label (GgitCheckoutOptions  *options,
                                                           const gchar          *label);

const gchar          *ggit_checkout_options_get_their_label (
                                                           GgitCheckoutOptions  *options);
void                  ggit_checkout_options_set_their_label (
                                                           GgitCheckoutOptions  *options,
                                                           const gchar          *label);

G_END_DECLS

#endif /* __GGIT_CHECKOUT_OPTIONS_H__ */

/* ex:set ts=8 noet: */
