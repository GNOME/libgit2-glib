/*
 * ggit-cred-ssh-interactive.c
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
#include "ggit-cred-ssh-interactive.h"

struct _GgitCredSshInteractivePrompt
{
	gint ref_count;
	gchar *name;
	gchar *text;
	gchar *instruction;
	gchar *response;
	gboolean is_masked;
};

typedef struct _GgitCredSshInteractivePrivate
{
	gchar *username;
} GgitCredSshInteractivePrivate;

enum
{
	PROP_0,
	PROP_USERNAME
};

G_DEFINE_BOXED_TYPE (GgitCredSshInteractivePrompt,
                     ggit_cred_ssh_interactive_prompt,
                     ggit_cred_ssh_interactive_prompt_ref,
                     ggit_cred_ssh_interactive_prompt_unref);

static void ggit_cred_ssh_interactive_initable_iface_init (GInitableIface  *iface);

/**
 * GgitCredSshInteractiveClass::prompt:
 * @cred: a #GgitCredSshInteractive
 * @prompts: (array length=num_prompts): a list of prompts.
 * @num_prompts: number of prompts.
  */

G_DEFINE_TYPE_EXTENDED (GgitCredSshInteractive,
                        ggit_cred_ssh_interactive,
                        GGIT_TYPE_CRED,
                        0,
                        G_ADD_PRIVATE (GgitCredSshInteractive)
                        G_IMPLEMENT_INTERFACE (G_TYPE_INITABLE,
                                               ggit_cred_ssh_interactive_initable_iface_init))

static void
ggit_cred_ssh_interactive_finalize (GObject *object)
{
	GgitCredSshInteractive *cred = GGIT_CRED_SSH_INTERACTIVE (object);
	GgitCredSshInteractivePrivate *priv;

	priv = ggit_cred_ssh_interactive_get_instance_private (cred);

	g_free (priv->username);

	G_OBJECT_CLASS (ggit_cred_ssh_interactive_parent_class)->finalize (object);
}

