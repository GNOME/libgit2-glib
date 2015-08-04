/*
 * ggit-clone-options.c
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2013 - Ignacio Casal Quinteiro
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
#include <gio/gio.h>

#include "ggit-clone-options.h"
#include "ggit-transfer-progress.h"
#include "ggit-native.h"
#include "ggit-remote.h"
#include "ggit-repository.h"

typedef struct _GgitCloneOptionsPrivate
{
	git_clone_options native;
	GgitFetchOptions *fetch_options;
} GgitCloneOptionsPrivate;

/**
 * GgitCloneOptionsClass::create_repository:
 * @options: a #GgitCloneOptions.
 * @path: the repository path.
 * @is_bare: whether a bare repository should be created.
 * @error: a #GError for error reporting.
 *
 * Returns: (transfer full) (nullable): a #GgitRepository or %NULL in case of an error.
 */

/**
 * GgitCloneOptionsClass::create_remote:
 * @options: a #GgitCloneOptions.
 * @repository: the repository.
 * @name: the remote name.
 * @url: the remote url.
 * @error: a #GError for error reporting.
 *
 * Returns: (transfer full) (nullable): a #GgitRemote or %NULL in case of an error.
 */

G_DEFINE_TYPE_WITH_PRIVATE (GgitCloneOptions, ggit_clone_options, G_TYPE_OBJECT)

const git_clone_options *
_ggit_clone_options_get_native (GgitCloneOptions *options)
{
	GgitCloneOptionsPrivate *priv;

	/* NULL is common for clone_options as it specifies to use the default
	 * so handle a NULL clone_options here instead of in every caller.
	 */
	if (options == NULL)
	{
		return NULL;
	}

	g_return_val_if_fail (GGIT_IS_CLONE_OPTIONS (options), NULL);

	priv = ggit_clone_options_get_instance_private (options);

	return (const git_clone_options *)&priv->native;
}

static void
ggit_clone_options_finalize (GObject *object)
{
	GgitCloneOptions *options = GGIT_CLONE_OPTIONS (object);
	GgitCloneOptionsPrivate *priv;

	priv = ggit_clone_options_get_instance_private (options);

	g_free ((gchar *)priv->native.checkout_branch);

	ggit_fetch_options_free (priv->fetch_options);

	priv->native.repository_cb = NULL;
	priv->native.repository_cb_payload = NULL;

	priv->native.remote_cb = NULL;
	priv->native.remote_cb_payload = NULL;

	G_OBJECT_CLASS (ggit_clone_options_parent_class)->finalize (object);
}

static GgitRepository *
create_repository_default (GgitCloneOptions  *options,
                           const gchar       *path,
                           gboolean           is_bare,
                           GError           **error)
{
	GFile *location;
	GgitRepository *ret;

	location = g_file_new_for_path (path);
	ret = ggit_repository_init_repository (location, is_bare, error);
	g_object_unref (location);

	return ret;
}

static GgitRemote *
create_remote_default (GgitCloneOptions  *options,
                       GgitRepository    *repository,
                       const gchar       *name,
                       const gchar       *url,
                       GError           **error)
{
	return ggit_remote_new (repository, name, url, error);
}

static void
ggit_clone_options_class_init (GgitCloneOptionsClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->finalize = ggit_clone_options_finalize;

	klass->create_repository = create_repository_default;
	klass->create_remote = create_remote_default;
}

static gint
create_repository_wrapper (git_repository **out,
                           const char      *path,
                           gint             is_bare,
                           void            *payload)
{
	GgitCloneOptions *options = GGIT_CLONE_OPTIONS (payload);
	GgitCloneOptionsClass *cls = GGIT_CLONE_OPTIONS_GET_CLASS (options);
	GError *error = NULL;
	GgitRepository *repository;

	repository = cls->create_repository(options, path, is_bare ? TRUE : FALSE, &error);

	if (error != NULL)
	{
		giterr_set_str (GIT_ERROR, error->message);
		g_error_free (error);

		if (repository != NULL)
		{
			g_object_unref (repository);
		}

		return GIT_ERROR;
	}

	if (repository != NULL)
	{
		*out = _ggit_native_release (repository);
		g_object_unref (repository);
	}

	return GIT_OK;
}

static gint
create_remote_wrapper (git_remote     **out,
                       git_repository  *repo,
                       const char      *name,
                       const char      *url,
                       void            *payload)
{
	GgitCloneOptions *options = GGIT_CLONE_OPTIONS (payload);
	GgitCloneOptionsClass *cls = GGIT_CLONE_OPTIONS_GET_CLASS (options);
	GgitRepository *repository = _ggit_repository_wrap (repo, FALSE);
	GError *error = NULL;
	GgitRemote *remote;

	remote = cls->create_remote(options, repository, name, url, &error);
	g_object_unref (repository);

	if (error)
	{
		giterr_set_str (GIT_ERROR, error->message);
		g_error_free (error);

		if (remote != NULL)
		{
			g_object_unref (remote);
		}

		return GIT_ERROR;
	}

	if (remote != NULL)
	{
		*out = _ggit_native_release (GGIT_NATIVE (remote));
		g_object_unref (remote);
	}

	return GIT_OK;
}

static void
ggit_clone_options_init (GgitCloneOptions *options)
{
	git_clone_options native = GIT_CLONE_OPTIONS_INIT;
	GgitCloneOptionsPrivate *priv;

	native.repository_cb = create_repository_wrapper;
	native.repository_cb_payload = options;

	native.remote_cb = create_remote_wrapper;
	native.remote_cb_payload = options;

	priv = ggit_clone_options_get_instance_private (options);
	priv->native = native;
}

