/*
 * ggit-index.h
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2012 - Jesse van den Kieboom
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

#ifndef __GGIT_INDEX_H__
#define __GGIT_INDEX_H__

#include <glib-object.h>
#include <gio/gio.h>
#include <git2.h>
#include <libgit2-glib/ggit-types.h>
#include <libgit2-glib/ggit-index-entry.h>
#include <libgit2-glib/ggit-index-entry-resolve-undo.h>
#include "ggit-native.h"

G_BEGIN_DECLS

#define GGIT_TYPE_INDEX			(ggit_index_get_type ())
#define GGIT_INDEX(obj)			(G_TYPE_CHECK_INSTANCE_CAST ((obj), GGIT_TYPE_INDEX, GgitIndex))
#define GGIT_INDEX_CLASS(klass)		(G_TYPE_CHECK_CLASS_CAST ((klass), GGIT_TYPE_INDEX, GgitIndexClass))
#define GGIT_IS_INDEX(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), GGIT_TYPE_INDEX))
#define GGIT_IS_INDEX_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), GGIT_TYPE_INDEX))
#define GGIT_INDEX_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS ((obj), GGIT_TYPE_INDEX, GgitIndexClass))

typedef struct _GgitIndexClass		GgitIndexClass;
typedef struct _GgitIndexPrivate	GgitIndexPrivate;

struct _GgitIndex
{
	/*< private >*/
	GgitNative parent;

	GgitIndexPrivate *priv;
};

/**
 * GgitIndexClass:
 * @parent_class: The parent class.
 *
 * The class structure for #GgitIndexClass.
 */
struct _GgitIndexClass
{
	/*< private >*/
	GgitNativeClass parent_class;
};

GType                     ggit_index_get_type                 (void) G_GNUC_CONST;

git_index                *_ggit_index_get_index               (GgitIndex  *idx);
GgitIndex                *_ggit_index_wrap                    (git_index  *idx);

GgitIndex                *ggit_index_open                     (GFile      *file,
                                                               GError    **error);

gboolean                  ggit_index_read                     (GgitIndex  *idx,
                                                               gboolean    force,
                                                               GError    **error);

gboolean                  ggit_index_write                    (GgitIndex  *idx,
                                                               GError    **error);

GgitOId                  *ggit_index_write_tree               (GgitIndex  *idx,
                                                               GError    **error);

gboolean                  ggit_index_remove                   (GgitIndex  *idx,
                                                               GFile      *file,
                                                               gint        stage,
                                                               GError    **error);

gboolean                  ggit_index_add                      (GgitIndex       *idx,
                                                               GgitIndexEntry  *entry,
                                                               GError         **error);

gboolean                  ggit_index_add_file                 (GgitIndex       *idx,
                                                               GFile           *file,
                                                               GError         **error);

gboolean                  ggit_index_add_path                 (GgitIndex       *idx,
                                                               const gchar     *path,
                                                               GError         **error);

GgitRepository           *ggit_index_get_owner                (GgitIndex  *idx);

GgitIndexEntries         *ggit_index_get_entries              (GgitIndex  *idx);
GgitIndexEntriesResolveUndo *
                          ggit_index_get_entries_resolve_undo (GgitIndex  *idx);

G_END_DECLS

#endif /* __GGIT_INDEX_H__ */

/* ex:set ts=8 noet: */
