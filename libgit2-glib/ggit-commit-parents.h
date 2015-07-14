/*
 * ggit-commit.h
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2014 - Jesse van den Kieboom
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


#ifndef __GGIT_COMMIT_PARENTS_H__
#define __GGIT_COMMIT_PARENTS_H__

#include <glib-object.h>

#include <libgit2-glib/ggit-types.h>
#include <libgit2-glib/ggit-commit.h>

G_BEGIN_DECLS

#define GGIT_TYPE_COMMIT_PARENTS    (ggit_commit_parents_get_type ())
#define GGIT_COMMIT_PARENTS(obj)    (G_TYPE_CHECK_INSTANCE_CAST ((obj), GGIT_TYPE_COMMIT_PARENTS, GgitCommitParents))
#define GGIT_COMMIT_PARENTS_CLASS(klass)  (G_TYPE_CHECK_CLASS_CAST ((klass), GGIT_TYPE_COMMIT_PARENTS, GgitCommitParentsClass))
#define GGIT_IS_COMMIT_PARENTS(obj)   (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GGIT_TYPE_COMMIT_PARENTS))
#define GGIT_IS_COMMIT_PARENTS_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GGIT_TYPE_COMMIT_PARENTS))
#define GGIT_COMMIT_PARENTS_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GGIT_TYPE_COMMIT_PARENTS, GgitCommitParentsClass))

typedef struct _GgitCommitParentsClass   GgitCommitParentsClass;
typedef struct _GgitCommitParentsPrivate GgitCommitParentsPrivate;

struct _GgitCommitParents
{
  /*< private >*/
  GObject parent;

  /* priv padding */
  GgitCommitParentsPrivate *priv;
};

/**
 * GgitCommitParentsClass:
 * @parent_class: The parent class.
 *
 * The class structure for #GgitCommitParentClass.
 */
struct _GgitCommitParentsClass
{
  /*< private >*/
  GObjectClass parent_class;
};

GType                ggit_commit_parents_get_type     (void) G_GNUC_CONST;
GgitCommitParents   *ggit_commit_parents_new          (GgitCommit        *commit);

guint                ggit_commit_parents_get_size     (GgitCommitParents *parents);

GgitCommit          *ggit_commit_parents_get          (GgitCommitParents *parents,
                                                       guint              idx);

GgitOId             *ggit_commit_parents_get_id       (GgitCommitParents *parents,
                                                       guint              idx);

G_END_DECLS

#endif /* __GGIT_COMMIT_PARENTS_H__ */

/* ex:set ts=8 noet: */
