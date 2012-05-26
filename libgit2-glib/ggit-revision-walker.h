/*
 * ggit-revision-walker.h
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2011 - Ignacio Casal Quinteiro
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


#ifndef __GGIT_REVISION_WALKER_H__
#define __GGIT_REVISION_WALKER_H__

#include <glib-object.h>
#include "ggit-types.h"
#include "ggit-native.h"

G_BEGIN_DECLS

#define GGIT_TYPE_REVISION_WALKER		(ggit_revision_walker_get_type ())
#define GGIT_REVISION_WALKER(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), GGIT_TYPE_REVISION_WALKER, GgitRevisionWalker))
#define GGIT_REVISION_WALKER_CONST(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), GGIT_TYPE_REVISION_WALKER, GgitRevisionWalker const))
#define GGIT_REVISION_WALKER_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST ((klass), GGIT_TYPE_REVISION_WALKER, GgitRevisionWalkerClass))
#define GGIT_IS_REVISION_WALKER(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), GGIT_TYPE_REVISION_WALKER))
#define GGIT_IS_REVISION_WALKER_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), GGIT_TYPE_REVISION_WALKER))
#define GGIT_REVISION_WALKER_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS ((obj), GGIT_TYPE_REVISION_WALKER, GgitRevisionWalkerClass))

typedef struct _GgitRevisionWalkerClass		GgitRevisionWalkerClass;
typedef struct _GgitRevisionWalkerPrivate	GgitRevisionWalkerPrivate;

struct _GgitRevisionWalker
{
	GgitNative parent;

	GgitRevisionWalkerPrivate *priv;
};

struct _GgitRevisionWalkerClass
{
	GgitNativeClass parent_class;
};

GType                   ggit_revision_walker_get_type       (void) G_GNUC_CONST;

GgitRevisionWalker     *ggit_revision_walker_new            (GgitRepository  *repository,
                                                             GError         **error);

void                    ggit_revision_walker_reset          (GgitRevisionWalker *walker);

void                    ggit_revision_walker_push           (GgitRevisionWalker  *walker,
                                                             GgitOId             *oid,
                                                             GError             **error);

void                    ggit_revision_walker_hide           (GgitRevisionWalker  *walker,
                                                             GgitOId             *oid,
                                                             GError             **error);

GgitOId                *ggit_revision_walker_next           (GgitRevisionWalker  *walker,
                                                             GError             **error);

void                    ggit_revision_walker_set_sort_mode  (GgitRevisionWalker *walker,
                                                             GgitSortMode        sort_mode);

GgitRepository         *ggit_revision_walker_get_repository (GgitRevisionWalker *walker);

G_END_DECLS

#endif /* __GGIT_REVISION_WALKER_H__ */

/* ex:set ts=8 noet: */
