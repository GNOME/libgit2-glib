/*
 * ggit-repository.c
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
#include <git2/errors.h>
#include <git2/repository.h>
#include <git2/refs.h>
#include <git2/status.h>
#include <git2/tag.h>
#include <git2/branch.h>

#include "ggit-error.h"
#include "ggit-oid.h"
#include "ggit-ref.h"
#include "ggit-repository.h"
#include "ggit-utils.h"

#define GGIT_REPOSITORY_GET_PRIVATE(object)(G_TYPE_INSTANCE_GET_PRIVATE((object), GGIT_TYPE_REPOSITORY, GgitRepositoryPrivate))

struct _GgitRepositoryPrivate
{
	GFile *location;
	GFile *workdir;

	guint is_bare : 1;
	guint init : 1;
};

enum
{
	PROP_0,
	PROP_LOCATION,
	PROP_IS_BARE,
	PROP_INIT,
	PROP_WORKDIR,
	PROP_HEAD
};

static void         ggit_repository_initable_iface_init (GInitableIface  *iface);

G_DEFINE_TYPE_EXTENDED (GgitRepository, ggit_repository, GGIT_TYPE_NATIVE,
                        0,
                        G_IMPLEMENT_INTERFACE (G_TYPE_INITABLE,
                                               ggit_repository_initable_iface_init))

static void
ggit_repository_finalize (GObject *object)
{
	GgitRepositoryPrivate *priv = GGIT_REPOSITORY (object)->priv;

	g_clear_object (&priv->location);
	g_clear_object (&priv->workdir);

	G_OBJECT_CLASS (ggit_repository_parent_class)->finalize (object);
}

static void
ggit_repository_get_property (GObject    *object,
                              guint       prop_id,
                              GValue     *value,
                              GParamSpec *pspec)
{
	GgitRepository *repository = GGIT_REPOSITORY (object);
	GgitRepositoryPrivate *priv = repository->priv;

	switch (prop_id)
	{
		case PROP_LOCATION:
			g_value_take_object (value,
			                     ggit_repository_get_location (repository));
			break;
		case PROP_IS_BARE:
			g_value_set_boolean (value,
			                     ggit_repository_is_bare (repository));
			break;
		case PROP_INIT:
			g_value_set_boolean (value,
			                     priv->init);
			break;
		case PROP_WORKDIR:
			g_value_take_object (value,
			                    ggit_repository_get_workdir (repository));
			break;
		case PROP_HEAD:
			g_value_set_object (value,
			                    ggit_repository_get_head (repository, NULL));
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void
set_workdir (GgitRepository *repository,
             GFile          *workdir)
{
	GgitRepositoryPrivate *priv;

	priv = repository->priv;

	g_clear_object (&priv->workdir);

	if (workdir)
	{
		priv->workdir = g_file_dup (workdir);

		if (_ggit_native_get (repository))
		{
			gchar *path;

			path = g_file_get_path (priv->workdir);

			git_repository_set_workdir (_ggit_native_get (repository), path);
			g_free (path);
		}
	}
	else if (_ggit_native_get (repository))
	{
		git_repository_set_workdir (_ggit_native_get (repository), NULL);
	}
}

static void
ggit_repository_set_property (GObject      *object,
                              guint         prop_id,
                              const GValue *value,
                              GParamSpec   *pspec)
{
	GgitRepository *repository;
	GgitRepositoryPrivate *priv;

	repository = GGIT_REPOSITORY (object);
	priv = repository->priv;

	switch (prop_id)
	{
		case PROP_LOCATION:
		{
			GFile *f;

			f = g_value_get_object (value);

			g_clear_object (&priv->location);

			if (f)
			{
				priv->location = g_file_dup (f);
			}

			break;
		}
		case PROP_WORKDIR:
			set_workdir (repository, g_value_get_object (value));
			break;
		case PROP_IS_BARE:
			priv->is_bare = g_value_get_boolean (value);
			break;
		case PROP_INIT:
			priv->init = g_value_get_boolean (value);
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void
ggit_repository_class_init (GgitRepositoryClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->finalize = ggit_repository_finalize;
	object_class->get_property = ggit_repository_get_property;
	object_class->set_property = ggit_repository_set_property;

	g_object_class_install_property (object_class,
	                                 PROP_LOCATION,
	                                 g_param_spec_object ("location",
	                                                      "Location of repository",
	                                                      "The location of the repository",
	                                                      G_TYPE_FILE,
	                                                      G_PARAM_READWRITE |
	                                                      G_PARAM_CONSTRUCT_ONLY |
	                                                      G_PARAM_STATIC_STRINGS));

	g_object_class_install_property (object_class,
	                                 PROP_IS_BARE,
	                                 g_param_spec_boolean ("is-bare",
	                                                       "Is bare",
	                                                       "Is a bare repository",
	                                                       FALSE,
	                                                       G_PARAM_READWRITE |
	                                                       G_PARAM_CONSTRUCT_ONLY |
	                                                       G_PARAM_STATIC_STRINGS));

	g_object_class_install_property (object_class,
	                                 PROP_INIT,
	                                 g_param_spec_boolean ("init",
	                                                       "Init",
	                                                       "Whether to initialize a repository",
	                                                       FALSE,
	                                                       G_PARAM_READWRITE |
	                                                       G_PARAM_CONSTRUCT_ONLY |
	                                                       G_PARAM_STATIC_STRINGS));

	g_object_class_install_property (object_class,
	                                 PROP_WORKDIR,
	                                 g_param_spec_object ("workdir",
	                                                      "Path to repository working directory",
	                                                      "The path to the repository working directory",
	                                                      G_TYPE_FILE,
	                                                      G_PARAM_READWRITE |
	                                                      G_PARAM_CONSTRUCT |
	                                                      G_PARAM_STATIC_STRINGS));

	g_type_class_add_private (object_class, sizeof (GgitRepositoryPrivate));

	g_object_class_install_property (object_class,
	                                 PROP_HEAD,
	                                 g_param_spec_object ("head",
	                                                      "Head",
	                                                      "Head",
	                                                      GGIT_TYPE_REF,
	                                                      G_PARAM_READABLE));
}

static void
ggit_repository_init (GgitRepository *repository)
{
	repository->priv = GGIT_REPOSITORY_GET_PRIVATE (repository);
}

static gboolean
ggit_repository_initable_init (GInitable    *initable,
                               GCancellable *cancellable,
                               GError      **error)
{
	GgitRepositoryPrivate *priv;
	gboolean success = TRUE;
	gint err;
	gchar *path = NULL;
	git_repository *repo = NULL;

	if (cancellable != NULL)
	{
		g_set_error_literal (error, G_IO_ERROR, G_IO_ERROR_NOT_SUPPORTED,
		                     "Cancellable initialization not supported");
		return FALSE;
	}

	priv = GGIT_REPOSITORY (initable)->priv;

	if (priv->location != NULL)
	{
		path = g_file_get_path (priv->location);
	}

	if (path == NULL)
	{
		err = GGIT_ERROR_NOTFOUND;
	}
	else if (priv->init == TRUE)
	{
		err = git_repository_init (&repo,
		                           path,
		                           priv->is_bare);
	}
	else
	{
		err = git_repository_open (&repo,
		                           path);
	}

	g_free (path);

	if (err != GIT_OK)
	{
		_ggit_error_set (error, err);

		success = FALSE;
	}
	else if (priv->workdir)
	{
		path = g_file_get_path (priv->workdir);

		if (path)
		{
			git_repository_set_workdir (_ggit_native_get (GGIT_REPOSITORY (initable)),
			                            path);
		}

		g_free (path);
	}

	_ggit_native_set (GGIT_REPOSITORY (initable),
	                  repo,
	                  (GDestroyNotify)git_repository_free);

	return success;
}

static void
ggit_repository_initable_iface_init (GInitableIface *iface)
{
	iface->init = ggit_repository_initable_init;
}

GgitRepository *
_ggit_repository_wrap (git_repository *repository,
                       gboolean        owned)
{
	GgitRepository *ret;

	ret = g_object_new (GGIT_TYPE_REPOSITORY,
	                    "native", repository,
	                    NULL);

	if (owned)
	{
		_ggit_native_set_destroy_func (ret,
		                               (GDestroyNotify)git_repository_free);
	}

	return ret;
}

git_repository *
_ggit_repository_get_repository (GgitRepository *repository)
{
	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), NULL);

	return _ggit_native_get (repository);
}

/**
 * ggit_repository_open:
 * @location: the location of the repository.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Open a git repository.
 *
 * The @path must point to an existing git repository folder, e.g.
 *
 *		/path/to/my_repo/.git/	(normal repository)
 *							objects/
 *							index
 *							HEAD
 *
 *		/path/to/bare_repo/		(bare repository)
 *						objects/
 *						index
 *						HEAD
 *
 *	The method will automatically detect if @path is a normal
 *	or bare repository or fail if it is neither.
 *
 * Returns: (transfer full): a newly created #GgitRepository.
 */
