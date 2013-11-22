/*
 * ggit-branc-enumeratir.h
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2013 - Jesse van den Kieboom
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

#ifndef __GGIT_BRANCH_ENUMERATOR_H__
#define __GGIT_BRANCH_ENUMERATOR_H__

#include <glib-object.h>
#include <git2.h>

#include <libgit2-glib/ggit-types.h>
#include <libgit2-glib/ggit-branch.h>

G_BEGIN_DECLS

#define GGIT_TYPE_BRANCH_ENUMERATOR (ggit_branch_enumerator_get_type ())
#define GGIT_BRANCH_ENUMERATOR(obj) ((GgitBranchEnumerator *)obj)

GType                 ggit_branch_enumerator_get_type   (void) G_GNUC_CONST;

GgitBranchEnumerator *_ggit_branch_enumerator_wrap      (git_branch_iterator *iter);

GgitBranchEnumerator *ggit_branch_enumerator_ref        (GgitBranchEnumerator *enumerator);
void                  ggit_branch_enumerator_unref      (GgitBranchEnumerator *enumerator);

gboolean              ggit_branch_enumerator_next       (GgitBranchEnumerator *enumerator);
GgitRef              *ggit_branch_enumerator_get        (GgitBranchEnumerator *enumerator);

#endif /* __GGIT_BRANCH_ENUMERATOR_H__ */
