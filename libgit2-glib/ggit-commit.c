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
#include "ggit-commit-parents.h"

/**
 * GgitCommit:
 *
 * Represents a commit object.
 */

typedef struct _GgitCommitPrivate
{
	gchar *message_utf8;
	gchar *subject_utf8;
} GgitCommitPrivate;

G_DEFINE_TYPE_WITH_PRIVATE (GgitCommit, ggit_commit, GGIT_TYPE_OBJECT)

static void
ggit_commit_finalize (GObject *object)
{
	GgitCommit *commit = GGIT_COMMIT (object);
	GgitCommitPrivate *priv;

	priv = ggit_commit_get_instance_private (commit);

	g_free (priv->message_utf8);
	g_free (priv->subject_utf8);

	G_OBJECT_CLASS (ggit_commit_parent_class)->finalize (object);
}

static void
ggit_commit_class_init (GgitCommitClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->finalize = ggit_commit_finalize;
}

static void
ggit_commit_init (GgitCommit *self)
{
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
 * Returns: (transfer none) (allow-none): the encoding of the commit message or %NULL.
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
	GgitCommitPrivate *priv;
	git_commit *c;
	const gchar *msg;
	const gchar *encoding;
	const gchar *ptr;

	priv = ggit_commit_get_instance_private (commit);

	if (priv->message_utf8)
	{
		return;
	}

	c = _ggit_native_get (commit);
	msg = git_commit_message (c);

	encoding = ggit_commit_get_message_encoding (commit);

	priv->message_utf8 = ggit_convert_utf8 (msg,
	                                        -1,
	                                        encoding);

	/* Extract the subject */
	ptr = g_utf8_strchr (priv->message_utf8, -1, '\n');

	if (ptr != NULL)
	{
		priv->subject_utf8 = g_strndup (priv->message_utf8,
		                                ptr - priv->message_utf8);
	}
}

/**
 * ggit_commit_get_message:
 * @commit: a #GgitCommit.
 *
 * Gets the full message of @commit. The resulting message is always encoded
 * in UTF-8.
 *
 * Returns: (transfer none) (allow-none): the message of the commit.
 */
const gchar *
ggit_commit_get_message (GgitCommit *commit)
{
	GgitCommitPrivate *priv;

	g_return_val_if_fail (GGIT_IS_COMMIT (commit), NULL);

	priv = ggit_commit_get_instance_private (commit);

	ensure_message_utf8 (commit);

	return priv->message_utf8;
}

/**
 * ggit_commit_get_subject:
 * @commit: a #GgitCommit.
 *
 * Gets the subject of @commit. The subject of a commit is the first line of
 * the commit message (as per convention). The resulting subject is always
 * encoded in UTF-8.
 *
 * Returns: (transfer none) (allow-none): the subject of the commit.
 */
const gchar *
ggit_commit_get_subject (GgitCommit *commit)
{
	GgitCommitPrivate *priv;

	g_return_val_if_fail (GGIT_IS_COMMIT (commit), NULL);

	priv = ggit_commit_get_instance_private (commit);

	ensure_message_utf8 (commit);

	if (priv->subject_utf8)
	{
		return priv->subject_utf8;
	}
	else
	{
		return priv->message_utf8;
	}
}

/**
 * ggit_commit_get_committer:
 * @commit: a #GgitCommit.
 *
 * Gets the committer of @commit. The returned value must be free'd with
 * g_object_unref().
 *
 * Returns: (transfer full) (allow-none): the committer of the commit.
 */
GgitSignature *
ggit_commit_get_committer (GgitCommit *commit)
{
	git_commit *c;
	const git_signature *committer;
	git_signature *signature;

	g_return_val_if_fail (GGIT_IS_COMMIT (commit), NULL);

	c = _ggit_native_get (commit);
	committer = git_commit_committer (c);
	git_signature_dup (&signature, committer);

	return _ggit_signature_wrap (signature,
	                             ggit_commit_get_message_encoding (commit),
	                             TRUE);
}

