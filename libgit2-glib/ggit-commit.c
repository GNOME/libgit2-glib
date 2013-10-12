/*
 * ggit-commit.c
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

#include <git2.h>

#include "ggit-error.h"
#include "ggit-commit.h"
#include "ggit-signature.h"
#include "ggit-oid.h"
#include "ggit-convert.h"
#include "ggit-tree.h"

#define GGIT_COMMIT_GET_PRIVATE(object)(G_TYPE_INSTANCE_GET_PRIVATE((object), GGIT_TYPE_COMMIT, GgitCommitPrivate))

struct _GgitCommitParents
{
	GgitCommit *commit;
	gint ref_count;
};

struct _GgitCommitPrivate
{
	gchar *message_utf8;
	gchar *subject_utf8;
};

G_DEFINE_TYPE (GgitCommit, ggit_commit, GGIT_TYPE_OBJECT)

G_DEFINE_BOXED_TYPE (GgitCommitParents,
                     ggit_commit_parents,
                     ggit_commit_parents_ref,
                     ggit_commit_parents_unref)

static GgitCommitParents *
ggit_commit_parents_wrap (GgitCommit *commit)
{
	GgitCommitParents *ret;

	ret = g_slice_new (GgitCommitParents);
	ret->commit = g_object_ref (commit);
	ret->ref_count = 1;

	return ret;
}

/**
 * ggit_commit_parents_ref:
 * @parents: a #GgitCommitParents.
 *
 * Atomically increments the reference count of @parents by one.
 * This function is MT-safe and may be called from any thread.
 *
 * Returns: a #GgitCommitParents.
 **/
GgitCommitParents *
ggit_commit_parents_ref (GgitCommitParents *parents)
{
	g_return_val_if_fail (parents != NULL, NULL);

	g_atomic_int_inc (&parents->ref_count);

	return parents;
}

/**
 * ggit_commit_parents_unref:
 * @parents: a #GgitCommitParents.
 *
 * Atomically decrements the reference count of @parents by one.
 * If the reference count drops to 0, @parents is freed.
 **/
void
ggit_commit_parents_unref (GgitCommitParents *parents)
{
	g_return_if_fail (parents != NULL);

	if (g_atomic_int_dec_and_test (&parents->ref_count))
	{
		g_clear_object (&parents->commit);
		g_slice_free (GgitCommitParents, parents);
	}
}

static void
ggit_commit_finalize (GObject *object)
{
	GgitCommit *commit = GGIT_COMMIT (object);

	g_free (commit->priv->message_utf8);
	g_free (commit->priv->subject_utf8);

	G_OBJECT_CLASS (ggit_commit_parent_class)->finalize (object);
}

static void
ggit_commit_class_init (GgitCommitClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->finalize = ggit_commit_finalize;

	g_type_class_add_private (object_class, sizeof (GgitCommitPrivate));
}

static void
ggit_commit_init (GgitCommit *self)
{
	self->priv = GGIT_COMMIT_GET_PRIVATE (self);
}

GgitCommit *
_ggit_commit_wrap (git_commit *commit,
                   gboolean    owned)
{
	GgitCommit *gcommit;

	gcommit = g_object_new (GGIT_TYPE_COMMIT,
	                        "native", commit,
	                        NULL);

	if (owned)
	{
		_ggit_native_set_destroy_func (gcommit,
		                               (GDestroyNotify)git_object_free);
	}

	return gcommit;
}

/**
 * ggit_commit_get_message_encoding:
 * @commit: a #GgitCommit.
 *
 * Get the encoding for the message of a commit,
 * as a string representing a standard encoding name.
 *
 * The encoding may be %NULL if the 'encoding' header
 * in the commit is missing; in that case UTF-8 is assumed.
 *
 * Returns: the encoding of the commit message or %NULL.
 */
