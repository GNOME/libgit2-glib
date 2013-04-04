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

#define GGIT_TYPE_BRANCH		(ggit_branch_get_type ())
#define GGIT_BRANCH(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), GGIT_TYPE_BRANCH, GgitBranch))
#define GGIT_BRANCH_CONST(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), GGIT_TYPE_BRANCH, GgitBranch const))
#define GGIT_BRANCH_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST ((klass), GGIT_TYPE_BRANCH, GgitBranchClass))
#define GGIT_IS_BRANCH(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), GGIT_TYPE_BRANCH))
#define GGIT_IS_BRANCH_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), GGIT_TYPE_BRANCH))
#define GGIT_BRANCH_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS ((obj), GGIT_TYPE_BRANCH, GgitBranchClass))

typedef struct _GgitBranch		GgitBranch;
typedef struct _GgitBranchClass		GgitBranchClass;

struct _GgitBranch
{
	/*< private >*/
	GgitRef parent;

	gpointer *_priv;
};

struct _GgitBranchClass
{
	/*< private >*/
	GgitRefClass parent_class;
};

GType               ggit_branch_get_type           (void) G_GNUC_CONST;

GgitBranch        *_ggit_branch_wrap               (git_reference    *ref);

void                ggit_branch_delete             (GgitBranch       *branch,
                                                    GError          **error);

GgitBranch         *ggit_branch_move               (GgitBranch       *branch,
                                                    const gchar      *new_branch_name,
                                                    GgitCreateFlags   flags,
                                                    GError          **error);

const gchar        *ggit_branch_get_name           (GgitBranch       *branch,
                                                    GError          **error);

GgitBranch         *ggit_branch_get_tracking       (GgitBranch       *branch,
                                                    GError          **error);

gboolean            ggit_branch_is_head            (GgitBranch       *branch,
                                                    GError          **error);

G_END_DECLS

#endif /* __GGIT_BRANCH_H__ */

/* ex:set ts=8 noet: */
