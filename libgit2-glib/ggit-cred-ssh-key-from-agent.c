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

#include <git2.h>
#include <gio/gio.h>

#include "ggit-error.h"
#include "ggit-cred-ssh-key-from-agent.h"


struct _GgitCredSshKeyFromAgentPrivate
{
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
	GgitCredSshKeyFromAgentPrivate *priv = GGIT_CRED_SSH_KEY_FROM_AGENT (object)->priv;

	g_free (priv->username);

	G_OBJECT_CLASS (ggit_cred_ssh_key_from_agent_parent_class)->finalize (object);
}

static void
ggit_cred_ssh_key_from_agent_get_property (GObject    *object,
                                           guint       prop_id,
                                           GValue     *value,
                                           GParamSpec *pspec)
{
	GgitCredSshKeyFromAgentPrivate *priv = GGIT_CRED_SSH_KEY_FROM_AGENT (object)->priv;

	switch (prop_id)
	{
		case PROP_USERNAME:
			g_value_set_string (value, priv->username);
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
	GgitCredSshKeyFromAgentPrivate *priv = GGIT_CRED_SSH_KEY_FROM_AGENT (object)->priv;

	switch (prop_id)
	{
		case PROP_USERNAME:
			priv->username = g_value_dup_string (value);
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

	g_type_class_add_private (object_class, sizeof (GgitCredSshKeyFromAgentPrivate));
}

static void
ggit_cred_ssh_key_from_agent_init (GgitCredSshKeyFromAgent *cred)
{
	cred->priv = G_TYPE_INSTANCE_GET_PRIVATE(cred,
	                                         GGIT_TYPE_CRED_SSH_KEY_FROM_AGENT,
	                                         GgitCredSshKeyFromAgentPrivate);
}

static gboolean
ggit_cred_ssh_key_from_agent_initable_init (GInitable     *initable,
                                            GCancellable  *cancellable,
                                            GError       **error)
{
	GgitCredSshKeyFromAgentPrivate *priv;
	git_cred *cred;
	gint ret;

	if (cancellable != NULL)
	{
		g_set_error_literal (error, G_IO_ERROR, G_IO_ERROR_NOT_SUPPORTED,
		                     "Cancellable initialization not supported");
		return FALSE;
	}

	priv = GGIT_CRED_SSH_KEY_FROM_AGENT (initable)->priv;

	ret = git_cred_ssh_key_from_agent (&cred, priv->username);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return FALSE;
	}

	_ggit_native_set (initable,
	                  cred,
	                  (GDestroyNotify)cred->free);

	return TRUE;
}

static void
ggit_cred_ssh_key_from_agent_initable_iface_init (GInitableIface *iface)
{
	iface->init = ggit_cred_ssh_key_from_agent_initable_init;
}

GgitCredSshKeyFromAgent *
ggit_cred_ssh_key_from_agent_new (const gchar  *username,
                                  GError      **error)
{
	return g_initable_new (GGIT_TYPE_CRED_SSH_KEY_FROM_AGENT,
	                       NULL, error,
	                       "username", username,
	                       NULL);
}

const gchar *
ggit_cred_ssh_key_from_agent_get_username (GgitCredSshKeyFromAgent *cred)
{
	g_return_val_if_fail (GGIT_IS_CRED_SSH_KEY_FROM_AGENT (cred), NULL);

	return cred->priv->username;
}

/* ex:set ts=8 noet: */
