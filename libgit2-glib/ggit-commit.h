/*
 * ggit-commit.h
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2011 - Ignacio Casal Quinteiro
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


#ifndef __GGIT_COMMIT_H__
#define __GGIT_COMMIT_H__

#include <glib-object.h>
#include <git2.h>

#include <libgit2-glib/ggit-object.h>
#include <libgit2-glib/ggit-types.h>
#include <libgit2-glib/ggit-tree.h>
#include <libgit2-glib/ggit-commit-parents.h>
#include <libgit2-glib/ggit-signature.h>

G_BEGIN_DECLS

#define GGIT_TYPE_COMMIT (ggit_commit_get_type ())
G_DECLARE_DERIVABLE_TYPE (GgitCommit, ggit_commit, GGIT, COMMIT, GgitObject)

/**
 * GgitCommitClass:
 * @parent_class: The parent class.
 *
 * The class structure for #GgitCommitClass.
 */
struct _GgitCommitClass
{
	/*< private >*/
	GgitObjectClass parent_class;
};

GgitCommit          *_ggit_commit_wrap                (git_commit        *commit,
                                                       gboolean           owned);

const git_commit    *_ggit_commit_get_commit          (GgitCommit        *commit);

const gchar         *ggit_commit_get_message_encoding (GgitCommit        *commit);

const gchar         *ggit_commit_get_message          (GgitCommit        *commit);

const gchar         *ggit_commit_get_subject          (GgitCommit        *commit);

GgitSignature       *ggit_commit_get_committer        (GgitCommit        *commit);

GgitSignature       *ggit_commit_get_author           (GgitCommit        *commit);

GgitCommitParents   *ggit_commit_get_parents          (GgitCommit        *commit);

GgitTree            *ggit_commit_get_tree             (GgitCommit        *commit);

GgitOId             *ggit_commit_get_tree_id          (GgitCommit        *commit);

GgitCommit          *ggit_commit_get_nth_ancestor     (GgitCommit        *commit,
                                                       guint              n,
                                                       GError           **error);

GgitOId             *ggit_commit_amend                (GgitCommit        *commit,
                                                       const gchar       *update_ref,
                                                       GgitSignature     *author,
                                                       GgitSignature     *committer,
                                                       const gchar       *message_encoding,
                                                       const gchar       *message,
                                                       GgitTree          *tree,
                                                       GError           **error);

G_END_DECLS

#endif /* __GGIT_COMMIT_H__ */

/* ex:set ts=8 noet: */
