/*
 * ggit-revision-walker.c
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


#include <gio/gio.h>
#include <git2.h>

#include "ggit-error.h"
#include "ggit-oid.h"
#include "ggit-repository.h"
#include "ggit-revision-walker.h"

#define GGIT_REVISION_WALKER_GET_PRIVATE(object)(G_TYPE_INSTANCE_GET_PRIVATE((object), GGIT_TYPE_REVISION_WALKER, GgitRevisionWalkerPrivate))

struct _GgitRevisionWalkerPrivate
{
	GgitRepository *repository;
};

enum
{
	PROP_0,
	PROP_REPOSITORY
};

static void ggit_revision_walker_initable_iface_init (GInitableIface  *iface);

G_DEFINE_TYPE_EXTENDED (GgitRevisionWalker, ggit_revision_walker, GGIT_TYPE_NATIVE,
                        0,
                        G_IMPLEMENT_INTERFACE (G_TYPE_INITABLE,
                                               ggit_revision_walker_initable_iface_init))

static void
ggit_revision_walker_get_property (GObject    *object,
                                   guint       prop_id,
                                   GValue     *value,
                                   GParamSpec *pspec)
{
	GgitRevisionWalkerPrivate *priv = GGIT_REVISION_WALKER (object)->priv;

	switch (prop_id)
	{
		case PROP_REPOSITORY:
			g_value_set_object (value, priv->repository);
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void
ggit_revision_walker_set_property (GObject      *object,
                                   guint         prop_id,
                                   const GValue *value,
                                   GParamSpec   *pspec)
{
	GgitRevisionWalkerPrivate *priv = GGIT_REVISION_WALKER (object)->priv;

	switch (prop_id)
	{
		case PROP_REPOSITORY:
			priv->repository = g_value_get_object (value);
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void
ggit_revision_walker_class_init (GgitRevisionWalkerClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->get_property = ggit_revision_walker_get_property;
	object_class->set_property = ggit_revision_walker_set_property;

	g_object_class_install_property (object_class,
	                                 PROP_REPOSITORY,
	                                 g_param_spec_object ("repository",
	                                                      "Repository",
	                                                      "The repository where to make the walking",
	                                                      GGIT_TYPE_REPOSITORY,
	                                                      G_PARAM_READWRITE |
	                                                      G_PARAM_CONSTRUCT |
	                                                      G_PARAM_STATIC_STRINGS));

	g_type_class_add_private (object_class, sizeof (GgitRevisionWalkerPrivate));
}

static void
ggit_revision_walker_init (GgitRevisionWalker *revwalk)
{
	revwalk->priv = GGIT_REVISION_WALKER_GET_PRIVATE (revwalk);
}

static gboolean
ggit_revision_walker_initable_init (GInitable    *initable,
                                    GCancellable *cancellable,
                                    GError      **error)
{
	GgitRevisionWalkerPrivate *priv;
	git_revwalk *revwalk;
	gboolean success = TRUE;
	gint err;

	if (cancellable != NULL)
	{
		g_set_error_literal (error, G_IO_ERROR, G_IO_ERROR_NOT_SUPPORTED,
		                     "Cancellable initialization not supported");
		return FALSE;
	}

	priv = GGIT_REVISION_WALKER (initable)->priv;

	err = git_revwalk_new (&revwalk,
	                       _ggit_repository_get_repository (priv->repository));
	if (err != GIT_OK)
	{
		_ggit_error_set (error, err);
		success = FALSE;
	}

	_ggit_native_set (initable, revwalk,
	                  (GDestroyNotify) git_revwalk_free);

	return success;
}

static void
ggit_revision_walker_initable_iface_init (GInitableIface *iface)
{
	iface->init = ggit_revision_walker_initable_init;
}

/**
 * ggit_revision_walker_new:
 * @repository: a #GgitRepository.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Creates a new revision walker to iterate through @repository.
 *
 * This revision walker uses a custom memory pool and an internal
 * commit cache, so it is relatively expensive to allocate.
 *
 * For maximum performance, this revision walker should be
 * reused for different walks.
 *
 * This revision walker is *not* thread safe: it may only be
 * used to walk a repository on a single thread; however,
 * it is possible to have several revision walkers in
 * several different threads walking the same repository.
 *
 * Returns: (transfer full): a new #GgitRevisionWalker.
 */
GgitRevisionWalker *
ggit_revision_walker_new (GgitRepository  *repository,
                          GError         **error)
{
	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	return g_initable_new (GGIT_TYPE_REVISION_WALKER, NULL, error,
	                       "repository", repository,
	                       NULL);
}

