/*
 * ggit-commit.c
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2011 - Ignacio Casal Quinteiro
 *
 * libgit2-glib is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * libgit2-glib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libgit2-glib; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA  02110-1301  USA
 */

#include <git2/errors.h>

#include "ggit-commit.h"
#include "ggit-signature.h"
#include "ggit-oid.h"
#include "ggit-convert.h"

#define GGIT_COMMIT_GET_PRIVATE(object)(G_TYPE_INSTANCE_GET_PRIVATE((object), GGIT_TYPE_COMMIT, GgitCommitPrivate))

static GgitCommitParents *ggit_commit_parents_copy (GgitCommitParents *parents);
static void               ggit_commit_parents_free (GgitCommitParents *parents);

struct _GgitCommitParents
{
	GgitCommit *commit;
};

struct _GgitCommitPrivate
{
	gchar *message_utf8;
	gchar *subject_utf8;
};

G_DEFINE_TYPE (GgitCommit, ggit_commit, GGIT_TYPE_OBJECT)

G_DEFINE_BOXED_TYPE (GgitCommitParents,
                     ggit_commit_parents,
                     ggit_commit_parents_copy,
                     ggit_commit_parents_free)

static GgitCommitParents *
ggit_commit_parents_new (GgitCommit *commit)
{
	GgitCommitParents *ret;

	ret = g_slice_new (GgitCommitParents);

	ret->commit = g_object_ref (commit);
	return ret;
}

static GgitCommitParents *
ggit_commit_parents_copy (GgitCommitParents *parents)
{
	if (parents == NULL)
	{
		return NULL;
	}

	return ggit_commit_parents_new (parents->commit);
}

static void
ggit_commit_parents_free (GgitCommitParents *parents)
{
	if (parents == NULL)
	{
		return;
	}

	g_clear_object (&parents->commit);
	g_slice_free (GgitCommitParents, parents);
}

static void
ggit_commit_class_init (GgitCommitClass *klass)
{
	GObjectClass *object_class;

	object_class = G_OBJECT_CLASS (klass);

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

	// Extract the subject
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

/* TODO: use gdatetime */
/**
 * ggit_commit_get_time:
 * @commit: a #GgitCommit.
 *
 * Gets the commit time (i.e. committer time) of @commit.
 *
 * Returns: the time of the commit.
 */
gint64
ggit_commit_get_time (GgitCommit *commit)
{
	git_commit *c;

	g_return_val_if_fail (GGIT_IS_COMMIT (commit), 0);

	c = _ggit_native_get (commit);

	return git_commit_time (c);
}

/**
 * ggit_commit_get_time_offset:
 * @commit: a #GgitCommit.
 *
 * Gets the commit timezone offset (i.e. committer's preferred timezone)
 * of @commit.
 *
 * Returns: positive or negative timezone offset, in minutes from UTC.
 */
gint
ggit_commit_get_time_offset (GgitCommit *commit)
{
	git_commit *c;

	g_return_val_if_fail (GGIT_IS_COMMIT (commit), 0);

	c = _ggit_native_get (commit);

	return git_commit_time_offset (c);
}

/**
 * ggit_commit_get_committer:
 * @commit: a #GgitCommit.
 *
 * Gets the committer of @commit. The returned value must be free'd with
 * ggit_signature_free().
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
	                             ggit_commit_get_message_encoding (commit));
}

/**
 * ggit_commit_get_author:
 * @commit: a #GgitCommit.
 *
 * Gets the author of @commit. The returned value must be free'd with
 * ggit_signature_free().
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
	                             ggit_commit_get_message_encoding (commit));
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

	return ggit_commit_parents_new (commit);
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

	if (git_commit_parent (&p, c, idx) == GIT_SUCCESS)
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
 * Returns: (transfer full): a #GgitOid.
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

	oid = git_commit_parent_oid (c, idx);
	return _ggit_oid_new (oid);
}

/* ex:set ts=8 noet: */
