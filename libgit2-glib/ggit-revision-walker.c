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

/**
 * GgitRevisionWalker:
 *
 * Represents a revision walker.
 */

typedef struct _GgitRevisionWalkerPrivate
{
	GgitRepository *repository;
} GgitRevisionWalkerPrivate;

enum
{
	PROP_0,
	PROP_REPOSITORY
};

static void ggit_revision_walker_initable_iface_init (GInitableIface  *iface);

G_DEFINE_TYPE_EXTENDED (GgitRevisionWalker, ggit_revision_walker, GGIT_TYPE_NATIVE,
                        0,
                        G_ADD_PRIVATE (GgitRevisionWalker)
                        G_IMPLEMENT_INTERFACE (G_TYPE_INITABLE,
                                               ggit_revision_walker_initable_iface_init))

static void
ggit_revision_walker_get_property (GObject    *object,
                                   guint       prop_id,
                                   GValue     *value,
                                   GParamSpec *pspec)
{
	GgitRevisionWalker *walker = GGIT_REVISION_WALKER (object);
	GgitRevisionWalkerPrivate *priv;

	priv = ggit_revision_walker_get_instance_private (walker);

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
	GgitRevisionWalker *walker = GGIT_REVISION_WALKER (object);
	GgitRevisionWalkerPrivate *priv;

	priv = ggit_revision_walker_get_instance_private (walker);

	switch (prop_id)
	{
		case PROP_REPOSITORY:
			priv->repository = g_value_dup_object (value);
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void
ggit_revision_walker_dispose (GObject *object)
{
	GgitRevisionWalker *walker = GGIT_REVISION_WALKER (object);
	GgitRevisionWalkerPrivate *priv;

	priv = ggit_revision_walker_get_instance_private (walker);

	g_clear_object (&priv->repository);

	G_OBJECT_CLASS (ggit_revision_walker_parent_class)->dispose (object);
}

static void
ggit_revision_walker_class_init (GgitRevisionWalkerClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->get_property = ggit_revision_walker_get_property;
	object_class->set_property = ggit_revision_walker_set_property;
	object_class->dispose = ggit_revision_walker_dispose;

	g_object_class_install_property (object_class,
	                                 PROP_REPOSITORY,
	                                 g_param_spec_object ("repository",
	                                                      "Repository",
	                                                      "The repository where to make the walking",
	                                                      GGIT_TYPE_REPOSITORY,
	                                                      G_PARAM_READWRITE |
	                                                      G_PARAM_CONSTRUCT |
	                                                      G_PARAM_STATIC_STRINGS));
}

static void
ggit_revision_walker_init (GgitRevisionWalker *revwalk)
{
}

static gboolean
ggit_revision_walker_initable_init (GInitable    *initable,
                                    GCancellable *cancellable,
                                    GError      **error)
{
	GgitRevisionWalker *walker = GGIT_REVISION_WALKER (initable);
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

	priv = ggit_revision_walker_get_instance_private (walker);

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
 * Returns: (transfer full) (nullable): a new #GgitRevisionWalker or %NULL.
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
 * ggit_revision_walker_push_glob:
 * @walker: a #GgitRevisionWalker.
 * @item: the glob to push.
 * @error: a #GError.
 *
 * Push all OIDs pointed to by references that match the given glob pattern
 * to the revsision walker. A leading 'refs/' is implied if not present, as well
 * as a trailing '/ \ *' if the glob lacks '?', '\ *' or '['.
 *
 **/
void
ggit_revision_walker_push_glob (GgitRevisionWalker  *walker,
                                const gchar         *item,
                                GError             **error)
{
	gint ret;

	g_return_if_fail (GGIT_IS_REVISION_WALKER (walker));
	g_return_if_fail (item != NULL);
	g_return_if_fail (error == NULL || *error == NULL);

	ret = git_revwalk_push_glob (_ggit_native_get (walker),
	                             item);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
	}
}

/**
 * ggit_revision_walker_push_ref:
 * @walker: a #GgitRevisionWalker.
 * @item: the reference to push.
 * @error: a #GError.
 *
 * Push the OID pointed to by the named reference to the revision walker.
 *
 **/
void
ggit_revision_walker_push_ref (GgitRevisionWalker  *walker,
                               const gchar         *item,
                               GError             **error)
{
	gint ret;

	g_return_if_fail (GGIT_IS_REVISION_WALKER (walker));
	g_return_if_fail (item != NULL);
	g_return_if_fail (error == NULL || *error == NULL);

	ret = git_revwalk_push_ref (_ggit_native_get (walker),
	                            item);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
	}
}

/**
 * ggit_revision_walker_push_range:
 * @walker: a #GgitRevisionWalker.
 * @range: the range to push.
 * @error: a #GError.
 *
 * Push and hide the respective endpoints of the given range. The range
 * should be of the form: &ltcommit&gt..&lt;commit&gt;, where each &lt;commit&gt; is in the
 * form accepted by revparse. The left-hand commit will be hidden and the
 * right-hand commit pushed.
 *
 **/
void
ggit_revision_walker_push_range (GgitRevisionWalker  *walker,
                                 const gchar         *range,
                                 GError             **error)
{
	gint ret;

	g_return_if_fail (GGIT_IS_REVISION_WALKER (walker));
	g_return_if_fail (range != NULL);
	g_return_if_fail (error == NULL || *error == NULL);

	ret = git_revwalk_push_range (_ggit_native_get (walker),
	                              range);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
	}
}

