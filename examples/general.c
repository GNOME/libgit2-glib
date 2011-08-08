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
#include "git-glib-repository.h"
#include "git-glib-oid.h"

int
main (int argc, char *argv[])
{
	GitGlibRepository *repository;
	GitGlibOId *oid;
	GError *error = NULL;
	const gchar hex[] = "82576c09c3fac738a54582c6c04a47684882d1a1";
	gchar *oid_str;

	g_type_init ();

	if (argc < 2)
	{
		g_message ("Use: ./general path_to_git_repository");
		return 1;
	}

	repository = git_glib_repository_open (argv[1], &error);

	if (error != NULL)
	{
		g_message (error->message);
		return 1;
	}

	oid = git_glib_oid_fromstr (hex);
	oid_str = git_glib_oid_to_string (oid);

	g_message ("OId str: %s", oid_str);

	git_glib_oid_free (oid);
	g_object_unref (repository);

	return 0;
}
