/*
 * repository.c
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2011 - Paolo Borelli
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include <glib.h>
#include <glib/gstdio.h>

#include "ggit-repository.h"

#define TESTREPO_NAME "testrepo.git"

typedef struct {
	gchar *git_dir;
} TestFixture;

/*
 * This function is copied from
 * http://bugzilla.gnome.org/show_bug.cgi?id=524831
 * and renamed from g_mkdtemp to _g_mkdtemp.
 *
 * If/when this function gets added to glib, it can be removed.
 *
 *
 * g_mkdtemp:
 * @tmpl: template directory name
 *
 * Creates a temporary directory. See the mkdtemp() documentation
 * on most UNIX-like systems.
 *
 * The parameter is a string that should follow the rules for
 * mkdtemp() templates, i.e. contain the string "XXXXXX".  g_mkdtemp()
 * is slightly more flexible than mkdtemp() in that the sequence does
 * not have to occur at the very end of the template. The X string
 * will be modified to form the name of a directory that didn't
 * already exist.  The string should be in the GLib file name
 * encoding. Most importantly, on Windows it should be in UTF-8.
 *
 * Return value: If a temporary directory was successfully created,
 * @tmpl will be returned with the XXXXXX string modified in such a
 * way as to make the path unique.  In case of errors, %NULL is
 * returned.
 */
static gchar *
_g_mkdtemp (gchar *tmpl)
{
	static const char letters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	static const int NLETTERS = sizeof (letters) - 1;
	static int counter = 0;
	char *XXXXXX;
	GTimeVal tv;
	glong value;
	int count;

	/* find the last occurrence of "XXXXXX" */
	XXXXXX = g_strrstr (tmpl, "XXXXXX");

	if (!XXXXXX || strncmp (XXXXXX, "XXXXXX", 6))
	{
		errno = EINVAL;
		return NULL;
	}

	/* Get some more or less random data.  */
	g_get_current_time (&tv);
	value = (tv.tv_usec ^ tv.tv_sec) + counter++;

	for (count = 0; count < 100; value += 7777, ++count)
	{
		glong v = value;

		/* Fill in the random bits.  */
		XXXXXX[0] = letters[v % NLETTERS];
		v /= NLETTERS;
		XXXXXX[1] = letters[v % NLETTERS];
		v /= NLETTERS;
		XXXXXX[2] = letters[v % NLETTERS];
		v /= NLETTERS;
		XXXXXX[3] = letters[v % NLETTERS];
		v /= NLETTERS;
		XXXXXX[4] = letters[v % NLETTERS];
		v /= NLETTERS;
		XXXXXX[5] = letters[v % NLETTERS];

		/* tmpl is in UTF-8 on Windows, thus use g_mkdir() */
		if (g_mkdir (tmpl, 0700) == 0)
			return tmpl;

		if (errno != EEXIST)
			 /* Any other error will apply also to other names we might
			 *  try, and there are 2^32 or so of them, so give up now.
			 */
			 return NULL;
	}

	/* We got out of the loop because we ran out of combinations to try.  */
	errno = EEXIST;
	return NULL;
}

static void
test_setup (TestFixture   *fixture,
            gconstpointer  data)
{
	fixture->git_dir = g_build_filename (g_get_tmp_dir (),
	                                     TESTREPO_NAME "XXXXXX",
	                                     NULL);

	fixture->git_dir = _g_mkdtemp (fixture->git_dir);
}

static void
test_teardown (TestFixture   *fixture,
               gconstpointer  data)
{
	gchar *cmd;
	gint ret;

	/* remove all files in the directory, it sucks that
	* there isn't a glib util for this, for now lets just cheat */
	cmd = g_strdup_printf ("rm -rf '%s'", fixture->git_dir);
	ret = system(cmd);
	g_assert (ret == 0);

	g_free (cmd);
	g_free (fixture->git_dir);
}

static void
test_runner (TestFixture   *fixture,
             gconstpointer  data)
{
	((void (*) (const gchar *git_dir)) data) (fixture->git_dir);
}

static void
do_test_init (const gchar *git_dir,
              gboolean     bare)
{
	const gchar *repo_path;
	gchar *dotgit, *dotgit_full;
	GError *err = NULL;
	GgitRepository *repo;

	repo = ggit_repository_init_repository (git_dir, bare, &err);
	g_assert_no_error (err);

	dotgit = bare ? g_strdup (git_dir) : g_build_filename (git_dir, ".git", NULL);

	/* libgit api includes the terminating separator */
	dotgit_full = g_strdup_printf ("%s%s", dotgit, G_DIR_SEPARATOR_S);

	repo_path = ggit_repository_get_path (repo, GGIT_REPO_PATH);
	g_assert_cmpstr (dotgit_full, ==, repo_path);
	g_assert (g_file_test (repo_path, G_FILE_TEST_IS_DIR));
	g_free (dotgit);
	g_free (dotgit_full);

	g_assert (ggit_repository_is_bare (repo) == bare);

	g_object_unref (repo);
}

static void
test_repository_init (const gchar *git_dir)
{
	do_test_init (git_dir, FALSE);
}

static void
test_repository_init_bare (const gchar *git_dir)
{
	do_test_init (git_dir, TRUE);
}

int
main (int    argc,
      char **argv)
{
	g_type_init ();
	g_test_init (&argc, &argv, NULL);

#define TEST(path, ftest) \
  g_test_add ("/repository/" path, TestFixture, \
              (gpointer) test_repository_##ftest, \
              test_setup, test_runner, test_teardown)

	TEST ("init", init);
	TEST ("init-bare", init_bare);

	return g_test_run();
}
