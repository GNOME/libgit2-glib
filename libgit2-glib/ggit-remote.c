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


struct _GgitRemote
{
	git_remote *remote;
	gint ref_count;
};

struct _GgitRemoteHead
{
	gboolean is_local;
	GgitOId *oid;
	GgitOId *local_oid;
	gchar *name;

	gint ref_count;
};

G_DEFINE_BOXED_TYPE (GgitRemote, ggit_remote, ggit_remote_ref, ggit_remote_unref)
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
 * Returns: (transfer none): the remote oid.
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
 * Returns: (transfer none): the local oid.
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
 * Returns: the remote head name.
 */
const gchar *
ggit_remote_head_get_name (GgitRemoteHead *remote_head)
{
	g_return_val_if_fail (remote_head != NULL, NULL);
	return remote_head->name;
}

GgitRemote *
_ggit_remote_wrap (const git_remote *remote)
{
	GgitRemote *glib_remote;

	glib_remote = g_slice_new (GgitRemote);
	glib_remote->remote = (git_remote *)remote;
	glib_remote->ref_count = 1;

	return glib_remote;
}

/**
 * ggit_remote_new:
 * @repository: a #GgitRepository.
 * @url: the remote repository's URL.
 * @fetch_spec: the fetch refspec to use for this remote.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Creates a remote with the default refspecs in memory. You can use
 * this when you have a URL instead of a remote's name.
 *
 * Returns: (transfer full): a newly allocated #GgitRemote.
 */
GgitRemote *
ggit_remote_new (GgitRepository   *repository,
                 const gchar      *url,
                 const gchar      *fetch_spec,
                 GError          **error)
{
	gint ret;
	git_remote *remote;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), NULL);
	g_return_val_if_fail (url != NULL, NULL);
	g_return_val_if_fail (fetch_spec != NULL, NULL);

	ret = git_remote_create_inmemory (&remote, _ggit_native_get (repository),
	                                  url, fetch_spec);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return NULL;
	}

	return _ggit_remote_wrap (remote);
}

/**
 * ggit_remote_ref:
 * @remote: a #GgitRemote.
 *
 * Atomically increments the reference count of @remote by one.
 * This function is MT-safe and may be called from any thread.
 *
 * Returns: (transfer none): the passed in #GgitRemote.
 */
GgitRemote *
ggit_remote_ref (GgitRemote *remote)
{
	g_return_val_if_fail (remote != NULL, NULL);

	g_atomic_int_inc (&remote->ref_count);

	return remote;
}

/**
 * ggit_remote_unref:
 * @remote: a #GgitRemote.
 *
 * Atomically decrements the reference count of @remote by one.
 * If the reference count drops to 0, @remote is freed.
 */
void
ggit_remote_unref (GgitRemote *remote)
{
	g_return_if_fail (remote != NULL);

	if (g_atomic_int_dec_and_test (&remote->ref_count))
	{
		git_remote_free (remote->remote);
		g_slice_free (GgitRemote, remote);
	}
}

/**
 * ggit_remote_save:
 * @remote: a #GgitRemote.
 * @error: a #GError or %NULL.
 *
 * Saves a remote to its repository's configuration.
 */
void
ggit_remote_save (GgitRemote  *remote,
                  GError     **error)
{
	gint ret;

	g_return_if_fail (remote != NULL);
	g_return_if_fail (error == NULL || *error == NULL);

	ret = git_remote_save (remote->remote);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
	}
}

/**
 * ggit_remote_get_name:
 * @remote: a #GgitRemote.
 *
 * Gets the remote's name.
 *
 * Returns: the name of @remote.
 */
const gchar *
ggit_remote_get_name (GgitRemote *remote)
{
	g_return_val_if_fail (remote != NULL, NULL);

	return git_remote_name (remote->remote);
}

/**
 * ggit_remote_get_url:
 * @remote: GgitRemote.
 *
 * Gets the remote's url.
 *
 * Returns: the url of @remote.
 */
const gchar *
ggit_remote_get_url (GgitRemote *remote)
{
	g_return_val_if_fail (remote != NULL, NULL);

	return git_remote_url (remote->remote);
}

