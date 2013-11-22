/*
 * ggit-patch.c
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2013 - Sindhu S
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

#include "ggit-patch.h"
#include "ggit-error.h"

struct _GgitPatch
{
	git_patch *patch;
	gint ref_count;
};

G_DEFINE_BOXED_TYPE (GgitPatch, ggit_patch,
                     ggit_patch_ref, ggit_patch_unref)

GgitPatch *
_ggit_patch_wrap (git_patch *patch)
{
	GgitPatch *gpatch;

	gpatch = g_slice_new (GgitPatch);
	gpatch->patch = patch;
	gpatch->ref_count = 1;

	return gpatch;
}

/**
 * ggit_patch_ref:
 * @patch: a #GgitPatch.
 *
 * Atomically increments the reference count of @entry by one.
 * This function is MT-safe and may be called from any thread.
 *
 * Returns: (transfer none): a #GgitPatch.
 */
GgitPatch *
ggit_patch_ref (GgitPatch *patch)
{
	g_return_val_if_fail (patch != NULL, NULL);

	g_atomic_int_inc (&patch->ref_count);

	return patch;
}

/**
 * ggit_patch_unref:
 * @patch: a #GgitPatch.
 *
 * Atomically decrements the reference count of @entry by one.
 * If the reference count drops to 0, @entry is freed.
 */
void
ggit_patch_unref (GgitPatch *patch)
{
	g_return_if_fail (patch != NULL);

	if (g_atomic_int_dec_and_test (&patch->ref_count))
	{
		git_patch_free (patch->patch);
		g_slice_free (GgitPatch, patch);
	}
}

/**
 * ggit_patch_to_string:
 * @patch: a #GgitPatch.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Gets the content of a patch as a single diff text.
 *
 * Returns: the content of a patch as a single diff text.
 */
gchar *
ggit_patch_to_string (GgitPatch  *patch,
                      GError    **error)
{
	char *retval;
	gchar *result = NULL;
	gint ret;

	g_return_val_if_fail (patch != NULL, NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	ret = git_patch_to_str (&retval, diff_patch->diff_patch);
	if (ret == GIT_OK)
	{
		result = g_strdup (retval);
		free (retval);
	}

	return result;
}

typedef struct
{
	GOutputStream  *stream;
	GError        **error;
} PatchToStream;

static int
patch_to_stream (const git_diff_delta *delta,
                 const git_diff_range *range,
                 char                  line_origin,
                 const char           *content,
                 size_t                content_len,
                 void                 *payload)
{
	PatchToStream *info = payload;
	gboolean ret;
	gsize written;

	ret = g_output_stream_write_all (info->stream,
	                                 content,
	                                 content_len,
	                                 &written,
	                                 NULL,
	                                 info->error);

	if (!ret)
	{
		return -1;
	}
	else
	{
		return written;
	}
}

/**
 * ggit_patch_to_stream:
 * @patch: a #GgitPatch.
 * @stream: a #GOutputStream.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Write the contents of a patch to the provided stream.
 *
 * Returns: %TRUE if the patch was written successfully, %FALSE otherwise.
 *
 **/
gboolean
ggit_patch_to_stream (GgitPatch      *patch,
                      GOutputStream  *stream,
                      GError        **error)
{
	PatchToStream info;
	gint ret;

	g_return_val_if_fail (patch != NULL, FALSE);
	g_return_val_if_fail (G_IS_OUTPUT_STREAM (stream), FALSE);
	g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

	info.stream = stream;
	info.error = error;

	ret = git_patch_print (patch->patch,
	                       patch_to_stream,
	                       &info);

	if (ret != GIT_OK)
	{
		if (error != NULL && *error == NULL)
		{
			_ggit_error_set (error, ret);
		}

		return FALSE;
	}

	return TRUE;
}

/**
 * ggit_patch_get_line_stats:
 * @patch: a #GgitPatch.
 * @total_context: (allow-none) (out): return value for the number of context lines.
 * @total_additions: (allow-none) (out): return value for the number of added lines.
 * @total_deletions: (allow-none) (out): return value for the number of deleted lines.
 * @error: a #GError.
 *
 * Get the line statistics of the patch.
 *
 * Returns: %TRUE if successfull, %FALSE otherwise.
 *
 **/
gboolean
ggit_patch_get_line_stats (GgitPatch      *patch,
                           gsize          *total_context,
                           gsize          *total_additions,
                           gsize          *total_deletions,
                           GError        **error)
{
	size_t tc;
	size_t ta;
	size_t td;
	gint ret;

	g_return_val_if_fail (patch != NULL, FALSE);
	g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

	ret = git_patch_line_stats (&tc, &ta, &td, patch->patch);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return FALSE;
	}

	if (total_context)
	{
		*total_context = tc;
	}

	if (total_additions)
	{
		*total_additions = ta;
	}

	if (total_deletions)
	{
		*total_deletions = td;
	}

	return TRUE;
}

/**
 * ggit_patch_get_num_hunks:
 * @patch: a #GgitPatch.
 *
 * Get the number of hunks in the patch.
 *
 * Returns: the number of hunks.
 *
 **/
gsize
ggit_patch_get_num_hunks (GgitPatch *patch)
{
	g_return_val_if_fail (patch != NULL, FALSE);

	return git_patch_num_hunks (patch->patch);
}

/**
 * ggit_patch_get_num_lines_in_hunk:
 * @patch: a #GgitPatch.
 * @hunk: the hunk index.
 *
 * Get the number of lines in @hunk.
 *
 * Returns: the number of lines.
 *
 **/
gint
ggit_patch_get_num_lines_in_hunk (GgitPatch *patch,
                                  gsize      hunk)
{
	g_return_val_if_fail (patch != NULL, FALSE);

	return git_patch_num_lines_in_hunk (patch->patch, hunk);
}

/* ex:set ts=8 noet: */
