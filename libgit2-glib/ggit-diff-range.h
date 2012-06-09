/*
 * ggit-diff-range.h
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

#ifndef __GGIT_DIFF_RANGE_H__
#define __GGIT_DIFF_RANGE_H__

#include "ggit-types.h"

G_BEGIN_DECLS

#define GGIT_TYPE_DIFF_RANGE       (ggit_diff_range_get_type ())
#define GGIT_DIFF_RANGE(obj)       ((GgitDiffRange *)obj)

GType ggit_diff_range_get_type                 (void) G_GNUC_CONST;

GgitDiffRange  *_ggit_diff_range_wrap          (git_diff_range *range);

GgitDiffRange  *ggit_diff_range_ref            (GgitDiffRange  *range);
void            ggit_diff_range_unref          (GgitDiffRange  *range);

gint            ggit_diff_range_get_old_start  (GgitDiffRange  *range);
gint            ggit_diff_range_get_old_lines  (GgitDiffRange  *range);
gint            ggit_diff_range_get_new_start  (GgitDiffRange  *range);
gint            ggit_diff_range_get_new_lines  (GgitDiffRange  *range);

G_END_DECLS

#endif /* __GGIT_DIFF_RANGE_H__ */

/* ex:set ts=8 noet: */
