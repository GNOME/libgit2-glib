/*
 * ggit-list.c
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2012 - Garrett Regier
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

#include "ggit-diff.h"
#include "ggit-diff-delta.h"
#include "ggit-diff-options.h"
#include "ggit-diff-line.h"
#include "ggit-diff-hunk.h"
#include "ggit-patch.h"
#include "ggit-error.h"
#include "ggit-repository.h"

typedef struct {
	gpointer user_data;

	GgitDiffFileCallback file_cb;
	GgitDiffHunkCallback hunk_cb;
	GgitDiffLineCallback line_cb;
} CallbackWrapperData;

G_DEFINE_TYPE (GgitDiff, ggit_diff, GGIT_TYPE_NATIVE)

static gint
ggit_diff_file_callback_wrapper (const git_diff_delta *delta,
                                 gfloat                progress,
                                 gpointer              user_data)
{
	CallbackWrapperData *data = user_data;
	GgitDiffDelta *gdelta;
	gint ret;

	gdelta = _ggit_diff_delta_wrap (delta);

	ret = data->file_cb (gdelta, progress, data->user_data);

	ggit_diff_delta_unref (gdelta);

	return ret;
}

static gint
ggit_diff_hunk_callback_wrapper (const git_diff_delta *delta,
                                 const git_diff_hunk  *hunk,
                                 gpointer              user_data)
{
	CallbackWrapperData *data = user_data;
	GgitDiffDelta *gdelta;
	GgitDiffHunk *ghunk;
	gint ret;

	gdelta = _ggit_diff_delta_wrap (delta);
	ghunk = _ggit_diff_hunk_wrap (hunk);

	ret = data->hunk_cb (gdelta, ghunk, data->user_data);

	ggit_diff_hunk_unref (ghunk);
	ggit_diff_delta_unref (gdelta);

	return ret;
}

static gint
ggit_diff_line_callback_wrapper (const git_diff_delta *delta,
                                 const git_diff_hunk  *hunk,
                                 const git_diff_line  *line,
                                 gpointer              user_data)
{
	CallbackWrapperData *data = user_data;
	GgitDiffDelta *gdelta;
	GgitDiffHunk *ghunk;
	GgitDiffLine *gline;
	gint ret;

	gdelta = _ggit_diff_delta_wrap (delta);
	ghunk = hunk == NULL ? NULL : _ggit_diff_hunk_wrap (hunk);
	gline = line == NULL ? NULL : _ggit_diff_line_wrap (line);

	ret = data->line_cb (gdelta, ghunk, gline, data->user_data);

	if (gline != NULL)
	{
		ggit_diff_line_unref (gline);
	}

	if (ghunk != NULL)
	{
		ggit_diff_hunk_unref (ghunk);
	}

	ggit_diff_delta_unref (gdelta);

	return ret;
}

static void
ggit_diff_class_init (GgitDiffClass *klass)
{
}

static void
ggit_diff_init (GgitDiff *self)
{
}

GgitDiff *
_ggit_diff_wrap (git_diff *diff,
                 gboolean  owned)
{
	GgitDiff *gdiff;

	gdiff = g_object_new (GGIT_TYPE_DIFF, NULL);
	_ggit_native_set (gdiff, diff, NULL);

	if (owned)
	{
		_ggit_native_set_destroy_func (gdiff,
		                               (GDestroyNotify)git_diff_free);
	}

	return gdiff;
}

/**
 * ggit_diff_new_tree_to_tree:
 * @repository: a #GgitRepository.
 * @old_tree: (allow-none): a #GgitTree to diff from.
 * @new_tree: (allow-none): a #GgitTree to diff to.
 * @diff_options: (allow-none): a #GgitDiffOptions, or %NULL.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Creates a #GgitDiff which compares @old_tree and @new_tree.
 *
 * If @diff_options is %NULL then the defaults specified in
 * ggit_diff_options_new() are used.
 *
 * Returns: a newly allocated #GgitDiff if there was no error, %NULL otherwise.
 */
GgitDiff *
ggit_diff_new_tree_to_tree (GgitRepository   *repository,
                            GgitTree         *old_tree,
                            GgitTree         *new_tree,
                            GgitDiffOptions  *diff_options,
                            GError          **error)
{
	git_diff *diff;
	gint ret;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), NULL);
	g_return_val_if_fail (old_tree == NULL || GGIT_IS_TREE (old_tree), NULL);
	g_return_val_if_fail (new_tree == NULL || GGIT_IS_TREE (new_tree), NULL);
	g_return_val_if_fail (old_tree != NULL || new_tree != NULL, NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	ret = git_diff_tree_to_tree (&diff,
	                             _ggit_native_get (repository),
	                             old_tree ? _ggit_native_get (old_tree) : NULL,
	                             new_tree ? _ggit_native_get (new_tree) : NULL,
	                             _ggit_diff_options_get_diff_options (diff_options));

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return NULL;
	}

	return _ggit_diff_wrap (diff, TRUE);
}

