#include <glib.h>
#include <stdio.h>
#include <termios.h>
#include <string.h>
#include <glib/gprintf.h>

#include "libgit2-glib/ggit.h"

typedef struct
{
	GgitRemoteCallbacksClass parent_class;
} ClonerClass;

typedef struct
{
	GgitRemoteCallbacks parent;
} Cloner;

static void cloner_class_init (ClonerClass *klass);

G_DEFINE_TYPE (Cloner, cloner, GGIT_TYPE_REMOTE_CALLBACKS)

static void
cloner_init (Cloner *cloner)
{
}

static void
cloner_transfer_progress (GgitRemoteCallbacks   *callbacks,
                          GgitTransferProgress  *stats)
{
	guint recvobjs;
	guint totobjs;
	guint indexobjs;

	recvobjs = ggit_transfer_progress_get_received_objects (stats);
	totobjs = ggit_transfer_progress_get_total_objects (stats);
	indexobjs = ggit_transfer_progress_get_indexed_objects (stats);

	guint network_percent = recvobjs * 100 / totobjs;
	guint index_percent = indexobjs * 100 / totobjs;
	guint kbytes = ggit_transfer_progress_get_received_bytes (stats) / 1024;

	g_printf ("\rnet %3d%% (%4d kb, %5d/%5d)  /  idx %3d%% (%5d/%5d)",
	          network_percent, kbytes,
	          recvobjs, totobjs,
	          index_percent,
	          indexobjs, totobjs);

	if (recvobjs == totobjs && indexobjs == totobjs)
	{
		g_printf("\n");
	}
}

static void
chomp (gchar *s)
{
	int l;

	l = strlen (s);

	if (s[l - 1] == '\n')
	{
		s[l - 1] = '\0';
	}
}

static gboolean
cloner_credentials (GgitRemoteCallbacks  *callbacks,
                    const gchar          *url,
                    const gchar          *username_from_url,
                    guint                 allowed_types,
                    GgitCred            **cred,
                    GError              **error)
{
	gchar username[128];
	gchar password[128];
	struct termios old, new;

	g_printf ("Username: ");

	if (fgets (username, sizeof (username) - 1, stdin) == NULL)
	{
		return FALSE;
	}

	g_printf ("Password: ");


	if (tcgetattr (STDIN_FILENO, &old) != 0)
	{
		return FALSE;
	}

	new = old;
	new.c_lflag &= ~ECHO;

	if (tcsetattr (STDIN_FILENO, TCSAFLUSH, &new) != 0)
	{
		return FALSE;
	}

	if (fgets (password, sizeof (password) - 1, stdin) == NULL)
	{
		tcsetattr (STDIN_FILENO, TCSAFLUSH, &old);

		return FALSE;
	}

	tcsetattr (STDIN_FILENO, TCSAFLUSH, &old);

	chomp (username);
	chomp (password);

	*cred = GGIT_CRED (ggit_cred_plaintext_new (username, password, error));

	if (*error != NULL)
	{
		return FALSE;
	}

	return TRUE;
}

static void
cloner_class_init (ClonerClass *klass)
{
	GgitRemoteCallbacksClass *cb_klass = GGIT_REMOTE_CALLBACKS_CLASS (klass);

	cb_klass->transfer_progress = cloner_transfer_progress;
	cb_klass->credentials = cloner_credentials;
}

int main(int argc, char **argv)
{
	GgitRepository *cloned_repo = NULL;
	GgitCloneOptions *options = NULL;
	const char *url;
	const char *path;
	GError *error = NULL;
	GFile *location;
	Cloner *cloner;

	// Validate args
	if (argc < 3) {
		g_printf ("USAGE: %s <url> <path>\n", argv[0]);
		return -1;
	}

	ggit_init ();

	url = argv[1];
	path = argv[2];

	location = g_file_new_for_commandline_arg (path);

	options = ggit_clone_options_new ();
	cloner = g_object_new (cloner_get_type(), NULL);

	ggit_clone_options_set_remote_callbacks (options,
	                                         GGIT_REMOTE_CALLBACKS (cloner));

	g_object_unref (cloner);

	// Do the clone
	cloned_repo = ggit_repository_clone (url, location, options, &error);

	if (error != NULL)
	{
		g_warning ("%s", error->message);
		g_error_free (error);
	}
	else
	{
		g_object_unref (cloned_repo);
	}

	ggit_clone_options_free (options);
	return 0;
}
