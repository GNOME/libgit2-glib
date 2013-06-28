/*
 * ggit-tree-builder.h
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

#ifndef __GGIT_TREE_BUILDER_H__
#define __GGIT_TREE_BUILDER_H__

#include <git2.h>
#include <libgit2-glib/ggit-native.h>
#include <libgit2-glib/ggit-types.h>
#include <libgit2-glib/ggit-oid.h>

G_BEGIN_DECLS

#define GGIT_TYPE_TREE_BUILDER			(ggit_tree_builder_get_type ())
#define GGIT_TREE_BUILDER(obj)			(G_TYPE_CHECK_INSTANCE_CAST ((obj), GGIT_TYPE_TREE_BUILDER, GgitTreeBuilder))
#define GGIT_TREE_BUILDER_CLASS(klass)		(G_TYPE_CHECK_CLASS_CAST ((klass), GGIT_TYPE_TREE_BUILDER, GgitTreeBuilderClass))
#define GGIT_IS_TREE_BUILDER(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), GGIT_TYPE_TREE_BUILDER))
#define GGIT_IS_TREE_BUILDER_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), GGIT_TYPE_TREE_BUILDER))
#define GGIT_TREE_BUILDER_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS ((obj), GGIT_TYPE_TREE_BUILDER, GgitTreeBuilderClass))

typedef struct _GgitTreeBuilderClass	GgitTreeBuilderClass;
typedef struct _GgitTreeBuilderPrivate  GgitTreeBuilderPrivate;

struct _GgitTreeBuilder
{
	/*< private >*/
	GgitNative parent;

	/* priv */
	GgitTreeBuilderPrivate *priv;
};

/**
 * GgitTreeBuilderClass:
 * @parent_class: The parent class.
 *
 * The class structure for #GgitTreeBuilderClass.
 */
struct _GgitTreeBuilderClass
{
	/*< private >*/
	GgitNativeClass parent_class;
};

GType            ggit_tree_builder_get_type  (void) G_GNUC_CONST;

GgitTreeBuilder *_ggit_tree_builder_wrap     (git_treebuilder *builder,
                                              GgitRepository  *repository,
                                              gboolean         owned);

void             ggit_tree_builder_remove    (GgitTreeBuilder  *builder,
                                              const gchar      *path,
                                              GError          **error);

void             ggit_tree_builder_clear     (GgitTreeBuilder *builder);

GgitOId         *ggit_tree_builder_write     (GgitTreeBuilder  *builder,
                                              GError          **error);

GgitTreeEntry   *ggit_tree_builder_insert    (GgitTreeBuilder  *builder,
                                              const gchar      *filename,
                                              GgitOId          *oid,
                                              GgitFileMode      file_mode,
                                              GError          **error);

GgitTreeEntry   *ggit_tree_builder_get_entry (GgitTreeBuilder *builder,
                                              const gchar     *path);

G_END_DECLS

#endif /* __GGIT_TREE_BUILDER_H__ */

/* ex:set ts=8 noet: */
