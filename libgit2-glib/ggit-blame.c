/*
 * ggit-blame.h
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2013 - Jesse van den Kieboom
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

#include "ggit-blame.h"
#include "ggit-oid.h"
#include "ggit-signature.h"
#include "ggit-error.h"

struct _GgitBlameHunk
{
	guint16 lines_in_hunk;

	GgitOId       *final_commit_id;
	guint16        final_start_line_number;
	GgitSignature *final_signature;

	GgitOId       *orig_commit_id;
	gchar         *orig_path;
	guint16        orig_start_line_number;
	GgitSignature *orig_signature;

	gboolean       boundary;

	gint           ref_count;
};

G_DEFINE_TYPE (GgitBlame, ggit_blame, GGIT_TYPE_NATIVE)

G_DEFINE_BOXED_TYPE (GgitBlameHunk, ggit_blame_hunk,
                     ggit_blame_hunk_ref, ggit_blame_hunk_unref);

/**
 * ggit_blame_hunk_ref:
 * @blame_hunk: a #GgitBlameHunk.
 *
 * Atomically increments the reference count of @blame_hunk by one.
 * This function is MT-safe and may be called from any thread.
 *
 * Returns: (transfer none): a #GgitBlameHunk.
 */
GgitBlameHunk *
ggit_blame_hunk_ref (GgitBlameHunk *blame_hunk)
{
	g_return_val_if_fail (blame_hunk != NULL, NULL);

	g_atomic_int_inc (&blame_hunk->ref_count);

	return blame_hunk;
}

/**
 * ggit_blame_hunk_unref:
 * @blame_hunk: a #GgitBlameHunk.
 *
 * Atomically decrements the reference count of @blame_hunk by one.
 * If the reference count drops to 0, @blame_hunk is freed.
 */
void
ggit_blame_hunk_unref (GgitBlameHunk *blame_hunk)
{
	g_return_if_fail (blame_hunk != NULL);

	if (g_atomic_int_dec_and_test (&blame_hunk->ref_count))
	{
		if (blame_hunk->final_commit_id != NULL)
		{
			ggit_oid_free (blame_hunk->final_commit_id);
		}

		g_clear_object (&blame_hunk->final_signature);

		if (blame_hunk->orig_commit_id != NULL)
		{
			ggit_oid_free (blame_hunk->orig_commit_id);
		}

		g_clear_object (&blame_hunk->orig_signature);

		g_free (blame_hunk->orig_path);

		g_slice_free (GgitBlameHunk, blame_hunk);
	}
}

static GgitBlameHunk *
ggit_blame_hunk_wrap (const git_blame_hunk *gblame_hunk)
{
	GgitBlameHunk *blame_hunk;

	blame_hunk = g_slice_new0 (GgitBlameHunk);
	blame_hunk->ref_count = 1;

	blame_hunk->lines_in_hunk = gblame_hunk->lines_in_hunk;

	blame_hunk->final_commit_id = _ggit_oid_wrap (&gblame_hunk->final_commit_id);
	blame_hunk->final_start_line_number = gblame_hunk->final_start_line_number;

	if (gblame_hunk->final_signature)
	{
		blame_hunk->final_signature =
			_ggit_signature_wrap (gblame_hunk->final_signature,
			                      NULL,
			                      FALSE);
	}

	blame_hunk->orig_commit_id = _ggit_oid_wrap (&gblame_hunk->orig_commit_id);
	blame_hunk->orig_start_line_number = gblame_hunk->orig_start_line_number;

	if (gblame_hunk->orig_signature)
	{
		blame_hunk->orig_signature =
			_ggit_signature_wrap (gblame_hunk->orig_signature,
			                      NULL,
			                      FALSE);
	}

	blame_hunk->orig_path = g_strdup (gblame_hunk->orig_path);
	return blame_hunk;
}

/**
 * ggit_blame_hunk_get_lines_in_hunk:
 * @blame_hunk: a #GgitBlameHunk.
 *
 * Get the number of lines in the hunk.
 *
 * Returns: the number of lines in the hunk.
 *
 **/
guint16
ggit_blame_hunk_get_lines_in_hunk (GgitBlameHunk *blame_hunk)
{
	g_return_val_if_fail (blame_hunk != NULL, 0);

	return blame_hunk->lines_in_hunk;
}

/**
 * ggit_blame_hunk_get_final_commit_id:
 * @blame_hunk: a #GgitBlameHunk.
 *
 * Get the id of the commit where this hunk was last changed.
 *
 * Returns: (transfer none): a #GgitOId.
 *
 **/
GgitOId *
ggit_blame_hunk_get_final_commit_id (GgitBlameHunk *blame_hunk)
{
	g_return_val_if_fail (blame_hunk != NULL, NULL);

	return blame_hunk->final_commit_id;
}

/**
 * ggit_blame_hunk_get_final_start_line_number:
 * @blame_hunk: a #GgitBlameHunk.
 *
 * Get the line number where the hunk begins in the final version of the file.
 * Line numbers start at 1.
 *
 * Returns: the final hunk line number.
 *
 **/
guint16
ggit_blame_hunk_get_final_start_line_number (GgitBlameHunk *blame_hunk)
{
	g_return_val_if_fail (blame_hunk != NULL, 0);

	return blame_hunk->final_start_line_number;
}

/**
 * ggit_blame_hunk_get_final_signature:
 * @blame_hunk: a #GgitBlameHunk.
 *
 * Get the signature of the final version of the hunk.
 *
 * Returns: (transfer none): a #GgitSignature.
 *
 **/