const gchar *
ggit_commit_get_message_encoding (GgitCommit *commit)
{
	git_commit *c;
	const gchar *encoding;

	g_return_val_if_fail (GGIT_IS_COMMIT (commit), NULL);

	c = _ggit_native_get (commit);

	encoding = git_commit_message_encoding (c);

	if (encoding == NULL)
	{
		return "UTF-8";
	}
	else
	{
		return encoding;
	}
}

static void
ensure_message_utf8 (GgitCommit *commit)
{
	git_commit *c;
	const gchar *msg;
	const gchar *encoding;
	const gchar *ptr;

	if (commit->priv->message_utf8)
	{
		return;
	}

	c = _ggit_native_get (commit);
	msg = git_commit_message (c);

	encoding = ggit_commit_get_message_encoding (commit);

	commit->priv->message_utf8 = ggit_convert_utf8 (msg,
	                                                -1,
	                                                encoding);

	/* Extract the subject */
	ptr = g_utf8_strchr (commit->priv->message_utf8, -1, '\n');

	if (ptr != NULL)
	{
		commit->priv->subject_utf8 = g_strndup (commit->priv->message_utf8,
		                                        ptr - commit->priv->message_utf8);
	}
}

/**
 * ggit_commit_get_message:
 * @commit: a #GgitCommit.
 *
 * Gets the full message of @commit. The resulting message is always encoded
 * in UTF-8.
 *
 * Returns: the message of the commit.
 */
const gchar *
ggit_commit_get_message (GgitCommit *commit)
{
	g_return_val_if_fail (GGIT_IS_COMMIT (commit), NULL);

	ensure_message_utf8 (commit);

	return commit->priv->message_utf8;
}

/**
 * ggit_commit_get_subject:
 * @commit: a #GgitCommit.
 *
 * Gets the subject of @commit. The subject of a commit is the first line of
 * the commit message (as per convention). The resulting subject is always
 * encoded in UTF-8.
 *
 * Returns: the subject of the commit.
 */
const gchar *
ggit_commit_get_subject (GgitCommit *commit)
{
	g_return_val_if_fail (GGIT_IS_COMMIT (commit), NULL);

	ensure_message_utf8 (commit);

	if (commit->priv->subject_utf8)
	{
		return commit->priv->subject_utf8;
	}
	else
	{
		return commit->priv->message_utf8;
	}
}

/**
 * ggit_commit_get_committer:
 * @commit: a #GgitCommit.
 *
 * Gets the committer of @commit. The returned value must be free'd with
 * g_object_unref().
 *
 * Returns: (transfer full): the committer of the commit.
 */
GgitSignature *
ggit_commit_get_committer (GgitCommit *commit)
{
	git_commit *c;
	const git_signature *signature;

	g_return_val_if_fail (GGIT_IS_COMMIT (commit), NULL);

	c = _ggit_native_get (commit);
	signature = git_commit_committer (c);

	return _ggit_signature_wrap ((git_signature *)signature,
	                             ggit_commit_get_message_encoding (commit),
	                             FALSE);
}

/**
 * ggit_commit_get_author:
 * @commit: a #GgitCommit.
 *
 * Gets the author of @commit. The returned value must be free'd with
 * g_object_unref().
 *
 * Returns: (transfer full): the author of the commit.
 */
GgitSignature *
ggit_commit_get_author (GgitCommit *commit)
{
	git_commit *c;
	const git_signature *signature;

	g_return_val_if_fail (GGIT_IS_COMMIT (commit), NULL);

	c = _ggit_native_get (commit);
	signature = git_commit_author (c);

	return _ggit_signature_wrap ((git_signature *)signature,
	                             ggit_commit_get_message_encoding (commit),
	                             FALSE);
}

/**
 * ggit_commit_get_parents:
 * @commit: a #GgitCommit.
 *
 * Gets the parents collection for @commit.
 *
 * Returns: (transfer full): the parents collection of the commit.
 */
GgitCommitParents *
ggit_commit_get_parents (GgitCommit *commit)
{
	g_return_val_if_fail (GGIT_IS_COMMIT (commit), NULL);

	return ggit_commit_parents_wrap (commit);
}