/**
 * ggit_diff_new_tree_to_index:
 * @repository: a #GgitRepository.
 * @old_tree: (allow-none): a #GgitTree to diff from.
 * @index: (allow-none): a #GgitIndex, or %NULL.
 * @diff_options: (allow-none): a #GgitDiffOptions, or %NULL.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Creates a #GgitDiff which compares @old_tree and the index.
 *
 * If @index is %NULL then @repository index is used.
 * If @diff_options is %NULL then the defaults specified in
 * ggit_diff_options_new() are used.
 *
 * Returns: a newly allocated #GgitDiff if there was no error, %NULL otherwise.
 */
GgitDiff *
ggit_diff_new_tree_to_index (GgitRepository   *repository,
                             GgitTree         *old_tree,
                             GgitIndex        *index,
                             GgitDiffOptions  *diff_options,
                             GError          **error)
{
	git_diff *diff;
	gint ret;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), NULL);
	g_return_val_if_fail (old_tree == NULL || GGIT_IS_TREE (old_tree), NULL);
	g_return_val_if_fail (index == NULL || GGIT_IS_INDEX (index), NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	ret = git_diff_tree_to_index (&diff,
	                              _ggit_native_get (repository),
	                              old_tree ? _ggit_native_get (old_tree) : NULL,
	                              index ? _ggit_native_get (index) : NULL,
	                              _ggit_diff_options_get_diff_options (diff_options));

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return NULL;
	}

	return _ggit_diff_wrap (diff, TRUE);
}

/**
 * ggit_diff_new_index_to_workdir:
 * @repository: a #GgitRepository.
 * @index: (allow-none): a #GgitIndex, or %NULL.
 * @diff_options: (allow-none): a #GgitDiffOptions, or %NULL.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Creates a #GgitDiff which compares the working directory and the index.
 *
 * If @index is %NULL then @repository index is used.
 * If @diff_options is %NULL then the defaults specified in
 * ggit_diff_options_new() are used.
 *
 * Returns: a newly allocated #GgitDiff if there was no error, %NULL otherwise.
 */
GgitDiff *
ggit_diff_new_index_to_workdir (GgitRepository   *repository,
                                GgitIndex        *index,
                                GgitDiffOptions  *diff_options,
                                GError          **error)
{
	git_diff *diff;
	gint ret;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), NULL);
	g_return_val_if_fail (index == NULL || GGIT_IS_INDEX (index), NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	ret = git_diff_index_to_workdir (&diff,
	                                 _ggit_native_get (repository),
	                                 index ? _ggit_native_get (index) : NULL,
	                                 _ggit_diff_options_get_diff_options (diff_options));

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return NULL;
	}

	return _ggit_diff_wrap (diff, TRUE);
}

/**
 * ggit_diff_new_tree_to_workdir:
 * @repository: a #GgitRepository.
 * @old_tree: (allow-none): a #GgitTree to diff from.
 * @diff_options: (allow-none): a #GgitDiffOptions, or %NULL.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Creates a #GgitDiff which compares the working directory and @old_tree.
 *
 * If @diff_options is %NULL then the defaults specified in
 * ggit_diff_options_new() are used.
 *
 * Returns: a newly allocated #GgitDiff if there was no error, %NULL otherwise.
 */
GgitDiff *
ggit_diff_new_tree_to_workdir (GgitRepository   *repository,
                               GgitTree         *old_tree,
                               GgitDiffOptions  *diff_options,
                               GError          **error)
{
	git_diff *diff;
	gint ret;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), NULL);
	g_return_val_if_fail (old_tree == NULL || GGIT_IS_TREE (old_tree), NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	ret = git_diff_tree_to_workdir (&diff,
	                                _ggit_native_get (repository),
	                                old_tree ? _ggit_native_get (old_tree) : NULL,
	                                _ggit_diff_options_get_diff_options (diff_options));

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return NULL;
	}

	return _ggit_diff_wrap (diff, TRUE);
}

/**
 * ggit_diff_merge:
 * @onto: the #GgitDiff to merge into.
 * @from: the #GgitDiff to merge.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Merges @from into @onto unless @error is set.
 */
void
ggit_diff_merge (GgitDiff  *onto,
                 GgitDiff  *from,
                 GError   **error)
{
	gint ret;

	g_return_if_fail (GGIT_IS_DIFF (onto));
	g_return_if_fail (GGIT_IS_DIFF (from));
	g_return_if_fail (error == NULL || *error == NULL);

	ret = git_diff_merge (_ggit_native_get (onto),
	                      _ggit_native_get (from));

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
	}
}

