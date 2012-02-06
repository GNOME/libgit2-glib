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
#include <gio/gio.h>
#include <libgit2-glib/ggit-config.h>
#include <libgit2-glib/ggit-index.h>
#include <libgit2-glib/ggit-types.h>
#include <libgit2-glib/ggit-native.h>
#include <libgit2-glib/ggit-object.h>
#include <libgit2-glib/ggit-tree.h>

G_BEGIN_DECLS

#define GGIT_TYPE_REPOSITORY			(ggit_repository_get_type ())
#define GGIT_REPOSITORY(obj)			(G_TYPE_CHECK_INSTANCE_CAST ((obj), GGIT_TYPE_REPOSITORY, GgitRepository))
#define GGIT_REPOSITORY_CONST(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), GGIT_TYPE_REPOSITORY, GgitRepository const))
#define GGIT_REPOSITORY_CLASS(klass)		(G_TYPE_CHECK_CLASS_CAST ((klass), GGIT_TYPE_REPOSITORY, GgitRepositoryClass))
#define GGIT_IS_REPOSITORY(obj)			(G_TYPE_CHECK_INSTANCE_TYPE ((obj), GGIT_TYPE_REPOSITORY))
#define GGIT_IS_REPOSITORY_CLASS(klass)		(G_TYPE_CHECK_CLASS_TYPE ((klass), GGIT_TYPE_REPOSITORY))
#define GGIT_REPOSITORY_GET_CLASS(obj)		(G_TYPE_INSTANCE_GET_CLASS ((obj), GGIT_TYPE_REPOSITORY, GgitRepositoryClass))

typedef struct _GgitRepositoryClass	GgitRepositoryClass;
typedef struct _GgitRepositoryPrivate	GgitRepositoryPrivate;

struct _GgitRepository
{
	GgitNative parent;

	GgitRepositoryPrivate *priv;
};

struct _GgitRepositoryClass
{
	GgitNativeClass parent_class;
};

GType               ggit_repository_get_type          (void) G_GNUC_CONST;

GgitRepository     *_ggit_repository_wrap             (git_repository        *repository,
                                                       gboolean               owned);

git_repository     *_ggit_repository_get_repository   (GgitRepository        *repository);

GgitRepository     *ggit_repository_open              (GFile                 *location,
                                                       GError               **error);

GgitRepository     *ggit_repository_init_repository   (GFile                 *location,
                                                       gboolean               is_bare,
                                                       GError               **error);

GgitObject         *ggit_repository_lookup            (GgitRepository        *repository,
                                                       GgitOId               *oid,
                                                       GType                  gtype,
                                                       GError               **error);

GgitRef            *ggit_repository_lookup_reference  (GgitRepository        *repository,
                                                       const gchar           *name,
                                                       GError               **error);

GgitRef            *ggit_repository_create_reference  (GgitRepository        *repository,
                                                       const gchar           *name,
                                                       GgitOId               *oid,
                                                       GError               **error);

GgitRef            *ggit_repository_create_symbolic_reference
                                                      (GgitRepository        *repository,
                                                       const gchar           *name,
                                                       const gchar           *target,
                                                       GError               **error);

GgitRef            *ggit_repository_get_head          (GgitRepository        *repository,
                                                       GError               **error);

GFile              *ggit_repository_discover          (GFile                 *location,
                                                       GError               **error);

gboolean            ggit_repository_is_head_detached  (GgitRepository        *repository,
                                                       GError               **error);

gboolean            ggit_repository_is_head_orphan    (GgitRepository        *repository,
                                                       GError               **error);

gboolean            ggit_repository_is_empty          (GgitRepository        *repository,
                                                       GError               **error);

GFile              *ggit_repository_get_location      (GgitRepository        *repository);
GFile              *ggit_repository_get_workdir       (GgitRepository        *repository);

void                ggit_repository_set_workdir       (GgitRepository        *repository,
                                                       GFile                 *workdir);

gboolean            ggit_repository_is_bare           (GgitRepository        *repository);

GgitStatusFlags     ggit_repository_file_status       (GgitRepository        *repository,
                                                       GFile                 *location,
                                                       GError               **error);

gboolean            ggit_repository_file_status_foreach
                                                     (GgitRepository         *repository,
                                                      GgitStatusCallback      callback,
                                                      gpointer                user_data,
                                                      GError                **error);

gboolean            ggit_repository_references_foreach (GgitRepository          *repository,
                                                        GgitRefType              reftype,
                                                        GgitReferencesCallback   callback,
                                                        gpointer                 user_data,
                                                        GError                 **error);

GgitConfig         *ggit_repository_get_config         (GgitRepository          *repository,
                                                        GError                 **error);

GgitIndex          *ggit_repository_get_index          (GgitRepository          *repository,
                                                        GError                 **error);

G_END_DECLS

#endif /* __GGIT_REPOSITORY_H__ */

/* ex:set ts=8 noet: */
