/*
 * ggit-remote-callbacks.c
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

#include <git2.h>

#include "ggit-remote-callbacks.h"

struct _GgitRemoteCbs
{
	git_remote_callbacks remote_cbs;
	gint ref_count;
};

G_DEFINE_BOXED_TYPE (GgitRemoteCbs, ggit_remote_cbs,
                     ggit_remote_cbs_ref, ggit_remote_cbs_unref)

const git_remote_callbacks *
_ggit_remote_cbs_get_remote_callbacks (GgitRemoteCbs *remote_cbs)
{
	/* NULL is common for remote_cbs as it specifies to use the default
	 * so handle a NULL remote_cbs here instead of in every caller.
	 */
	if (remote_cbs == NULL)
	{
		return NULL;
	}

	return (const git_remote_callbacks *)&remote_cbs->remote_cbs;
}

/**
 * ggit_remote_cbs_ref:
 * @remote_cbs: a #GgitRemoteCbs.
 *
 * Copies @remote_cbs into a newly allocated #GgitRemoteCbs.
 *
 * Returns: (transfer full): a newly allocated #GgitRemoteCbs.
 */
GgitRemoteCbs *
ggit_remote_cbs_ref (GgitRemoteCbs *remote_cbs)
{
	g_return_val_if_fail (remote_cbs != NULL, NULL);

	g_atomic_int_inc (&remote_cbs->ref_count);

	return remote_cbs;
}

/**
 * ggit_remote_cbs_unref:
 * @remote_cbs: a #GgitRemoteCbs.
 *
 * Frees @remote_cbs.
 */
void
ggit_remote_cbs_unref (GgitRemoteCbs *remote_cbs)
{
	g_return_if_fail (remote_cbs != NULL);

	if (g_atomic_int_dec_and_test (&remote_cbs->ref_count))
	{
		g_slice_free (GgitRemoteCbs, remote_cbs);
	}
}

/**
 * ggit_remote_cbs_new:
 *
 * Creates a new #GgitRemoteCbs.
 *
 * Returns: a newly allocated #GgitRemoteCbs.
 */
GgitRemoteCbs *
ggit_remote_cbs_new (void)
{
	GgitRemoteCbs *remote_cbs;
	git_remote_callbacks gremote_cbs = GIT_REMOTE_CALLBACKS_INIT;

	remote_cbs = g_slice_new (GgitRemoteCbs);
	remote_cbs->remote_cbs = gremote_cbs;

	return remote_cbs;
}

/* ex:set ts=8 noet: */
