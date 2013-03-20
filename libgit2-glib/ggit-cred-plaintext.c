/*
 * ggit-cred-plaintext.c
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2013 - Ignacio Casal Quinteiro
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

#include <git2/transport.h>
#include <git2/errors.h>
#include <gio/gio.h>

#include "ggit-error.h"
#include "ggit-cred-plaintext.h"


struct _GgitCredPlaintextPrivate
{
	gchar *username;
	gchar *password;
};

enum
{
	PROP_0,
	PROP_USERNAME,
	PROP_PASSWORD
};

static void ggit_cred_plaintext_initable_iface_init (GInitableIface  *iface);

G_DEFINE_TYPE_EXTENDED (GgitCredPlaintext, ggit_cred_plaintext, GGIT_TYPE_NATIVE,
                        0,
                        G_IMPLEMENT_INTERFACE (G_TYPE_INITABLE,
                                               ggit_cred_plaintext_initable_iface_init))

static void
ggit_cred_plaintext_finalize (GObject *object)
{
	GgitCredPlaintextPrivate *priv = GGIT_CRED_PLAINTEXT (object)->priv;

	g_free (priv->username);
	g_free (priv->password);

	G_OBJECT_CLASS (ggit_cred_plaintext_parent_class)->finalize (object);
}

static void
ggit_cred_plaintext_get_property (GObject    *object,
                                  guint       prop_id,
                                  GValue     *value,
                                  GParamSpec *pspec)
{
	GgitCredPlaintextPrivate *priv = GGIT_CRED_PLAINTEXT (object)->priv;

	switch (prop_id)
	{
		case PROP_USERNAME:
			g_value_set_string (value, priv->username);
			break;
		case PROP_PASSWORD:
			g_value_set_string (value, priv->password);
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void
ggit_cred_plaintext_set_property (GObject      *object,
                                  guint         prop_id,
                                  const GValue *value,
                                  GParamSpec   *pspec)
{
	GgitCredPlaintextPrivate *priv = GGIT_CRED_PLAINTEXT (object)->priv;

	switch (prop_id)
	{
		case PROP_USERNAME:
			priv->username = g_value_dup_string (value);
			break;
		case PROP_PASSWORD:
			priv->password = g_value_dup_string (value);
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void
ggit_cred_plaintext_class_init (GgitCredPlaintextClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->finalize = ggit_cred_plaintext_finalize;
	object_class->get_property = ggit_cred_plaintext_get_property;
	object_class->set_property = ggit_cred_plaintext_set_property;

	g_object_class_install_property (object_class,
	                                 PROP_USERNAME,
	                                 g_param_spec_string ("username",
	                                                      "user name",
	                                                      "The user name",
	                                                      NULL,
	                                                      G_PARAM_READWRITE |
	                                                      G_PARAM_CONSTRUCT_ONLY |
	                                                      G_PARAM_STATIC_STRINGS));

	g_object_class_install_property (object_class,
	                                 PROP_PASSWORD,
	                                 g_param_spec_string ("password",
	                                                      "password",
	                                                      "The password",
	                                                      NULL,
	                                                      G_PARAM_READWRITE |
	                                                      G_PARAM_CONSTRUCT_ONLY |
	                                                      G_PARAM_STATIC_STRINGS));

	g_type_class_add_private (object_class, sizeof (GgitCredPlaintextPrivate));
}

static void
ggit_cred_plaintext_init (GgitCredPlaintext *cred)
{
	cred->priv = G_TYPE_INSTANCE_GET_PRIVATE(cred,
	                                         GGIT_TYPE_CRED_PLAINTEXT,
	                                         GgitCredPlaintextPrivate);
}

static gboolean
ggit_cred_plaintext_initable_init (GInitable    *initable,
                                   GCancellable *cancellable,
                                   GError      **error)
{
	GgitCredPlaintextPrivate *priv;
	git_cred *cred;
	gint ret;

	if (cancellable != NULL)
	{
		g_set_error_literal (error, G_IO_ERROR, G_IO_ERROR_NOT_SUPPORTED,
		                     "Cancellable initialization not supported");
		return FALSE;
	}

	priv = GGIT_CRED_PLAINTEXT (initable)->priv;

	ret = git_cred_userpass_plaintext_new (&cred, priv->username, priv->password);

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
ggit_cred_plaintext_initable_iface_init (GInitableIface *iface)
{
	iface->init = ggit_cred_plaintext_initable_init;
}

GgitCredPlaintext *
ggit_cred_plaintext_new (const gchar  *username,
                         const gchar  *password,
                         GError      **error)
{
	return g_initable_new (GGIT_TYPE_CRED_PLAINTEXT,
	                       NULL, error,
	                       "username", username,
	                       "password", password,
	                       NULL);
}

const gchar *
ggit_cred_plaintext_get_username (GgitCredPlaintext *cred)
{
	g_return_val_if_fail (GGIT_IS_CRED_PLAINTEXT (cred), NULL);

	return cred->priv->username;
}

const gchar *
ggit_cred_plaintext_get_password (GgitCredPlaintext *cred)
{
	g_return_val_if_fail (GGIT_IS_CRED_PLAINTEXT (cred), NULL);

	return cred->priv->password;
}

/* ex:set ts=8 noet: */
