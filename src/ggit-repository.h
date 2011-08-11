/*
 * ggit-repository.h
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2011 - Ignacio Casal Quinteiro
 *
 * libggit is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * libggit is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libggit; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA  02110-1301  USA
 */


#ifndef __GGIT_REPOSITORY_H__
#define __GGIT_REPOSITORY_H__

#include <glib-object.h>
#include <git2/repository.h>
#include "ggit-object.h"

G_BEGIN_DECLS

#define GGIT_TYPE_REPOSITORY			(ggit_repository_get_type ())
#define GGIT_REPOSITORY(obj)			(G_TYPE_CHECK_INSTANCE_CAST ((obj), GGIT_TYPE_REPOSITORY, GgitRepository))
#define GGIT_REPOSITORY_CONST(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), GGIT_TYPE_REPOSITORY, GgitRepository const))
#define GGIT_REPOSITORY_CLASS(klass)		(G_TYPE_CHECK_CLASS_CAST ((klass), GGIT_TYPE_REPOSITORY, GgitRepositoryClass))
#define GGIT_IS_REPOSITORY(obj)			(G_TYPE_CHECK_INSTANCE_TYPE ((obj), GGIT_TYPE_REPOSITORY))
#define GGIT_IS_REPOSITORY_CLASS(klass)		(G_TYPE_CHECK_CLASS_TYPE ((klass), GGIT_TYPE_REPOSITORY))
#define GGIT_REPOSITORY_GET_CLASS(obj)		(G_TYPE_INSTANCE_GET_CLASS ((obj), GGIT_TYPE_REPOSITORY, GgitRepositoryClass))

typedef struct _GgitRepository		GgitRepository;
typedef struct _GgitRepositoryClass	GgitRepositoryClass;
typedef struct _GgitRepositoryPrivate	GgitRepositoryPrivate;

struct _GgitRepository
{
	GObject parent;

	GgitRepositoryPrivate *priv;
};

struct _GgitRepositoryClass
{
	GObjectClass parent_class;
};

typedef enum
{
	GGIT_REPO_PATH,
	GGIT_REPO_PATH_INDEX,
	GGIT_REPO_PATH_ODB,
	GGIT_REPO_PATH_WORKDIR
} GgitRepositoryPathid;

GType                     ggit_repository_get_type          (void) G_GNUC_CONST;

GgitRepository          *_ggit_repository_new               (git_repository *repository);

GgitRepository           *ggit_repository_open              (const gchar *path,
                                                             GError     **error);

GgitRepository           *ggit_repository_init_repository   (const gchar *path,
                                                             gboolean     is_bare,
                                                             GError     **error);

struct _GgitObject       *ggit_repository_lookup            (GgitRepository *repository,
                                                             GgitOId        *oid,
                                                             GType           gtype,
                                                             GError        **error);

gchar                    *ggit_repository_discover           (const gchar    *path,
                                                              GError        **error);

gboolean                  ggit_repository_head_detached     (GgitRepository *repository,
                                                             GError        **error);

gboolean                  ggit_repository_head_orphan       (GgitRepository *repository,
                                                             GError        **error);

gboolean                  ggit_repository_is_empty          (GgitRepository *repository,
                                                             GError        **error);

const gchar              *ggit_repository_path              (GgitRepository      *repository,
                                                             GgitRepositoryPathid id);

gboolean                  ggit_repository_is_bare           (GgitRepository  *repository);

G_END_DECLS

#endif /* __GGIT_REPOSITORY_H__ */
