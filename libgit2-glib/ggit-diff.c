/*
 * ggit-diff.c
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

#include <string.h>

#include <git2.h>

#include "ggit-diff.h"
#include "ggit-diff-binary.h"
#include "ggit-diff-delta.h"
#include "ggit-diff-options.h"
#include "ggit-diff-line.h"
#include "ggit-diff-hunk.h"
#include "ggit-patch.h"
#include "ggit-error.h"
#include "ggit-repository.h"
#include "ggit-diff-file.h"
#include "ggit-diff-find-options.h"
#include "ggit-diff-format-email-options.h"


/**
 * GgitDiff:
 *
 * Represents a diff list.
 */

typedef struct _GgitDiffPrivate
{
	GgitRepository *repository;
	gchar *encoding;
} GgitDiffPrivate;

typedef struct {
	GgitDiff *diff;
	const gchar *encoding;

	gpointer user_data;

	GHashTable *cached_deltas;
	GHashTable *cached_hunks;

	GgitDiffFileCallback file_cb;
	GgitDiffBinaryCallback binary_cb;
	GgitDiffHunkCallback hunk_cb;
	GgitDiffLineCallback line_cb;
} CallbackWrapperData;

G_DEFINE_TYPE_WITH_PRIVATE (GgitDiff, ggit_diff, GGIT_TYPE_NATIVE)

enum
{
	PROP_0,
	PROP_REPOSITORY
};

static void
wrapper_data_init (CallbackWrapperData *data)
{
	if (data != NULL)
	{
		data->cached_deltas =
			g_hash_table_new_full (g_str_hash,
			                       g_str_equal,
			                       g_free,
			                       (GDestroyNotify) ggit_diff_delta_unref);

		data->cached_hunks =
			g_hash_table_new_full (g_str_hash,
			                       g_str_equal,
			                       g_free,
			                       (GDestroyNotify) ggit_diff_hunk_unref);
	}
}

static GgitDiffDelta *
wrap_diff_delta_cached (CallbackWrapperData  *data,
                        const git_diff_delta *delta)
{
	GgitDiffDelta *gdelta;
	const gsize size = GIT_OID_HEXSZ + 1;
	gchar key[size];

	if (!delta)
	{
		return NULL;
	}

	git_oid_tostr (key, size, &delta->old_file.id);

	gdelta = g_hash_table_lookup (data->cached_deltas, key);

	if (!gdelta)
	{
		gdelta = _ggit_diff_delta_wrap (delta);
		g_hash_table_insert (data->cached_deltas, g_strdup (key), gdelta);
	}

	return gdelta;
}

static GgitDiffHunk *
wrap_diff_hunk_cached (CallbackWrapperData *data,
                       const git_diff_delta *delta,
                       const git_diff_hunk *hunk)
{
	GgitDiffHunk *ghunk;
	const gsize oid_size = GIT_OID_HEXSZ;
	const gsize hdr_size = GIT_DIFF_HUNK_HEADER_SIZE + 1;
	const gsize size = oid_size + hdr_size;
	gchar key[size];

	if (!delta || !hunk)
	{
		return NULL;
	}

	git_oid_nfmt (key, oid_size, &delta->old_file.id);
	memcpy(key + oid_size, hunk->header, hunk->header_len + 1);

	ghunk = g_hash_table_lookup (data->cached_hunks, key);

	if (!ghunk)
	{
		ghunk = _ggit_diff_hunk_wrap (hunk);
		g_hash_table_insert (data->cached_hunks, g_strdup (key), ghunk);
	}

	return ghunk;
}

