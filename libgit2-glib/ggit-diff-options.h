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
#include <git2/diff.h>

#include "ggit-types.h"

G_BEGIN_DECLS

#define GGIT_TYPE_DIFF_OPTIONS       (ggit_diff_options_get_type ())
#define GGIT_DIFF_OPTIONS(obj)       ((GgitDiffOptions *)obj)
#define GGIT_DIFF_OPTIONS_CONST(obj) ((GgitDiffOptions const *)obj)

GType                   ggit_diff_options_get_type          (void) G_GNUC_CONST;

const git_diff_options *_ggit_diff_options_get_diff_options (GgitDiffOptions        *diff_options);

GgitDiffOptions        *ggit_diff_options_copy              (GgitDiffOptions        *diff_options);
void                    ggit_diff_options_free              (GgitDiffOptions        *diff_options);

GgitDiffOptions        *ggit_diff_options_new               (GgitDiffFlags           flags,
                                                             gint                    n_context_lines,
                                                             gint                    n_interhunk_lines,
                                                             const gchar            *old_prefix,
                                                             const gchar            *new_prefix,
                                                             const gchar           **pathspec);

G_END_DECLS

#endif /* __GGIT_DIFF_OPTIONS_H__ */

/* ex:set ts=8 noet: */
