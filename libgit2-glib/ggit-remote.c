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

#include <git2/errors.h>

#include "ggit-remote.h"
#include "ggit-error.h"


struct _GgitRemote
{
	git_remote *remote;
	gint ref_count;
};

G_DEFINE_BOXED_TYPE (GgitRemote, ggit_remote, ggit_remote_ref, ggit_remote_unref)

GgitRemote *
_ggit_remote_new (const git_remote *remote)
{
	GgitRemote *glib_remote;

	glib_remote = g_slice_new (GgitRemote);
	glib_remote->remote = (git_remote *)remote;
	glib_remote->ref_count = 1;

	return glib_remote;
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
 * @error: a #GError or %NULL.
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
