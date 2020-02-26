/*
 * ggit-cred-ssh-key-from-agent.c
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2014 - Jesse van den Kieboom
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

#ifdef GIT_SSH
#include <libssh2.h>
#endif

#include <git2.h>
#include <gio/gio.h>

#include "ggit-error.h"
#include "ggit-cred-ssh-key-from-agent.h"

/**
 * GgitCredSshKeyFromAgent:
 *
 * Represents a ssh key agent credential.
 */

struct _GgitCredSshKeyFromAgent
{
	GgitCred parent_instance;

	gchar *username;
};

enum
{
	PROP_0,
	PROP_USERNAME
};

static void ggit_cred_ssh_key_from_agent_initable_iface_init (GInitableIface  *iface);

G_DEFINE_TYPE_EXTENDED (GgitCredSshKeyFromAgent,
                        ggit_cred_ssh_key_from_agent,
                        GGIT_TYPE_CRED,
                        0,
                        G_IMPLEMENT_INTERFACE (G_TYPE_INITABLE,
                                               ggit_cred_ssh_key_from_agent_initable_iface_init))

static void
ggit_cred_ssh_key_from_agent_finalize (GObject *object)
{
	GgitCredSshKeyFromAgent *cred = GGIT_CRED_SSH_KEY_FROM_AGENT (object);

	g_free (cred->username);

	G_OBJECT_CLASS (ggit_cred_ssh_key_from_agent_parent_class)->finalize (object);
}

static void
ggit_cred_ssh_key_from_agent_get_property (GObject    *object,
                                           guint       prop_id,
                                           GValue     *value,
                                           GParamSpec *pspec)
{
	GgitCredSshKeyFromAgent *cred = GGIT_CRED_SSH_KEY_FROM_AGENT (object);

	switch (prop_id)
	{
		case PROP_USERNAME:
			g_value_set_string (value, cred->username);
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void
ggit_cred_ssh_key_from_agent_set_property (GObject      *object,
                                           guint         prop_id,
                                           const GValue *value,
                                           GParamSpec   *pspec)
{
	GgitCredSshKeyFromAgent *cred = GGIT_CRED_SSH_KEY_FROM_AGENT (object);

	switch (prop_id)
	{
		case PROP_USERNAME:
			cred->username = g_value_dup_string (value);
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void
ggit_cred_ssh_key_from_agent_class_init (GgitCredSshKeyFromAgentClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->finalize = ggit_cred_ssh_key_from_agent_finalize;
	object_class->get_property = ggit_cred_ssh_key_from_agent_get_property;
	object_class->set_property = ggit_cred_ssh_key_from_agent_set_property;

	g_object_class_install_property (object_class,
	                                 PROP_USERNAME,
	                                 g_param_spec_string ("username",
	                                                      "user name",
	                                                      "The user name",
	                                                      NULL,
	                                                      G_PARAM_READWRITE |
	                                                      G_PARAM_CONSTRUCT_ONLY |
	                                                      G_PARAM_STATIC_STRINGS));
}

static void
ggit_cred_ssh_key_from_agent_init (GgitCredSshKeyFromAgent *cred)
{
}

static gboolean
ggit_cred_ssh_key_from_agent_initable_init (GInitable     *initable,
                                            GCancellable  *cancellable,
                                            GError       **error)
{
	GgitCredSshKeyFromAgent *gcred = GGIT_CRED_SSH_KEY_FROM_AGENT (initable);
	git_cred *cred;
	gint ret;

	if (cancellable != NULL)
	{
		g_set_error_literal (error, G_IO_ERROR, G_IO_ERROR_NOT_SUPPORTED,
		                     "Cancellable initialization not supported");
		return FALSE;
	}

	ret = git_cred_ssh_key_from_agent (&cred, gcred->username);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return FALSE;
	}

	_ggit_native_set (initable,
	                  cred,
#if LIBGIT2_VER_MAJOR > 0 || (LIBGIT2_VER_MAJOR == 0 && LIBGIT2_VER_MINOR >= 99)
	                  (GDestroyNotify)git_credential_free);
#else
	                  (GDestroyNotify)git_cred_free);
#endif

	return TRUE;
}

static void
ggit_cred_ssh_key_from_agent_initable_iface_init (GInitableIface *iface)
{
	iface->init = ggit_cred_ssh_key_from_agent_initable_init;
}

/**
 * ggit_cred_ssh_key_from_agent_new:
 * @username: the username.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Creates a new #GgitCredSshKeyFromAgent.
 *
 * Returns: (transfer full) (nullable): a new #GgitCredSshKeyFromAgent or %NULL if there was an error.
 */
GgitCredSshKeyFromAgent *
ggit_cred_ssh_key_from_agent_new (const gchar  *username,
                                  GError      **error)
{
	return g_initable_new (GGIT_TYPE_CRED_SSH_KEY_FROM_AGENT,
	                       NULL, error,
	                       "username", username,
	                       NULL);
}

/**
 * ggit_cred_ssh_key_from_agent_get_username:
 * @cred: a #GgitCredSshKeyFromAgent.
 *
 * Gets the username for the credential.
 *
 * Returns: (transfer none) (nullable): the username or %NULL.
 */
const gchar *
ggit_cred_ssh_key_from_agent_get_username (GgitCredSshKeyFromAgent *cred)
{
	g_return_val_if_fail (GGIT_IS_CRED_SSH_KEY_FROM_AGENT (cred), NULL);

	return cred->username;
}

/* ex:set ts=8 noet: */