GgitRepository *
ggit_repository_open (GFile   *location,
                      GError **error)
{
	g_return_val_if_fail (G_IS_FILE (location), NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	return g_initable_new (GGIT_TYPE_REPOSITORY, NULL, error,
	                       "location", location,
	                       NULL);
}

/**
 * ggit_repository_init_repository:
 * @location: the location of the repository.
 * @is_bare: if %TRUE, a git repository without a working directory is created
 *           at the pointed path. If %FALSE, provided path will be considered as the working
 *           directory into which the .git directory will be created.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Creates a new git repository in the given folder.
 *
 * Returns: (transfer full): a newly created #GgitRepository.
 */
GgitRepository *
ggit_repository_init_repository (GFile     *location,
                                 gboolean   is_bare,
                                 GError   **error)
{
	g_return_val_if_fail (G_IS_FILE (location), NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	return g_initable_new (GGIT_TYPE_REPOSITORY, NULL, error,
	                       "location", location,
	                       "is-bare", is_bare,
	                       "init", TRUE,
	                       NULL);
}

/**
 * ggit_repository_lookup:
 * @repository: a #GgitRepository.
 * @oid: a #GgitOId.
 * @gtype: a #GType.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Lookups a reference to one of the objects in the @repository.
 *
 * The generated reference must be freed with g_object_unref().
 *
 * The @gtype must match the type of the object
 * in the odb; the method will fail otherwise.
 * The special value %G_TYPE_NONE may be passed to let
 * the method guess the object's type.
 *
 * Returns: (transfer full): the found #GgitObject, or %NULL on error.
 */
GgitObject *
ggit_repository_lookup (GgitRepository  *repository,
                        GgitOId         *oid,
                        GType            gtype,
                        GError         **error)
{
	GgitObject *object = NULL;
	git_object *obj;
	const git_oid *id;
	git_otype otype;
	gint ret;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), NULL);

	id = (const git_oid *)_ggit_oid_get_oid (oid);
	otype = ggit_utils_get_otype_from_gtype (gtype);

	ret = git_object_lookup (&obj,
	                         _ggit_native_get (repository),
	                         id,
	                         otype);

	if (ret == GIT_OK)
	{
		object = ggit_utils_create_real_object (obj, TRUE);
	}
	else
	{
		_ggit_error_set (error, ret);
	}

	return object;
}

