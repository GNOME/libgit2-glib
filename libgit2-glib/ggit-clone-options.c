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

#include "ggit-clone-options.h"
#include "ggit-transfer-progress.h"
#include "ggit-native.h"

struct _GgitCloneOptions
{
	git_clone_options clone_options;
	GgitCredAcquireCallback cred_callback;
	gpointer cred_user_data;
	GgitTransferProgressCallback fetch_progress_callback;
	gpointer fetch_progress_user_data;
};

G_DEFINE_BOXED_TYPE (GgitCloneOptions, ggit_clone_options,
                     ggit_clone_options_copy, ggit_clone_options_free)

const git_clone_options *
_ggit_clone_options_get_clone_options (GgitCloneOptions *clone_options)
{
	/* NULL is common for clone_options as it specifies to use the default
	 * so handle a NULL clone_options here instead of in every caller.
	 */
	if (clone_options == NULL)
	{
		return NULL;
	}

	return (const git_clone_options *)&clone_options->clone_options;
}

static gint
wrap_cred_acquire (git_cred **cred,
                   const char *url,
                   const char *username_from_url,
                   unsigned int allowed_types,
                   void *payload)
{
	GgitCred *gcred;
	GgitCloneOptions *options = GGIT_CLONE_OPTIONS (payload);
	gint ret;

	ret= options->cred_callback (url, username_from_url, allowed_types, &gcred,
	                             options->cred_user_data);

	*cred = NULL;

	if (gcred != NULL)
	{
		*cred = _ggit_native_get (gcred);

		// NOTE: this is to be able to unref the cred and do not
		// free wrapped object
		_ggit_native_set_destroy_func (gcred, NULL);
		g_object_unref (gcred);
	}

	return ret;
}

/**
 * ggit_clone_options_copy:
 * @clone_options: a #GgitCloneOptions.
 *
 * Copies @clone_options into a newly allocated #GgitCloneOptions.
 *
 * Returns: (transfer full): a newly allocated #GgitCloneOptions.
 */
GgitCloneOptions *
ggit_clone_options_copy (GgitCloneOptions *clone_options)
{
	GgitCloneOptions *new_clone_options;
	git_clone_options *gclone_options;
	git_clone_options gnew_clone_options = GIT_CLONE_OPTIONS_INIT;

	g_return_val_if_fail (clone_options != NULL, NULL);

	gclone_options = &clone_options->clone_options;

	new_clone_options = g_slice_new (GgitCloneOptions);

	gnew_clone_options.bare = gclone_options->bare;
	gnew_clone_options.fetch_progress_cb = gclone_options->fetch_progress_cb;
	gnew_clone_options.fetch_progress_payload = gclone_options->fetch_progress_payload;
	gnew_clone_options.remote_name = g_strdup (gclone_options->remote_name);
	gnew_clone_options.pushurl = g_strdup (gclone_options->pushurl);
	gnew_clone_options.fetch_spec = g_strdup (gclone_options->fetch_spec);
	gnew_clone_options.push_spec = g_strdup (gclone_options->push_spec);
	gnew_clone_options.remote_autotag = gclone_options->remote_autotag;
	gnew_clone_options.checkout_branch = g_strdup (gclone_options->checkout_branch);

	new_clone_options->clone_options = gnew_clone_options;
	new_clone_options->cred_callback = clone_options->cred_callback;
	new_clone_options->cred_user_data = clone_options->cred_user_data;

	return new_clone_options;
}

/**
 * ggit_clone_options_free:
 * @clone_options: a #GgitCloneOptions.
 *
 * Frees @clone_options.
 */
