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
#include "ggit-diff.h"
#include "ggit-diff-delta.h"
#include "ggit-diff-hunk.h"
#include "ggit-error.h"
#include "ggit-diff-options.h"

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
 * Atomically increments the reference count of @patch by one.
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
 * Atomically decrements the reference count of @patch by one.
 * If the reference count drops to 0, @patch is freed.
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
 * ggit_patch_new_from_diff:
 * @diff: a #GgitDiff.
 * @idx: index into diff list.
 * @error: a #GError for error reporting, or %NULL.
 *
 * The #GgitPatch is a newly created object contains the text diffs
 * for the delta.  You have to call ggit_patch_unref() when you are
 * done with it.  You can use the patch object to loop over all the hunks
 * and lines in the diff of the one delta.
 *
 * Returns: (transfer full): a newly created #GgitPatch.
 */
GgitPatch *
ggit_patch_new_from_diff (GgitDiff  *diff,
                          gsize      idx,
                          GError   **error)
{
	git_patch *patch;
	gint ret;

	g_return_val_if_fail (GGIT_IS_DIFF (diff), NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	ret = git_patch_from_diff (&patch, _ggit_native_get (diff), idx);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return NULL;
	}

	return _ggit_patch_wrap (patch);
}

/**
 * ggit_patch_new_from_blobs:
 * @old_blob: (allow-none): a #GgitBlob to diff from.
 * @old_as_path: (allow-none): treat @old_blob as if it had this filename, or %NULL,
 * @new_blob: (allow-none): a #GgitBlob to diff to.
 * @new_as_path: (allow-none): treat @new_blob as if it had this filename, or %NULL,
 * @diff_options: (allow-none): a #GgitDiffOptions, or %NULL.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Directly generate a patch from the difference between two blobs.
 *
 * This is just like ggit_diff_blobs() except it generates a patch object
 * for the difference instead of directly making callbacks.  You can use the
 * standard ggit_patch accessor functions to read the patch data, and
 * you must call ggit_patch_unref on the patch when done.
 */
GgitPatch *
ggit_patch_new_from_blobs (GgitBlob         *old_blob,
                           const gchar      *old_as_path,
                           GgitBlob         *new_blob,
                           const gchar      *new_as_path,
                           GgitDiffOptions  *diff_options,
                           GError          **error)
{
	gint ret;
	const git_diff_options *gdiff_options;
	git_patch *patch;

	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	gdiff_options = _ggit_diff_options_get_diff_options (diff_options);

	ret = git_patch_from_blobs (&patch,
	                            old_blob ? _ggit_native_get (old_blob) : NULL,
	                            old_as_path,
	                            new_blob ? _ggit_native_get (new_blob) : NULL,
	                            new_as_path,
	                            (git_diff_options *) gdiff_options);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return NULL;
	}

	return _ggit_patch_wrap (patch);
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
	git_buf buf;
	gchar *result = NULL;
	gint ret;

	g_return_val_if_fail (patch != NULL, NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	ret = git_patch_to_buf (&buf, patch->patch);

	if (ret == GIT_OK)
	{
		result = g_strdup (buf.ptr);
		git_buf_free (&buf);
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
                 const git_diff_hunk  *hunk,
                 const git_diff_line  *line,
                 void                 *payload)
{
	PatchToStream *info = payload;
	gboolean ret;
	gsize written;

	ret = g_output_stream_write_all (info->stream,
	                                 line->content,
	                                 line->content_len,
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

/**
 * ggit_patch_get_delta:
 * @patch: a #GgitPatch.
 *
 * Get the diff delta corresponding to the patch.
 *
 * Returns: (transfer full): the #GgitDiffDelta of the patch.
 */
GgitDiffDelta *
ggit_patch_get_delta (GgitPatch *patch)
{
	g_return_val_if_fail (patch != NULL, NULL);

	return _ggit_diff_delta_wrap (git_patch_get_delta (patch->patch));
}

/**
 * ggit_patch_get_hunk:
 * @patch: a #GgitPatch
 * @idx: the hunk index.
 * @error: a #GError
 *
 * Get the @idx'th hunk in the patch.
 *
 * Returns: (transfer full): a new #GgitDiffHunk or %NULL on error.
 */
GgitDiffHunk *
ggit_patch_get_hunk (GgitPatch  *patch,
                     gsize       idx,
                     GError    **error)
{
	const git_diff_hunk *hunk;
	size_t tlines;
	gint ret;

	g_return_val_if_fail (patch != NULL, NULL);

	ret = git_patch_get_hunk (&hunk, &tlines, patch->patch, idx);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return NULL;
	}

	return _ggit_diff_hunk_wrap (hunk);
}

/* ex:set ts=8 noet: */
