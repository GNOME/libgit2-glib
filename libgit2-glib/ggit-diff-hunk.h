/*
 * ggit-diff-hunk.h
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

#ifndef __GGIT_DIFF_HUNK_H__
#define __GGIT_DIFF_HUNK_H__

#include "ggit-types.h"

G_BEGIN_DECLS

#define GGIT_TYPE_DIFF_HUNK       (ggit_diff_hunk_get_type ())
#define GGIT_DIFF_HUNK(obj)       ((GgitDiffHunk *)obj)

GType           ggit_diff_hunk_get_type           (void) G_GNUC_CONST;

GgitDiffHunk  *_ggit_diff_hunk_wrap               (const git_diff_hunk *hunk);

GgitDiffHunk   *ggit_diff_hunk_ref                (GgitDiffHunk        *hunk);
void            ggit_diff_hunk_unref              (GgitDiffHunk        *hunk);

gchar           ggit_diff_hunk_get_old_start      (GgitDiffHunk        *hunk);
gint            ggit_diff_hunk_get_old_lines      (GgitDiffHunk        *hunk);
gint            ggit_diff_hunk_get_new_start      (GgitDiffHunk        *hunk);
gint            ggit_diff_hunk_get_new_lines      (GgitDiffHunk        *hunk);
gsize           ggit_diff_hunk_get_header_len     (GgitDiffHunk        *hunk);
const gchar    *ggit_diff_hunk_get_header         (GgitDiffHunk        *hunk);

G_END_DECLS

#endif /* __GGIT_DIFF_HUNK_H__ */

/* ex:set ts=8 noet: */