/**
 * ggit_commit_parents_size:
 * @parents: a #GgitCommitParents.
 *
 * Get the number of parents in the parents collection.
 *
 * Returns: the number of parents.
 *
 **/
guint
ggit_commit_parents_size (GgitCommitParents *parents)
{
	git_commit *c;

	g_return_val_if_fail (parents != NULL, 0);

	c = _ggit_native_get (parents->commit);
	return (guint)git_commit_parentcount (c);
}

/**
 * ggit_commit_parents_get:
 * @parents: a #GgitCommitParents.
 * @idx: the parent index.
 *
 * Get the #GgitCommit of a parent.
 *
 * Returns: (transfer full): a #GgitCommit.
 *
 **/
GgitCommit *
ggit_commit_parents_get (GgitCommitParents *parents,
                         guint              idx)
{
	git_commit *c;
	git_commit *p;

	g_return_val_if_fail (parents != NULL, NULL);

	c = _ggit_native_get (parents->commit);

	if (git_commit_parent (&p, c, idx) == GIT_OK)
	{
		return _ggit_commit_wrap (p, TRUE);
	}

	return NULL;
}

/**
 * ggit_commit_parents_get_id:
 * @parents: a #GgitCommitParents.
 * @idx: the parent index.
 *
 * Get the #GgitOId of a parent.
 *
 * Returns: (transfer full): a #GgitOId.
 *
 **/
GgitOId *
ggit_commit_parents_get_id (GgitCommitParents *parents,
                            guint              idx)
{
	git_commit *c;
	const git_oid *oid;

	g_return_val_if_fail (parents != NULL, NULL);

	c = _ggit_native_get (parents->commit);

	oid = git_commit_parent_id (c, idx);
	return _ggit_oid_wrap (oid);
}

/**
 * ggit_commit_get_tree:
 * @commit: a #GgitCommit.
 *
 * Get the tree object for @commit.
 *
 * Returns: (transfer full): a #GgitTree.
 *
 **/
GgitTree *
ggit_commit_get_tree (GgitCommit *commit)
{
	git_commit *c;
	git_tree *t;

	g_return_val_if_fail (GGIT_IS_COMMIT (commit), NULL);

	c = _ggit_native_get (commit);

	if (git_commit_tree (&t, c) == GIT_OK)
	{
		return _ggit_tree_wrap (t, TRUE);
	}
	else
	{
		return NULL;
	}
}

/**
 * ggit_commit_get_tree_id:
 * @commit: a #GgitCommit.
 *
 * Get the #GgitOId of the tree of @commit. Note that this is more efficient
 * than getting the tree object with ggit_commit_get_tree() because no additional
 * files need to be read from disk.
 *
 * Returns: (transfer full): a #GgitOId.
 *
 **/
GgitOId *
ggit_commit_get_tree_id (GgitCommit *commit)
{
	git_commit *c;
	const git_oid *oid;

	g_return_val_if_fail (GGIT_IS_COMMIT (commit), NULL);

	c = _ggit_native_get (commit);

	oid = git_commit_tree_id (c);

	return _ggit_oid_wrap (oid);
}

/**
 * ggit_commit_get_nth_ancestor:
 * @commit: a #GgitCommit.
 * @n: the requested ancestor.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Gets the commit object that is the n-th generation ancestor
 * of the named commit object, following only the first parents.
 * Passing %0 to the @n parameter returns another instance of @commit.
 *
 * Returns: (transfer full): the @n ancestor commit.
 */
GgitCommit *
ggit_commit_get_nth_ancestor (GgitCommit  *commit,
                              guint        n,
                              GError     **error)
{
	git_commit *ancestor;
	gint ret;

	g_return_val_if_fail (GGIT_IS_COMMIT (commit), NULL);
	g_return_val_if_fail (n >= 0, NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	ret = git_commit_nth_gen_ancestor (&ancestor, _ggit_native_get (commit), n);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return NULL;
	}

	return _ggit_commit_wrap (ancestor, TRUE);
}

/* ex:set ts=8 noet: */