/**
 * ggit_repository_lookup_reference:
 * @repository: a #GgitRepository.
 * @name: the long name for the reference (e.g. HEAD, ref/heads/master, refs/tags/v0.1.0, ...).
 * @error: a #GError for error reporting, or %NULL.
 *
 * Lookups a reference by its name in @repository. The returned #GgitRef must
 * be freed with ggit_ref_free().
 *
 * Returns: (transfer full): the searched reference.
 */
GgitRef *
ggit_repository_lookup_reference (GgitRepository  *repository,
                                  const gchar     *name,
                                  GError         **error)
{
	GgitRef *ref = NULL;
	git_reference *reference;
	gint ret;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), NULL);
	g_return_val_if_fail (name != NULL, NULL);

	ret = git_reference_lookup (&reference, _ggit_native_get (repository),
	                            name);

	if (ret == GIT_OK)
	{
		ref = _ggit_ref_wrap (reference);
	}
	else
	{
		_ggit_error_set (error, ret);
	}

	return ref;
}

/**
 * ggit_repository_create_reference:
 * @repository: a #GgitRepository.
 * @name: the name for the new #GgitRef.
 * @oid: the #GgitOId pointed to by the reference.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Creates a new object id reference.
 *
 * The reference will be created in the repository and written
 * to the disk. The returned value must be freed with ggit_ref_free().
 *
 * Returns: (transfer full): the newly created reference.
 */