/**
 * ggit_diff_foreach:
 * @diff: a #GgitDiff.
 * @file_cb: (allow-none) (scope call) (closure user_data):
 *  a #GgitDiffFileCallback.
 * @hunk_cb: (allow-none) (scope call) (closure user_data):
 *  a #GgitDiffHunkCallback.
 * @line_cb: (allow-none) (scope call) (closure user_data):
 *  a #GgitDiffLineCallback.
 * @user_data: callback user data.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Iterates over the diff calling @file_cb, @hunk_cb and @line_cb.
 */
void
ggit_diff_foreach (GgitDiff              *diff,
                   GgitDiffFileCallback   file_cb,
                   GgitDiffHunkCallback   hunk_cb,
                   GgitDiffLineCallback   line_cb,
                   gpointer              *user_data,
                   GError               **error)
{
	gint ret;
	CallbackWrapperData wrapper_data;
	git_diff_file_cb real_file_cb = NULL;
	git_diff_hunk_cb real_hunk_cb = NULL;
	git_diff_line_cb real_line_cb = NULL;

	g_return_if_fail (GGIT_IS_DIFF (diff));
	g_return_if_fail (file_cb != NULL && hunk_cb != NULL && line_cb != NULL);
	g_return_if_fail (error == NULL || *error == NULL);

	wrapper_data.user_data = user_data;

	if (file_cb != NULL)
	{
		real_file_cb = ggit_diff_file_callback_wrapper;
		wrapper_data.file_cb = file_cb;
	}

	if (hunk_cb != NULL)
	{
		real_hunk_cb = ggit_diff_hunk_callback_wrapper;
		wrapper_data.hunk_cb = hunk_cb;
	}

	if (line_cb != NULL)
	{
		real_line_cb = ggit_diff_line_callback_wrapper;
		wrapper_data.line_cb = line_cb;
	}

	ret = git_diff_foreach (_ggit_native_get (diff),
	                        real_file_cb, real_hunk_cb, real_line_cb,
	                        &wrapper_data);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
	}
}

/**
 * ggit_diff_print:
 * @diff: a #GgitDiff.
 * @type: a #GgitDiffFormatType.
 * @print_cb: (scope call) (closure user_data): a #GgitDiffLineCallback.
 * @user_data: callback user data.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Iterates over @diff generating text output like "git diff".
 */
void
ggit_diff_print (GgitDiff              *diff,
                 GgitDiffFormatType     type,
                 GgitDiffLineCallback   print_cb,
                 gpointer              *user_data,
                 GError               **error)
{
	gint ret;
	CallbackWrapperData wrapper_data;

	g_return_if_fail (GGIT_IS_DIFF (diff));
	g_return_if_fail (print_cb != NULL);
	g_return_if_fail (error == NULL || *error == NULL);

	wrapper_data.user_data = user_data;
	wrapper_data.line_cb = print_cb;

	ret = git_diff_print (_ggit_native_get (diff), type,
	                      ggit_diff_line_callback_wrapper,
	                      &wrapper_data);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
	}
}

/**
 * ggit_diff_get_num_deltas:
 * @diff: a #GgitDiff.
 *
 * Get the number of deltas in the diff list.
 *
 * Returns: the number of deltas.
 *
 **/
gint
ggit_diff_get_num_deltas (GgitDiff *diff)
{
	g_return_val_if_fail (GGIT_IS_DIFF (diff), 0);

	return git_diff_num_deltas (_ggit_native_get (diff));
}

/**
 * ggit_diff_blobs:
 * @old_blob: (allow-none): a #GgitBlob to diff from.
 * @old_as_path: (allow-none): treat @old_blob as if it had this filename, or %NULL,
 * @new_blob: (allow-none): a #GgitBlob to diff to.
 * @new_as_path: (allow-none): treat @new_blob as if it had this filename, or %NULL,
 * @diff_options: (allow-none): a #GgitDiffOptions, or %NULL.
 * @file_cb: (allow-none) (scope call) (closure user_data):
 *  a #GgitDiffFileCallback.
 * @hunk_cb: (allow-none) (scope call) (closure user_data):
 *  a #GgitDiffHunkCallback.
 * @line_cb: (allow-none) (scope call) (closure user_data):
 *  a #GgitDiffLineCallback.
 * @user_data: callback user data.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Iterates over the diff calling @file_cb, @hunk_cb and @line_cb.
 *
 * The #GgitDiffFile mode always be 0, path will be %NULL and when a blob is
 * %NULL the oid will be 0.
 *
 * If @diff_options is %NULL then the defaults specified in
 * ggit_diff_options_new() are used.
 */