void
ggit_clone_options_free (GgitCloneOptions *clone_options)
{
	git_clone_options *gclone_options;

	g_return_if_fail (clone_options != NULL);

	gclone_options = &clone_options->clone_options;
	g_free ((gchar *)gclone_options->remote_name);
	g_free ((gchar *)gclone_options->pushurl);
	g_free ((gchar *)gclone_options->fetch_spec);
	g_free ((gchar *)gclone_options->push_spec);
	g_free ((gchar *)gclone_options->checkout_branch);

	g_slice_free (GgitCloneOptions, clone_options);
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
	GgitCloneOptions *clone_options;
	git_clone_options gclone_options = GIT_CLONE_OPTIONS_INIT;

	clone_options = g_slice_new (GgitCloneOptions);
	clone_options->clone_options = gclone_options;

	return clone_options;
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
	g_return_val_if_fail (options != NULL, FALSE);

	return options->clone_options.bare;
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
	g_return_if_fail (options != NULL);

	options->clone_options.bare = bare;
}

static int
wrap_fetch_progress (const git_transfer_progress *stats,
                     gpointer                     data)
{
	GgitCloneOptions *options = (GgitCloneOptions *)data;
	GgitTransferProgress *gstats;
	gint ret;

	gstats = _ggit_transfer_progress_wrap (stats);

	ret = options->fetch_progress_callback (gstats, options->fetch_progress_user_data);
	ggit_transfer_progress_free (gstats);

	return ret;
}

/**
 * ggit_clone_options_set_fetch_progress_callback:
 * @options: a #GgitCloneOptions.
 * @callback: (allow-none) (scope call) (closure user_data): callback for fetch progress.
 * @user_data: callback user data.
 *
 * Sets the callback for fetch progress. Be aware that this is called inline
 * with network and indexing operations, so performance may be affected.
 */
void
ggit_clone_options_set_fetch_progress_callback (GgitCloneOptions             *options,
                                                GgitTransferProgressCallback  callback,
                                                gpointer                      user_data)
{
	g_return_if_fail (options != NULL);

	options->fetch_progress_callback = callback;
	options->fetch_progress_user_data = user_data;

	options->clone_options.fetch_progress_cb = wrap_fetch_progress;
	options->clone_options.fetch_progress_payload = options;
}

/**
 * ggit_clone_options_get_remote_name:
 * @options: a #GgitCloneOptions.
 *
 * Gets the name given to the "origin" remote.  The default is "origin".
 *
 * Returns: the name given to the "origin" remote.  The default is "origin".
 */
const gchar *
ggit_clone_options_get_remote_name (GgitCloneOptions *options)
{
	g_return_val_if_fail (options != NULL, NULL);

	return options->clone_options.remote_name;
}

/**
 * ggit_clone_options_set_remote_name:
 * @options: a #GgitCloneOptions.
 * @remote_name: (allow-none): the name given to the "origin" remote.
 *
 * Sets the name given to the "origin" remote.
 */
void
ggit_clone_options_set_remote_name (GgitCloneOptions *options,
                                    const gchar      *remote_name)
{
	g_return_if_fail (options != NULL);

	options->clone_options.remote_name = g_strdup (remote_name);
}

/**
 * ggit_clone_options_get_push_url:
 * @options: a #GgitCloneOptions.
 *
 * Gets the URL to be used for pushing or %NULL.
 *
 * Returns: the URL to be used for pushing or %NULL.
 */
const gchar *
ggit_clone_options_get_push_url (GgitCloneOptions *options)
{
	g_return_val_if_fail (options != NULL, NULL);

	return options->clone_options.pushurl;
}

/**
 * ggit_clone_options_set_push_url:
 * @options: a #GgitCloneOptions.
 * @push_url: (allow-none): the URL to be used for pushing. %NULL means use the fetch url.
 *
 * Sets the URL to be used for pushing. %NULL means use the fetch url.
 */
void
ggit_clone_options_set_push_url (GgitCloneOptions *options,
                                 const gchar      *push_url)
{
	g_return_if_fail (options != NULL);

	options->clone_options.pushurl = g_strdup (push_url);
}

/**
 * ggit_clone_options_get_fetch_spec:
 * @options: a #GgitCloneOptions.
 *
 * Gets the fetch specification to be used or %NULL.
 *
 * Returns: the fetch specification to be used for fetching or %NULL.
 */