GgitRef *
ggit_repository_create_reference (GgitRepository  *repository,
                                  const gchar     *name,
                                  GgitOId         *oid,
                                  GError         **error)
{
	GgitRef *ref = NULL;
	git_reference *reference;
	gint ret;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), NULL);
	g_return_val_if_fail (name != NULL, NULL);
	g_return_val_if_fail (oid != NULL, NULL);

	ret = git_reference_create_oid (&reference, _ggit_native_get (repository),
	                                name, _ggit_oid_get_oid (oid), FALSE);

	if (ret == GIT_OK)
	{
		ref = _ggit_ref_wrap (reference);
	}
	else
	{
		_ggit_error_set (error, ret);
	}

	return ref;
}

/**
 * ggit_repository_create_symbolic_reference:
 * @repository: a #GgitRepository.
 * @name: the name for the new #GgitRef.
 * @target: the full name to the reference.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Creates a new symbolic reference.
 *
 * The reference will be created in the repository and written
 * to the disk. The returned value must be freed with ggit_ref_free().
 *
 * Returns: (transfer full): the newly created reference.
 */
GgitRef *
ggit_repository_create_symbolic_reference (GgitRepository  *repository,
                                           const gchar     *name,
                                           const gchar     *target,
                                           GError         **error)
{
	GgitRef *ref = NULL;
	git_reference *reference;
	gint ret;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), NULL);
	g_return_val_if_fail (name != NULL, NULL);
	g_return_val_if_fail (target != NULL, NULL);

	ret = git_reference_create_symbolic (&reference, _ggit_native_get (repository),
	                                     name, target, FALSE);

	if (ret == GIT_OK)
	{
		ref = _ggit_ref_wrap (reference);
	}
	else
	{
		_ggit_error_set (error, ret);
	}

	return ref;
}

/**
 * ggit_repository_get_head:
 * @repository: a #GgitRepository.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Get the current HEAD reference of the repository.
 *
 * Returns: (transfer full): a #GgitRef
 *
 **/
GgitRef *
ggit_repository_get_head (GgitRepository  *repository,
                          GError         **error)
{
	GgitRef *ref = NULL;
	git_reference *reference;
	gint ret;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), NULL);

	ret = git_repository_head (&reference, _ggit_native_get (repository));

	if (ret == GIT_OK)
	{
		ref = _ggit_ref_wrap (reference);
	}
	else
	{
		_ggit_error_set (error, ret);
	}

	return ref;
}

/**
 * ggit_repository_discover:
 * @location: the base location where the lookup starts.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Looks for a git repository.
 *
 * The lookup starts from @path and walks up the parent directories
 * and stops when a repository is found.
 *
 * Returns: (transfer full): the repository location.
 */
