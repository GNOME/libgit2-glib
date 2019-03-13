/*
 * ggit-remote.h
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2012 - Ignacio Casal Quinteiro
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

#include "ggit-remote.h"
#include "ggit-error.h"
#include "ggit-oid.h"
#include "ggit-ref-spec.h"
#include "ggit-repository.h"
#include "ggit-utils.h"
#include "ggit-signature.h"
#include "ggit-transfer-progress.h"
#include "ggit-fetch-options.h"
#include "ggit-push-options.h"

struct _GgitRemoteHead
{
	gboolean is_local;
	GgitOId *oid;
	GgitOId *local_oid;
	gchar *name;

	gint ref_count;
};

G_DEFINE_TYPE (GgitRemote, ggit_remote, GGIT_TYPE_NATIVE)
G_DEFINE_BOXED_TYPE (GgitRemoteHead, ggit_remote_head, ggit_remote_head_ref, ggit_remote_head_unref)

static GgitRemoteHead *
_ggit_remote_head_wrap (const git_remote_head *remote_head)
{
	GgitRemoteHead *ret;

	ret = g_slice_new (GgitRemoteHead);
	ret->ref_count = 1;
	ret->is_local = remote_head->local;
	ret->oid = _ggit_oid_wrap (&remote_head->oid);
	ret->local_oid = _ggit_oid_wrap (&remote_head->loid);
	ret->name = g_strdup (remote_head->name);

	return ret;
}

GgitRemoteHead *
ggit_remote_head_ref (GgitRemoteHead *remote_head)
{
	g_atomic_int_inc (&remote_head->ref_count);
	return remote_head;
}

void
ggit_remote_head_unref (GgitRemoteHead *remote_head)
{
	if (g_atomic_int_dec_and_test (&remote_head->ref_count))
	{
		ggit_oid_free (remote_head->oid);
		ggit_oid_free (remote_head->local_oid);
		g_free (remote_head->name);

		g_slice_free (GgitRemoteHead, remote_head);
	}
}

/**
 * ggit_remote_head_get_oid:
 * @remote_head: a #GgitRemoteHead.
 *
 * Get the remote oid of the remote head.
 *
 * Returns: (transfer none) (nullable): the remote oid or %NULL.
 */
GgitOId *
ggit_remote_head_get_oid (GgitRemoteHead *remote_head)
{
	g_return_val_if_fail (remote_head != NULL, NULL);
	return remote_head->oid;
}

/**
 * ggit_remote_head_get_local_oid:
 * @remote_head: a #GgitRemoteHead.
 *
 * Get the local oid of the remote head.
 *
 * Returns: (transfer none) (nullable): the local oid or %NULL.
 */
GgitOId *
ggit_remote_head_get_local_oid (GgitRemoteHead *remote_head)
{
	g_return_val_if_fail (remote_head != NULL, NULL);
	return remote_head->local_oid;
}

/**
 * ggit_remote_head_is_local:
 * @remote_head: a #GgitRemoteHead.
 *
 * Get whether the remote head is local.
 *
 * Returns: whether the remote head is local.
 */
gboolean
ggit_remote_head_is_local (GgitRemoteHead *remote_head)
{
	g_return_val_if_fail (remote_head != NULL, FALSE);
	return remote_head->is_local;
}

/**
 * ggit_remote_head_get_name:
 * @remote_head: a #GgitRemoteHead.
 *
 * Get the remote head name.
 *
 * Returns: (transfer none) (nullable): the remote head name or %NULL.
 */
const gchar *
ggit_remote_head_get_name (GgitRemoteHead *remote_head)
{
	g_return_val_if_fail (remote_head != NULL, NULL);
	return remote_head->name;
}

GgitRemote *
_ggit_remote_wrap (git_remote *remote)
{
	GgitRemote *ret;

	ret = g_object_new (GGIT_TYPE_REMOTE, "native", remote, NULL);
	_ggit_native_set_destroy_func (ret, (GDestroyNotify)git_remote_free);

	return ret;
}

static void
ggit_remote_dispose (GObject *object)
{
	G_OBJECT_CLASS (ggit_remote_parent_class)->dispose (object);
}

static void
ggit_remote_class_init (GgitRemoteClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->dispose = ggit_remote_dispose;
}

static void
ggit_remote_init (GgitRemote *remote)
{
}

/**
 * ggit_remote_new:
 * @repository: a #GgitRepository.
 * @name: the remote's name.
 * @url: the remote repository's URL.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Creates a remote with the default refspecs in memory. You can use
 * this when you have a URL instead of a remote's name.
 *
 * Returns: (transfer full) (nullable): a newly allocated #GgitRemote or %NULL.
 */
GgitRemote *
ggit_remote_new (GgitRepository   *repository,
                 const gchar      *name,
                 const gchar      *url,
                 GError          **error)
{
	gint ret;
	git_remote *remote;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), NULL);
	g_return_val_if_fail (name != NULL, NULL);
	g_return_val_if_fail (url != NULL, NULL);

	ret = git_remote_create (&remote, _ggit_native_get (repository),
	                         name, url);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return NULL;
	}

	return _ggit_remote_wrap (remote);
}

