/*
 * ggit-fetch-options.c
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2015 - Ignacio Casal Quinteiro
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

#include "ggit-fetch-options.h"
#include "ggit-transfer-progress.h"
#include "ggit-native.h"

struct _GgitFetchOptions
{
	git_fetch_options fetch_options;
	GgitRemoteCallbacks *remote_callbacks;
};

G_DEFINE_BOXED_TYPE (GgitFetchOptions, ggit_fetch_options,
                     ggit_fetch_options_copy, ggit_fetch_options_free)

const git_fetch_options *
_ggit_fetch_options_get_fetch_options (GgitFetchOptions *fetch_options)
{
	/* NULL is common for fetch_options as it specifies to use the default
	 * so handle a NULL fetch_options here instead of in every caller.
	 */
	if (fetch_options == NULL)
	{
		return NULL;
	}

	return (const git_fetch_options *)&fetch_options->fetch_options;
}

/**
 * ggit_fetch_options_copy:
 * @fetch_options: a #GgitFetchOptions.
 *
 * Copies @fetch_options into a newly allocated #GgitFetchOptions.
 *
 * Returns: (transfer full): a newly allocated #GgitFetchOptions.
 */
GgitFetchOptions *
ggit_fetch_options_copy (GgitFetchOptions *fetch_options)
{
	GgitFetchOptions *new_fetch_options;
	git_fetch_options *gfetch_options;
	git_fetch_options gnew_fetch_options = GIT_FETCH_OPTIONS_INIT;

	g_return_val_if_fail (fetch_options != NULL, NULL);

	gfetch_options = &fetch_options->fetch_options;

	new_fetch_options = g_slice_new0 (GgitFetchOptions);

	gnew_fetch_options.prune = gfetch_options->prune;
	gnew_fetch_options.update_fetchhead = gfetch_options->update_fetchhead;
	gnew_fetch_options.download_tags = gfetch_options->download_tags;

	if (fetch_options->remote_callbacks)
	{
		new_fetch_options->remote_callbacks = g_object_ref (fetch_options->remote_callbacks);
		gnew_fetch_options.callbacks = *_ggit_remote_callbacks_get_native (new_fetch_options->remote_callbacks);
	}

	new_fetch_options->fetch_options = gnew_fetch_options;

	return new_fetch_options;
}

/**
 * ggit_fetch_options_free:
 * @fetch_options: a #GgitFetchOptions.
 *
 * Frees @fetch_options.
 */
void
ggit_fetch_options_free (GgitFetchOptions *fetch_options)
{
	g_return_if_fail (fetch_options != NULL);

	g_clear_object (&fetch_options->remote_callbacks);
	g_slice_free (GgitFetchOptions, fetch_options);
}

/**
 * ggit_fetch_options_new:
 *
 * Creates a new #GgitFetchOptions.
 *
 * Returns: a newly allocated #GgitFetchOptions.
 */
GgitFetchOptions *
ggit_fetch_options_new (void)
{
	GgitFetchOptions *fetch_options;
	git_fetch_options gfetch_options = GIT_FETCH_OPTIONS_INIT;

	fetch_options = g_slice_new0 (GgitFetchOptions);
	fetch_options->fetch_options = gfetch_options;

	return fetch_options;
}

/**
 * ggit_fetch_options_get_remote_callbacks:
 * @options: a #GgitFetchOptions.
 *
 * Get the remote callbacks object or %NULL if not set.
 *
 * Returns: (transfer none): the remote callbacks or %NULL.
 */
GgitRemoteCallbacks *
ggit_fetch_options_get_remote_callbacks (GgitFetchOptions *options)
{
	g_return_val_if_fail (options != NULL, NULL);
	return options->remote_callbacks;
}

/**
 * ggit_fetch_options_set_remote_callbacks:
 * @options: a #GgitFetchOptions.
 * @callbacks: (allow-none): a #GgitRemoteCallbacks or %NULL.
 *
 * Set the fetch options object.
 */
void
ggit_fetch_options_set_remote_callbacks (GgitFetchOptions    *options,
                                         GgitRemoteCallbacks *callbacks)
{
	g_return_if_fail (options != NULL);

	g_clear_object (&options->remote_callbacks);

	if (callbacks != NULL)
	{
		options->remote_callbacks = g_object_ref (callbacks);
		options->fetch_options.callbacks = *_ggit_remote_callbacks_get_native (callbacks);
	}
	else
	{
		git_remote_callbacks i = GIT_REMOTE_CALLBACKS_INIT;
		options->fetch_options.callbacks = i;
	}
}

/**
 * ggit_fetch_options_get_download_tags:
 * @options: a #GgitFetchOptions.
 *
 * Get how tags are being handled when fetching/downloading.
 *
 * Returns: a #GgitRemoteDownloadTagsType.
 *
 **/
GgitRemoteDownloadTagsType
ggit_fetch_options_get_download_tags (GgitFetchOptions *options)
{
	g_return_val_if_fail (options != NULL, GGIT_REMOTE_DOWNLOAD_TAGS_UNSPECIFIED);
	return (GgitRemoteDownloadTagsType)options->fetch_options.download_tags;
}

/**
 * ggit_fetch_options_set_download_tags:
 * @options: a #GgitFetchOptions.
 * @download_tags: a #GgitRemoteDownloadTagsType.
 *
 * Set how tags are being handled when fetching/downloading.
 *
 **/
void
ggit_fetch_options_set_download_tags (GgitFetchOptions            *options,
                                      GgitRemoteDownloadTagsType   download_tags)
{
	g_return_if_fail (options != NULL);

	options->fetch_options.download_tags = (git_remote_autotag_option_t)download_tags;
}

/* ex:set ts=8 noet: */
