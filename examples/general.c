/*
 * general.c
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


#include <glib.h>
#include "ggit-repository.h"
#include "ggit-oid.h"

int
main (int argc, char *argv[])
{
	GgitRepository *repository;
	GgitOId *oid;
	GError *error = NULL;
	const gchar hex[] = "82576c09c3fac738a54582c6c04a47684882d1a1";
	gchar *oid_str;
	gchar *repo_path;

	g_type_init ();

	if (argc < 2)
	{
		g_message ("Use: ./general path_to_git_repository");
		return 1;
	}

	repo_path = ggit_repository_discover (argv[1], &error);

	if (error != NULL)
	{
		g_message (error->message);
		return 1;
	}

	g_message ("Path repository: %s", repo_path);
	repository = ggit_repository_open (repo_path, &error);
	g_free (repo_path);

	if (error != NULL)
	{
		g_message (error->message);
		return 1;
	}

	g_message ("Working dir: %s", ggit_repository_get_path (repository,
	                                                        GGIT_REPO_PATH_WORKDIR));

	oid = ggit_oid_new_from_string (hex);
	oid_str = ggit_oid_to_string (oid);

	g_message ("OId str: %s", oid_str);

	g_free (oid_str);
	ggit_oid_free (oid);
	g_object_unref (repository);

	return 0;
}