GFile *
ggit_repository_discover (GFile   *location,
                          GError **error)
{
	gchar found_path[GIT_PATH_MAX];
	GFile *rep = NULL;
	gchar *path;
	gint ret;

	g_return_val_if_fail (G_IS_FILE (location), NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	path = g_file_get_path (location);

	g_return_val_if_fail (path != NULL, NULL);

	ret = git_repository_discover (found_path,
	                               sizeof (found_path),
	                               path,
	                               0,
	                               "");

	g_free (path);

	if (ret == GIT_OK)
	{
		rep = g_file_new_for_path (found_path);
	}
	else
	{
		_ggit_error_set (error, ret);
	}

	return rep;
}

/**
 * ggit_repository_is_head_detached:
 * @repository: a #GgitRepository.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Checks if @repository's HEAD is detached.
 *
 * A repository's HEAD is detached when it points directly to a commit
 * instead of a branch.
 *
 * Returns: %TRUE if HEAD is detached.
 */
gboolean
ggit_repository_is_head_detached (GgitRepository  *repository,
                                  GError         **error)
{
	gint ret;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), FALSE);
	g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

	ret = git_repository_head_detached (_ggit_native_get (repository));

	if (ret < 0)
	{
		_ggit_error_set (error, ret);
	}

	return ret == 1;
}

/**
 * ggit_repository_is_head_orphan:
 * @repository: a #GgitRepository.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Checks if @repository's HEAD is an orphan.
 *
 * An orphan branch is one named from HEAD but doesn't exist in
 * the refs namespace, because it doesn't have any commit to point to.
 *
 * Returns: %TRUE if the current branch is an orphan.
 */
gboolean
ggit_repository_is_head_orphan (GgitRepository  *repository,
                                GError         **error)
{
	gint ret;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), FALSE);
	g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

	ret = git_repository_head_orphan (_ggit_native_get (repository));

	if (ret < 0)
	{
		_ggit_error_set (error, ret);
	}

	return ret == 1;
}

/**
 * ggit_repository_is_empty:
 * @repository: a #GgitRepository.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Checks if @repository is empty.
 *
 * An empty repository has just been initialized and contains
 * no commits.
 *
 * Returns: %TRUE if the repository is empty.
 */
gboolean
ggit_repository_is_empty (GgitRepository  *repository,
                          GError         **error)
{
	gint ret;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), FALSE);
	g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

	ret = git_repository_is_empty (_ggit_native_get (repository));

	if (ret < 0)
	{
		_ggit_error_set (error, ret);
	}

	return ret == 1;
}

/**
 * ggit_repository_get_location:
 * @repository: a #GgitRepository.
 *
 * Get the gitdir location of the repository.
 *
 * Returns: (transfer full): the location of the gitdir of the repository.
 */
GFile *
ggit_repository_get_location (GgitRepository *repository)
{
	const gchar *path;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), NULL);

	path = git_repository_path (_ggit_native_get (repository));

	return path ? g_file_new_for_path (path) : NULL;
}

/**
 * ggit_repository_get_workdir:
 * @repository: a #GgitRepository.
 *
 * Gets the working directory of the repository.
 *
 * Returns: (transfer full): the location of the working directory of the repository.
 */
GFile *
ggit_repository_get_workdir (GgitRepository *repository)
{
	const gchar *path;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), NULL);

	path = git_repository_workdir (_ggit_native_get (repository));

	return path ? g_file_new_for_path (path) : NULL;
}

/**
 * ggit_repository_set_workdir:
 * @repository: a #GgitRepository.
 * @workdir: the working directory
 *
 * Sets the working directory of the repository.
 *
 */
void
ggit_repository_set_workdir (GgitRepository *repository,
                             GFile          *workdir)
{
	g_return_if_fail (GGIT_IS_REPOSITORY (repository));
	g_return_if_fail (G_IS_FILE (workdir));

	set_workdir (repository, workdir);
}

/**
 * ggit_repository_is_bare:
 * @repository: a #GgitRepository.
 *
 * Checks if @repository is bare.
 *
 * Returns: %TRUE if the repository is empty.
 */
gboolean
ggit_repository_is_bare (GgitRepository *repository)
{
	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), FALSE);

	return git_repository_is_bare (_ggit_native_get (repository));
}

/**
 * ggit_repository_file_status:
 * @repository: a #GgitRepository.
 * @location: the file to retrieve status for, rooted at the repository working dir.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Gets the file status for a single file.
 *
 * Returns: the status for a single file.
 */
