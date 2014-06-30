/*
 * ggit-diff-options.h
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2012 - Garrett Regier
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

#ifndef __GGIT_DIFF_OPTIONS_H__
#define __GGIT_DIFF_OPTIONS_H__

#include <glib-object.h>
#include <git2.h>

#include "ggit-types.h"

G_BEGIN_DECLS

#define GGIT_TYPE_DIFF_OPTIONS			(ggit_diff_options_get_type ())
#define GGIT_DIFF_OPTIONS(obj)			(G_TYPE_CHECK_INSTANCE_CAST ((obj), GGIT_TYPE_DIFF_OPTIONS, GgitDiffOptions))
#define GGIT_DIFF_OPTIONS_CONST(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), GGIT_TYPE_DIFF_OPTIONS, GgitDiffOptions const))
#define GGIT_DIFF_OPTIONS_CLASS(klass)		(G_TYPE_CHECK_CLASS_CAST ((klass), GGIT_TYPE_DIFF_OPTIONS, GgitDiffOptionsClass))
#define GGIT_IS_DIFF_OPTIONS(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), GGIT_TYPE_DIFF_OPTIONS))
#define GGIT_IS_DIFF_OPTIONS_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), GGIT_TYPE_DIFF_OPTIONS))
#define GGIT_DIFF_OPTIONS_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS ((obj), GGIT_TYPE_DIFF_OPTIONS, GgitDiffOptionsClass))

typedef struct _GgitDiffOptionsClass	GgitDiffOptionsClass;
typedef struct _GgitDiffOptionsPrivate	GgitDiffOptionsPrivate;

struct _GgitDiffOptions
{
	GObject parent;

	GgitDiffOptionsPrivate *priv;
};

struct _GgitDiffOptionsClass
{
	GObjectClass parent_class;
};

const git_diff_options *
                 _ggit_diff_options_get_diff_options     (GgitDiffOptions  *options);

GType            ggit_diff_options_get_type              (void) G_GNUC_CONST;
GgitDiffOptions *ggit_diff_options_new                   (void);

GgitDiffOption   ggit_diff_options_get_flags             (GgitDiffOptions  *options);
void             ggit_diff_options_set_flags             (GgitDiffOptions  *options,
                                                          GgitDiffOption    flags);

gint             ggit_diff_options_get_n_context_lines   (GgitDiffOptions  *options);
void             ggit_diff_options_set_n_context_lines   (GgitDiffOptions  *options,
                                                          gint              n);

gint             ggit_diff_options_get_n_interhunk_lines (GgitDiffOptions  *options);
void             ggit_diff_options_set_n_interhunk_lines (GgitDiffOptions  *options,
                                                          gint              n);

const gchar     *ggit_diff_options_get_old_prefix        (GgitDiffOptions  *options);
void             ggit_diff_options_set_old_prefix        (GgitDiffOptions  *options,
                                                          const gchar      *prefix);

const gchar     *ggit_diff_options_get_new_prefix        (GgitDiffOptions  *options);
void             ggit_diff_options_set_new_prefix        (GgitDiffOptions  *options,
                                                          const gchar      *prefix);

const gchar    **ggit_diff_options_get_pathspec          (GgitDiffOptions  *options);
void             ggit_diff_options_set_pathspec          (GgitDiffOptions  *options,
                                                          const gchar     **pathspec);

G_END_DECLS

#endif /* __GGIT_DIFF_OPTIONS_H__ */

/* ex:set ts=8 noet: */
