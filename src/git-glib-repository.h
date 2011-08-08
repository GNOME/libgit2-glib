/*
 * git-glib-repository.h
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2011 - Ignacio Casal Quinteiro
 *
 * libgit-glib is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * libgit-glib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libgit-glib; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA  02110-1301  USA
 */


#ifndef __GIT_GLIB_REPOSITORY_H__
#define __GIT_GLIB_REPOSITORY_H__

#include <glib-object.h>
#include <git2/repository.h>

G_BEGIN_DECLS

#define GIT_TYPE_GLIB_REPOSITORY		(git_glib_repository_get_type ())
#define GIT_GLIB_REPOSITORY(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), GIT_TYPE_GLIB_REPOSITORY, GitGlibRepository))
#define GIT_GLIB_REPOSITORY_CONST(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), GIT_TYPE_GLIB_REPOSITORY, GitGlibRepository const))
#define GIT_GLIB_REPOSITORY_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST ((klass), GIT_TYPE_GLIB_REPOSITORY, GitGlibRepositoryClass))
#define GIT_IS_GLIB_REPOSITORY(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), GIT_TYPE_GLIB_REPOSITORY))
#define GIT_IS_GLIB_REPOSITORY_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), GIT_TYPE_GLIB_REPOSITORY))
#define GIT_GLIB_REPOSITORY_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS ((obj), GIT_TYPE_GLIB_REPOSITORY, GitGlibRepositoryClass))

typedef struct _GitGlibRepository		GitGlibRepository;
typedef struct _GitGlibRepositoryClass		GitGlibRepositoryClass;
typedef struct _GitGlibRepositoryPrivate	GitGlibRepositoryPrivate;

struct _GitGlibRepository
{
	GObject parent;

	GitGlibRepositoryPrivate *priv;
};

struct _GitGlibRepositoryClass
{
	GObjectClass parent_class;
};

typedef enum
{
	GIT_GLIB_REPO_PATH,
	GIT_GLIB_REPO_PATH_INDEX,
	GIT_GLIB_REPO_PATH_ODB,
	GIT_GLIB_REPO_PATH_WORKDIR
} GitGlibRepositoryPathid;

GType                     git_glib_repository_get_type          (void) G_GNUC_CONST;

GitGlibRepository        *git_glib_repository_open              (const gchar *path,
                                                                 GError     **error);

GitGlibRepository        *git_glib_repository_init_repository   (const gchar *path,
                                                                 gboolean     is_bare,
                                                                 GError     **error);

gboolean                  git_glib_repository_head_detached     (GitGlibRepository *repository,
                                                                 GError           **error);

gboolean                  git_glib_repository_head_orphan       (GitGlibRepository *repository,
                                                                 GError           **error);

gboolean                  git_glib_repository_is_empty          (GitGlibRepository *repository,
                                                                 GError           **error);

const gchar              *git_glib_repository_path              (GitGlibRepository      *repository,
                                                                 GitGlibRepositoryPathid id);

gboolean                  git_glib_repository_is_bare           (GitGlibRepository  *repository);

G_END_DECLS

#endif /* __GIT_GLIB_REPOSITORY_H__ */