static gint
ggit_diff_file_callback_wrapper (const git_diff_delta *delta,
                                 gfloat                progress,
                                 gpointer              user_data)
{
	CallbackWrapperData *data = user_data;
	GgitDiffDelta *gdelta;
	gint ret;

	gdelta = wrap_diff_delta_cached (data, delta);

	data->encoding = NULL;

	if (data->diff != NULL)
	{
		GgitDiffPrivate *priv;
		GgitDiffFile *file;

		priv = ggit_diff_get_instance_private (data->diff);

		if (ggit_diff_delta_get_status (gdelta) == GGIT_DELTA_DELETED)
		{
			file = ggit_diff_delta_get_old_file (gdelta);
		}
		else
		{
			file = ggit_diff_delta_get_new_file (gdelta);
		}

		if (file != NULL)
		{
			const gchar *path;

			path = ggit_diff_file_get_path (file);

			data->encoding =
				ggit_repository_get_attribute (priv->repository,
			                                       path,
			                                       "encoding",
			                                       GGIT_ATTRIBUTE_CHECK_FILE_THEN_INDEX,
			                                       NULL);
		}
	}

	ret = data->file_cb (gdelta, progress, data->user_data);

	return ret;
}

static gint
ggit_diff_binary_callback_wrapper (const git_diff_delta  *delta,
                                   const git_diff_binary *binary,
                                   gpointer               user_data)
{
	CallbackWrapperData *data = user_data;
	GgitDiffDelta *gdelta;
	GgitDiffBinary *gbinary;
	gint ret;

	gdelta = wrap_diff_delta_cached (data, delta);
	gbinary = _ggit_diff_binary_wrap (binary);

	ret = data->binary_cb (gdelta, gbinary, data->user_data);

	ggit_diff_binary_unref (gbinary);

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

	gdelta = wrap_diff_delta_cached (data, delta);
	ghunk = wrap_diff_hunk_cached (data, delta, hunk);

	ret = data->hunk_cb (gdelta, ghunk, data->user_data);

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
	GgitDiffHunk *ghunk = NULL;
	GgitDiffLine *gline;
	gint ret;
	const gchar *encoding = NULL;

	if (data->encoding != NULL)
	{
		encoding = data->encoding;
	}
	else if (data->diff != NULL)
	{
		GgitDiffPrivate *priv;

		priv = ggit_diff_get_instance_private (data->diff);
		encoding = priv->encoding;
	}

	gdelta = wrap_diff_delta_cached (data, delta);
	ghunk = wrap_diff_hunk_cached (data, delta, hunk);

	gline = line == NULL ? NULL : _ggit_diff_line_wrap (line, encoding);

	ret = data->line_cb (gdelta, ghunk, gline, data->user_data);

	if (gline != NULL)
	{
		ggit_diff_line_unref (gline);
	}

	return ret;
}

static void
ggit_diff_finalize (GObject *object)
{
	GgitDiff *diff = GGIT_DIFF (object);
	GgitDiffPrivate *priv;

	priv = ggit_diff_get_instance_private (diff);

	g_free (priv->encoding);

	G_OBJECT_CLASS (ggit_diff_parent_class)->finalize (object);
}

