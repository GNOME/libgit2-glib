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

#define GGIT_TYPE_REVISION_WALKER (ggit_revision_walker_get_type ())
G_DECLARE_DERIVABLE_TYPE (GgitRevisionWalker, ggit_revision_walker, GGIT, REVISION_WALKER, GgitNative)

/**
 * GgitRevisionWalkerClass:
 * @parent_class: The parent class.
 *
 * The class structure for #GgitRevisionWalkerClass.
 */
struct _GgitRevisionWalkerClass
{
	/*< private >*/
	GgitNativeClass parent_class;
};

GgitRevisionWalker     *ggit_revision_walker_new            (GgitRepository  *repository,
                                                             GError         **error);

void                    ggit_revision_walker_reset          (GgitRevisionWalker *walker);

void                    ggit_revision_walker_push           (GgitRevisionWalker  *walker,
                                                             GgitOId             *oid,
                                                             GError             **error);

void                    ggit_revision_walker_push_glob      (GgitRevisionWalker  *walker,
                                                             const gchar         *item,
                                                             GError             **error);

void                    ggit_revision_walker_push_ref       (GgitRevisionWalker  *walker,
                                                             const gchar         *item,
                                                             GError             **error);

void                    ggit_revision_walker_push_head      (GgitRevisionWalker  *walker,
                                                             GError             **error);

void                    ggit_revision_walker_push_range     (GgitRevisionWalker  *walker,
                                                             const gchar         *range,
                                                             GError             **error);

void                    ggit_revision_walker_hide           (GgitRevisionWalker  *walker,
                                                             GgitOId             *oid,
                                                             GError             **error);

void                    ggit_revision_walker_hide_glob      (GgitRevisionWalker  *walker,
                                                             const gchar         *item,
                                                             GError             **error);

void                    ggit_revision_walker_hide_ref      (GgitRevisionWalker  *walker,
                                                             const gchar         *item,
                                                             GError             **error);

void                    ggit_revision_walker_hide_head      (GgitRevisionWalker  *walker,
                                                             GError             **error);

GgitOId                *ggit_revision_walker_next           (GgitRevisionWalker  *walker,
                                                             GError             **error);

void                    ggit_revision_walker_set_sort_mode  (GgitRevisionWalker *walker,
                                                             GgitSortMode        sort_mode);

GgitRepository         *ggit_revision_walker_get_repository (GgitRevisionWalker *walker);

G_END_DECLS

#endif /* __GGIT_REVISION_WALKER_H__ */

/* ex:set ts=8 noet: */
