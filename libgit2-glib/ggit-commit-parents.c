/*
 * ggit-commit-parents.c
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

#include <git2.h>

#include "ggit-commit-parents.h"
#include "ggit-commit.h"
#include "ggit-oid.h"

/**
 * GgitCommitParents:
 *
 * Represents the parents of a commit object.
 */
struct _GgitCommitParents
{
	GObject parent_instance;

	GgitCommit *commit;
};

enum
{
	PROP_0,
	PROP_COMMIT,
	PROP_SIZE
};

G_DEFINE_TYPE (GgitCommitParents, ggit_commit_parents, G_TYPE_OBJECT)

static void
ggit_commit_parents_get_property (GObject    *object,
                                  guint       prop_id,
                                  GValue     *value,
                                  GParamSpec *pspec)
{
	GgitCommitParents *parents = GGIT_COMMIT_PARENTS (object);

	switch (prop_id)
	{
		case PROP_COMMIT:
			g_value_set_object (value, parents->commit);
			break;
		case PROP_SIZE:
			g_value_set_uint (value, ggit_commit_parents_get_size (parents));
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void
ggit_commit_parents_set_property (GObject      *object,
                                  guint         prop_id,
                                  const GValue *value,
                                  GParamSpec   *pspec)
{
	GgitCommitParents *parents = GGIT_COMMIT_PARENTS (object);

	switch (prop_id)
	{
		case PROP_COMMIT:
			parents->commit = g_value_dup_object (value);
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

GgitCommitParents *
ggit_commit_parents_new (GgitCommit *commit)
{
	return g_object_new (GGIT_TYPE_COMMIT_PARENTS, "commit", commit, NULL);
}

static void
ggit_commit_parents_finalize (GObject *object)
{
	GgitCommitParents *parents = GGIT_COMMIT_PARENTS (object);

	g_object_unref (parents->commit);

	G_OBJECT_CLASS (ggit_commit_parents_parent_class)->finalize (object);
}

static void
ggit_commit_parents_class_init (GgitCommitParentsClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->finalize = ggit_commit_parents_finalize;
	object_class->get_property = ggit_commit_parents_get_property;
	object_class->set_property = ggit_commit_parents_set_property;

	g_object_class_install_property (object_class,
	                                 PROP_COMMIT,
	                                 g_param_spec_object ("commit",
	                                                      "Commit",
	                                                      "The commit for the parents collection",
	                                                      GGIT_TYPE_COMMIT,
	                                                      G_PARAM_READWRITE |
	                                                      G_PARAM_CONSTRUCT_ONLY |
	                                                      G_PARAM_STATIC_STRINGS));

	g_object_class_install_property (object_class,
	                                 PROP_SIZE,
	                                 g_param_spec_uint ("size",
	                                                    "Size",
	                                                    "The size of the parents collection",
	                                                    0,
	                                                    G_MAXUINT,
	                                                    0,
	                                                    G_PARAM_READABLE |
	                                                    G_PARAM_STATIC_STRINGS));
}

static void
ggit_commit_parents_init (GgitCommitParents *self)
{
}

/**
 * ggit_commit_parents_get_size:
 * @parents: a #GgitCommitParents.
 *
 * Get the number of parents in the parents collection.
 *
 * Returns: the number of parents.
 *
 **/
guint
ggit_commit_parents_get_size (GgitCommitParents *parents)
{
	git_commit *c;

	g_return_val_if_fail (GGIT_IS_COMMIT_PARENTS (parents), 0);

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

	g_return_val_if_fail (GGIT_IS_COMMIT_PARENTS (parents), NULL);

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

	g_return_val_if_fail (GGIT_IS_COMMIT_PARENTS (parents), NULL);

	c = _ggit_native_get (parents->commit);

	oid = git_commit_parent_id (c, idx);
	return _ggit_oid_wrap (oid);
}

/* ex:set ts=8 noet: */