GgitStatusFlags
ggit_repository_file_status (GgitRepository  *repository,
                             GFile           *location,
                             GError         **error)
{
	GgitStatusFlags status_flags;
	gint ret;
	gchar *path;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), GGIT_STATUS_IGNORED);
	g_return_val_if_fail (G_IS_FILE (location), GGIT_STATUS_IGNORED);
	g_return_val_if_fail (error == NULL || *error == NULL, GGIT_STATUS_IGNORED);

	path = g_file_get_path (location);

	g_return_val_if_fail (path != NULL, GGIT_STATUS_IGNORED);

	ret = git_status_file (&status_flags,
	                       _ggit_native_get (repository),
	                       path);

	g_free (path);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
	}

	return status_flags;
}

/**
 * ggit_repository_file_status_foreach:
 * @repository: a #GgitRepository.
 * @callback: (scope call): a #GgitStatusCallback.
 * @user_data: callback user data.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Gathers file statuses and run a callback for each one.
 *
 * To the callback is passed the path of the file, the status and the data pointer
 * passed to this function. If the callback returns something other than
 * 0, the iteration will stop and @error will be set.
 *
 * Returns: %TRUE if there was no error, %FALSE otherwise
 *
 */
gboolean
ggit_repository_file_status_foreach (GgitRepository     *repository,
                                     GgitStatusCallback  callback,
                                     gpointer            user_data,
                                     GError            **error)
{
	gint ret;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), FALSE);
	g_return_val_if_fail (callback != NULL, FALSE);
	g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

	ret = git_status_foreach (_ggit_native_get (repository),
	                          callback,
	                          user_data);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return FALSE;
	}

	return TRUE;
}

/**
 * ggit_repository_references_foreach:
 * @repository: a #GgitRepository.
 * @reftype: a #GgitRefType
 * @callback: (scope call): a #GgitReferencesCallback.
 * @user_data: callback user data.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Gathers references and run a callback for each one.
 *
 * To the callback is passed the name of the reference and the data pointer
 * passed to this function. If the callback returns something other than
 * 0, the iteration will stop and @error will be set. The references are
 * filtered by @reftype.
 *
 * Returns: %TRUE if there was no error, %FALSE otherwise
 *
 */
gboolean
ggit_repository_references_foreach (GgitRepository          *repository,
                                    GgitRefType              reftype,
                                    GgitReferencesCallback   callback,
                                    gpointer                 user_data,
                                    GError                 **error)
{
	gint ret;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), FALSE);
	g_return_val_if_fail (callback != NULL, FALSE);
	g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

	ret = git_reference_foreach (_ggit_native_get (repository),
	                             reftype,
	                             callback,
	                             user_data);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return FALSE;
	}

	return TRUE;
}

/**
 * ggit_repository_get_config:
 * @repository: a #GgitRepository
 * @error: a #GError
 *
 * Get the config for a specific repository.
 *
 * Returns: (transfer full): a #GgitConfig.
 *
 **/
GgitConfig *
ggit_repository_get_config (GgitRepository  *repository,
                            GError         **error)
{
	git_config *config;
	gint ret;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

	ret = git_repository_config (&config,
	                             _ggit_native_get (repository));

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return NULL;
	}

	return _ggit_config_wrap (config);
}

/**
 * ggit_repository_get_index:
 * @repository: a #GgitRepository.
 * @error: a #GError.
 *
 * Get the index for a specific repository.
 *
 * Returns: (transfer full): a #GgitIndex.
 *
 **/
GgitIndex *
ggit_repository_get_index (GgitRepository  *repository,
                           GError         **error)
{
	git_index *idx;
	gint ret;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

	ret = git_repository_index (&idx,
	                            _ggit_native_get (repository));

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return NULL;
	}

	return _ggit_index_wrap (idx);
}

/**
 * ggit_repository_create_tag:
 * @repository: a #GgitRepository.
 * @tag_name: the tag name.
 * @target: a #GgitObject.
 * @tagger: a #GgitSignature.
 * @message: the tag message.
 * @flags: a #GgitCreateFlags.
 * @error: a #GError.
 *
 * Create a new tag object.
 *
 * Returns: (transfer full) (allow-none): the id to which the tag points, or
 *                                        %NULL in case of an error.
 *
 **/
