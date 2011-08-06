/*
 * git2-glib-repository.h
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2011 - Ignacio Casal Quinteiro
 *
 * libgit2-glib is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * libgit2-glib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libgit2-glib; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA  02110-1301  USA
 */


#ifndef __GIT2_GLIB_REPOSITORY_H__
#define __GIT2_GLIB_REPOSITORY_H__

#include <glib-object.h>

G_BEGIN_DECLS

#define GIT2_TYPE_GLIB_REPOSITORY		(git2_glib_repository_get_type ())
#define GIT2_GLIB_REPOSITORY(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), GIT2_TYPE_GLIB_REPOSITORY, Git2GlibRepository))
#define GIT2_GLIB_REPOSITORY_CONST(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), GIT2_TYPE_GLIB_REPOSITORY, Git2GlibRepository const))
#define GIT2_GLIB_REPOSITORY_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST ((klass), GIT2_TYPE_GLIB_REPOSITORY, Git2GlibRepositoryClass))
#define GIT2_IS_GLIB_REPOSITORY(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), GIT2_TYPE_GLIB_REPOSITORY))
#define GIT2_IS_GLIB_REPOSITORY_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), GIT2_TYPE_GLIB_REPOSITORY))
#define GIT2_GLIB_REPOSITORY_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS ((obj), GIT2_TYPE_GLIB_REPOSITORY, Git2GlibRepositoryClass))

typedef struct _Git2GlibRepository		Git2GlibRepository;
typedef struct _Git2GlibRepositoryClass		Git2GlibRepositoryClass;
typedef struct _Git2GlibRepositoryPrivate	Git2GlibRepositoryPrivate;

struct _Git2GlibRepository
{
	GObject parent;

	Git2GlibRepositoryPrivate *priv;
};

struct _Git2GlibRepositoryClass
{
	GObjectClass parent_class;
};

GType                     git2_glib_repository_get_type      (void) G_GNUC_CONST;

Git2GlibRepository       *git2_glib_repository_open          (const gchar *path,
                                                              GError     **error);

G_END_DECLS

#endif /* __GIT2_GLIB_REPOSITORY_H__ */
