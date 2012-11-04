/*
 * ggit-reflog.c
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

#include "ggit-reflog.h"

#include "ggit-reflog-entry.h"

struct _GgitReflog
{
	git_reflog *reflog;
	gint ref_count;
};

G_DEFINE_BOXED_TYPE (GgitReflog, ggit_reflog, ggit_reflog_ref, ggit_reflog_unref)

GgitReflog *
_ggit_reflog_wrap (git_reflog *reflog)
{
	GgitReflog *greflog;

	greflog = g_slice_new (GgitReflog);
	greflog->reflog = reflog;
	greflog->ref_count = 1;

	return greflog;
}

/**
 * ggit_reflog_ref:
 * @reflog: a #GgitReflog.
 *
 * Atomically increments the reference count of @reflog by one.
 * This function is MT-safe and may be called from any thread.
 *
 * Returns: (transfer none): the passed in #GgitReflog.
 */
GgitReflog *
ggit_reflog_ref (GgitReflog *reflog)
{
	g_return_val_if_fail (reflog != NULL, NULL);

	g_atomic_int_inc (&reflog->ref_count);

	return reflog;
}

/**
 * ggit_reflog_unref:
 * @reflog: a #GgitReflog.
 *
 * Atomically decrements the reference count of @reflog by one.
 * If the reference count drops to 0, @reflog is freed.
 */
void
ggit_reflog_unref (GgitReflog *reflog)
{
	g_return_if_fail (reflog != NULL);

	if (g_atomic_int_dec_and_test (&reflog->ref_count))
	{
		git_reflog_free (reflog->reflog);
		g_slice_free (GgitReflog, reflog);
	}
}

/**
 * ggit_reflog_get_entry_count:
 * @reflog: a #GgitReflog.
 *
 * Gets the number of log entries in @reflog.
 *
 * Returns: the number of log entries.
 */
guint
ggit_reflog_get_entry_count (GgitReflog *reflog)
{
	g_return_val_if_fail (reflog != NULL, 0);

	return git_reflog_entrycount (reflog->reflog);
}

/**
 * ggit_reflog_get_entry_from_index:
 * @reflog: a #GgitReflog.
 * @idx: the position to lookup.
 *
 * Gets the #GgitReflogEntry at @idx in @reflog, or %NULL if not found.
 *
 * Returns: the reflog entry at the index, or %NULL if not found.
 */
GgitReflogEntry *
ggit_reflog_get_entry_from_index (GgitReflog *reflog,
                                  guint       idx)
{
	const git_reflog_entry *reflog_entry;

	g_return_val_if_fail (reflog != NULL, NULL);

	reflog_entry = git_reflog_entry_byindex (reflog->reflog, idx);

	if (reflog_entry == NULL)
	{
		return NULL;
	}

	return _ggit_reflog_entry_wrap (reflog_entry);
}

/* ex:set ts=8 noet: */
