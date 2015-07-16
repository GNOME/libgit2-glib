/*
 * ggit-commit-parents.h
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2014 - Jesse van den Kieboom
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


#ifndef __GGIT_COMMIT_PARENTS_H__
#define __GGIT_COMMIT_PARENTS_H__

#include <glib-object.h>

#include <libgit2-glib/ggit-types.h>

G_BEGIN_DECLS

typedef struct _GgitCommit GgitCommit;

#define GGIT_TYPE_COMMIT_PARENTS    (ggit_commit_parents_get_type ())
G_DECLARE_FINAL_TYPE (GgitCommitParents, ggit_commit_parents, GGIT, COMMIT_PARENTS, GObject)

GgitCommitParents   *ggit_commit_parents_new          (GgitCommit        *commit);

guint                ggit_commit_parents_get_size     (GgitCommitParents *parents);

GgitCommit          *ggit_commit_parents_get          (GgitCommitParents *parents,
                                                       guint              idx);

GgitOId             *ggit_commit_parents_get_id       (GgitCommitParents *parents,
                                                       guint              idx);

G_END_DECLS

#endif /* __GGIT_COMMIT_PARENTS_H__ */

/* ex:set ts=8 noet: */
