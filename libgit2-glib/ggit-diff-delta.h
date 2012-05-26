/*
 * ggit-diff-delta.h
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

#ifndef __GGIT_DIFF_DELTA_H__
#define __GGIT_DIFF_DELTA_H__

#include <git2/diff.h>

#include "ggit-types.h"

G_BEGIN_DECLS

#define GGIT_TYPE_DIFF_DELTA       (ggit_diff_delta_get_type ())
#define GGIT_DIFF_DELTA(obj)       ((GgitDiffDelta *)obj)
#define GGIT_DIFF_DELTA_CONST(obj) ((GgitDiffDelta const *)obj)

GType           ggit_diff_delta_get_type       (void) G_GNUC_CONST;

GgitDiffDelta  *_ggit_diff_delta_wrap          (git_diff_delta *delta);

GgitDiffDelta  *ggit_diff_delta_ref            (GgitDiffDelta  *delta);
void            ggit_diff_delta_unref          (GgitDiffDelta  *delta);

GgitDiffFile   *ggit_diff_delta_get_old_file   (GgitDiffDelta  *delta);
GgitDiffFile   *ggit_diff_delta_get_new_file   (GgitDiffDelta  *delta);
GgitDeltaType   ggit_diff_delta_get_status     (GgitDiffDelta  *delta);
guint           ggit_diff_delta_get_similarity (GgitDiffDelta  *delta);
gint            ggit_diff_delta_get_binary     (GgitDiffDelta  *delta);

G_END_DECLS

#endif /* __GGIT_DIFF_DELTA_H__ */

/* ex:set ts=8 noet: */
