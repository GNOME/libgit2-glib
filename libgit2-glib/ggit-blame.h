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

#ifndef __GGIT_BLAME_H__
#define __GGIT_BLAME_H__

#include <glib-object.h>
#include <libgit2-glib/ggit-native.h>
#include <libgit2-glib/ggit-types.h>
#include <git2.h>

G_BEGIN_DECLS

#define GGIT_TYPE_BLAME			(ggit_blame_get_type ())
#define GGIT_BLAME(obj)			(G_TYPE_CHECK_INSTANCE_CAST ((obj), GGIT_TYPE_BLAME, GgitBlame))
#define GGIT_BLAME_CONST(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), GGIT_TYPE_BLAME, GgitBlame const))
#define GGIT_BLAME_CLASS(klass)		(G_TYPE_CHECK_CLASS_CAST ((klass), GGIT_TYPE_BLAME, GgitBlameClass))
#define GGIT_IS_BLAME(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), GGIT_TYPE_BLAME))
#define GGIT_IS_BLAME_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), GGIT_TYPE_BLAME))
#define GGIT_BLAME_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS ((obj), GGIT_TYPE_BLAME, GgitBlameClass))

typedef struct _GgitBlameClass		GgitBlameClass;

struct _GgitBlame
{
	/*< private >*/
	GgitNative parent;

	/* priv padding */
	gpointer priv;
};

struct _GgitBlameClass
{
	/*< private >*/
	GgitNativeClass parent_class;
};

GType          ggit_blame_hunk_get_type (void) G_GNUC_CONST;
GgitBlameHunk *ggit_blame_hunk_ref      (GgitBlameHunk *blame_hunk);
void           ggit_blame_hunk_unref    (GgitBlameHunk *blame_hunk);

guint16        ggit_blame_hunk_get_lines_in_hunk (GgitBlameHunk *blame_hunk);

GgitOId       *ggit_blame_hunk_get_final_commit_id         (GgitBlameHunk *blame_hunk);
guint16        ggit_blame_hunk_get_final_start_line_number (GgitBlameHunk *blame_hunk);
GgitSignature *ggit_blame_hunk_get_final_signature         (GgitBlameHunk *blame_hunk);

GgitOId       *ggit_blame_hunk_get_orig_commit_id          (GgitBlameHunk *blame_hunk);
guint16        ggit_blame_hunk_get_orig_start_line_number  (GgitBlameHunk *blame_hunk);
GgitSignature *ggit_blame_hunk_get_orig_signature          (GgitBlameHunk *blame_hunk);
const gchar   *ggit_blame_hunk_get_orig_path               (GgitBlameHunk *blame_hunk);

gboolean       ggit_blame_hunk_is_boundary                 (GgitBlameHunk *blame_hunk);

GType ggit_blame_get_type (void) G_GNUC_CONST;

GgitBlame     *_ggit_blame_wrap             (git_blame *blame);

guint32        ggit_blame_get_hunk_count    (GgitBlame *blame);

GgitBlameHunk *ggit_blame_get_hunk_by_line  (GgitBlame *blame,
                                             guint32    line);

GgitBlameHunk *ggit_blame_get_hunk_by_index (GgitBlame *blame,
                                             guint32    idx);

G_END_DECLS

#endif /* __GGIT_BLAME_H__ */
