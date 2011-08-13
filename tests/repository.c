#include "config.h"
#include <stdlib.h>
#include <glib.h>

#include "ggit-repository.h"

#define TESTREPO_NAME "testrepo.git"

static const char *testrepo_dir = NULL;

static const gchar *
get_testrepo_dir (void)
{
	if (testrepo_dir == NULL)
	{
		testrepo_dir = g_build_filename (g_get_tmp_dir (), TESTREPO_NAME, NULL);
		g_assert (!g_file_test (testrepo_dir, G_FILE_TEST_EXISTS));
	}

	return testrepo_dir;
}

static void
delete_testrepo_dir (void)
{
	if (testrepo_dir != NULL)
	{
		gchar *cmd;
		gint ret;

		/* remove all files in the directory, it sucks that
		 * there isn't a glib util for this, for now lets just cheat */
		cmd = g_strdup_printf ("rm -rf %s", testrepo_dir);
		ret = system(cmd);
		g_assert (ret == 0);

		g_free (cmd);

		testrepo_dir = NULL;
	}
}

static void
do_test_init (gboolean bare)
{
	const gchar *dir, *repo_path;
	gchar *dotgit, *dotgit_full;
	GError *err = NULL;
	GgitRepository *repo;

	dir = get_testrepo_dir();

	repo = ggit_repository_init_repository (dir, bare, &err);
	g_assert_no_error (err);

	dotgit = bare ? g_strdup (dir) : g_build_filename (dir, ".git", NULL);

	/* libgit api includes the terminating separator */
	dotgit_full = g_strdup_printf ("%s%s", dotgit, G_DIR_SEPARATOR_S);

	repo_path = ggit_repository_get_path (repo, GGIT_REPO_PATH);
	g_assert_cmpstr (dotgit_full, ==, repo_path);
	g_assert (g_file_test (repo_path, G_FILE_TEST_IS_DIR));
	g_free (dotgit);
	g_free (dotgit_full);

	g_assert (ggit_repository_is_bare (repo) == bare);

	g_object_unref (repo);
	delete_testrepo_dir ();
}

static void
test_init (void)
{
	do_test_init (FALSE);
}

static void
test_init_bare (void)
{
	do_test_init (TRUE);
}

int
main (int argc, char** argv)
{
	g_type_init ();
	g_test_init (&argc, &argv, NULL);

	g_test_add_func ("/repository/init", test_init);
	g_test_add_func ("/repository/init-bare", test_init_bare);

	return g_test_run();
}
