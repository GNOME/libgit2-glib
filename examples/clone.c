#include <glib.h>
#include <stdio.h>
#include "ggit.h"

static int
fetch_progress (const GgitTransferProgress *stats,
                gpointer                    useless)
{
	gint network_percent = (100 * stats->received_objects) / stats->total_objects;
	gint index_percent = (100 * stats->indexed_objects) / stats->total_objects;
	gint kbytes = stats->received_bytes / 1024;

	g_message ("net %3d%% (%4d kb, %5d/%5d)  /  idx %3d%% (%5d/%5d)",
	           network_percent, kbytes,
	           stats->received_objects, stats->total_objects,
	           index_percent, stats->indexed_objects, stats->total_objects);

	return 0;
}

static gint
cred_acquire (const gchar *url,
              const gchar *username_from_url,
              guint        allowed_types,
              GgitCred   **cred,
              gpointer     user_data)
{
	gchar username[128] = "";
	gchar password[128] = "";
	GError *error = NULL;

	g_message ("Username: ");
	scanf ("%s", username);

	/* Yup. Right there on your terminal. Careful where you copy/paste output. */
	g_message ("Password: ");
	scanf ("%s", password);

	*cred = GGIT_CRED (ggit_cred_plaintext_new (username, password, &error));

	if (error != NULL)
	{
		return -1;
	}

	return 0;
}

int main(int argc, char **argv)
{
	GgitRepository *cloned_repo = NULL;
	GgitCloneOptions *options = NULL;
	const char *url;
	const char *path;
	GError *error = NULL;
	GFile *location;

	// Validate args
	if (argc < 3) {
		g_warning ("USAGE: %s <url> <path>\n", argv[0]);
		return -1;
	}

	ggit_init ();

	url = argv[1];
	path = argv[2];

	location = g_file_new_for_commandline_arg (path);

	options = ggit_clone_options_new ();
	ggit_clone_options_set_fetch_progress_callback (options,
	                                                fetch_progress,
	                                                NULL);
	ggit_clone_options_set_cred_acquire_callback (options,
	                                              cred_acquire,
	                                              NULL);

	// Do the clone
	cloned_repo = ggit_repository_clone(url, location, options, &error);
	if (error != NULL) {
		g_warning ("%s", error->message);
		g_error_free (error);
	}
	else
	{
		g_object_unref (cloned_repo);
	}

	return 0;
}
