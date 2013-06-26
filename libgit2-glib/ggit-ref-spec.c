/*
 * ggit-ref-spec.c
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

#include <glib-object.h>
#include <git2.h>

#include "ggit-ref-spec.h"

struct _GgitRefSpec {
	gint ref_count;

	const git_refspec *refspec;
};

G_DEFINE_BOXED_TYPE (GgitRefSpec, ggit_ref_spec,
                     ggit_ref_spec_ref, ggit_ref_spec_unref)


GgitRefSpec *
_ggit_ref_spec_wrap (const git_refspec *refspec)
{
	GgitRefSpec *grefspec;

	g_return_val_if_fail (refspec != NULL, NULL);

	grefspec = g_slice_new (GgitRefSpec);
	grefspec->ref_count = 1;
	grefspec->refspec = refspec;

	return grefspec;
}

/**
 * ggit_ref_spec_ref:
 * @refspec: a #GgitRefSpec.
 *
 * Atomically increments the reference count of @refspec by one.
 * This function is MT-safe and may be called from any thread.
 *
 * Returns: a #GgitRefSpec.
 **/
GgitRefSpec *
ggit_ref_spec_ref (GgitRefSpec *refspec)
{
	g_return_val_if_fail (refspec != NULL, NULL);

	g_atomic_int_inc (&refspec->ref_count);

	return refspec;
}

/**
 * ggit_ref_spec_unref:
 * @refspec: a #GgitRefSpec.
 *
 * Atomically decrements the reference count of @refspec by one.
 * If the reference count drops to 0, @refspec is freed.
 **/
void
ggit_ref_spec_unref (GgitRefSpec *refspec)
{
	g_return_if_fail (refspec != NULL);

	if (g_atomic_int_dec_and_test (&refspec->ref_count))
	{
		g_slice_free (GgitRefSpec, refspec);
	}
}

/**
 * ggit_ref_spec_get_source:
 * @refspec: a #GgitRefSpec.
 *
 * Gets the @refspec's source.
 *
 * Returns: the refspec's source.
 */
const gchar *
ggit_ref_spec_get_source (GgitRefSpec *refspec)
{
	g_return_val_if_fail (refspec != NULL, 0);

	return git_refspec_src (refspec->refspec);
}

/**
 * ggit_ref_spec_get_destination:
 * @refspec: a #GgitRefSpec.
 *
 * Gets the @refspec's destination.
 *
 * Returns: the refspec's destination.
 */
const gchar *
ggit_ref_spec_get_destination (GgitRefSpec *refspec)
{
	g_return_val_if_fail (refspec != NULL, 0);

	return git_refspec_dst (refspec->refspec);
}

/**
 * ggit_ref_spec_is_forced:
 * @refspec: a #GgitRefSpec.
 *
 * Whether updating is done forcefully.
 *
 * Returns: if updating is done forcefully.
 */
gboolean
ggit_ref_spec_is_forced (GgitRefSpec *refspec)
{
	g_return_val_if_fail (refspec != NULL, 0);

	return git_refspec_force (refspec->refspec);
}

/* ex:set ts=8 noet: */
