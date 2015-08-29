/*
 * ggit-diff-line.c
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

#include <glib-object.h>
#include <git2.h>

#include "ggit-diff-line.h"
#include "ggit-convert.h"

struct _GgitDiffLine {
	gint ref_count;

	GgitDiffLineType origin;
	gint old_lineno;
	gint new_lineno;
	gint num_lines;
	gint64 content_offset;
	GBytes *content;

	gchar *text;
	gchar *encoding;
};

G_DEFINE_BOXED_TYPE (GgitDiffLine, ggit_diff_line,
                     ggit_diff_line_ref, ggit_diff_line_unref)


GgitDiffLine *
_ggit_diff_line_wrap (const git_diff_line *line,
                      const gchar         *encoding)
{
	GgitDiffLine *gline;

	g_return_val_if_fail (line != NULL, NULL);

	gline = g_slice_new (GgitDiffLine);
	gline->ref_count = 1;
	gline->origin = (GgitDiffLineType)line->origin;
	gline->old_lineno = line->old_lineno;
	gline->new_lineno = line->new_lineno;
	gline->content_offset = line->content_offset;
	gline->content = g_bytes_new (line->content, line->content_len);
	gline->encoding = g_strdup (encoding);
	gline->text = NULL;

	return gline;
}

/**
 * ggit_diff_line_ref:
 * @line: a #GgitDiffLine.
 *
 * Atomically increments the reference count of @line by one.
 * This function is MT-safe and may be called from any thread.
 *
 * Returns: a #GgitDiffLine.
 **/
GgitDiffLine *
ggit_diff_line_ref (GgitDiffLine *line)
{
	g_return_val_if_fail (line != NULL, NULL);

	g_atomic_int_inc (&line->ref_count);

	return line;
}

/**
 * ggit_diff_line_unref:
 * @line: a #GgitDiffLine.
 *
 * Atomically decrements the reference count of @line by one.
 * If the reference count drops to 0, @line is freed.
 **/
void
ggit_diff_line_unref (GgitDiffLine *line)
{
	g_return_if_fail (line != NULL);

	if (g_atomic_int_dec_and_test (&line->ref_count))
	{
		g_bytes_unref (line->content);
		g_free (line->text);
		g_slice_free (GgitDiffLine, line);
	}
}

/**
 * ggit_diff_line_get_origin:
 * @line: a #GgitDiffLine.
 *
 * Gets the #GgitDiffLineType value.
 *
 * Returns: the #GgitDiffLineType value.
 */
GgitDiffLineType
ggit_diff_line_get_origin (GgitDiffLine *line)
{
	g_return_val_if_fail (line != NULL, 0);

	return line->origin;
}

/**
 * ggit_diff_line_get_old_lineno:
 * @line: a #GgitDiffLine.
 *
 * Gets the line number in old file or -1 for added line.
 *
 * Returns: the line's old line number.
 */
gint
ggit_diff_line_get_old_lineno (GgitDiffLine *line)
{
	g_return_val_if_fail (line != NULL, 0);

	return line->old_lineno;
}

/**
 * ggit_diff_line_get_new_lineno:
 * @line: a #GgitDiffLine.
 *
 * Gets the Line number in new file or -1 for deleted line.
 *
 * Returns: the line's old number of lines.
 */
gint
ggit_diff_line_get_new_lineno (GgitDiffLine *line)
{
	g_return_val_if_fail (line != NULL, 0);

	return line->new_lineno;
}

/**
 * ggit_diff_line_get_content_offset:
 * @line: a #GgitDiffLine.
 *
 * Gets the content offset.
 *
 * Returns: the content offset.
 */
gint64
ggit_diff_line_get_content_offset (GgitDiffLine *line)
{
	g_return_val_if_fail (line != NULL, 0);

	return line->content_offset;
}

/**
 * ggit_diff_line_get_content:
 * @line: a #GgitDiffLine.
 * @length: (out): the number of returned bytes.
 *
 * Gets the content in bytes.
 *
 * Returns: (array length=length): the content in bytes.
 */
const guint8 *
ggit_diff_line_get_content (GgitDiffLine *line,
                            gsize        *length)
{
	g_return_val_if_fail (line != NULL, 0);

	if (length)
	{
		*length = g_bytes_get_size (line->content);
	}

	return g_bytes_get_data (line->content, NULL);
}

/**
 * ggit_diff_line_get_text:
 * @line: a #GgitDiffLine.
 *
 * Get the content of the diff line as UTF-8 encoded text.
 *
 * Returns: the content in utf-8 encoding.
 *
 **/
const gchar *
ggit_diff_line_get_text (GgitDiffLine *line)
{
	g_return_val_if_fail (line != NULL, NULL);

	if (line->text == NULL)
	{
		const gchar *content;
		gsize content_len;

		content = g_bytes_get_data (line->content, &content_len);
		line->text = ggit_convert_utf8 (content,
		                                content_len,
		                                line->encoding);
	}

	return line->text;
}

/* ex:set ts=8 noet: */