const gchar *
ggit_clone_options_get_fetch_spec (GgitCloneOptions *options)
{
	g_return_val_if_fail (options != NULL, NULL);

	return options->clone_options.fetch_spec;
}

/**
 * ggit_clone_options_set_fetch_spec:
 * @options: a #GgitCloneOptions.
 * @fetch_spec: (allow-none): the fetch specification to be used for fetching or %NULL.
 *
 * Sets the fetch specification to be used for fetching. %NULL
 * results in the same behavior as %GGIT_REMOTE_DEFAULT_FETCH.
 */
void
ggit_clone_options_set_fetch_spec (GgitCloneOptions *options,
                                   const gchar      *fetch_spec)
{
	g_return_if_fail (options != NULL);

	options->clone_options.fetch_spec = g_strdup (fetch_spec);
}

/**
 * ggit_clone_options_get_push_spec:
 * @options: a #GgitCloneOptions.
 *
 * Gets the fetch specification to be used for pushing or %NULL.
 *
 * Returns: the fetch specification to be used for pushing or %NULL.
 */
const gchar *
ggit_clone_options_get_push_spec (GgitCloneOptions *options)
{
	g_return_val_if_fail (options != NULL, NULL);

	return options->clone_options.push_spec;
}

/**
 * ggit_clone_options_set_push_spec:
 * @options: a #GgitCloneOptions.
 * @push_spec: (allow-none): the fetch specification to be used for pushing or %NULL.
 *
 * Sets fetch specification to be used for pushing. %NULL means
 * use the same spec as for fetching.
 */
void
ggit_clone_options_set_push_spec (GgitCloneOptions *options,
                                  const gchar      *push_spec)
{
	g_return_if_fail (options != NULL);

	options->clone_options.push_spec = g_strdup (push_spec);
}

/**
 * ggit_clone_options_set_cred_acquire_callback:
 * @options: a #GgitCloneOptions.
 * @callback: (allow-none) (scope call) (closure user_data): callback if credentials are required.
 * @user_data: callback user data.
 *
 * Sets the callback to be used if credentials are required
 * during the initial fetch.
 */
void
ggit_clone_options_set_cred_acquire_callback (GgitCloneOptions        *options,
                                              GgitCredAcquireCallback  callback,
                                              gpointer                 user_data)
{
	g_return_if_fail (options != NULL);

	options->cred_callback = callback;
	options->cred_user_data = user_data;
	options->clone_options.cred_acquire_cb = wrap_cred_acquire;
	options->clone_options.cred_acquire_payload = options;
}

/**
 * ggit_clone_options_get_remote_auto_tag:
 * @options: a #GgitCloneOptions.
 *
 * Gets the auto tag setting before the initial fetch.
 * The default is @GGIT_REMOTE_DOWNLOAD_TAGS_ALL.
 *
 * Returns: a #GgitRemoteDownloadTagsType
 */
GgitRemoteDownloadTagsType
ggit_clone_options_get_remote_auto_tag (GgitCloneOptions *options)
{
	g_return_val_if_fail (options != NULL, GGIT_REMOTE_DOWNLOAD_TAGS_ALL);

	return options->clone_options.remote_autotag;
}

/**
 * ggit_clone_options_set_remote_auto_tag:
 * @options: a #GgitCloneOptions.
 * @remote_auto_tag: a #GgitRemoteDownloadTagsType.
 *
 * Specifies the auto tag setting before the initial fetch.
 */
void
ggit_clone_options_set_remote_auto_tag (GgitCloneOptions           *options,
                                        GgitRemoteDownloadTagsType  remote_auto_tag)
{
	g_return_if_fail (options != NULL);

	options->clone_options.remote_autotag = remote_auto_tag;
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
	g_return_val_if_fail (options != NULL, NULL);

	return options->clone_options.checkout_branch;
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
	g_return_if_fail (options != NULL);

	options->clone_options.checkout_branch = g_strdup (checkout_branch);
}

/* ex:set ts=8 noet: */
