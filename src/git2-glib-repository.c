/*
 * git2-glib-repository.c
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


#include "git2-glib-repository.h"
#include <git2/repository.h>
#include <gio/gio.h>


#define GIT2_GLIB_REPOSITORY_GET_PRIVATE(object)(G_TYPE_INSTANCE_GET_PRIVATE((object), GIT2_TYPE_GLIB_REPOSITORY, Git2GlibRepositoryPrivate))

struct _Git2GlibRepositoryPrivate
{
	git_repository *repository;
	gchar *path;
};

enum
{
	PROP_0,
	PROP_PATH
};

static void         git2_glib_repository_initable_iface_init (GInitableIface  *iface);

G_DEFINE_TYPE_EXTENDED (Git2GlibRepository, git2_glib_repository, G_TYPE_OBJECT,
                        0,
                        G_IMPLEMENT_INTERFACE (G_TYPE_INITABLE,
                                               git2_glib_repository_initable_iface_init))

static void
git2_glib_repository_finalize (GObject *object)
{
	Git2GlibRepositoryPrivate *priv = GIT2_GLIB_REPOSITORY (object)->priv;

	g_free (priv->path);
	git_repository_free (priv->repository);

	G_OBJECT_CLASS (git2_glib_repository_parent_class)->finalize (object);
}

static void
git2_glib_repository_class_init (Git2GlibRepositoryClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->finalize = git2_glib_repository_finalize;

	g_object_class_install_property (object_class,
	                                 PROP_PATH,
	                                 g_param_spec_string ("path",
	                                                      "Path to repository",
	                                                      "The path to the repository",
	                                                      NULL,
	                                                      G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY |
	                                                      G_PARAM_STATIC_STRINGS));

	g_type_class_add_private (object_class, sizeof (Git2GlibRepositoryPrivate));
}

static void
git2_glib_repository_init (Git2GlibRepository *repository)
{
	repository->priv = GIT2_GLIB_REPOSITORY_GET_PRIVATE (repository);
}

static gboolean
git2_glib_repository_initable_init (GInitable *initable,
                                    GCancellable *cancellable,
                                    GError  **error)
{
	Git2GlibRepository *repository;
	gboolean success = TRUE;
	gint errno;

	if (cancellable != NULL)
	{
		g_set_error_literal (error, G_IO_ERROR, G_IO_ERROR_NOT_SUPPORTED,
		                     "Cancellable initialization not supported");
		return FALSE;
	}

	repository = GIT2_GLIB_REPOSITORY (initable);

	if (repository->priv->path != NULL)
	{
		errno = git_repository_open (&repository->priv->repository,
		                             repository->priv->path);
	}

	if (errno != 0)
	{
		g_set_error (error, G_IO_ERROR, G_IO_ERROR_NOT_INITIALIZED,
		             "Failed to open repository: %s",
		             g_strerror (errno));
		success = FALSE;
	}

	return success;
}

static void
git2_glib_repository_initable_iface_init (GInitableIface *iface)
{
	iface->init = git2_glib_repository_initable_init;
}

Git2GlibRepository *
git2_glib_repository_open (const gchar *path,
                           GError     **error)
{
	return g_initable_new (GIT2_TYPE_GLIB_REPOSITORY, NULL, error,
	                       "path", path, NULL);
}
