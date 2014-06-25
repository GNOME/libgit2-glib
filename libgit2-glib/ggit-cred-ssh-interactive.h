/*
 * ggit-cred-ssh-interactive.h
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


#ifndef __GGIT_CRED_SSH_INTERACTIVE_H__
#define __GGIT_CRED_SSH_INTERACTIVE_H__

#include <glib-object.h>
#include <libgit2-glib/ggit-cred.h>

G_BEGIN_DECLS

#define GGIT_TYPE_CRED_SSH_INTERACTIVE		(ggit_cred_ssh_interactive_get_type ())
#define GGIT_CRED_SSH_INTERACTIVE(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), GGIT_TYPE_CRED_SSH_INTERACTIVE, GgitCredSshInteractive))
#define GGIT_CRED_SSH_INTERACTIVE_CONST(obj)	(G_TYPE_CHECK_INSTANCE_CAST ((obj), GGIT_TYPE_CRED_SSH_INTERACTIVE, GgitCredSshInteractive const))
#define GGIT_CRED_SSH_INTERACTIVE_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST ((klass), GGIT_TYPE_CRED_SSH_INTERACTIVE, GgitCredSshInteractiveClass))
#define GGIT_IS_CRED_SSH_INTERACTIVE(obj)	(G_TYPE_CHECK_INSTANCE_TYPE ((obj), GGIT_TYPE_CRED_SSH_INTERACTIVE))
#define GGIT_IS_CRED_SSH_INTERACTIVE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GGIT_TYPE_CRED_SSH_INTERACTIVE))
#define GGIT_CRED_SSH_INTERACTIVE_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), GGIT_TYPE_CRED_SSH_INTERACTIVE, GgitCredSshInteractiveClass))

typedef struct _GgitCredSshInteractiveClass	GgitCredSshInteractiveClass;
typedef struct _GgitCredSshInteractivePrivate	GgitCredSshInteractivePrivate;

struct _GgitCredSshInteractive
{
	/*< private >*/
	GgitCred parent;

	GgitCredSshInteractivePrivate *priv;
};

struct _GgitCredSshInteractiveClass
{
	/*< private >*/
	GgitCredClass parent_class;

	void (*prompt) (GgitCredSshInteractivePrompt **prompts,
	                gsize                          num_prompts);
};

GType         ggit_cred_ssh_interactive_get_type      (void) G_GNUC_CONST;

GgitCredSshInteractive *
              ggit_cred_ssh_interactive_new           (const gchar             *username,
                                                       GError                 **error);

const gchar  *ggit_cred_ssh_interactive_get_username  (GgitCredSshInteractive  *cred);

GType        ggit_cred_ssh_interactive_prompt_get_type (void) G_GNUC_CONST;

GgitCredSshInteractivePrompt *
              ggit_cred_ssh_interactive_prompt_new
                                               (const gchar                  *name,
                                                const gchar                  *instruction,
                                                const gchar                  *text,
                                                gboolean                      is_masked);

GgitCredSshInteractivePrompt *
              ggit_cred_ssh_interactive_prompt_ref
                                               (GgitCredSshInteractivePrompt *prompt);

void          ggit_cred_ssh_interactive_prompt_unref
                                               (GgitCredSshInteractivePrompt *prompt);

const gchar  *ggit_cred_ssh_interactive_prompt_get_name
                                               (GgitCredSshInteractivePrompt *prompt);

const gchar  *ggit_cred_ssh_interactive_prompt_get_text
                                               (GgitCredSshInteractivePrompt *prompt);

const gchar  *ggit_cred_ssh_interactive_prompt_get_instruction
                                               (GgitCredSshInteractivePrompt *prompt);

gboolean      ggit_cred_ssh_interactive_prompt_is_masked
                                               (GgitCredSshInteractivePrompt *prompt);

void          ggit_cred_ssh_interactive_prompt_set_response
                                               (GgitCredSshInteractivePrompt *prompt,
                                                const gchar                  *response);

const gchar  *ggit_cred_ssh_interactive_prompt_get_response
                                               (GgitCredSshInteractivePrompt *prompt);

G_END_DECLS

#endif /* __GGIT_CRED_SSH_INTERACTIVE_H__ */

/* ex:set ts=8 noet: */