GgitOId *
ggit_repository_create_tag (GgitRepository   *repository,
                            gchar const      *tag_name,
                            GgitObject       *target,
                            GgitSignature    *tagger,
                            gchar const      *message,
                            GgitCreateFlags   flags,
                            GError          **error)
{
	git_oid oid;
	gboolean force;
	gint ret;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), NULL);
	g_return_val_if_fail (tag_name != NULL, NULL);
	g_return_val_if_fail (GGIT_IS_OBJECT (target), NULL);
	g_return_val_if_fail (tagger != NULL, NULL);
	g_return_val_if_fail (message != NULL, NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	force = flags & GGIT_CREATE_FORCE;

	ret = git_tag_create (&oid,
	                     _ggit_native_get (repository),
	                     tag_name,
	                     _ggit_native_get (target),
	                     _ggit_native_get (tagger),
	                     message,
	                     force ? 1 : 0);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return NULL;
	}

	return _ggit_oid_new (&oid);
}

/**
 * ggit_repository_create_tag_from_buffer:
 * @repository: a #GgitRepository.
 * @tag: the tag buffer.
 * @flags: a #GgitCreateFlags.
 * @error: a #GError.
 *
 * Create a new tag from a buffer describing the tag object. The buffer must
 * be correctly formatted.
 *
 * Returns: (transfer full) (allow-none): the id to which the tag points, or
 *                                        %NULL in case of an error.
 *
 **/
GgitOId *
ggit_repository_create_tag_from_buffer (GgitRepository   *repository,
                                        const gchar      *tag,
                                        GgitCreateFlags   flags,
                                        GError          **error)
{
	git_oid oid;
	gboolean force;
	gint ret;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), NULL);
	g_return_val_if_fail (tag != NULL, NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	force = flags & GGIT_CREATE_FORCE;

	ret = git_tag_create_frombuffer (&oid,
	                                 _ggit_native_get (repository),
	                                 tag,
	                                 force ? 1 : 0);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return NULL;
	}

	return _ggit_oid_new (&oid);
}

/**
 * ggit_repository_create_tag_lightweight:
 * @repository: a #GgitRepository.
 * @tag_name: the name of the tag.
 * @target: a #GgitObject.
 * @flags: a #GgitCreateFlags.
 * @error: a #GError.
 *
 * Creates a new lightweight tag.
 *
 * Returns: (transfer full) (allow-none): the id to which the tag points, or
 *                                        %NULL in case of an error.
 *
 **/
GgitOId *
ggit_repository_create_tag_lightweight (GgitRepository   *repository,
                                        const gchar      *tag_name,
                                        GgitObject       *target,
                                        GgitCreateFlags   flags,
                                        GError          **error)
{
	gboolean force;
	git_oid oid;
	gint ret;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), FALSE);
	g_return_val_if_fail (tag_name != NULL, FALSE);
	g_return_val_if_fail (GGIT_IS_OBJECT (target), FALSE);
	g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

	force = flags & GGIT_CREATE_FORCE;

	ret = git_tag_create_lightweight (&oid,
	                                  _ggit_native_get (repository),
	                                  tag_name,
	                                  _ggit_native_get (target),
	                                  force ? 1 : 0);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return NULL;
	}

	return _ggit_oid_new (&oid);
}

/**
 * ggit_repository_list_tags:
 * @repository: a #GgitRepository.
 * @error: a #GError.
 *
 * Fill a list with all the tags in the @repository.
 *
 * Returns: (transfer full) (allow-none): a list with the tags in @repository.
 **/
gchar **
ggit_repository_list_tags (GgitRepository  *repository,
                           GError         **error)
{
	gint ret;
	git_strarray tag_names;
	gchar **tags;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), FALSE);
	g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

	ret = git_tag_list (&tag_names,
	                    _ggit_native_get (repository));

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		tags = NULL;
	}
	else
	{
		tags = ggit_utils_get_str_array_from_git_strarray (&tag_names);
	}

	return tags;
}

