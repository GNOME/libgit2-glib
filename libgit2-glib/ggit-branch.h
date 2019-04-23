/*
 * ggit-branch.h
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2012 - Ignacio Casal Quinteiro
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

#ifndef __GGIT_BRANCH_H__
#define __GGIT_BRANCH_H__

#include <libgit2-glib/ggit-ref.h>

G_BEGIN_DECLS

#define GGIT_TYPE_BRANCH (ggit_branch_get_type ())
G_DECLARE_DERIVABLE_TYPE (GgitBranch, ggit_branch, GGIT, BRANCH, GgitRef)

struct _GgitBranchClass
{
	/*< private >*/
	GgitRefClass parent_class;
};

GgitBranch        *_ggit_branch_wrap               (git_reference    *ref);

void                ggit_branch_delete             (GgitBranch       *branch,
                                                    GError          **error);

GgitBranch         *ggit_branch_move               (GgitBranch       *branch,
                                                    const gchar      *new_branch_name,
                                                    GgitCreateFlags   flags,
                                                    GError          **error);

const gchar        *ggit_branch_get_name           (GgitBranch       *branch,
                                                    GError          **error);

GgitRef            *ggit_branch_get_upstream       (GgitBranch       *branch,
                                                    GError          **error);

void                ggit_branch_set_upstream       (GgitBranch       *branch,
                                                    const gchar      *upstream_branch_name,
                                                    GError          **error);

gboolean            ggit_branch_is_head            (GgitBranch       *branch,
                                                    GError          **error);

G_END_DECLS

#endif /* __GGIT_BRANCH_H__ */

/* ex:set ts=8 noet: */