void
ggit_diff_blobs (GgitBlob              *old_blob,
                 const gchar           *old_as_path,
                 GgitBlob              *new_blob,
                 const gchar           *new_as_path,
                 GgitDiffOptions       *diff_options,
                 GgitDiffFileCallback   file_cb,
                 GgitDiffHunkCallback   hunk_cb,
                 GgitDiffLineCallback   line_cb,
                 gpointer              *user_data,
                 GError               **error)
{
	gint ret;
	const git_diff_options *gdiff_options;
	CallbackWrapperData wrapper_data;
	git_diff_file_cb real_file_cb = NULL;
	git_diff_hunk_cb real_hunk_cb = NULL;
	git_diff_line_cb real_line_cb = NULL;

	g_return_if_fail (error == NULL || *error == NULL);

	gdiff_options = _ggit_diff_options_get_diff_options (diff_options);

	wrapper_data.user_data = user_data;

	if (file_cb != NULL)
	{
		real_file_cb = ggit_diff_file_callback_wrapper;
		wrapper_data.file_cb = file_cb;
	}

	if (hunk_cb != NULL)
	{
		real_hunk_cb = ggit_diff_hunk_callback_wrapper;
		wrapper_data.hunk_cb = hunk_cb;
	}

	if (line_cb != NULL)
	{
		real_line_cb = ggit_diff_line_callback_wrapper;
		wrapper_data.line_cb = line_cb;
	}

	ret = git_diff_blobs (old_blob ? _ggit_native_get (old_blob) : NULL,
	                      old_as_path,
	                      new_blob ? _ggit_native_get (new_blob) : NULL,
	                      new_as_path,
	                      (git_diff_options *) gdiff_options,
	                      real_file_cb, real_hunk_cb, real_line_cb,
	                      &wrapper_data);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
	}
}

/**
 * ggit_diff_blob_to_buffer:
 * @old_blob: (allow-none): a #GgitBlob to diff from.
 * @old_as_path: (allow-none): treat @old_blob as if it had this filename, or %NULL,
 * @buffer: (allow-none) (array length=buffer_len): a buffer to diff to.
 * @buffer_len: length of @buffer.
 * @buffer_as_path: (allow-none): treat @buffer as if it had this filename, or %NULL,
 * @diff_options: (allow-none): a #GgitDiffOptions, or %NULL.
 * @file_cb: (allow-none) (scope call) (closure user_data):
 *  a #GgitDiffFileCallback.
 * @hunk_cb: (allow-none) (scope call) (closure user_data):
 *  a #GgitDiffHunkCallback.
 * @line_cb: (allow-none) (scope call) (closure user_data):
 *  a #GgitDiffLineCallback.
 * @user_data: callback user data.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Same as ggit_diff_blobs() but using a buffer.
 */
void
ggit_diff_blob_to_buffer (GgitBlob              *old_blob,
                          const gchar           *old_as_path,
                          const gchar           *buffer,
                          gsize                  buffer_len,
                          const gchar           *buffer_as_path,
                          GgitDiffOptions       *diff_options,
                          GgitDiffFileCallback   file_cb,
                          GgitDiffHunkCallback   hunk_cb,
                          GgitDiffLineCallback   line_cb,
                          gpointer              *user_data,
                          GError               **error)
{
	gint ret;
	const git_diff_options *gdiff_options;
	CallbackWrapperData wrapper_data;
	git_diff_file_cb real_file_cb = NULL;
	git_diff_hunk_cb real_hunk_cb = NULL;
	git_diff_line_cb real_line_cb = NULL;

	g_return_if_fail (error == NULL || *error == NULL);

	gdiff_options = _ggit_diff_options_get_diff_options (diff_options);

	wrapper_data.user_data = user_data;

	if (file_cb != NULL)
	{
		real_file_cb = ggit_diff_file_callback_wrapper;
		wrapper_data.file_cb = file_cb;
	}

	if (hunk_cb != NULL)
	{
		real_hunk_cb = ggit_diff_hunk_callback_wrapper;
		wrapper_data.hunk_cb = hunk_cb;
	}

	if (line_cb != NULL)
	{
		real_line_cb = ggit_diff_line_callback_wrapper;
		wrapper_data.line_cb = line_cb;
	}

	ret = git_diff_blob_to_buffer (old_blob ? _ggit_native_get (old_blob) : NULL,
	                               old_as_path,
	                               buffer,
	                               buffer_len,
	                               buffer_as_path,
	                               (git_diff_options *) gdiff_options,
	                               real_file_cb, real_hunk_cb, real_line_cb,
	                               &wrapper_data);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
	}
}

/* ex:set ts=8 noet: */
