/*
 * ggit-diff-patch.h
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2013 - Sindhu S
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

#ifndef __GGIT_DIFF_PATCH_H__
#define __GGIT_DIFF_PATCH_H__

#include <glib-object.h>
#include <gio/gio.h>
#include <git2.h>
#include "ggit-types.h"

G_BEGIN_DECLS

#define GGIT_TYPE_DIFF_PATCH       (ggit_diff_patch_get_type ())
#define GGIT_DIFF_PATCH(obj)       ((GgitDiffPatch *)obj)

GType           ggit_diff_patch_get_type        (void) G_GNUC_CONST;

GgitDiffPatch   *_ggit_diff_patch_wrap          (git_diff_patch *diff_patch);

GgitDiffPatch   *ggit_diff_patch_ref            (GgitDiffPatch    *diff_patch);

void             ggit_diff_patch_unref          (GgitDiffPatch    *diff_patch);

gchar           *ggit_diff_patch_to_string      (GgitDiffPatch    *diff_patch,
                                                 GError          **error);

gboolean         ggit_diff_patch_to_stream      (GgitDiffPatch    *diff_patch,
                                                 GOutputStream    *stream,
                                                 GError          **error);

gboolean         ggit_diff_patch_get_line_stats (GgitDiffPatch    *diff_patch,
                                                 gsize            *total_context,
                                                 gsize            *total_additions,
                                                 gsize            *total_deletions,
                                                 GError          **error);

gsize            ggit_diff_patch_get_num_hunks  (GgitDiffPatch    *diff_patch);

gint             ggit_diff_patch_get_num_lines_in_hunk (
                                                 GgitDiffPatch    *diff_patch,
                                                 gsize             hunk);

G_END_DECLS

#endif /* __GGIT_DIFF_PATCH_H__ */

/* ex:set ts=8 noet: */