/**
 * ggit_repository_create_branch:
 * @repository: a #GgitRepository.
 * @branch_name: the name of the branch.
 * @target: a #GgitObject.
 * @flags: a #GgitCreateFlags.
 * @error: a #GError.
 *
 * Creates a new branch pointing at a target commit.
 *
 * Returns: (transfer full) (allow-none): the id to which the branch points, or
 *                                        %NULL in case of an error.
 **/
GgitOId *
ggit_repository_create_branch (GgitRepository   *repository,
                               const gchar      *branch_name,
                               GgitObject       *target,
                               GgitCreateFlags   flags,
                               GError          **error)
{
	gboolean force;
	git_oid oid;
	gint ret;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), FALSE);
	g_return_val_if_fail (branch_name != NULL, FALSE);
	g_return_val_if_fail (GGIT_IS_OBJECT (target), FALSE);
	g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

	force = flags & GGIT_CREATE_FORCE;

	ret = git_branch_create (&oid,
	                         _ggit_native_get (repository),
	                         branch_name,
	                         _ggit_native_get (target),
	                         force ? 1 : 0);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return NULL;
	}

	return _ggit_oid_new (&oid);
}

/**
 * ggit_repository_delete_branch:
 * @repository: a #GgitRepository.
 * @branch_name: the name of the branch.
 * @branch_type: a #GgitBranchType.
 * @error: a #GError.
 *
 * Deletes an existing branch reference.
 **/
void
ggit_repository_delete_branch (GgitRepository   *repository,
                               const gchar      *branch_name,
                               GgitBranchType    branch_type,
                               GError          **error)
{
	gint ret;

	g_return_if_fail (GGIT_IS_REPOSITORY (repository));
	g_return_if_fail (branch_name != NULL);
	g_return_if_fail (error == NULL || *error == NULL);

	ret = git_branch_delete (_ggit_native_get (repository),
	                         branch_name,
	                         branch_type);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
	}
}

/**
 * ggit_repository_move_branch:
 * @repository: a #GgitRepository.
 * @old_branch_name: the old name of the branch.
 * @new_branch_name: the new name of the branch.
 * @flags: a GgitCreateFlags.
 * @error: a #GError.
 *
 * Moves/renames an existing branch reference.
 **/
void
ggit_repository_move_branch (GgitRepository   *repository,
                             const gchar      *old_branch_name,
                             const gchar      *new_branch_name,
                             GgitCreateFlags   flags,
                             GError          **error)
{
	gboolean force;
	gint ret;

	g_return_if_fail (GGIT_IS_REPOSITORY (repository));
	g_return_if_fail (old_branch_name != NULL);
	g_return_if_fail (new_branch_name != NULL);
	g_return_if_fail (error == NULL || *error == NULL);

	force = flags & GGIT_CREATE_FORCE;

	ret = git_branch_move (_ggit_native_get (repository),
	                       old_branch_name,
	                       new_branch_name,
	                       force ? 1 : 0);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
	}
}

/**
 * ggit_repository_list_branches:
 * @repository: a #GgitRepository.
 * @branch_type: a GgitBranchType.
 * @error: a #GError.
 *
 * Fill a list with all the branches in the Repository.
 *
 * Returns: (transfer full) (allow-none): a list with the branches specified by @branch_type.
 **/
gchar **
ggit_repository_list_branches (GgitRepository  *repository,
                               GgitBranchType   branch_type,
                               GError         **error)
{
	gint ret;
	git_strarray branch_names;
	gchar **branches;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), FALSE);
	g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

	ret = git_branch_list (&branch_names,
	                       _ggit_native_get (repository),
	                       branch_type);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		branches = NULL;
	}
	else
	{
		branches = ggit_utils_get_str_array_from_git_strarray (&branch_names);
	}

	return branches;
}

/* ex:set ts=8 noet: */