static void
ggit_diff_set_property (GObject      *object,
                        guint         prop_id,
                        const GValue *value,
                        GParamSpec   *pspec)
{
	GgitDiff *diff = GGIT_DIFF (object);
	GgitDiffPrivate *priv;

	priv = ggit_diff_get_instance_private (diff);

	switch (prop_id)
	{
		case PROP_REPOSITORY:
			priv->repository = g_value_dup_object (value);
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void
ggit_diff_get_property (GObject    *object,
                        guint       prop_id,
                        GValue     *value,
                        GParamSpec *pspec)
{
	GgitDiff *diff = GGIT_DIFF (object);
	GgitDiffPrivate *priv;

	priv = ggit_diff_get_instance_private (diff);

	switch (prop_id)
	{
		case PROP_REPOSITORY:
			g_value_set_object (value, priv->repository);
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void
ggit_diff_constructed (GObject *object)
{
	GgitDiff *diff = GGIT_DIFF (object);
	GgitDiffPrivate *priv;
	GgitConfig *config = NULL;

	priv = ggit_diff_get_instance_private (diff);

	if (priv->repository != NULL)
	{
		config = ggit_repository_get_config (priv->repository, NULL);
	}

	if (config != NULL)
	{
		const gchar *enc;

		enc = ggit_config_get_string (config, "gui.encoding", NULL);

		if (enc != NULL)
		{
			priv->encoding = g_strdup (enc);
		}

		g_object_unref (config);
	}
}

static void
ggit_diff_class_init (GgitDiffClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->finalize = ggit_diff_finalize;
	object_class->constructed = ggit_diff_constructed;

	object_class->get_property = ggit_diff_get_property;
	object_class->set_property = ggit_diff_set_property;

	g_object_class_install_property (object_class,
	                                 PROP_REPOSITORY,
	                                 g_param_spec_object ("repository",
	                                                      "Repository",
	                                                      "Repository",
	                                                      GGIT_TYPE_REPOSITORY,
	                                                      G_PARAM_READWRITE |
	                                                      G_PARAM_CONSTRUCT_ONLY |
	                                                      G_PARAM_STATIC_STRINGS));
}

static void
ggit_diff_init (GgitDiff *self)
{
}

static GgitDiff *
_ggit_diff_wrap (GgitRepository *repository,
                 git_diff       *diff)
{
	GgitDiff *gdiff;

	gdiff = g_object_new (GGIT_TYPE_DIFF, "repository", repository, NULL);
	_ggit_native_set (gdiff, diff, (GDestroyNotify)git_diff_free);

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
 * Returns: (transfer full) (nullable): a newly allocated #GgitDiff if
 * there was no error, %NULL otherwise.
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

	return _ggit_diff_wrap (repository, diff);
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
 * Returns: (transfer full) (nullable): a newly allocated #GgitDiff if
 * there was no error, %NULL otherwise.
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

	return _ggit_diff_wrap (repository, diff);
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
 * Returns: (transfer full) (nullable): a newly allocated #GgitDiff if
 * there was no error, %NULL otherwise.
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

	return _ggit_diff_wrap (repository, diff);
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
 * Returns: (transfer full) (nullable): a newly allocated #GgitDiff if
 * there was no error, %NULL otherwise.
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

	return _ggit_diff_wrap (repository, diff);
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
 * @binary_cb: (allow-none) (scope call) (closure user_data):
 *  a #GgitDiffBinaryCallback.
 * @hunk_cb: (allow-none) (scope call) (closure user_data):
 *  a #GgitDiffHunkCallback.
 * @line_cb: (allow-none) (scope call) (closure user_data):
 *  a #GgitDiffLineCallback.
 * @user_data: callback user data.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Iterates over the diff calling @file_cb, @binary_cb, @hunk_cb and @line_cb.
 */
void
ggit_diff_foreach (GgitDiff              *diff,
                   GgitDiffFileCallback   file_cb,
                   GgitDiffBinaryCallback binary_cb,
                   GgitDiffHunkCallback   hunk_cb,
                   GgitDiffLineCallback   line_cb,
                   gpointer              *user_data,
                   GError               **error)
{
	gint ret;
	CallbackWrapperData wrapper_data = { 0 };
	git_diff_file_cb real_file_cb = NULL;
	git_diff_binary_cb real_binary_cb = NULL;
	git_diff_hunk_cb real_hunk_cb = NULL;
	git_diff_line_cb real_line_cb = NULL;

	g_return_if_fail (GGIT_IS_DIFF (diff));
	g_return_if_fail (file_cb != NULL && binary_cb != NULL && hunk_cb != NULL && line_cb != NULL);
	g_return_if_fail (error == NULL || *error == NULL);

	wrapper_data_init (&wrapper_data);

	wrapper_data.user_data = user_data;
	wrapper_data.diff = diff;


	if (file_cb != NULL)
	{
		real_file_cb = ggit_diff_file_callback_wrapper;
		wrapper_data.file_cb = file_cb;
	}

	if (binary_cb != NULL)
	{
		real_binary_cb = ggit_diff_binary_callback_wrapper;
		wrapper_data.binary_cb = binary_cb;
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
	                        real_file_cb, real_binary_cb,
	                        real_hunk_cb, real_line_cb,
	                        &wrapper_data);

	g_hash_table_destroy (wrapper_data.cached_deltas);
	g_hash_table_destroy (wrapper_data.cached_hunks);

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
	CallbackWrapperData wrapper_data = { 0 };

	g_return_if_fail (GGIT_IS_DIFF (diff));
	g_return_if_fail (print_cb != NULL);
	g_return_if_fail (error == NULL || *error == NULL);

	wrapper_data_init (&wrapper_data);

	wrapper_data.user_data = user_data;
	wrapper_data.diff = diff;

	wrapper_data.line_cb = print_cb;

	ret = git_diff_print (_ggit_native_get (diff), (git_diff_format_t)type,
	                      ggit_diff_line_callback_wrapper,
	                      &wrapper_data);

	g_hash_table_destroy (wrapper_data.cached_deltas);
	g_hash_table_destroy (wrapper_data.cached_hunks);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
	}
}

/**
 * ggit_diff_format_email:
 * @diff: a #GgitDiff.
 * @options: a #GgitDiffFormatEmailOptions.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Create an e-mail ready patch from a diff.
 *
 * Returns: (transfer full) (nullable): the patch or %NULL if an error occurred.
 *
 **/
gchar *
ggit_diff_format_email (GgitDiff                    *diff,
                        GgitDiffFormatEmailOptions  *options,
                        GError                     **error)
{
	gint ret;
	gchar *retval;
	git_buf buf = {0,};

	g_return_val_if_fail (GGIT_IS_DIFF (diff), NULL);
	g_return_val_if_fail (options == NULL || GGIT_IS_DIFF_FORMAT_EMAIL_OPTIONS (options), NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	ret = git_diff_format_email (&buf,
	                             _ggit_native_get (diff),
	                             options ? _ggit_diff_format_email_options_get_diff_format_email_options (options) : NULL);

	if (ret != GIT_OK)
	{
		git_buf_free (&buf);
		_ggit_error_set (error, ret);
		return NULL;
	}

	retval = g_strndup (buf.ptr, buf.size);
	git_buf_free (&buf);

	return retval;
}

/**
 * ggit_diff_get_num_deltas:
 * @diff: a #GgitDiff.
 *
 * Get the number of deltas in the diff.
 *
 * Returns: the number of deltas.
 *
 **/
gsize
ggit_diff_get_num_deltas (GgitDiff *diff)
{
	g_return_val_if_fail (GGIT_IS_DIFF (diff), 0);

	return git_diff_num_deltas (_ggit_native_get (diff));
}

/**
 * ggit_diff_get_delta:
 * @diff: a #GgitDiff.
 * @index: the index.
 *
 * Get the delta at the specified index.
 *
 * Returns: (transfer full) (nullable): a #GgitDiffDelta or %NULL.
 *
 **/
GgitDiffDelta *
ggit_diff_get_delta (GgitDiff *diff,
                     gsize     index)
{
	const git_diff_delta *delta;

	g_return_val_if_fail (GGIT_IS_DIFF (diff), NULL);

	delta = git_diff_get_delta (_ggit_native_get (diff), index);

	return _ggit_diff_delta_wrap (delta);
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
 * @binary_cb: (allow-none) (scope call) (closure user_data):
 *  a #GgitDiffBinaryCallback.
 * @hunk_cb: (allow-none) (scope call) (closure user_data):
 *  a #GgitDiffHunkCallback.
 * @line_cb: (allow-none) (scope call) (closure user_data):
 *  a #GgitDiffLineCallback.
 * @user_data: callback user data.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Iterates over the diff calling @file_cb, @binary_cb, @hunk_cb and @line_cb.
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
                 GgitDiffBinaryCallback binary_cb,
                 GgitDiffHunkCallback   hunk_cb,
                 GgitDiffLineCallback   line_cb,
                 gpointer              *user_data,
                 GError               **error)
{
	gint ret;
	const git_diff_options *gdiff_options;
	CallbackWrapperData wrapper_data = { 0 };
	git_diff_file_cb real_file_cb = NULL;
	git_diff_binary_cb real_binary_cb = NULL;
	git_diff_hunk_cb real_hunk_cb = NULL;
	git_diff_line_cb real_line_cb = NULL;

	g_return_if_fail (error == NULL || *error == NULL);

	gdiff_options = _ggit_diff_options_get_diff_options (diff_options);

	wrapper_data_init (&wrapper_data);

	wrapper_data.user_data = user_data;

	if (file_cb != NULL)
	{
		real_file_cb = ggit_diff_file_callback_wrapper;
		wrapper_data.file_cb = file_cb;
	}

	if (binary_cb != NULL)
	{
		real_binary_cb = ggit_diff_binary_callback_wrapper;
		wrapper_data.binary_cb = binary_cb;
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
	                      real_file_cb, real_binary_cb,
	                      real_hunk_cb, real_line_cb,
	                      &wrapper_data);

	g_hash_table_destroy (wrapper_data.cached_deltas);
	g_hash_table_destroy (wrapper_data.cached_hunks);

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
 * @binary_cb: (allow-none) (scope call) (closure user_data):
 *  a #GgitDiffBinaryCallback.
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
                          const guint8          *buffer,
                          gssize                 buffer_len,
                          const gchar           *buffer_as_path,
                          GgitDiffOptions       *diff_options,
                          GgitDiffFileCallback   file_cb,
                          GgitDiffBinaryCallback binary_cb,
                          GgitDiffHunkCallback   hunk_cb,
                          GgitDiffLineCallback   line_cb,
                          gpointer               user_data,
                          GError               **error)
{
	gint ret;
	const git_diff_options *gdiff_options;
	CallbackWrapperData wrapper_data = { 0 };
	git_diff_file_cb real_file_cb = NULL;
	git_diff_binary_cb real_binary_cb = NULL;
	git_diff_hunk_cb real_hunk_cb = NULL;
	git_diff_line_cb real_line_cb = NULL;

	g_return_if_fail (error == NULL || *error == NULL);

	gdiff_options = _ggit_diff_options_get_diff_options (diff_options);

	wrapper_data_init (&wrapper_data);

	wrapper_data.user_data = user_data;

	if (buffer_len == -1)
	{
		buffer_len = strlen((const gchar *) buffer);
	}

	if (file_cb != NULL)
	{
		real_file_cb = ggit_diff_file_callback_wrapper;
		wrapper_data.file_cb = file_cb;
	}

	if (binary_cb != NULL)
	{
		real_binary_cb = ggit_diff_binary_callback_wrapper;
		wrapper_data.binary_cb = binary_cb;
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
	                               (const gchar *) buffer,
	                               buffer_len,
	                               buffer_as_path,
	                               (git_diff_options *) gdiff_options,
	                               real_file_cb, real_binary_cb,
	                               real_hunk_cb, real_line_cb,
	                               &wrapper_data);

	g_hash_table_destroy (wrapper_data.cached_deltas);
	g_hash_table_destroy (wrapper_data.cached_hunks);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
	}
}

/**
 * ggit_diff_find_similar:
 * @diff: a #GgitDiff.
 * @options: (allow-none): a #GgitDiffFindOptions or %NULL.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Transform @diff marking file renames, copies, etc.. If @options is set to
 * %NULL, then the default options will be used.
 *
 * Returns: %TRUE if there were no errors, %FALSE otherwise.
 *
 **/
gboolean
ggit_diff_find_similar (GgitDiff             *diff,
                        GgitDiffFindOptions  *options,
                        GError              **error)
{
	gint ret;

	g_return_val_if_fail (GGIT_IS_DIFF (diff), FALSE);
	g_return_val_if_fail (options == NULL || GGIT_IS_DIFF_FIND_OPTIONS (options), FALSE);
	g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

	ret = git_diff_find_similar (_ggit_native_get (diff),
	                             _ggit_diff_find_options_get_diff_find_options (options));

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return FALSE;
	}

	return TRUE;
}

/* ex:set ts=8 noet: */
