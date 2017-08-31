/*
 * ggit-reflog-entry.c
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

#include "ggit-reflog-entry.h"

#include "ggit-oid.h"
#include "ggit-signature.h"

struct _GgitReflogEntry
{
	const git_reflog_entry *reflog_entry;
	gint ref_count;
};

G_DEFINE_BOXED_TYPE (GgitReflogEntry, ggit_reflog_entry,
                     ggit_reflog_entry_ref, ggit_reflog_entry_unref)

GgitReflogEntry *
_ggit_reflog_entry_wrap (const git_reflog_entry *reflog_entry)
{
	GgitReflogEntry *greflog_entry;

	greflog_entry = g_slice_new (GgitReflogEntry);
	greflog_entry->reflog_entry = reflog_entry;
	greflog_entry->ref_count = 1;

	return greflog_entry;
}

/**
 * ggit_reflog_entry_ref:
 * @reflog_entry: a #GgitReflogEntry.
 *
 * Atomically increments the reference count of @reflog_entry by one.
 * This function is MT-safe and may be called from any thread.
 *
 * Returns: (transfer none) (allow-none): the passed in #GgitReflogEntry or %NULL.
 */
GgitReflogEntry *
ggit_reflog_entry_ref (GgitReflogEntry *reflog_entry)
{
	g_return_val_if_fail (reflog_entry != NULL, NULL);

	g_atomic_int_inc (&reflog_entry->ref_count);

	return reflog_entry;
}

/**
 * ggit_reflog_entry_unref:
 * @reflog_entry: a #GgitReflogEntry.
 *
 * Atomically decrements the reference count of @reflog_entry by one.
 * If the reference count drops to 0, @reflog_entry is freed.
 */
void
ggit_reflog_entry_unref (GgitReflogEntry *reflog_entry)
{
	g_return_if_fail (reflog_entry != NULL);

	if (g_atomic_int_dec_and_test (&reflog_entry->ref_count))
	{
		g_slice_free (GgitReflogEntry, reflog_entry);
	}
}

/**
 * ggit_reflog_entry_get_old_id:
 * @reflog_entry: a #GgitReflogEntry.
 *
 * Gets the old #GgitOId.
 *
 * Returns: (transfer full) (allow-none): the old oid or %NULL.
 */
GgitOId *
ggit_reflog_entry_get_old_id (GgitReflogEntry *reflog_entry)
{
	const git_oid *oid;

	g_return_val_if_fail (reflog_entry != NULL, NULL);

	oid = git_reflog_entry_id_old (reflog_entry->reflog_entry);

	return _ggit_oid_wrap (oid);
}

/**
 * ggit_reflog_entry_get_new_id:
 * @reflog_entry: a #GgitReflogEntry.
 *
 * Gets the new #GgitOId.
 *
 * Returns: (transfer full) (allow-none): the new oid or %NULL.
 */
GgitOId *
ggit_reflog_entry_get_new_id (GgitReflogEntry *reflog_entry)
{
	const git_oid *oid;

	g_return_val_if_fail (reflog_entry != NULL, NULL);

	oid = git_reflog_entry_id_new (reflog_entry->reflog_entry);

	return _ggit_oid_wrap (oid);
}

/**
 * ggit_reflog_entry_get_committer:
 * @reflog_entry: a #GgitReflogEntry.
 *
 * Gets the committer as a #GgitSignature.
 *
 * Returns: (transfer none) (allow-none): the committer or %NULL.
 */
GgitSignature *
ggit_reflog_entry_get_committer (GgitReflogEntry *reflog_entry)
{
	const git_signature *committer;

	g_return_val_if_fail (reflog_entry != NULL, NULL);

	committer = git_reflog_entry_committer (reflog_entry->reflog_entry);

	return _ggit_signature_wrap (committer, "UTF-8", FALSE);
}

/**
 * ggit_reflog_entry_get_message:
 * @reflog_entry: a #GgitReflogEntry.
 *
 * Gets the message.
 *
 * Returns: (transfer none) (allow-none): the message or %NULL.
 */
const gchar *
ggit_reflog_entry_get_message (GgitReflogEntry *reflog_entry)
{
	g_return_val_if_fail (reflog_entry != NULL, NULL);

	return git_reflog_entry_message (reflog_entry->reflog_entry);
}

/* ex:set ts=8 noet: */