/**
 * ggit_remote_connect:
 * @remote: a #GgitRemote.
 * @direction: whether you want to receive or send data.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Opens a connection to a remote.
 * The transport is selected based on the URL. The direction argument
 * is due to a limitation of the git protocol (over TCP or SSH) which
 * starts up a specific binary which can only do the one or the other.
 */
void
ggit_remote_connect (GgitRemote  *remote,
                     gboolean     direction,
                     GError     **error)
{
	gint ret;

	g_return_if_fail (remote != NULL);
	g_return_if_fail (error == NULL || *error == NULL);

	ret = git_remote_connect (remote->remote, direction);

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
	g_return_val_if_fail (remote != NULL, FALSE);

	return git_remote_connected (remote->remote);
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
	g_return_if_fail (remote != NULL);

	git_remote_disconnect (remote->remote);
}

/**
 * ggit_remote_add_fetch_spec:
 * @remote: a #GgitRemote.
 * @fetch_spec: the fetch refspec.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Sets @remote's fetch spec to @fetch_spec.
 */
void
ggit_remote_add_fetch_spec (GgitRemote   *remote,
                            const gchar  *fetch_spec,
                            GError      **error)
{
	gint ret;

	g_return_if_fail (remote != NULL);
	g_return_if_fail (fetch_spec != NULL && fetch_spec[0] != '\0');
	g_return_if_fail (error == NULL || *error == NULL);

	ret = git_remote_add_fetch (remote->remote, fetch_spec);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
	}
}

/**
 * ggit_remote_add_push_spec:
 * @remote: a #GgitRemote.
 * @push_spec: the push refspec.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Sets @remote's push spec to @fetch_spec.
 */
void
ggit_remote_add_push_spec (GgitRemote   *remote,
                           const gchar  *push_spec,
                           GError      **error)
{
	gint ret;

	g_return_if_fail (remote != NULL);
	g_return_if_fail (push_spec != NULL && push_spec[0] != '\0');
	g_return_if_fail (error == NULL || *error == NULL);

	ret = git_remote_add_push (remote->remote, push_spec);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
	}
}

typedef struct {
	GgitRemoteListCallback callback;
	gpointer user_data;
} RemoteListData;

static gint
remote_list_callback_wrapper (git_remote_head *head,
                              gpointer         user_data)
{
	RemoteListData *data = user_data;
	GgitOId *oid;
	GgitOId *loid;
	gint ret;

	oid = _ggit_oid_wrap (&head->oid);
	loid = _ggit_oid_wrap (&head->loid);

	ret = data->callback (head->name, oid, loid,
	                      head->local, data->user_data);

	ggit_oid_free (loid);
	ggit_oid_free (oid);

	return ret;
}

/**
 * ggit_remote_list:
 * @remote: a #GgitRemote.
 * @callback: (scope call) (closure user_data): a #GgitRemoteListCallback.
 * @user_data: callback user data.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Calls @callback for each ref at @remote.
 */
void
ggit_remote_list (GgitRemote              *remote,
                  GgitRemoteListCallback   callback,
                  gpointer                 user_data,
                  GError                 **error)
{
	RemoteListData data;
	gint ret;

	g_return_if_fail (remote != NULL);
	g_return_if_fail (callback != NULL);
	g_return_if_fail (error == NULL || *error == NULL);

	data.callback = callback;
	data.user_data = user_data;

	ret = git_remote_ls (remote->remote,
	                     remote_list_callback_wrapper, &data);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
	}
}

/**
 * ggit_remote_is_valid_url:
 * @url: a url string.
 *
 * Checks if @url is a valid remote URL.
 *
 * Returns: whether a string is a valid remote URL.
 */
gboolean
ggit_remote_is_valid_url (const gchar *url)
{
	g_return_val_if_fail (url != NULL, FALSE);

	return git_remote_valid_url (url);
}

/**
 * ggit_remote_is_supported_url:
 * @url: a url string.
 *
 * Checks if @url is a supported remote URL.
 *
 * Returns: whether a string is a supported remote URL.
 */
gboolean
ggit_remote_is_supported_url (const gchar *url)
{
	g_return_val_if_fail (url != NULL, FALSE);

	return git_remote_supported_url (url);
}

/* ex:set ts=8 noet: */
