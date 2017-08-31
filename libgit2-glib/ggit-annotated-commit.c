/*
 * ggit-annotated-commit.c
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

#include <git2.h>

#include "ggit-annotated-commit.h"
#include "ggit-oid.h"

struct _GgitAnnotatedCommit
{
	gint ref_count;

	git_annotated_commit *annotated_commit;
};

G_DEFINE_BOXED_TYPE (GgitAnnotatedCommit, ggit_annotated_commit,
                     ggit_annotated_commit_ref, ggit_annotated_commit_unref)

GgitAnnotatedCommit *
_ggit_annotated_commit_wrap (git_annotated_commit *annotated_commit)
{
	GgitAnnotatedCommit *glib_annotated_commit;

	glib_annotated_commit = g_slice_new (GgitAnnotatedCommit);
	glib_annotated_commit->ref_count = 1;
	glib_annotated_commit->annotated_commit = annotated_commit;

	return glib_annotated_commit;
}

git_annotated_commit *
_ggit_annotated_commit_get_annotated_commit (GgitAnnotatedCommit *annotated_commit)
{
	return annotated_commit->annotated_commit;
}

/**
 * ggit_annotated_commit_ref:
 * @annotated_commit: a #GgitAnnotatedCommit.
 *
 * Atomically increments the reference count of @annotated_commit by one.
 * This function is MT-safe and may be called from any thread.
 *
 * Returns: (transfer none) (nullable): a newly allocated #GgitAnnotatedCommit or %NULL.
 */
GgitAnnotatedCommit *
ggit_annotated_commit_ref (GgitAnnotatedCommit *annotated_commit)
{
	g_return_val_if_fail (annotated_commit != NULL, NULL);

	g_atomic_int_inc (&annotated_commit->ref_count);

	return annotated_commit;
}

/**
 * ggit_annotated_commit_unref:
 * @annotated_commit: a #GgitAnnotatedCommit.
 *
 * Atomically decrements the reference count of @annotated_commit by one.
 * If the reference count drops to 0, @annotated_commit is freed.
 */
void
ggit_annotated_commit_unref (GgitAnnotatedCommit *annotated_commit)
{
	g_return_if_fail (annotated_commit != NULL);

	if (g_atomic_int_dec_and_test (&annotated_commit->ref_count))
	{
		git_annotated_commit_free (annotated_commit->annotated_commit);
		g_slice_free (GgitAnnotatedCommit, annotated_commit);
	}
}

/**
 * ggit_annotated_commit_get_id:
 * @annotated_commit: a #GgitAnnotatedCommit.
 *
 * Gets the commit ID that the given @annotated_commit refs to.
 *
 * Returns: (transfer full) (nullable): the commit ID that the given @annotated_commit refs to or %NULL.
 */
GgitOId *
ggit_annotated_commit_get_id (GgitAnnotatedCommit *annotated_commit)
{
	const git_oid *oid;

	g_return_val_if_fail (annotated_commit != NULL, NULL);

	oid = git_annotated_commit_id (annotated_commit->annotated_commit);

	return _ggit_oid_wrap ((git_oid *)oid);
}

/* ex:set ts=8 noet: */