/**
 * ggit_revision_walker_reset:
 * @walker: a #GgitRevisionWalker.
 *
 * Resets the revision walker for reuse.
 *
 * This will clear all the pushed and hidden commits, and
 * leave the walker in a blank state (just like at
 * creation) ready to receive new commit pushes and
 * start a new walk.
 *
 * The revision walk is automatically reset when a walk
 * is over.
 */
void
ggit_revision_walker_reset (GgitRevisionWalker *walker)
{
	g_return_if_fail (GGIT_IS_REVISION_WALKER (walker));

	git_revwalk_reset (_ggit_native_get (walker));
}

/**
 * ggit_revision_walker_push:
 * @walker: a #GgitRevisionWalker.
 * @oid: a #GgitOId.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Marks a commit to start traversal from.
 *
 * The given OID must belong to a commit on the walked
 * repository.
 *
 * The given commit will be used as one of the roots
 * when starting the revision walk. At least one commit
 * must be pushed the repository before a walk can
 * be started.
 */
void
ggit_revision_walker_push (GgitRevisionWalker  *walker,
                           GgitOId             *oid,
                           GError             **error)
{
	gint ret;

	g_return_if_fail (GGIT_IS_REVISION_WALKER (walker));
	g_return_if_fail (oid != NULL);
	g_return_if_fail (error == NULL || *error == NULL);

	ret = git_revwalk_push (_ggit_native_get (walker),
	                        _ggit_oid_get_oid (oid));

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
	}
}

/**
 * ggit_revision_walker_hide:
 * @walker: a #GgitRevisionWalker.
 * @oid: a #GgitOId.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Marks a commit (and its ancestors) uninteresting for the output.
 *
 * The given OID must belong to a commit on the walked
 * repository.
 *
 * The resolved commit and all its parents will be hidden from the
 * output on the revision walk.
 */
void
ggit_revision_walker_hide (GgitRevisionWalker  *walker,
                           GgitOId             *oid,
                           GError             **error)
{
	gint ret;

	g_return_if_fail (GGIT_IS_REVISION_WALKER (walker));
	g_return_if_fail (oid != NULL);
	g_return_if_fail (error == NULL || *error == NULL);

	ret = git_revwalk_hide (_ggit_native_get (walker),
	                        _ggit_oid_get_oid (oid));

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
	}
}

/**
 * ggit_revision_walker_next:
 * @walker: a #GgitRevisionWalker.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Gets the next commit from the revision walk.
 *
 * The initial call to this method is *not* blocking when
 * iterating through a repository with a time-sorting mode.
 *
 * Iterating with Topological or inverted modes makes the initial
 * call blocking to preprocess the commit list, but this block should be
 * mostly unnoticeable on most repositories (topological preprocessing
 * times at 0.3s on the git.git repo).
 *
 * The revision walker is reset when the walk is over.
 *
 * Returns: (transfer full): the next commit from the revision walk.
 */
GgitOId *
ggit_revision_walker_next (GgitRevisionWalker  *walker,
                           GError             **error)
{
	GgitOId *goid = NULL;
	git_oid oid;
	gint ret;

	g_return_val_if_fail (GGIT_IS_REVISION_WALKER (walker), NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	ret = git_revwalk_next (&oid, _ggit_native_get (walker));

	if (ret == GIT_OK)
	{
		goid = _ggit_oid_wrap (&oid);
	}
	else if (ret != GIT_ITEROVER)
	{
		_ggit_error_set (error, ret);
	}

	return goid;
}

/**
 * ggit_revision_walker_set_sort_mode:
 * @walker: a #GgitRevisionWalker.
 * @sort_mode: a #GgitSortMode value.
 *
 * Change the sorting mode when iterating through the
 * repository's contents.
 *
 * Changing the sorting mode resets the walker.
 */
void
ggit_revision_walker_set_sort_mode (GgitRevisionWalker *walker,
                                    GgitSortMode        sort_mode)
{
	g_return_if_fail (GGIT_IS_REVISION_WALKER (walker));

	git_revwalk_sorting (_ggit_native_get (walker), sort_mode);
}

/**
 * ggit_revision_walker_get_repository:
 * @walker: a #GgitRepository.
 *
 * Gets the repository on which this walker is operating.
 *
 * Returns: (transfer none): the repository on which this walker is operating.
 */
GgitRepository *
ggit_revision_walker_get_repository (GgitRevisionWalker *walker)
{
	g_return_val_if_fail (GGIT_IS_REVISION_WALKER (walker), NULL);

	return walker->priv->repository;
}

/* ex:set ts=8 noet: */
