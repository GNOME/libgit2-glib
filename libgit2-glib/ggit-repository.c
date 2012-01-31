/*
 * ggit-repository.c
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2011 - Ignacio Casal Quinteiro
 *
 * libggit is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * libggit is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libggit; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA  02110-1301  USA
 */

#include <gio/gio.h>
#include <git2/errors.h>
#include <git2/repository.h>
#include <git2/refs.h>
#include <git2/status.h>

#include "ggit-error.h"
#include "ggit-oid.h"
#include "ggit-ref.h"
#include "ggit-repository.h"
#include "ggit-utils.h"

#define GGIT_REPOSITORY_GET_PRIVATE(object)(G_TYPE_INSTANCE_GET_PRIVATE((object), GGIT_TYPE_REPOSITORY, GgitRepositoryPrivate))

struct _GgitRepositoryPrivate
{
	git_repository *repository;
	gchar *path;
	gboolean is_bare;
	gboolean init;
	gchar *workdir;
};

enum
{
	PROP_0,
	PROP_PATH,
	PROP_IS_BARE,
	PROP_INIT,
	PROP_WORKDIR
};

static void         ggit_repository_initable_iface_init (GInitableIface  *iface);

G_DEFINE_TYPE_EXTENDED (GgitRepository, ggit_repository, G_TYPE_OBJECT,
                        0,
                        G_IMPLEMENT_INTERFACE (G_TYPE_INITABLE,
                                               ggit_repository_initable_iface_init))

static void
ggit_repository_finalize (GObject *object)
{
	GgitRepositoryPrivate *priv = GGIT_REPOSITORY (object)->priv;

	g_free (priv->path);
	git_repository_free (priv->repository);

	g_free (priv->workdir);

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
		case PROP_PATH:
			g_value_set_string (value, ggit_repository_get_path (repository));
			break;
		case PROP_IS_BARE:
			g_value_set_boolean (value, ggit_repository_is_bare (repository));
			break;
		case PROP_INIT:
			g_value_set_boolean (value, priv->init);
			break;
		case PROP_WORKDIR:
			g_value_set_string (value,
			                    ggit_repository_get_workdir (repository));
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
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
		case PROP_PATH:
			g_free (priv->path);
			priv->path = g_value_dup_string (value);
			break;
		case PROP_IS_BARE:
			priv->is_bare = g_value_get_boolean (value);
			break;
		case PROP_INIT:
			priv->init = g_value_get_boolean (value);
			break;
		case PROP_WORKDIR:
			g_free (priv->workdir);
			priv->workdir = g_value_dup_string (value);

			if (repository->priv->repository)
			{
				ggit_repository_set_workdir (repository,
				                             priv->workdir);
			}

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
	                                 PROP_PATH,
	                                 g_param_spec_string ("path",
	                                                      "Path to repository",
	                                                      "The path to the repository",
	                                                      NULL,
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
	                                 PROP_PATH,
	                                 g_param_spec_string ("workdir",
	                                                      "Path to repository working directory",
	                                                      "The path to the repository working directory",
	                                                      NULL,
	                                                      G_PARAM_READWRITE |
	                                                      G_PARAM_CONSTRUCT |
	                                                      G_PARAM_STATIC_STRINGS));

	g_type_class_add_private (object_class, sizeof (GgitRepositoryPrivate));
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

	if (cancellable != NULL)
	{
		g_set_error_literal (error, G_IO_ERROR, G_IO_ERROR_NOT_SUPPORTED,
		                     "Cancellable initialization not supported");
		return FALSE;
	}

	priv = GGIT_REPOSITORY (initable)->priv;

	if (priv->init == TRUE)
	{
		err = git_repository_init (&priv->repository,
		                           priv->path,
		                           priv->is_bare);
	}
	else if (priv->path != NULL)
	{
		err = git_repository_open (&priv->repository,
		                           priv->path);
	}

	if (err != GIT_SUCCESS)
	{
		g_set_error_literal (error, G_IO_ERROR, G_IO_ERROR_NOT_INITIALIZED,
		                     git_lasterror ());
		success = FALSE;
	}
	else if (priv->workdir)
	{
		git_repository_set_workdir (priv->repository, priv->workdir);
	}

	return success;
}

static void
ggit_repository_initable_iface_init (GInitableIface *iface)
{
	iface->init = ggit_repository_initable_init;
}

GgitRepository *
_ggit_repository_new (git_repository *repository)
{
	GgitRepository *rep;

	rep = g_object_new (GGIT_TYPE_REPOSITORY, NULL);
	rep->priv->repository = repository;

	return rep;
}

git_repository *
_ggit_repository_get_repository (GgitRepository *repository)
{
	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), NULL);

	return repository->priv->repository;
}

