/*
 * ggit-diff-binary.h
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2015 - Ignacio Casal Quinteiro
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

#ifndef __GGIT_DIFF_BINARY_H__
#define __GGIT_DIFF_BINARY_H__

#include <git2.h>

#include "ggit-types.h"

G_BEGIN_DECLS

#define GGIT_TYPE_DIFF_BINARY       (ggit_diff_binary_get_type ())
#define GGIT_DIFF_BINARY(obj)       ((GgitDiffBinary *)obj)

GType                    ggit_diff_binary_get_type          (void) G_GNUC_CONST;

GgitDiffBinary          *_ggit_diff_binary_wrap             (const git_diff_binary *binary);

GgitDiffBinary          *ggit_diff_binary_ref               (GgitDiffBinary        *binary);
void                     ggit_diff_binary_unref             (GgitDiffBinary        *binary);

GgitDiffBinaryFile      *ggit_diff_binary_get_old_file      (GgitDiffBinary        *binary);

GgitDiffBinaryFile      *ggit_diff_binary_get_new_file      (GgitDiffBinary        *binary);

G_END_DECLS

#endif /* __GGIT_DIFF_BINARY_H__ */

/* ex:set ts=8 noet: */
