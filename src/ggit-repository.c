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

#include "ggit-repository.h"
#include "ggit-error.h"

#define GGIT_REPOSITORY_GET_PRIVATE(object)(G_TYPE_INSTANCE_GET_PRIVATE((object), GIT_TYPE_GLIB_REPOSITORY, GgitRepositoryPrivate))

struct _GgitRepositoryPrivate
{
	git_repository *repository;
	gchar *path;
	gboolean is_bare;
	gboolean init;
};

enum
{
	PROP_0,
	PROP_PATH,
	PROP_IS_BARE,
	PROP_INIT
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
			g_value_set_string (value, ggit_repository_path (repository,
			                                                 GGIT_REPO_PATH));
			break;
		case PROP_IS_BARE:
			g_value_set_boolean (value, ggit_repository_is_bare (repository));
			break;
		case PROP_INIT:
			g_value_set_boolean (value, priv->init);
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
	GgitRepositoryPrivate *priv = GGIT_REPOSITORY (object)->priv;

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
	                                                       "Wether to initialize a repository",
	                                                       FALSE,
	                                                       G_PARAM_READWRITE |
	                                                       G_PARAM_CONSTRUCT_ONLY |
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

	if (err != 0)
	{
		g_set_error_literal (error, G_IO_ERROR, G_IO_ERROR_NOT_INITIALIZED,
		                     git_lasterror ());
		success = FALSE;
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

	rep = g_object_new (GIT_TYPE_GLIB_REPOSITORY, NULL);
	rep->priv->repository = repository;

	return rep;
}

/**
 * ggit_repository_open:
 * @path: the path to the repository
 * @error: #GError for error reporting, or %NULL
 *
 * Open a git repository.
 *
 * The 'path' argument must point to an existing git repository
 * folder, e.g.
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
 *	The method will automatically detect if 'path' is a normal
 *	or bare repository or fail is 'path' is neither.
 *
 * Returns: (transfer full): a newly created #GgitRepository
 */
GgitRepository *
ggit_repository_open (const gchar *path,
                      GError     **error)
{
	return g_initable_new (GIT_TYPE_GLIB_REPOSITORY, NULL, error,
	                       "path", path, NULL);
}

/**
 * ggit_repository_init_repository:
 * @path: the path to the repository
 * @is_bare: if %TRUE, a Git repository without a working directory is created
 *           at the pointed path. If %FALSE, provided path will be considered as the working
 *           directory into which the .git directory will be created.
 * @error: #GError for error reporting, or %NULL
 *
 * Creates a new Git repository in the given folder.
 *
 * TODO:
 *	- Reinit the repository
 *	- Create config files
 *
 * Returns: (transfer full): a newly created #GgitRepository
 */
GgitRepository *
ggit_repository_init_repository (const gchar *path,
                                 gboolean     is_bare,
                                 GError     **error)
{
	return g_initable_new (GIT_TYPE_GLIB_REPOSITORY, NULL, error,
	                       "path", path,
	                       "is-bare", is_bare,
	                       "init", TRUE,
	                       NULL);
}

/**
 * ggit_repository_head_detached:
 * @repository: a #GgitRepository
 * @error: #GError for error reporting, or %NULL
 *
 * Check if a repository's HEAD is detached
 *
 * A repository's HEAD is detached when it points directly to a commit
 * instead of a branch.
 *
 * Returns: %TRUE if HEAD is detached.
 */
gboolean
ggit_repository_head_detached (GgitRepository *repository,
                               GError        **error)
{
	gint ret;

	g_return_val_if_fail (GIT_IS_GLIB_REPOSITORY (repository), FALSE);

	ret = git_repository_head_detached (repository->priv->repository);

	if (ret < 0)
	{
		_ggit_error_set (error, ret);
	}

	return ret == 1;
}

/**
 * ggit_repository_head_orphan:
 * @repository: a #GgitRepository
 * @error: #GError for error reporting, or %NULL
 *
 * Check if the current branch is an orphan
 *
 * An orphan branch is one named from HEAD but which doesn't exist in
 * the refs namespace, because it doesn't have any commit to point to.
 *
 * Returns: %TRUE if the current branch is an orphan.
 */
gboolean
ggit_repository_head_orphan (GgitRepository *repository,
                             GError        **error)
{
	gint ret;

	g_return_val_if_fail (GIT_IS_GLIB_REPOSITORY (repository), FALSE);

	ret = git_repository_head_orphan (repository->priv->repository);

	if (ret < 0)
	{
		_ggit_error_set (error, ret);
	}

	return ret == 1;
}

/**
 * ggit_repository_is_empty:
 * @repository: a #GgitRepository
 * @error: #GError for error reporting, or %NULL
 *
 * Check if a repository is empty
 *
 * An empty repository has just been initialized and contains
 * no commits.
 *
 * Returns: %TRUE if the repository is empty.
 */
gboolean
ggit_repository_is_empty (GgitRepository *repository,
                          GError        **error)
{
	gint ret;

	g_return_val_if_fail (GIT_IS_GLIB_REPOSITORY (repository), FALSE);

	ret = git_repository_is_empty (repository->priv->repository);

	if (ret < 0)
	{
		_ggit_error_set (error, ret);
	}

	return ret == 1;
}

/**
 * ggit_repository_path:
 * @repository: a #GgitRepository
 * @id: The ID of the path to return
 *
 * Get one of the paths to the repository
 *
 * Possible values for `id`:
 *
 *	GIT_REPO_PATH: return the path to the repository
 *	GIT_REPO_PATH_INDEX: return the path to the index
 *	GIT_REPO_PATH_ODB: return the path to the ODB
 *	GIT_REPO_PATH_WORKDIR: return the path to the working
 *		directory
 *
 * Returns: absolute path of the requested id
 */
const gchar *
ggit_repository_path (GgitRepository      *repository,
                      GgitRepositoryPathid id)
{
	g_return_val_if_fail (GIT_IS_GLIB_REPOSITORY (repository), NULL);

	return git_repository_path (repository->priv->repository, id);
}

/**
 * ggit_repository_is_bare:
 * @repository: a #GgitRepository
 *
 * Check if a repository is bare
 *
 * Returns: %TRUE if the repository is empty.
 */
gboolean
ggit_repository_is_bare (GgitRepository  *repository)
{
	g_return_val_if_fail (GIT_IS_GLIB_REPOSITORY (repository), FALSE);

	return git_repository_is_bare (repository->priv->repository);
}
