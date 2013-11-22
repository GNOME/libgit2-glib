/*
 * ggit-patch.h
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

#ifndef __GGIT_PATCH_H__
#define __GGIT_PATCH_H__

#include <glib-object.h>
#include <gio/gio.h>
#include <git2.h>
#include "ggit-types.h"

G_BEGIN_DECLS

#define GGIT_TYPE_PATCH       (ggit_patch_get_type ())
#define GGIT_PATCH(obj)       ((GgitPatch *)obj)

GType            ggit_patch_get_type        (void) G_GNUC_CONST;

GgitPatch      *_ggit_patch_wrap            (git_patch      *patch);

GgitPatch       *ggit_patch_ref             (GgitPatch      *patch);

void             ggit_patch_unref           (GgitPatch      *patch);

GgitPatch       *ggit_patch_new_from_diff   (GgitDiff       *diff,
                                             gsize           idx,
                                             GError        **error);

GgitPatch       *ggit_patch_new_from_blobs  (GgitBlob         *old_blob,
                                             const gchar      *old_as_path,
                                             GgitBlob         *new_blob,
                                             const gchar      *new_as_path,
                                             GgitDiffOptions  *diff_options,
                                             GError          **error);

gchar           *ggit_patch_to_string       (GgitPatch      *patch,
                                             GError        **error);

gboolean         ggit_patch_to_stream       (GgitPatch      *patch,
                                             GOutputStream  *stream,
                                             GError        **error);

gboolean         ggit_patch_get_line_stats  (GgitPatch      *patch,
                                             gsize          *total_context,
                                             gsize          *total_additions,
                                             gsize          *total_deletions,
                                             GError        **error);

gsize            ggit_patch_get_num_hunks   (GgitPatch      *patch);

gint             ggit_patch_get_num_lines_in_hunk (
                                             GgitPatch      *patch,
                                             gsize           hunk);

G_END_DECLS

#endif /* __GGIT_PATCH_H__ */

/* ex:set ts=8 noet: */