/**
 * ggit_remote_new_anonymous:
 * @repository: a #GgitRepository.
 * @url: the remote repository's URL.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Creates a remote with the specified refspec in memory. You can use
 * this when you have a URL instead of a remote's name.
 *
 * Returns: (transfer full) (nullable): a newly allocated #GgitRemote or %NULL.
 */
GgitRemote *
ggit_remote_new_anonymous (GgitRepository  *repository,
                           const gchar     *url,
                           GError         **error)
{
	gint ret;
	git_remote *remote;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), NULL);
	g_return_val_if_fail (url != NULL, NULL);

	ret = git_remote_create_anonymous (&remote,
	                                   _ggit_native_get (repository),
	                                   url);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return NULL;
	}

	return _ggit_remote_wrap (remote);
}

/**
 * ggit_remote_get_owner:
 * @remote: a #GgitRemote.
 *
 * Gets the repository where @remote resides.
 *
 * Returns: (transfer full) (nullable): the repository where the remote resides or %NULL.
 */
GgitRepository *
ggit_remote_get_owner (GgitRemote *remote)
{
	g_return_val_if_fail (GGIT_IS_REMOTE (remote), NULL);

	return _ggit_repository_wrap (git_remote_owner (_ggit_native_get (remote)),
	                              FALSE);
}

/**
 * ggit_remote_get_name:
 * @remote: a #GgitRemote.
 *
 * Gets the remote's name.
 *
 * Returns: (transfer none) (nullable): the name of @remote or %NULL.
 */
const gchar *
ggit_remote_get_name (GgitRemote *remote)
{
	g_return_val_if_fail (GGIT_IS_REMOTE (remote), NULL);

	return git_remote_name (_ggit_native_get (remote));
}

/**
 * ggit_remote_get_url:
 * @remote: GgitRemote.
 *
 * Gets the remote's url.
 *
 * Returns: (transfer none) (nullable): the url of @remote or %NULL.
 */
const gchar *
ggit_remote_get_url (GgitRemote *remote)
{
	g_return_val_if_fail (GGIT_IS_REMOTE (remote), NULL);

	return git_remote_url (_ggit_native_get (remote));
}

/**
 * ggit_remote_connect:
 * @remote: a #GgitRemote.
 * @direction: whether you want to receive or send data.
 * @callbacks: the callbacks to use for this connection.
 * @proxy_options: (allow-none): the proxy options.
 * @custom_headers: (allow-none): extra HTTP headers to use in this connection.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Opens a connection to a remote.
 * The transport is selected based on the URL. The direction argument
 * is due to a limitation of the git protocol (over TCP or SSH) which
 * starts up a specific binary which can only do the one or the other.
 */
void
ggit_remote_connect (GgitRemote           *remote,
                     GgitDirection         direction,
                     GgitRemoteCallbacks  *callbacks,
                     GgitProxyOptions     *proxy_options,
                     const gchar * const  *custom_headers,
                     GError              **error)
{
	gint ret;
	git_strarray headers;

	g_return_if_fail (GGIT_IS_REMOTE (remote));
	g_return_if_fail (error == NULL || *error == NULL);

	ggit_utils_get_git_strarray_from_str_array (custom_headers, &headers);

	ret = git_remote_connect (_ggit_native_get (remote),
	                          (git_direction)direction,
	                          _ggit_remote_callbacks_get_native (callbacks),
	                          proxy_options != NULL ? _ggit_proxy_options_get_proxy_options (proxy_options) : NULL,
	                          &headers);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
	}
}

/**
 * ggit_remote_get_connected:
 * @remote: a #GgitRemote.
 *
 * Check whether @remote is connected.
 *
 * Returns: %TRUE if it is connected.
 */
gboolean
ggit_remote_get_connected (GgitRemote *remote)
{
	g_return_val_if_fail (GGIT_IS_REMOTE (remote), FALSE);

	return git_remote_connected (_ggit_native_get (remote));
}

/**
 * ggit_remote_disconnect:
 * @remote: a #GgitRemote.
 *
 * Closes the connection to the remote and frees the underlying
 * transport.
 */
void
ggit_remote_disconnect (GgitRemote *remote)
{
	g_return_if_fail (GGIT_IS_REMOTE (remote));

	git_remote_disconnect (_ggit_native_get (remote));
}

/**
 * ggit_remote_download:
 * @remote: a #GgitRemote.
 * @specs: (array zero-terminated=1) (allow-none): the ref specs.
 * @fetch_options: a #GgitFetchOptions.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Connect to the remote if not yet connected, negotiate with the remote
 * about which objects are missing and download the resulting packfile and
 * its index.
 *
 * Returns: %TRUE if successful, %FALSE otherwise.
 */
