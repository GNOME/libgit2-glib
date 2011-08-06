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
#include <git2/repository.h>
#include <gio/gio.h>

#include <errno.h>


#define GIT_GLIB_REPOSITORY_GET_PRIVATE(object)(G_TYPE_INSTANCE_GET_PRIVATE((object), GIT_TYPE_GLIB_REPOSITORY, GitGlibRepositoryPrivate))

struct _GitGlibRepositoryPrivate
{
	git_repository *repository;
	gchar *path;
};

enum
{
	PROP_0,
	PROP_PATH
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
git_glib_repository_class_init (GitGlibRepositoryClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->finalize = git_glib_repository_finalize;

	g_object_class_install_property (object_class,
	                                 PROP_PATH,
	                                 g_param_spec_string ("path",
	                                                      "Path to repository",
	                                                      "The path to the repository",
	                                                      NULL,
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
	GitGlibRepository *repository;
	gboolean success = TRUE;
	gint err;

	if (cancellable != NULL)
	{
		g_set_error_literal (error, G_IO_ERROR, G_IO_ERROR_NOT_SUPPORTED,
		                     "Cancellable initialization not supported");
		return FALSE;
	}

	repository = GIT_GLIB_REPOSITORY (initable);

	if (repository->priv->path != NULL)
	{
		err = git_repository_open (&repository->priv->repository,
		                           repository->priv->path);
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
 * @Returns: a newly created #GitGlibRepository
 */
GitGlibRepository *
git_glib_repository_open (const gchar *path,
                          GError     **error)
{
	return g_initable_new (GIT_TYPE_GLIB_REPOSITORY, NULL, error,
	                       "path", path, NULL);
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