/**
 * ggit_clone_options_new:
 *
 * Creates a new #GgitCloneOptions.
 *
 * Returns: a newly allocated #GgitCloneOptions.
 */
GgitCloneOptions *
ggit_clone_options_new (void)
{
	return GGIT_CLONE_OPTIONS (g_object_new (GGIT_TYPE_CLONE_OPTIONS, NULL));
}

/**
 * ggit_clone_options_get_is_bare:
 * @options: a #GgitCloneOptions.
 *
 * Gets if the repository will be bare.
 *
 * Returns: %TRUE to clone a bare repository.
 */
gboolean
ggit_clone_options_get_is_bare (GgitCloneOptions *options)
{
	GgitCloneOptionsPrivate *priv;

	g_return_val_if_fail (GGIT_IS_CLONE_OPTIONS (options), FALSE);

	priv = ggit_clone_options_get_instance_private (options);
	return priv->native.bare ? TRUE : FALSE;
}

/**
 * ggit_clone_options_set_is_bare:
 * @options: a #GgitCloneOptions.
 * @bare: %TRUE to clone a bare repository.
 *
 * Sets whether to clone a bare repository.
 */
void
ggit_clone_options_set_is_bare (GgitCloneOptions *options,
                                gboolean          bare)
{
	GgitCloneOptionsPrivate *priv;

	g_return_if_fail (GGIT_IS_CLONE_OPTIONS (options));

	priv = ggit_clone_options_get_instance_private (options);
	priv->native.bare = (gint)bare;
}

/**
 * ggit_clone_options_get_checkout_branch:
 * @options: a #GgitCloneOptions.
 *
 * Gets the name of the branch to checkout or %NULL.
 *
 * Returns: the name of the branch to checkout or %NULL.
 */
const gchar *
ggit_clone_options_get_checkout_branch (GgitCloneOptions *options)
{
	GgitCloneOptionsPrivate *priv;

	g_return_val_if_fail (GGIT_IS_CLONE_OPTIONS (options), NULL);

	priv = ggit_clone_options_get_instance_private (options);
	return priv->native.checkout_branch;
}

/**
 * ggit_clone_options_set_checkout_branch:
 * @options: a #GgitCloneOptions.
 * @checkout_branch: (allow-none): the name of the branch to checkout or %NULL.
 *
 * Gives the name of the branch to checkout. %NULL means
 * use the remote's HEAD.
 */
void
ggit_clone_options_set_checkout_branch (GgitCloneOptions *options,
                                        const gchar      *checkout_branch)
{
	GgitCloneOptionsPrivate *priv;

	g_return_if_fail (GGIT_IS_CLONE_OPTIONS (options));

	priv = ggit_clone_options_get_instance_private (options);
	priv->native.checkout_branch = g_strdup (checkout_branch);
}

/**
 * ggit_clone_options_get_fetch_options:
 * @options: a #GgitCloneOptions.
 *
 * Get the fetch options object or %NULL if not set.
 *
 * Returns: (transfer none): the fetch options or %NULL.
 */
GgitFetchOptions *
ggit_clone_options_get_fetch_options (GgitCloneOptions *options)
{
	GgitCloneOptionsPrivate *priv;

	g_return_val_if_fail (GGIT_IS_CLONE_OPTIONS (options), NULL);

	priv = ggit_clone_options_get_instance_private (options);
	return priv->fetch_options;
}

/**
 * ggit_clone_options_set_fetch_options:
 * @options: a #GgitCloneOptions.
 * @fetch_options: (allow-none): a #GgitFetchOptions or %NULL.
 *
 * Set the fetch options object.
 */
void
ggit_clone_options_set_fetch_options (GgitCloneOptions *options,
                                      GgitFetchOptions *fetch_options)
{
	GgitCloneOptionsPrivate *priv;

	g_return_if_fail (GGIT_IS_CLONE_OPTIONS (options));

	priv = ggit_clone_options_get_instance_private (options);

	g_clear_object (&priv->fetch_options);

	if (fetch_options != NULL)
	{
		priv->fetch_options = ggit_fetch_options_copy (fetch_options);
		priv->native.fetch_opts = *_ggit_fetch_options_get_fetch_options (fetch_options);
	}
	else
	{
		git_fetch_options i = GIT_FETCH_OPTIONS_INIT;
		priv->native.fetch_opts = i;
	}
}

/**
 * ggit_clone_options_get_local:
 * @options: a #GgitCloneOptions.
 *
 * Get setting for bypassing the git-aware transport when cloning. The
 * default auto setting bypasses the git-aware transport for local paths,
 * but use a normal fetch for file:// URIs.
 *
 * Returns: the local clone setting.
 *
 **/
GgitCloneLocal
ggit_clone_options_get_local (GgitCloneOptions *options)
{
	GgitCloneOptionsPrivate *priv;

	g_return_val_if_fail (GGIT_IS_CLONE_OPTIONS (options), 0);

	priv = ggit_clone_options_get_instance_private (options);
	return (GgitCloneLocal)priv->native.local;
}

/**
 * ggit_clone_options_set_local:
 * @options: a #GgitCloneOptions.
 * @local: the local clone setting.
 *
 * Set setting for bypassing the git-aware transport when cloning. The
 * default auto setting bypasses the git-aware transport for local paths,
 * but use a normal fetch for file:// URIs.
 *
 **/
void
ggit_clone_options_set_local (GgitCloneOptions *options,
                              GgitCloneLocal    local)
{
	GgitCloneOptionsPrivate *priv;

	g_return_if_fail (GGIT_IS_CLONE_OPTIONS (options));

	priv = ggit_clone_options_get_instance_private (options);
	priv->native.local = (git_clone_local_t)local;
}


/* ex:set ts=8 noet: */
