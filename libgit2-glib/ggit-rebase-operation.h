/*
 * ggit-rebase-operation.h
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


#ifndef __GGIT_REBASE_OPERATION_H__
#define __GGIT_REBASE_OPERATION_H__

#include <glib-object.h>
#include <git2.h>

#include "ggit-types.h"

G_BEGIN_DECLS

#define GGIT_TYPE_REBASE_OPERATION       (ggit_rebase_operation_get_type ())
#define GGIT_REBASE_OPERATION(obj)       ((GgitRebaseOperation *)obj)

GType                        ggit_rebase_operation_get_type                    (void) G_GNUC_CONST;

GgitRebaseOperation        *_ggit_rebase_operation_wrap                        (git_rebase_operation *rebase_operation);

git_rebase_operation       *_ggit_rebase_operation_get_rebase_operation        (GgitRebaseOperation  *rebase_operation);

GgitRebaseOperation         *ggit_rebase_operation_ref                         (GgitRebaseOperation  *rebase_operation);
void                         ggit_rebase_operation_unref                       (GgitRebaseOperation  *rebase_operation);

GgitRebaseOperationType      ggit_rebase_operation_get_operation_type          (GgitRebaseOperation  *rebase_operation);

GgitOId                     *ggit_rebase_operation_get_id                      (GgitRebaseOperation  *rebase_operation);

const gchar                 *ggit_rebase_operation_get_exec                    (GgitRebaseOperation  *rebase_operation);

G_DEFINE_AUTOPTR_CLEANUP_FUNC (GgitRebaseOperation, ggit_rebase_operation_unref)

G_END_DECLS

#endif /* __GGIT_REBASE_OPERATION_H__ */

/* ex:set ts=8 noet: */
