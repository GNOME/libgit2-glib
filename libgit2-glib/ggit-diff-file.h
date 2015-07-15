/*
 * ggit-diff-file.h
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

#ifndef __GGIT_DIFF_FILE_H__
#define __GGIT_DIFF_FILE_H__

#include <glib-object.h>
#include <git2.h>

#include "ggit-types.h"

G_BEGIN_DECLS

#define GGIT_TYPE_DIFF_FILE       (ggit_diff_file_get_type ())
#define GGIT_DIFF_FILE(obj)       ((GgitDiffFile *)obj)

GType              ggit_diff_file_get_type  (void) G_GNUC_CONST;

GgitDiffFile      *_ggit_diff_file_wrap     (const git_diff_file *file);

GgitDiffFile      *ggit_diff_file_ref       (GgitDiffFile        *file);
void               ggit_diff_file_unref     (GgitDiffFile        *file);

GgitOId           *ggit_diff_file_get_oid   (GgitDiffFile        *file);
const gchar       *ggit_diff_file_get_path  (GgitDiffFile        *file);
guint16            ggit_diff_file_get_mode  (GgitDiffFile        *file);
gint64             ggit_diff_file_get_size  (GgitDiffFile        *file);
GgitDiffFlag       ggit_diff_file_get_flags (GgitDiffFile        *file);

G_DEFINE_AUTOPTR_CLEANUP_FUNC (GgitDiffFile, ggit_diff_file_unref)

G_END_DECLS

#endif /* __GGIT_DIFF_FILE_H__ */

/* ex:set ts=8 noet: */