/**
 * ggit_repository_open:
 * @path: the path to the repository.
 * @error: (error-domains GIOError): a #GError for error reporting, or %NULL.
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
ggit_repository_open (const gchar  *path,
                      GError      **error)
{
	g_return_val_if_fail (path != NULL, NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	return g_initable_new (GGIT_TYPE_REPOSITORY, NULL, error,
	                       "path", path,
	                       NULL);
}

/**
 * ggit_repository_init_repository:
 * @path: the path to the repository.
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
ggit_repository_init_repository (const gchar  *path,
                                 gboolean      is_bare,
                                 GError      **error)
{
	g_return_val_if_fail (path != NULL, NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	return g_initable_new (GGIT_TYPE_REPOSITORY, NULL, error,
	                       "path", path,
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

	ret = git_object_lookup (&obj, repository->priv->repository,
	                         id, otype);

	if (ret == GIT_SUCCESS)
	{
		object = ggit_utils_create_real_object (obj);
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

	ret = git_reference_lookup (&reference, repository->priv->repository,
	                            name);
	if (ret == GIT_SUCCESS)
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

	ret = git_reference_create_oid (&reference, repository->priv->repository,
	                                name, _ggit_oid_get_oid (oid), FALSE);
	if (ret == GIT_SUCCESS)
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

	ret = git_reference_create_symbolic (&reference, repository->priv->repository,
	                                     name, target, FALSE);

	if (ret == GIT_SUCCESS)
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

	ret = git_repository_head (&reference, repository->priv->repository);

	if (ret == GIT_SUCCESS)
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
 * @path: the base path where the lookup starts.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Looks for a git repository.
 *
 * The lookup starts from @path and walks up the parent directories
 * and stops when a repository is found.
 *
 * Returns: (transfer full): the repository path.
 */
gchar *
ggit_repository_discover (const gchar  *path,
                          GError      **error)
{
	gchar found_path[GIT_PATH_MAX];
	gchar *rep_path = NULL;
	gint ret;

	g_return_val_if_fail (path != NULL || *path == '\0', NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	ret = git_repository_discover (found_path, sizeof (found_path),
	                               path, 0, "");

	if (ret == GIT_SUCCESS)
	{
		rep_path = g_strdup (found_path);
	}
	else
	{
		_ggit_error_set (error, ret);
	}

	return rep_path;
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

	ret = git_repository_head_detached (repository->priv->repository);

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

	ret = git_repository_head_orphan (repository->priv->repository);

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

	ret = git_repository_is_empty (repository->priv->repository);

	if (ret < 0)
	{
		_ggit_error_set (error, ret);
	}

	return ret == 1;
}

/**
 * ggit_repository_get_path:
 * @repository: a #GgitRepository.
 *
 * Get the gitdir path of the repository.
 *
 * Returns: the absolute path of the gitdir of the repository.
 */
const gchar *
ggit_repository_get_path (GgitRepository *repository)
{
	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), NULL);

	return git_repository_path (repository->priv->repository);
}

/**
 * ggit_repository_get_workdir:
 * @repository: a #GgitRepository.
 *
 * Gets the working directory of the repository.
 *
 * Returns: the absolute path of working directory of the repository.
 */
const gchar *
ggit_repository_get_workdir (GgitRepository *repository)
{
	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), NULL);

	return git_repository_workdir (repository->priv->repository);
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
                             const gchar    *workdir)
{
	g_return_if_fail (GGIT_IS_REPOSITORY (repository));

	git_repository_set_workdir (repository->priv->repository, workdir);
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

	return git_repository_is_bare (repository->priv->repository);
}

/**
 * ggit_repository_file_status:
 * @repository: a #GgitRepository.
 * @path: the file to retrieve status for, rooted at the repository working dir.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Gets the file status for a single file.
 *
 * Returns: the status for a single file.
 */
GgitStatusFlags
ggit_repository_file_status (GgitRepository  *repository,
                             const gchar     *path,
                             GError         **error)
{
	GgitStatusFlags status_flags;
	gint ret;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), GGIT_STATUS_IGNORED);
	g_return_val_if_fail (path != NULL, GGIT_STATUS_IGNORED);
	g_return_val_if_fail (error == NULL || *error == NULL, GGIT_STATUS_IGNORED);

	ret = git_status_file (&status_flags, repository->priv->repository,
	                       path);
	if (ret != GIT_SUCCESS)
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
 */
void
ggit_repository_file_status_foreach (GgitRepository     *repository,
                                     GgitStatusCallback  callback,
                                     gpointer            user_data,
                                     GError            **error)
{
	gint ret;

	g_return_if_fail (GGIT_IS_REPOSITORY (repository));
	g_return_if_fail (callback != NULL);
	g_return_if_fail (error == NULL || *error == NULL);

	ret = git_status_foreach (repository->priv->repository, callback,
	                          user_data);
	if (ret != GIT_SUCCESS)
	{
		_ggit_error_set (error, ret);
	}
}

/* ex:set ts=8 noet: */