static void
ggit_cred_ssh_interactive_get_property (GObject    *object,
                                        guint       prop_id,
                                        GValue     *value,
                                        GParamSpec *pspec)
{
	GgitCredSshInteractive *cred = GGIT_CRED_SSH_INTERACTIVE (object);
	GgitCredSshInteractivePrivate *priv;

	priv = ggit_cred_ssh_interactive_get_instance_private (cred);

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
ggit_cred_ssh_interactive_set_property (GObject      *object,
                                        guint         prop_id,
                                        const GValue *value,
                                        GParamSpec   *pspec)
{
	GgitCredSshInteractive *cred = GGIT_CRED_SSH_INTERACTIVE (object);
	GgitCredSshInteractivePrivate *priv;

	priv = ggit_cred_ssh_interactive_get_instance_private (cred);

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
ggit_cred_ssh_interactive_class_init (GgitCredSshInteractiveClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->finalize = ggit_cred_ssh_interactive_finalize;
	object_class->get_property = ggit_cred_ssh_interactive_get_property;
	object_class->set_property = ggit_cred_ssh_interactive_set_property;

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
ggit_cred_ssh_interactive_init (GgitCredSshInteractive *cred)
{
}

static void
callback_wrapper (const char                            *name,
                  int                                    name_len,
                  const char                            *instruction,
                  int                                    instruction_len,
                  int                                    num_prompts,
                  const LIBSSH2_USERAUTH_KBDINT_PROMPT  *prompts,
                  LIBSSH2_USERAUTH_KBDINT_RESPONSE      *responses,
                  void                                 **abstract)
{
	GgitCredSshInteractive *interactive;
	GgitCredSshInteractivePrompt **wprompts;
	gint i;
	gchar *wname;
	gchar *winstruction;

	interactive = *abstract;

	if (GGIT_CRED_SSH_INTERACTIVE_GET_CLASS (interactive)->prompt == NULL)
	{
		return;
	}

	wprompts = g_new (GgitCredSshInteractivePrompt *, num_prompts);

	wname = g_strndup (name, name_len);
	winstruction = g_strndup (instruction, instruction_len);

	for (i = 0; i < num_prompts; i++)
	{
		gchar *text;

		text = g_strndup (prompts[i].text, prompts[i].length);

		wprompts[i] = ggit_cred_ssh_interactive_prompt_new (wname,
		                                                    winstruction,
		                                                    text,
		                                                    prompts[i].echo == 0);

		g_free (text);
	}

	g_free (wname);
	g_free (winstruction);

	GGIT_CRED_SSH_INTERACTIVE_GET_CLASS (interactive)->prompt (interactive, wprompts, num_prompts);

	for (i = 0; i < num_prompts; i++)
	{
		responses[i].text = g_strdup (wprompts[i]->response);
		responses[i].length = strlen (wprompts[i]->response);

		ggit_cred_ssh_interactive_prompt_unref (wprompts[i]);
	}
}

static gboolean
ggit_cred_ssh_interactive_initable_init (GInitable     *initable,
                                         GCancellable  *cancellable,
                                         GError       **error)
{
	GgitCredSshInteractive *gcred = GGIT_CRED_SSH_INTERACTIVE (initable);
	GgitCredSshInteractivePrivate *priv;
	git_cred *cred;
	gint ret;

	if (cancellable != NULL)
	{
		g_set_error_literal (error, G_IO_ERROR, G_IO_ERROR_NOT_SUPPORTED,
		                     "Cancellable initialization not supported");
		return FALSE;
	}

	priv = ggit_cred_ssh_interactive_get_instance_private (gcred);

	ret = git_cred_ssh_interactive_new (&cred, priv->username,
	                                    callback_wrapper,
	                                    initable);

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
ggit_cred_ssh_interactive_initable_iface_init (GInitableIface *iface)
{
	iface->init = ggit_cred_ssh_interactive_initable_init;
}

GgitCredSshInteractive *
ggit_cred_ssh_interactive_new (const gchar  *username,
                               GError      **error)
{
	return g_initable_new (GGIT_TYPE_CRED_SSH_INTERACTIVE,
	                       NULL, error,
	                       "username", username,
	                       NULL);
}

const gchar *
ggit_cred_ssh_interactive_get_username (GgitCredSshInteractive *cred)
{
	GgitCredSshInteractivePrivate *priv;

	g_return_val_if_fail (GGIT_IS_CRED_SSH_INTERACTIVE (cred), NULL);

	priv = ggit_cred_ssh_interactive_get_instance_private (cred);

	return priv->username;
}

GgitCredSshInteractivePrompt *
ggit_cred_ssh_interactive_prompt_new (const gchar *name,
                                      const gchar *text,
                                      const gchar *instruction,
                                      gboolean     is_masked)
{
	GgitCredSshInteractivePrompt *ret;

	ret = g_slice_new0 (GgitCredSshInteractivePrompt);
	ret->ref_count = 1;

	ret->name = g_strdup (name);
	ret->instruction = g_strdup (instruction);
	ret->text = g_strdup (text);

	ret->is_masked = is_masked;

	return ret;
}

GgitCredSshInteractivePrompt *
ggit_cred_ssh_interactive_prompt_ref (GgitCredSshInteractivePrompt *prompt)
{
	g_atomic_int_inc (&prompt->ref_count);
	return prompt;
}

void
ggit_cred_ssh_interactive_prompt_unref (GgitCredSshInteractivePrompt *prompt)
{
	if (g_atomic_int_dec_and_test (&prompt->ref_count))
	{
		g_free (prompt->name);
		g_free (prompt->instruction);
		g_free (prompt->text);
		g_free (prompt->response);

		g_slice_free (GgitCredSshInteractivePrompt, prompt);
	}
}

const gchar *
ggit_cred_ssh_interactive_prompt_get_name (GgitCredSshInteractivePrompt *prompt)
{
	g_return_val_if_fail (prompt != NULL, NULL);

	return prompt->name;
}

const gchar *
ggit_cred_ssh_interactive_prompt_get_instruction (GgitCredSshInteractivePrompt *prompt)
{
	g_return_val_if_fail (prompt != NULL, NULL);

	return prompt->instruction;
}

const gchar *
ggit_cred_ssh_interactive_prompt_get_text (GgitCredSshInteractivePrompt *prompt)
{
	g_return_val_if_fail (prompt != NULL, NULL);

	return prompt->text;
}

gboolean
ggit_cred_ssh_interactive_prompt_is_masked (GgitCredSshInteractivePrompt *prompt)
{
	g_return_val_if_fail (prompt != NULL, FALSE);

	return prompt->is_masked;
}

void
ggit_cred_ssh_interactive_prompt_set_response (GgitCredSshInteractivePrompt *prompt,
                                               const gchar                  *response)
{
	g_return_if_fail (prompt != NULL);

	g_free (prompt->response);
	prompt->response = g_strdup (response);
}

const gchar *
ggit_cred_ssh_interactive_prompt_get_response (GgitCredSshInteractivePrompt *prompt)
{
	g_return_val_if_fail (prompt != NULL, NULL);

	return prompt->response;
}

/* ex:set ts=8 noet: */
