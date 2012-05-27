/*
 * general.c
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


#include <glib.h>
#include "ggit.h"

int
main (int argc, char *argv[])
{
	GgitRepository *repository;
	GgitOId *oid;
	GError *error = NULL;
	const gchar hex[] = "82576c09c3fac738a54582c6c04a47684882d1a1";
	gchar *oid_str;
	GFile *repo_location;
	GFile *f;
	gchar *path;

	ggit_init ();
	g_type_init ();

	if (argc < 2)
	{
		g_message ("Use: ./general path_to_git_repository");
		return 1;
	}

	f = g_file_new_for_path (argv[1]);
	repo_location = ggit_repository_discover (f, &error);
	g_object_unref (f);

	if (error != NULL)
	{
		g_message (error->message);
		return 1;
	}

	path = g_file_get_path (repo_location);
	g_message ("Path repository: %s", path);
	g_free (path);

	repository = ggit_repository_open (repo_location, &error);

	if (error != NULL)
	{
		g_message (error->message);
		return 1;
	}

	f = ggit_repository_get_workdir (repository);
	path = g_file_get_path (f);

	g_message ("Working dir: %s", path);

	g_free (path);
	g_object_unref (f);

	oid = ggit_oid_new_from_string (hex);
	oid_str = ggit_oid_to_string (oid);

	g_message ("OId str: %s", oid_str);

	g_free (oid_str);
	ggit_oid_free (oid);
	g_object_unref (repository);

	return 0;
}

/* ex:set ts=8 noet: */
