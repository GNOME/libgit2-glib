/*
 * walk.c
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2012 - Garrett Regier
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

#include <gio/gio.h>

#include "ggit.h"

static gchar *
signature_to_string (GgitSignature *signature)
{
	const gchar *name;
	const gchar *email;
	GDateTime *time;
	gchar *time_str;
	gchar *signature_str;

	name = ggit_signature_get_name (signature);
	email = ggit_signature_get_email (signature);

	time = ggit_signature_get_time (signature);
	time_str = g_date_time_format (time, "%c");

	signature_str = g_strdup_printf ("%s <%s> (%s)", name, email, time_str);

	g_free (time_str);
	g_date_time_unref (time);

	return signature_str;
}

static int
diff_print_cb (GgitDiffDelta    *delta,
               GgitDiffRange    *range,
               GgitDiffLineType  line_type,
               const gchar      *content,
               gsize             content_len,
               gpointer          user_data)
{
	g_print ("%s", content);
	return 0;
}

int
main (int   argc,
      char *argv[])
{
	GFile *file;
	GgitRepository *repo;
	GgitRevisionWalker *revwalker;
	GgitRef *head;
	GgitOId *oid;
	gint64 n_revisions = 10;
	GError *err = NULL;

	ggit_init ();

	if (argc < 2 || argc > 3)
	{
		g_print ("Usage: %s path_to_git_repository [N_REVISIONS]\n",
		         argv[0]);
		return 1;
	}

	if (argc == 3)
	{
		n_revisions = g_ascii_strtoll (argv[2], NULL, 10);

		if (n_revisions < -1)
		{
			g_printerr ("Error: invalid number of "
				    "revisions: %" G_GINT64_FORMAT "\n",
				    n_revisions);
			return 1;
		}
	}

	file = g_file_new_for_path (argv[1]);

	if (!g_file_query_exists (file, NULL))
	{
		g_object_unref (file);
		g_printerr ("Error: invalid git repository: %s\n", argv[1]);
		return 1;
	}


	repo = ggit_repository_open (file, &err);
	g_assert_no_error (err);

	revwalker = ggit_revision_walker_new (repo, &err);
	g_assert_no_error (err);

	ggit_revision_walker_set_sort_mode (revwalker,
	                                    GGIT_SORT_TIME |
	                                    GGIT_SORT_TOPOLOGICAL);

	head = ggit_repository_get_head (repo, &err);
	g_assert_no_error (err);

	oid = ggit_ref_get_target (head);
	ggit_revision_walker_push (revwalker, oid, &err);
	g_assert_no_error (err);

	ggit_oid_free (oid);
	g_object_unref (head);

	while ((oid = ggit_revision_walker_next (revwalker, &err)) != NULL)
	{
		GgitCommit *commit;
		gchar *oid_str;
		GgitSignature *author;
		GgitSignature *committer;
		gchar *author_str;
		gchar *committer_str;
		const gchar *subject;
		const gchar *message;
		GgitCommitParents *commit_parents;

		commit = GGIT_COMMIT (ggit_repository_lookup (repo, oid,
		                                              GGIT_TYPE_COMMIT,
		                                              &err));
		g_assert_no_error (err);

		oid_str = ggit_oid_to_string (oid);

		author = ggit_commit_get_author (commit);
		committer = ggit_commit_get_committer (commit);

		author_str = signature_to_string (author);
		committer_str = signature_to_string (committer);

		subject = ggit_commit_get_subject (commit);
		message = ggit_commit_get_message (commit);

		g_print ("SHA:       %s\n"
		         "Author:    %s\n"
		         "Committer: %s\n"
		         "Subject:   %s\n"
		         "Message:   %s\n",
		         oid_str, author_str, committer_str, subject, message);

		commit_parents = ggit_commit_get_parents (commit);

		if (ggit_commit_parents_size (commit_parents) > 0)
		{
			GgitCommit *parent_commit;
			GgitTree *commit_tree;
			GgitTree *parent_tree;
			GgitDiff *diff;

			parent_commit = ggit_commit_parents_get (commit_parents, 0);
			commit_tree = ggit_commit_get_tree (commit);
			parent_tree = ggit_commit_get_tree (parent_commit);

			diff = ggit_diff_new_tree_to_tree (repo,
			                                   parent_tree,
			                                   commit_tree,
			                                   NULL, &err);
			g_assert_no_error (err);

			ggit_diff_print_patch (diff, diff_print_cb, NULL, &err);
			g_assert_no_error (err);

			g_object_unref (diff);
			g_object_unref (parent_tree);
			g_object_unref (commit_tree);
			g_object_unref (parent_commit);
		}

		g_print ("----------------------------------------\n");

		ggit_commit_parents_unref (commit_parents);
		g_free (committer_str);
		g_free (author_str);
		g_object_unref (committer);
		g_object_unref (author);
		g_free (oid_str);
		g_object_unref (commit);
		ggit_oid_free (oid);

		if (n_revisions != -1 && --n_revisions == 0)
		{
			break;
		}
	}

	g_assert_no_error (err);

	g_object_unref (revwalker);
	g_object_unref (repo);
	g_object_unref (file);

	return 0;
}

/* ex:set ts=8 noet: */
