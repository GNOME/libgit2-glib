/*
 * ggit-diff.h
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

#ifndef __GGIT_DIFF_H__
#define __GGIT_DIFF_H__

#include <git2.h>
#include "ggit-native.h"
#include "ggit-types.h"

G_BEGIN_DECLS

#define GGIT_TYPE_DIFF			(ggit_diff_get_type ())
#define GGIT_DIFF(obj)			(G_TYPE_CHECK_INSTANCE_CAST ((obj), GGIT_TYPE_DIFF, GgitDiff))
#define GGIT_DIFF_CLASS(klass)		(G_TYPE_CHECK_CLASS_CAST ((klass), GGIT_TYPE_DIFF, GgitDiffClass))
#define GGIT_IS_DIFF(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), GGIT_TYPE_DIFF))
#define GGIT_IS_DIFF_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), GGIT_TYPE_DIFF))
#define GGIT_DIFF_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS ((obj), GGIT_TYPE_DIFF, GgitDiffClass))

typedef struct _GgitDiffClass	GgitDiffClass;

struct _GgitDiff
{
	/*< private >*/
	GgitNative parent;

	/* priv padding */
	gpointer priv;
};

/**
 * GgitDiffClass:
 * @parent_class: The parent class.
 *
 * The class structure for #GgitDiffClass.
 */
struct _GgitDiffClass
{
	/*< private >*/
	GgitNativeClass parent_class;
};

GType          ggit_diff_get_type                  (void) G_GNUC_CONST;

GgitDiff      *_ggit_diff_wrap                     (git_diff              *diff,
                                                    gboolean               owned);

GgitDiff      *ggit_diff_new_tree_to_tree          (GgitRepository        *repository,
                                                    GgitTree              *old_tree,
                                                    GgitTree              *new_tree,
                                                    GgitDiffOptions       *diff_options,
                                                    GError               **error);
GgitDiff      *ggit_diff_new_tree_to_index         (GgitRepository        *repository,
                                                    GgitTree              *old_tree,
                                                    GgitIndex             *index,
                                                    GgitDiffOptions       *diff_options,
                                                    GError               **error);
GgitDiff      *ggit_diff_new_index_to_workdir      (GgitRepository        *repository,
                                                    GgitIndex             *index,
                                                    GgitDiffOptions       *diff_options,
                                                    GError               **error);
GgitDiff      *ggit_diff_new_tree_to_workdir       (GgitRepository        *repository,
                                                    GgitTree              *old_tree,
                                                    GgitDiffOptions       *diff_options,
                                                    GError               **error);

void           ggit_diff_merge                     (GgitDiff              *onto,
                                                    GgitDiff              *from,
                                                    GError               **error);

void           ggit_diff_foreach                   (GgitDiff              *diff,
                                                    GgitDiffFileCallback   file_cb,
                                                    GgitDiffHunkCallback   hunk_cb,
                                                    GgitDiffLineCallback   line_cb,
                                                    gpointer              *user_data,
                                                    GError               **error);
void           ggit_diff_print                     (GgitDiff              *diff,
                                                    GgitDiffFormatType     type,
                                                    GgitDiffLineCallback   print_cb,
                                                    gpointer              *user_data,
                                                    GError               **error);

gint           ggit_diff_get_num_deltas            (GgitDiff              *diff);

void           ggit_diff_blobs                     (GgitBlob              *old_blob,
                                                    const gchar           *old_as_path,
                                                    GgitBlob              *new_blob,
                                                    const gchar           *new_as_path,
                                                    GgitDiffOptions       *diff_options,
                                                    GgitDiffFileCallback   file_cb,
                                                    GgitDiffHunkCallback   hunk_cb,
                                                    GgitDiffLineCallback   line_cb,
                                                    gpointer              *user_data,
                                                    GError               **error);

void           ggit_diff_blob_to_buffer            (GgitBlob              *old_blob,
                                                    const gchar           *old_as_path,
                                                    const gchar           *buffer,
                                                    gsize                  buffer_len,
                                                    const gchar           *buffer_as_path,
                                                    GgitDiffOptions       *diff_options,
                                                    GgitDiffFileCallback   file_cb,
                                                    GgitDiffHunkCallback   hunk_cb,
                                                    GgitDiffLineCallback   line_cb,
                                                    gpointer              *user_data,
                                                    GError               **error);

G_END_DECLS

#endif /* __GGIT_DIFF_H__ */

/* ex:set ts=8 noet: */