/**
 * ggit_revision_walker_push_head:
 * @walker: a #GgitRevisionWalker.
 * @error: a #GError.
 *
 * Push the OID of the current HEAD to the revision walker.
 *
 **/
void
ggit_revision_walker_push_head (GgitRevisionWalker  *walker,
                                GError             **error)
{
	gint ret;

	g_return_if_fail (GGIT_IS_REVISION_WALKER (walker));
	g_return_if_fail (error == NULL || *error == NULL);

	ret = git_revwalk_push_head (_ggit_native_get (walker));

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
 * ggit_revision_walker_hide_glob:
 * @walker: a #GgitRevisionWalker.
 * @item: the glob to hide.
 * @error: a #GError.
 *
 * Hide all OIDs pointed to by references that match the given glob pattern
 * to the revsision walker. A leading 'refs/' is implied if not present, as well
 * as a trailing '/ \ *' if the glob lacks '?', '\ *' or '['.
 *
 **/
void
ggit_revision_walker_hide_glob (GgitRevisionWalker  *walker,
                                const gchar         *item,
                                GError             **error)
{
	gint ret;

	g_return_if_fail (GGIT_IS_REVISION_WALKER (walker));
	g_return_if_fail (item != NULL);
	g_return_if_fail (error == NULL || *error == NULL);

	ret = git_revwalk_hide_glob (_ggit_native_get (walker),
	                             item);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
	}
}

/**
 * ggit_revision_walker_hide_ref:
 * @walker: a #GgitRevisionWalker.
 * @item: the reference to hide.
 * @error: a #GError.
 *
 * Hide the OID pointed to by the named reference to the revision walker.
 *
 **/
void
ggit_revision_walker_hide_ref (GgitRevisionWalker  *walker,
                               const gchar         *item,
                               GError             **error)
{
	gint ret;

	g_return_if_fail (GGIT_IS_REVISION_WALKER (walker));
	g_return_if_fail (item != NULL);
	g_return_if_fail (error == NULL || *error == NULL);

	ret = git_revwalk_hide_ref (_ggit_native_get (walker),
	                            item);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
	}
}

/**
 * ggit_revision_walker_hide_head:
 * @walker: a #GgitRevisionWalker.
 * @error: a #GError.
 *
 * Hide the OID of the current HEAD to the revision walker.
 *
 **/
void
ggit_revision_walker_hide_head (GgitRevisionWalker  *walker,
                                GError             **error)
{
	gint ret;

	g_return_if_fail (GGIT_IS_REVISION_WALKER (walker));
	g_return_if_fail (error == NULL || *error == NULL);

	ret = git_revwalk_hide_head (_ggit_native_get (walker));

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
 * Returns: (transfer full) (nullable): the next commit from the revision walk or %NULL.
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
 * Returns: (transfer none) (nullable): the repository on which this walker is operating or %NULL.
 */
GgitRepository *
ggit_revision_walker_get_repository (GgitRevisionWalker *walker)
{
	GgitRevisionWalkerPrivate *priv;

	g_return_val_if_fail (GGIT_IS_REVISION_WALKER (walker), NULL);

	priv = ggit_revision_walker_get_instance_private (walker);

	return priv->repository;
}

/* ex:set ts=8 noet: */
