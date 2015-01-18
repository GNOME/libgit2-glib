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
	GgitRemoteCallbacks *remote_callbacks;
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

	new_clone_options = g_slice_new0 (GgitCloneOptions);

	gnew_clone_options.bare = gclone_options->bare;
	gnew_clone_options.checkout_branch = g_strdup (gclone_options->checkout_branch);

	if (clone_options->remote_callbacks)
	{
		new_clone_options->remote_callbacks = g_object_ref (clone_options->remote_callbacks);
		gnew_clone_options.remote_callbacks = *_ggit_remote_callbacks_get_native (new_clone_options->remote_callbacks);
	}

	new_clone_options->clone_options = gnew_clone_options;

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
	g_free ((gchar *)gclone_options->checkout_branch);

	g_clear_object (&clone_options->remote_callbacks);

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

/**
 * ggit_clone_options_get_remote_callbacks:
 * @options: a #GgitCloneOptions.
 *
 * Get the remote callbacks object or %NULL if not set.
 *
 * Returns: (transfer none): the remote callbacks or %NULL.
 */
GgitRemoteCallbacks *
ggit_clone_options_get_remote_callbacks (GgitCloneOptions *options)
{
	g_return_val_if_fail (options != NULL, NULL);
	return options->remote_callbacks;
}

/**
 * ggit_clone_options_set_remote_callbacks:
 * @options: a #GgitCloneOptions.
 * @callbacks: (allow-none): a #GgitRemoteCallbacks or %NULL.
 *
 * Set the remote callbacks object.
 */
void
ggit_clone_options_set_remote_callbacks (GgitCloneOptions    *options,
                                         GgitRemoteCallbacks *callbacks)
{
	g_return_if_fail (options != NULL);

	g_clear_object (&options->remote_callbacks);

	if (callbacks != NULL)
	{
		options->remote_callbacks = g_object_ref (callbacks);
		options->clone_options.remote_callbacks = *_ggit_remote_callbacks_get_native (callbacks);
	}
	else
	{
		git_remote_callbacks i = GIT_REMOTE_CALLBACKS_INIT;
		options->clone_options.remote_callbacks = i;
	}
}

/* ex:set ts=8 noet: */