GgitSignature *
ggit_blame_hunk_get_final_signature (GgitBlameHunk *blame_hunk)
{
	g_return_val_if_fail (blame_hunk != NULL, NULL);

	return blame_hunk->final_signature;
}

/**
 * ggit_blame_hunk_get_orig_commit_id:
 * @blame_hunk: a #GgitBlameHunk.
 *
 * Get the id of the commit where the hunk was found. This is usually the same
 * the final commit id, except when #GGIT_BLAME_TRACK_COPIES_ANY_COMMIT was used.
 *
 * Returns: (transfer none): a #GgitOId.
 *
 **/
GgitOId *
ggit_blame_hunk_get_orig_commit_id (GgitBlameHunk *blame_hunk)
{
	g_return_val_if_fail (blame_hunk != NULL, NULL);

	return blame_hunk->orig_commit_id;
}

/**
 * ggit_blame_hunk_get_orig_start_line_number:
 * @blame_hunk: a #GgitBlameHunk.
 *
 * Get the line number where the hunk begins in the file specified by
 * #ggit_blame_hunk_get_orig_path at the commit specified by
 * #ggit_blame_hunk_get_orig_commit_id. Line numbers start at 1.
 *
 * Returns: the orig hunk line number.
 *
 **/
guint16
ggit_blame_hunk_get_orig_start_line_number (GgitBlameHunk *blame_hunk)
{
	g_return_val_if_fail (blame_hunk != NULL, 0);

	return blame_hunk->orig_start_line_number;
}

/**
 * ggit_blame_hunk_get_orig_signature:
 * @blame_hunk: a #GgitBlameHunk.
 *
 * Get the signature of the commit specified by #ggit_blame_hunk_get_orig_commit_id.
 *
 * Returns: (transfer none): a #GgitSignature.
 *
 **/
GgitSignature *
ggit_blame_hunk_get_orig_signature (GgitBlameHunk *blame_hunk)
{
	g_return_val_if_fail (blame_hunk != NULL, NULL);

	return blame_hunk->orig_signature;
}

/**
 * ggit_blame_hunk_get_orig_path:
 * @blame_hunk: a #GgitBlameHunk.
 *
 * Get the path of the file where this hunk originated, as of the commit
 * specified by #ggit_blame_hunk_get_orig_commit_id.
 *
 * Returns: the path.
 */
const gchar *
ggit_blame_hunk_get_orig_path (GgitBlameHunk *blame_hunk)
{
	g_return_val_if_fail (blame_hunk != NULL, NULL);

	return blame_hunk->orig_path;
}

/**
 * ggit_blame_hunk_is_boundary:
 * @blame_hunk: a #GgitBlameHunk.
 *
 * Get whether the hunk has been tracked to a boundary commit (the root,
 * or the commit specified in #ggit_blame_options_set_oldest_commit).
 *
 * Returns: whether the hunk is at a boundary commit.
 *
 **/
gboolean
ggit_blame_hunk_is_boundary (GgitBlameHunk *blame_hunk)
{
	g_return_val_if_fail (blame_hunk != NULL, FALSE);

	return blame_hunk->boundary;
}

static void
ggit_blame_class_init (GgitBlameClass *klass)
{
}

static void
ggit_blame_init (GgitBlame *self)
{
}

GgitBlame *
_ggit_blame_wrap (git_blame *blame)
{
	GgitBlame *ret;

	ret = g_object_new (GGIT_TYPE_BLAME, NULL);
	_ggit_native_set (ret, blame, (GDestroyNotify)git_blame_free);

	return ret;
}

guint32
ggit_blame_get_hunk_count (GgitBlame *blame)
{
	g_return_val_if_fail (GGIT_IS_BLAME (blame), 0);
	return git_blame_get_hunk_count (_ggit_native_get (blame));
}

GgitBlameHunk *
ggit_blame_get_hunk_by_line (GgitBlame *blame,
                             guint32    line)
{
	g_return_val_if_fail (GGIT_IS_BLAME (blame), NULL);

	return ggit_blame_hunk_wrap (git_blame_get_hunk_byline (_ggit_native_get (blame),
	                                                        line));
}

GgitBlameHunk *
ggit_blame_get_hunk_by_index (GgitBlame *blame,
                              guint32    idx)
{
	g_return_val_if_fail (GGIT_IS_BLAME (blame), NULL);

	return ggit_blame_hunk_wrap (git_blame_get_hunk_byindex (_ggit_native_get (blame),
	                                                         idx));
}

/**
 * ggit_blame_from_buffer:
 * @blame: a #GgitBlame.
 * @buffer: (array length=buffer_length): the contents of the file.
 * @buffer_length: the length of the buffer.
 * @error: a #GError.
 *
 * Get blame data for a file that has been modified in memory. @blame is a
 * pre-calculated blame for the in-odb history of the file. This means that once
 * a file blame is completed (which can be expensitve), updating the buffer
 * blame is very fast.
 *
 * Lines that differ between the buffer and the committed version are marked as
 * having a zero id for their #ggit_blame_hunk_get_final_commit_id.
 *
 * Returns: (transfer full): a #GgitBlame.
 *
 **/
GgitBlame *
ggit_blame_from_buffer (GgitBlame       *blame,
                        guint8 const    *buffer,
                        gsize            buffer_length,
                        GError         **error)
{
	git_blame *gblame;
	int ret;

	g_return_val_if_fail (GGIT_IS_BLAME (blame), NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	ret = git_blame_buffer (&gblame,
	                        _ggit_native_get (blame),
	                        (const char *)buffer,
	                        buffer_length);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return NULL;
	}

	return _ggit_blame_wrap (gblame);
}

