/*
 * git-glib-repository.c
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2011 - Ignacio Casal Quinteiro
 *
 * libgit-glib is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * libgit-glib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libgit-glib; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA  02110-1301  USA
 */


#include "git-glib-repository.h"
#include <gio/gio.h>

#include <errno.h>


#define GIT_GLIB_REPOSITORY_GET_PRIVATE(object)(G_TYPE_INSTANCE_GET_PRIVATE((object), GIT_TYPE_GLIB_REPOSITORY, GitGlibRepositoryPrivate))

struct _GitGlibRepositoryPrivate
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

static void         git_glib_repository_initable_iface_init (GInitableIface  *iface);

G_DEFINE_TYPE_EXTENDED (GitGlibRepository, git_glib_repository, G_TYPE_OBJECT,
                        0,
                        G_IMPLEMENT_INTERFACE (G_TYPE_INITABLE,
                                               git_glib_repository_initable_iface_init))

static void
git_glib_repository_finalize (GObject *object)
{
	GitGlibRepositoryPrivate *priv = GIT_GLIB_REPOSITORY (object)->priv;

	g_free (priv->path);
	git_repository_free (priv->repository);

	G_OBJECT_CLASS (git_glib_repository_parent_class)->finalize (object);
}

static void
git_glib_repository_get_property (GObject    *object,
                                  guint       prop_id,
                                  GValue     *value,
                                  GParamSpec *pspec)
{
	GitGlibRepositoryPrivate *priv = GIT_GLIB_REPOSITORY (object)->priv;

	switch (prop_id)
	{
		case PROP_PATH:
			g_value_set_string (value, priv->path);
			break;
		case PROP_IS_BARE:
			g_value_set_boolean (value, priv->is_bare);
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
git_glib_repository_set_property (GObject      *object,
                                  guint         prop_id,
                                  const GValue *value,
                                  GParamSpec   *pspec)
{
	GitGlibRepositoryPrivate *priv = GIT_GLIB_REPOSITORY (object)->priv;

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
git_glib_repository_class_init (GitGlibRepositoryClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->finalize = git_glib_repository_finalize;
	object_class->get_property = git_glib_repository_get_property;
	object_class->set_property = git_glib_repository_set_property;

	g_object_class_install_property (object_class,
	                                 PROP_PATH,
	                                 g_param_spec_string ("path",
	                                                      "Path to repository",
	                                                      "The path to the repository",
	                                                      NULL,
	                                                      G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY |
	                                                      G_PARAM_STATIC_STRINGS));

	g_object_class_install_property (object_class,
	                                 PROP_IS_BARE,
	                                 g_param_spec_boolean ("is-bare",
	                                                       "Is bare",
	                                                       "Is a bare repository",
	                                                       FALSE,
	                                                       G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY |
	                                                       G_PARAM_STATIC_STRINGS));

	g_object_class_install_property (object_class,
	                                 PROP_INIT,
	                                 g_param_spec_boolean ("init",
	                                                       "Init",
	                                                       "Wether to initalize a repository",
	                                                       FALSE,
	                                                       G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY |
	                                                       G_PARAM_STATIC_STRINGS));

	g_type_class_add_private (object_class, sizeof (GitGlibRepositoryPrivate));
}

static void
git_glib_repository_init (GitGlibRepository *repository)
{
	repository->priv = GIT_GLIB_REPOSITORY_GET_PRIVATE (repository);
}

static gboolean
git_glib_repository_initable_init (GInitable *initable,
                                   GCancellable *cancellable,
                                   GError  **error)
{
	GitGlibRepositoryPrivate *priv;
	gboolean success = TRUE;
	gint err;

	if (cancellable != NULL)
	{
		g_set_error_literal (error, G_IO_ERROR, G_IO_ERROR_NOT_SUPPORTED,
		                     "Cancellable initialization not supported");
		return FALSE;
	}

	priv = GIT_GLIB_REPOSITORY (initable)->priv;

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
		g_set_error (error, G_IO_ERROR, G_IO_ERROR_NOT_INITIALIZED,
		             "Failed to open repository: %s",
		             g_strerror (err));
		success = FALSE;
	}

	return success;
}

static void
git_glib_repository_initable_iface_init (GInitableIface *iface)
{
	iface->init = git_glib_repository_initable_init;
}

GQuark
git_glib_repository_error_quark (void)
{
	static GQuark quark = 0;

	if (G_UNLIKELY (quark == 0))
		quark = g_quark_from_static_string ("git-glib-repository-error");

	return quark;
}

/**
 * git_glib_repository_open:
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
 * @Returns: (transfer full): a newly created #GitGlibRepository
 */
GitGlibRepository *
git_glib_repository_open (const gchar *path,
                          GError     **error)
{
	return g_initable_new (GIT_TYPE_GLIB_REPOSITORY, NULL, error,
	                       "path", path, NULL);
}

/**
 * git_glib_repository_init_repository:
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
 * @Returns: (transfer full): a newly created #GitGlibRepository
 */
GitGlibRepository *
git_glib_repository_init_repository (const gchar *path,
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
 * git_glib_repository_head_detached:
 * @repository: a #GitGlibRepository
 * @error: #GError for error reporting, or %NULL
 *
 * Check if a repository's HEAD is detached
 *
 * A repository's HEAD is detached when it points directly to a commit
 * instead of a branch.
 *
 * @Returns: %TRUE if HEAD is detached.
 */
gboolean
git_glib_repository_head_detached (GitGlibRepository *repository,
                                   GError           **error)
{
	gboolean ret;

	g_return_val_if_fail (GIT_IS_GLIB_REPOSITORY (repository), FALSE);

	ret = git_repository_head_detached (repository->priv->repository);

	if (errno != -1)
	{
		g_set_error (error, GIT_GLIB_REPOSITORY_ERROR,
		             GIT_GLIB_REPOSITORY_ERROR_REPORTING,
		             "Error: %s",
		             g_strerror (errno));
	}

	return ret;
}

/**
 * git_glib_repository_head_orphan:
 * @repository: a #GitGlibRepository
 * @error: #GError for error reporting, or %NULL
 *
 * Check if the current branch is an orphan
 *
 * An orphan branch is one named from HEAD but which doesn't exist in
 * the refs namespace, because it doesn't have any commit to point to.
 *
 * @Returns: %TRUE if the current branch is an orphan.
 */
gboolean
git_glib_repository_head_orphan (GitGlibRepository *repository,
                                 GError           **error)
{
	gboolean ret;

	g_return_val_if_fail (GIT_IS_GLIB_REPOSITORY (repository), FALSE);

	ret = git_repository_head_orphan (repository->priv->repository);

	if (errno != -1)
	{
		g_set_error (error, GIT_GLIB_REPOSITORY_ERROR,
		             GIT_GLIB_REPOSITORY_ERROR_REPORTING,
		             "Error: %s",
		             g_strerror (errno));
	}

	return ret;
}

/**
 * git_glib_repository_is_empty:
 * @repository: a #GitGlibRepository
 * @error: #GError for error reporting, or %NULL
 *
 * Check if a repository is empty
 *
 * An empty repository has just been initialized and contains
 * no commits.
 *
 * @Returns: %TRUE if the repository is empty.
 */
gboolean
git_glib_repository_is_empty (GitGlibRepository *repository,
                              GError           **error)
{
	gboolean ret;

	g_return_val_if_fail (GIT_IS_GLIB_REPOSITORY (repository), FALSE);

	ret = git_repository_is_empty (repository->priv->repository);

	if (errno != -1)
	{
		g_set_error (error, GIT_GLIB_REPOSITORY_ERROR,
		             GIT_GLIB_REPOSITORY_ERROR_REPORTING,
		             "Error: %s",
		             g_strerror (errno));
	}

	return ret;
}

/**
 * git_glib_repository_path:
 * @repository: a #GitGlibRepository
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
 * @Returns: absolute path of the requested id
 */
const gchar *
git_glib_repository_path (GitGlibRepository  *repository,
                          GitRepositoryPathid id)
{
	g_return_val_if_fail (GIT_IS_GLIB_REPOSITORY (repository), NULL);

	return git_repository_path (repository->priv->repository, id);
}

/**
 * git_glib_repository_is_bare:
 * @repository: a #GitGlibRepository
 *
 * Check if a repository is bare
 *
 * @Returns: %TRUE if the repository is empty.
 */
gboolean
git_glib_repository_is_bare (GitGlibRepository  *repository)
{
	g_return_val_if_fail (GIT_IS_GLIB_REPOSITORY (repository), FALSE);

	return git_repository_is_bare (repository->priv->repository);
}
