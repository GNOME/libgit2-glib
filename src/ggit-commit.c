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


#include "ggit-commit.h"
#include "ggit-object-private.h"


G_DEFINE_TYPE (GgitCommit, ggit_commit, GGIT_TYPE_OBJECT)

static void
ggit_commit_finalize (GObject *object)
{
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
_ggit_commit_new (git_commit *commit)
{
	GgitCommit *gcommit;

	gcommit = g_object_new (GGIT_TYPE_COMMIT, NULL);
	GGIT_OBJECT (gcommit)->priv->obj = (git_object *)commit;

	return gcommit;
}

/**
 * ggit_commit_message_short:
 * @commit: a #GgitCommit
 *
 * Get the short (one line) message of a commit.
 *
 * Returns: the short (one line) message of a commit.
 */
const gchar *
ggit_commit_message_short (GgitCommit *commit)
{
	git_commit *c;

	g_return_val_if_fail (GGIT_IS_COMMIT (commit), NULL);

	c = (git_commit *)GGIT_OBJECT (commit)->priv->obj;

	return git_commit_message_short (c);
}

/**
 * ggit_commit_message:
 * @commit: a #GgitCommit
 *
 * Get the full message of a commit.
 *
 * Returns: the message of a commit
 */
const gchar *
ggit_commit_message (GgitCommit *commit)
{
	git_commit *c;

	g_return_val_if_fail (GGIT_IS_COMMIT (commit), NULL);

	c = (git_commit *)GGIT_OBJECT (commit)->priv->obj;

	return git_commit_message (c);
}

/* TODO: use gdatetime */
/**
 * ggit_commit_time:
 * @commit: a #GgitCommit
 *
 * Get the commit time (i.e. committer time) of a commit.
 *
 * Returns: the time of a commit
 */
gint64
ggit_commit_time (GgitCommit *commit)
{
	git_commit *c;

	g_return_val_if_fail (GGIT_IS_COMMIT (commit), 0);

	c = (git_commit *)GGIT_OBJECT (commit)->priv->obj;

	return git_commit_time (c);
}

/**
 * ggit_commit_time_offset:
 * @commit: a #GgitCommit
 *
 * Get the commit timezone offset (i.e. committer's preferred timezone) of a commit.
 *
 * Returns: positive or negative timezone offset, in minutes from UTC
 */
gint
ggit_commit_time_offset (GgitCommit *commit)
{
	git_commit *c;

	g_return_val_if_fail (GGIT_IS_COMMIT (commit), 0);

	c = (git_commit *)GGIT_OBJECT (commit)->priv->obj;

	return git_commit_time_offset (c);
}

/**
 * ggit_commit_committer:
 * @commit: a #GgitCommit
 *
 * Get the committer of a commit. The returned value must be free with
 * ggit_signature_free()
 *
 * Returns: (transfer full): the committer of a commit
 */
GgitSignature *
ggit_commit_committer (GgitCommit *commit)
{
	git_commit *c;
	const git_signature *signature;

	g_return_val_if_fail (GGIT_IS_COMMIT (commit), 0);

	c = (git_commit *)GGIT_OBJECT (commit)->priv->obj;
	signature = git_commit_committer (c);

	return _ggit_signature_wrap ((git_signature *)signature);
}

/**
 * ggit_commit_author:
 * @commit: a #GgitCommit
 *
 * Get the author of a commit. The returned value must be free with
 * ggit_signature_free()
 *
 * Returns: (transfer full): the author of a commit
 */
GgitSignature *
ggit_commit_author (GgitCommit *commit)
{
	git_commit *c;
	const git_signature *signature;

	g_return_val_if_fail (GGIT_IS_COMMIT (commit), 0);

	c = (git_commit *)GGIT_OBJECT (commit)->priv->obj;
	signature = git_commit_author (c);

	return _ggit_signature_wrap ((git_signature *)signature);
}

/**
 * ggit_commit_parents:
 * @commit: a #GgitCommit
 *
 * Get the parents for @commit
 *
 * Returns: (transfer full): the parents for @commit
 */
GList *
ggit_commit_parents (GgitCommit *commit)
{
	GList *parents = NULL;
	git_commit *c;
	gint num_parents, i;

	g_return_val_if_fail (GGIT_IS_COMMIT (commit), NULL);

	c = (git_commit *)GGIT_OBJECT (commit)->priv->obj;

	num_parents = git_commit_parentcount (c);

	for (i = 0; i < num_parents; i++)
	{
		git_commit *p;
		gint ret;

		ret = git_commit_parent (&p, c, i);

		if (ret == 0)
		{
			parents = g_list_prepend (parents, _ggit_commit_new (p));
		}
	}

	parents = g_list_reverse (parents);

	return parents;
}