gboolean
ggit_remote_download (GgitRemote           *remote,
                      const gchar * const  *specs,
                      GgitFetchOptions     *fetch_options,
                      GError              **error)
{
	gint ret;
	git_strarray gspecs;

	g_return_val_if_fail (GGIT_IS_REMOTE (remote), FALSE);
	g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

	ggit_utils_get_git_strarray_from_str_array (specs, &gspecs);

	ret = git_remote_download (_ggit_native_get (remote), &gspecs,
	                           _ggit_fetch_options_get_fetch_options (fetch_options));

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return FALSE;
	}

	return TRUE;
}

/**
 * ggit_remote_update_tips:
 * @remote: a #GgitRemote.
 * @callbacks: a #GgitRemoteCallbacks.
 * @update_fetch_head: whether to write to FETCH_HEAD. %TRUE to behave like git.
 * @tags_type: what the behaviour for downloading tags is for this fetch. This is
 *             ignored for push. This must be the same value passed to
 *             ggit_remote_download().
 * @message: (allow-none): reflog_message The message to insert into the reflogs. If
 *                         %NULL and fetching, the default is "fetch <name>",
 *                         where <name> is the name of the remote (or its url,
 *                         for in-memory remotes). This parameter is ignored when pushing.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Update tips to the new state.
 *
 * Returns: %TRUE if successful, %FALSE otherwise.
 */
gboolean
ggit_remote_update_tips (GgitRemote                  *remote,
                         GgitRemoteCallbacks         *callbacks,
                         gboolean                     update_fetch_head,
                         GgitRemoteDownloadTagsType   tags_type,
                         const gchar                 *message,
                         GError                     **error)
{
	gint ret;

	g_return_val_if_fail (GGIT_IS_REMOTE (remote), FALSE);
	g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

	ret = git_remote_update_tips (_ggit_native_get (remote),
	                              _ggit_remote_callbacks_get_native (callbacks),
	                              update_fetch_head,
	                              (git_remote_autotag_option_t)tags_type,
	                              message);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return FALSE;
	}

	return TRUE;
}

/**
 * ggit_remote_get_fetch_specs:
 * @remote: a #GgitRemote.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Get the list of fetch refspecs for the given remote.
 *
 * Returns: (transfer full) (nullable): a list of fetch refspecs or %NULL.
 */
gchar **
ggit_remote_get_fetch_specs (GgitRemote  *remote,
                             GError     **error)
{
	gint ret;
	git_strarray specs;

	g_return_val_if_fail (GGIT_IS_REMOTE (remote), NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	ret = git_remote_get_fetch_refspecs (&specs, _ggit_native_get (remote));

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return NULL;
	}

	return ggit_utils_get_str_array_from_git_strarray (&specs);
}

/**
 * ggit_remote_get_push_specs:
 * @remote: a #GgitRemote.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Get the list of push refspecs for the given remote.
 *
 * Returns: (transfer full) (nullable): a list of push refspecs or %NULL.
 */
gchar **
ggit_remote_get_push_specs (GgitRemote  *remote,
                            GError     **error)
{
	gint ret;
	git_strarray specs;

	g_return_val_if_fail (GGIT_IS_REMOTE (remote), NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	ret = git_remote_get_push_refspecs (&specs, _ggit_native_get (remote));

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return NULL;
	}

	return ggit_utils_get_str_array_from_git_strarray (&specs);
}

/**
 * ggit_remote_list:
 * @remote: a #GgitRemote.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Get a list of refs at the remote.
 *
 * Returns: (array zero-terminated=1) (nullable): the remote heads or %NULL.
 */
GgitRemoteHead **
ggit_remote_list (GgitRemote              *remote,
                  GError                 **error)
{
	const git_remote_head **head;
	size_t size;
	size_t i;
	GgitRemoteHead **retval;
	gint ret;

	g_return_val_if_fail (GGIT_IS_REMOTE (remote), NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	ret = git_remote_ls (&head, &size, _ggit_native_get (remote));

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return NULL;
	}

	retval = g_new0 (GgitRemoteHead *, size + 1);

	for (i = 0; i < size; i++)
	{
		retval[i] = _ggit_remote_head_wrap (head[i]);
	}

	return retval;
}

/**
 * ggit_remote_upload:
 * @remote: a #GgitRemote.
 * @specs: (array zero-terminated=1) (allow-none): the ref specs.
 * @push_options: a #GgitPushOptions
 * @error: a #GError for error reporting, or %NULL.
 *
 * Create a packfile and send it to the server
 *
 * Returns: %TRUE if successful, %FALSE otherwise.
 */
gboolean
ggit_remote_upload (GgitRemote              *remote,
                    const gchar * const     *specs,
                    GgitPushOptions         *push_options,
                    GError                 **error)
{
	gint ret;
	git_strarray gspecs;

	g_return_val_if_fail (GGIT_IS_REMOTE (remote), FALSE);
	g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

	ggit_utils_get_git_strarray_from_str_array (specs, &gspecs);

	ret = git_remote_upload (_ggit_native_get (remote), &gspecs,
	                           _ggit_push_options_get_push_options (push_options));

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return FALSE;
	}

	return TRUE;
}

/* ex:set ts=8 noet: */
