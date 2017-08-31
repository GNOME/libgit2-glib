/*
 * ggit-revert-options.c
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

#include "ggit-revert-options.h"
#include "ggit-merge-options.h"
#include "ggit-checkout-options.h"

struct _GgitRevertOptions
{
	git_revert_options revert_options;

	GgitMergeOptions *merge_options;
	GgitCheckoutOptions *checkout_options;
};

G_DEFINE_BOXED_TYPE (GgitRevertOptions, ggit_revert_options,
                     ggit_revert_options_copy, ggit_revert_options_free)

const git_revert_options *
_ggit_revert_options_get_revert_options (GgitRevertOptions *revert_options)
{
	/* NULL is common for revert_options as it specifies to use the default
	 * so handle a NULL revert_options here instead of in every caller.
	 */
	if (revert_options == NULL)
	{
		return NULL;
	}

	return (const git_revert_options *)&revert_options->revert_options;
}

/**
 * ggit_revert_options_copy:
 * @revert_options: a #GgitRevertOptions.
 *
 * Copies @revert_options into a newly allocated #GgitRevertOptions.
 *
 * Returns: (transfer full) (nullable): a newly allocated #GgitRevertOptions or %NULL.
 */
GgitRevertOptions *
ggit_revert_options_copy (GgitRevertOptions *revert_options)
{
	g_return_val_if_fail (revert_options != NULL, NULL);

	return ggit_revert_options_new (revert_options->revert_options.mainline,
	                                revert_options->merge_options,
	                                revert_options->checkout_options);
}

/**
 * ggit_revert_options_free:
 * @revert_options: a #GgitRevertOptions.
 *
 * Frees @revert_options.
 */
void
ggit_revert_options_free (GgitRevertOptions *revert_options)
{
	g_return_if_fail (revert_options != NULL);

	if (revert_options->merge_options)
	{
		ggit_merge_options_free (revert_options->merge_options);
	}

	g_clear_object (&revert_options->checkout_options);

	g_slice_free (GgitRevertOptions, revert_options);
}

/**
 * ggit_revert_options_new:
 * @mainline: the mainline.
 * @merge_options: (allow-none): a #GgitMergeOptions.
 * @checkout_options: (allow-none): a #GgitCheckoutOptions.
 *
 * Create a new #GgitRevertOptions. Note that the passed in @merge_options and
 * @checkout_options are copied by this function, and alterations in either
 * after this call are therefore not reflected in the revert options.
 *
 * The @mainline indicates which parent to use for the revert when reverting
 * a merge commit.
 *
 * Returns: (transfer full) (nullable): a #GgitRevertOptions or %NULL.
 *
 **/
GgitRevertOptions *
ggit_revert_options_new (guint                mainline,
                         GgitMergeOptions    *merge_options,
                         GgitCheckoutOptions *checkout_options)
{
	GgitRevertOptions *ret;

	g_return_val_if_fail (checkout_options == NULL || GGIT_IS_CHECKOUT_OPTIONS (checkout_options), NULL);

	ret = g_slice_new0 (GgitRevertOptions);

	git_revert_init_options (&ret->revert_options, GIT_REVERT_OPTIONS_VERSION);

	if (merge_options)
	{
		ret->merge_options = ggit_merge_options_copy (merge_options);
		ret->revert_options.merge_opts =
			*_ggit_merge_options_get_merge_options (ret->merge_options);

	}

	if (checkout_options)
	{
		ret->checkout_options = g_object_ref (checkout_options);
		ret->revert_options.checkout_opts =
			*_ggit_checkout_options_get_checkout_options (ret->checkout_options);
	}

	ret->revert_options.mainline = mainline;
	return ret;
}

/* ex:set ts=8 noet: */
