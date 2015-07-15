/*
 * ggit-tree.h
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

#ifndef __GGIT_TREE_H__
#define __GGIT_TREE_H__

#include <git2.h>
#include <libgit2-glib/ggit-object.h>
#include <libgit2-glib/ggit-types.h>
#include <libgit2-glib/ggit-tree-entry.h>
#include <gio/gio.h>

G_BEGIN_DECLS

#define GGIT_TYPE_TREE (ggit_tree_get_type ())
G_DECLARE_DERIVABLE_TYPE (GgitTree, ggit_tree, GGIT, TREE, GgitObject)

/**
 * GgitTreeClass:
 * @parent_class: The parent class.
 *
 * The class structure for #GgitTreeClass.
 */
struct _GgitTreeClass
{
	/*< private >*/
	GgitObjectClass parent_class;
};

GgitTree      *_ggit_tree_wrap          (git_tree *tree,
                                         gboolean  owned);

GgitOId       *ggit_tree_get_id         (GgitTree *tree);

GgitTreeEntry *ggit_tree_get            (GgitTree *tree,
                                         guint     i);

guint          ggit_tree_size           (GgitTree *tree);

GgitTreeEntry *ggit_tree_get_by_name    (GgitTree    *tree,
                                         const gchar *name);

GgitTreeEntry *ggit_tree_get_by_path    (GgitTree     *tree,
                                         const gchar  *path,
                                         GError      **error);

void           ggit_tree_walk           (GgitTree              *tree,
                                         GgitTreeWalkMode       mode,
                                         GgitTreeWalkCallback   callback,
                                         gpointer               user_data,
                                         GError               **error);

G_END_DECLS

#endif /* __GGIT_TREE_H__ */

/* ex:set ts=8 noet: */
