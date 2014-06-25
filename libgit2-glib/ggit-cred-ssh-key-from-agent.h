/*
 * ggit-cred-ssh-key-from-agent.h
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


#ifndef __GGIT_CRED_SSH_KEY_FROM_AGENT_H__
#define __GGIT_CRED_SSH_KEY_FROM_AGENT_H__

#include <glib-object.h>
#include <libgit2-glib/ggit-cred.h>

G_BEGIN_DECLS

#define GGIT_TYPE_CRED_SSH_KEY_FROM_AGENT		(ggit_cred_ssh_key_from_agent_get_type ())
#define GGIT_CRED_SSH_KEY_FROM_AGENT(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), GGIT_TYPE_CRED_SSH_KEY_FROM_AGENT, GgitCredSshKeyFromAgent))
#define GGIT_CRED_SSH_KEY_FROM_AGENT_CONST(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), GGIT_TYPE_CRED_SSH_KEY_FROM_AGENT, GgitCredSshKeyFromAgent const))
#define GGIT_CRED_SSH_KEY_FROM_AGENT_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST ((klass), GGIT_TYPE_CRED_SSH_KEY_FROM_AGENT, GgitCredSshKeyFromAgentClass))
#define GGIT_IS_CRED_SSH_KEY_FROM_AGENT(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), GGIT_TYPE_CRED_SSH_KEY_FROM_AGENT))
#define GGIT_IS_CRED_SSH_KEY_FROM_AGENT_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), GGIT_TYPE_CRED_SSH_KEY_FROM_AGENT))
#define GGIT_CRED_SSH_KEY_FROM_AGENT_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS ((obj), GGIT_TYPE_CRED_SSH_KEY_FROM_AGENT, GgitCredSshKeyFromAgentClass))

typedef struct _GgitCredSshKeyFromAgentClass		GgitCredSshKeyFromAgentClass;
typedef struct _GgitCredSshKeyFromAgentPrivate		GgitCredSshKeyFromAgentPrivate;

struct _GgitCredSshKeyFromAgent
{
	/*< private >*/
	GgitCred parent;

	GgitCredSshKeyFromAgentPrivate *priv;
};

struct _GgitCredSshKeyFromAgentClass
{
	/*< private >*/
	GgitCredClass parent_class;
};

GType           ggit_cred_ssh_key_from_agent_get_type      (void) G_GNUC_CONST;

GgitCredSshKeyFromAgent *
                ggit_cred_ssh_key_from_agent_new           (const gchar  *username,
                                                            GError      **error);

const gchar    *ggit_cred_ssh_key_from_agent_get_username  (GgitCredSshKeyFromAgent *cred);

G_END_DECLS

#endif /* __GGIT_CRED_SSH_KEY_FROM_AGENT_H__ */

/* ex:set ts=8 noet: */