/**
 * ggit_commit_get_author:
 * @commit: a #GgitCommit.
 *
 * Gets the author of @commit. The returned value must be free'd with
 * g_object_unref().
 *
 * Returns: (transfer full) (allow-none): the author of the commit.
 */
GgitSignature *
ggit_commit_get_author (GgitCommit *commit)
{
	git_commit *c;
	const git_signature *author;
	git_signature *signature;

	g_return_val_if_fail (GGIT_IS_COMMIT (commit), NULL);

	c = _ggit_native_get (commit);
	author = git_commit_author (c);
	git_signature_dup (&signature, author);

	return _ggit_signature_wrap (signature,
	                             ggit_commit_get_message_encoding (commit),
	                             TRUE);
}

/**
 * ggit_commit_get_parents:
 * @commit: a #GgitCommit.
 *
 * Gets the parents collection for @commit.
 *
 * Returns: (transfer full) (allow-none): the parents collection of the commit.
 */
GgitCommitParents *
ggit_commit_get_parents (GgitCommit *commit)
{
	g_return_val_if_fail (GGIT_IS_COMMIT (commit), NULL);

	return ggit_commit_parents_new (commit);
}

/**
 * ggit_commit_get_tree:
 * @commit: a #GgitCommit.
 *
 * Get the tree object for @commit.
 *
 * Returns: (transfer full) (allow-none): a #GgitTree.
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
 * Returns: (transfer full) (allow-none): a #GgitOId.
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
 * Returns: (transfer full) (allow-none): the @n ancestor commit.
 */
GgitCommit *
ggit_commit_get_nth_ancestor (GgitCommit  *commit,
                              guint        n,
                              GError     **error)
{
	git_commit *ancestor;
	gint ret;

	g_return_val_if_fail (GGIT_IS_COMMIT (commit), NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	ret = git_commit_nth_gen_ancestor (&ancestor, _ggit_native_get (commit), n);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return NULL;
	}

	return _ggit_commit_wrap (ancestor, TRUE);
}

/**
 * ggit_commit_amend:
 * @commit: a #GgitCommit.
 * @update_ref: (allow-none): name of the reference to update.
 * @author: author signature.
 * @committer: committer signature (and time of commit).
 * @message_encoding: (allow-none): message encoding.
 * @message: commit message.
 * @tree: the tree of objects to commit.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Amend an existing commit. If @update_ref is not %NULL, the given reference will
 * be updated to point to the newly created commit. Use "HEAD" to update the
 * HEAD of the current branch and make it point to this commit.
 *
 * If @message_encoding is set to %NULL, "UTF-8" encoding is assumed for the
 * provided @message. Note that @message will not be cleaned up automatically.
 * You can use #ggit_message_prettify to do this yourself if needed.
 *
 * Returns: (transfer full) (allow-none): the #GgitOId of the created commit object,
 * or %NULL in case of an error.
 *
 */
GgitOId *
ggit_commit_amend (GgitCommit      *commit,
                   const gchar     *update_ref,
                   GgitSignature   *author,
                   GgitSignature   *committer,
                   const gchar     *message_encoding,
                   const gchar     *message,
                   GgitTree        *tree,
                   GError         **error)
{
	gint ret;
	git_oid oid;

	g_return_val_if_fail (GGIT_IS_COMMIT (commit), NULL);
	g_return_val_if_fail (GGIT_IS_SIGNATURE (author), NULL);
	g_return_val_if_fail (GGIT_IS_SIGNATURE (committer), NULL);
	g_return_val_if_fail (message != NULL, NULL);
	g_return_val_if_fail (GGIT_IS_TREE (tree), NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	ret = git_commit_amend (&oid,
	                         _ggit_native_get (commit),
	                         update_ref,
	                         _ggit_native_get (author),
	                         _ggit_native_get (committer),
	                         message_encoding,
	                         message,
	                         _ggit_native_get (tree));

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return NULL;
	}

	return _ggit_oid_wrap (&oid);
}

/* ex:set ts=8 noet: */
