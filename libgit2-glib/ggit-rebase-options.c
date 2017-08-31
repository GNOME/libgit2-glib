/*
 * ggit-rebase-options.c
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

#include "ggit-rebase-options.h"

struct _GgitRebaseOptions
{
	git_rebase_options rebase_options;
	gchar *rewrite_notes_ref;
	GgitCheckoutOptions *checkout_options;
};

G_DEFINE_BOXED_TYPE (GgitRebaseOptions, ggit_rebase_options,
                     ggit_rebase_options_copy, ggit_rebase_options_free)

const git_rebase_options *
_ggit_rebase_options_get_rebase_options (GgitRebaseOptions *rebase_options)
{
	/* NULL is common for rebase_options as it specifies to use the default
	 * so handle a NULL rebase_options here instead of in every caller.
	 */
	if (rebase_options == NULL)
	{
		return NULL;
	}

	return (const git_rebase_options *)&rebase_options->rebase_options;
}

/**
 * ggit_rebase_options_copy:
 * @rebase_options: a #GgitRebaseOptions.
 *
 * Copies @rebase_options into a newly allocated #GgitRebaseOptions.
 *
 * Returns: (transfer full) (nullable): a newly allocated #GgitRebaseOptions or %NULL.
 */
GgitRebaseOptions *
ggit_rebase_options_copy (GgitRebaseOptions *rebase_options)
{
	GgitRebaseOptions *new_rebase_options;

	g_return_val_if_fail (rebase_options != NULL, NULL);

	new_rebase_options = g_slice_new0 (GgitRebaseOptions);
	new_rebase_options->rebase_options = rebase_options->rebase_options;

	ggit_rebase_options_set_quiet (new_rebase_options,
	                               rebase_options->rebase_options.quiet);
	ggit_rebase_options_set_rewrite_notes_ref (new_rebase_options,
	                                           rebase_options->rewrite_notes_ref);
	ggit_rebase_options_set_checkout_options (new_rebase_options,
	                                          rebase_options->checkout_options);

	return new_rebase_options;
}

/**
 * ggit_rebase_options_free:
 * @rebase_options: a #GgitRebaseOptions.
 *
 * Frees @rebase_options.
 */
void
ggit_rebase_options_free (GgitRebaseOptions *rebase_options)
{
	g_return_if_fail (rebase_options != NULL);

	g_free (rebase_options->rewrite_notes_ref);
	g_clear_object (&rebase_options->checkout_options);
	g_slice_free (GgitRebaseOptions, rebase_options);
}

/**
 * ggit_rebase_options_new:
 *
 * Creates a new #GgitRebaseOptions.
 *
 * Returns: a newly allocated #GgitRebaseOptions.
 */
GgitRebaseOptions *
ggit_rebase_options_new (void)
{
	GgitRebaseOptions *rebase_options;
	git_rebase_options grebase_options = GIT_REBASE_OPTIONS_INIT;

	rebase_options = g_slice_new0 (GgitRebaseOptions);
	rebase_options->rebase_options = grebase_options;

	return rebase_options;
}

/**
 * ggit_rebase_options_get_quiet:
 * @rebase_options: a #GgitRebaseOptions.
 *
 * Gets whether you want a quiet rebase experience.
 *
 * Returns: returns whether you want a quiet rebase experience.
 */
gboolean
ggit_rebase_options_get_quiet (GgitRebaseOptions *rebase_options)
{
	g_return_val_if_fail (rebase_options != NULL, FALSE);

	return rebase_options->rebase_options.quiet != 0;
}

/**
 * ggit_rebase_options_set_quiet:
 * @rebase_options: a #GgitRebaseOptions.
 * @quiet: whether you want a quiet rebase experience.
 *
 * Used by ggit_rebase_init(), this will instruct other clients working
 * on this rebase that you want a quiet rebase experience, which they
 * may choose to provide in an application-specific manner.  This has no
 * effect upon libgit2-glib directly, but is provided for interoperability
 * between Git tools.
 */
void
ggit_rebase_options_set_quiet (GgitRebaseOptions *rebase_options,
                               gboolean           quiet)
{
	g_return_if_fail (rebase_options != NULL);

	rebase_options->rebase_options.quiet = !!quiet;
}

/**
 * ggit_rebase_options_get_rewrite_notes_ref:
 * @rebase_options: a #GgitRebaseOptions.
 *
 * Gets the the name of the notes reference used to rewrite notes for rebased
 * commits when finishing the rebase or %NULL if not set.
 *
 * Returns: (transfer none) (nullable): the name of the notes reference or %NULL.
 */
const gchar *
ggit_rebase_options_get_rewrite_notes_ref (GgitRebaseOptions *rebase_options)
{
	g_return_val_if_fail (rebase_options != NULL, NULL);

	return rebase_options->rebase_options.rewrite_notes_ref;
}

/**
 * ggit_rebase_options_set_rewrite_notes_ref:
 * @rebase_options: a #GgitRebaseOptions.
 * @rewrite_notes_ref: the name of the notes reference.
 *
 * Used by ggit_rebase_finish(), this is the name of the notes reference
 * used to rewrite notes for rebased commits when finishing the rebase;
 * if %NULL, the contents of the configuration option `notes.rewriteRef`
 * is examined, unless the configuration option `notes.rewrite.rebase`
 * is set to false.  If `notes.rewriteRef` is also %NULL, notes will
 * not be rewritten.
 */
void
ggit_rebase_options_set_rewrite_notes_ref (GgitRebaseOptions *rebase_options,
                                           const gchar       *rewrite_notes_ref)
{
	g_return_if_fail (rebase_options != NULL);

	g_free (rebase_options->rewrite_notes_ref);
	rebase_options->rewrite_notes_ref = g_strdup (rewrite_notes_ref);
	rebase_options->rebase_options.rewrite_notes_ref = rebase_options->rewrite_notes_ref;
}

/**
 * ggit_rebase_options_get_checkout_options:
 * @rebase_options: a #GgitRebaseOptions.
 *
 * Get the checkout options object or %NULL if not set.
 *
 * Returns: (transfer none) (nullable): the checkout options or %NULL.
 */
GgitCheckoutOptions *
ggit_rebase_options_get_checkout_options (GgitRebaseOptions *rebase_options)
{
	g_return_val_if_fail (rebase_options != NULL, NULL);
	return rebase_options->checkout_options;
}

/**
 * ggit_fetch_options_set_checkout_options:
 * @rebase_options: a #GgitRebaseOptions.
 * @checkout_options: (allow-none): a #GgitCheckoutOptions or %NULL.
 *
 * Set the checkout options object.
 * Options to control how files are written during ggit_rebase_init(),
 * ggit_checkout_next() and ggit_checkout_abort(). Note that a minimum
 * strategy of @GGIT_CHECKOUT_SAFE is defaulted in init and next,
 * and a minimum strategy of GGIT_CHECKOUT_FORCE is defaulted in
 * abort to match git semantics.
 */
void
ggit_rebase_options_set_checkout_options (GgitRebaseOptions   *rebase_options,
                                          GgitCheckoutOptions *checkout_options)
{
	g_return_if_fail (rebase_options != NULL);

	g_clear_object (&rebase_options->checkout_options);

	if (checkout_options != NULL)
	{
		rebase_options->checkout_options = g_object_ref (checkout_options);
		rebase_options->rebase_options.checkout_options = *_ggit_checkout_options_get_checkout_options (checkout_options);
	}
	else
	{
		git_checkout_options i = GIT_CHECKOUT_OPTIONS_INIT;
		rebase_options->rebase_options.checkout_options = i;
	}
}

/* ex:set ts=8 noet: */
