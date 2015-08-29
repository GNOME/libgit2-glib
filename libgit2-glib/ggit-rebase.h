/*
 * ggit-rebase.h
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


#ifndef __GGIT_REBASE_H__
#define __GGIT_REBASE_H__

#include <glib-object.h>
#include <git2.h>

#include "ggit-native.h"
#include "ggit-types.h"
#include "ggit-signature.h"

G_BEGIN_DECLS

#define GGIT_TYPE_REBASE (ggit_rebase_get_type ())
G_DECLARE_FINAL_TYPE (GgitRebase, ggit_rebase, GGIT, REBASE, GgitNative)

/* Indicates that a rebase operation is not (yet) in progress. */
#define GGIT_REBASE_NO_OPERATION SIZE_MAX

GgitRebase         *_ggit_rebase_wrap                           (git_rebase     *rebase);

gsize                ggit_rebase_get_operation_entry_count      (GgitRebase     *rebase);

gsize                ggit_rebase_get_operation_index            (GgitRebase     *rebase);

GgitRebaseOperation *ggit_rebase_get_operation_by_index         (GgitRebase     *rebase,
                                                                 gsize           idx);

GgitRebaseOperation *ggit_rebase_next                           (GgitRebase     *rebase,
                                                                 GError        **error);

GgitOId             *ggit_rebase_commit                         (GgitRebase     *rebase,
                                                                 GgitSignature  *author,
                                                                 GgitSignature  *committer,
                                                                 const gchar    *message,
                                                                 GError        **error);

void                 ggit_rebase_abort                          (GgitRebase     *rebase,
                                                                 GError        **error);

void                 ggit_rebase_finish                         (GgitRebase     *rebase,
                                                                 GgitSignature  *signature,
                                                                 GError        **error);

G_END_DECLS

#endif /* __GGIT_REBASE_H__ */

/* ex:set ts=8 noet: */
