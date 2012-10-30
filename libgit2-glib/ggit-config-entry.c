/*
 * ggit-config-entry.c
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2012 - Ignacio Casal Quinteiro
 *
 * libgit2-glib is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * libgit2-glib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libgit2-glib. If not, see <http://www.gnu.org/licenses/>.
 */

#include "ggit-config-entry.h"

struct _GgitConfigEntry
{
	const git_config_entry *entry;
	gint ref_count;
};

G_DEFINE_BOXED_TYPE (GgitConfigEntry,
                     ggit_config_entry,
                     ggit_config_entry_ref,
                     ggit_config_entry_unref)

GgitConfigEntry *
_ggit_config_entry_wrap (const git_config_entry *entry)
{
	GgitConfigEntry *ret;

	ret = g_slice_new (GgitConfigEntry);
	ret->entry = entry;
	ret->ref_count = 1;

	return ret;
}

/**
 * ggit_config_entry_ref:
 * @entry: a #GgitConfigEntry.
 *
 * Atomically increments the reference count of @entry by one.
 * This function is MT-safe and may be called from any thread.
 *
 * Returns: (transfer none): a #GgitConfigEntry.
 **/
GgitConfigEntry *
ggit_config_entry_ref (GgitConfigEntry *entry)
{
	g_return_val_if_fail (entry != NULL, NULL);

	g_atomic_int_inc (&entry->ref_count);

	return entry;
}

/**
 * ggit_config_entry_unref:
 * @entry: a #GgitConfigEntry.
 *
 * Atomically decrements the reference count of @entry by one.
 * If the reference count drops to 0, @entry is freed.
 **/
void
ggit_config_entry_unref (GgitConfigEntry *entry)
{
	g_return_if_fail (entry != NULL);

	if (g_atomic_int_dec_and_test (&entry->ref_count))
	{
		g_slice_free (GgitConfigEntry, entry);
	}
}

/**
 * ggit_config_entry_get_name:
 * @entry: a #GgitConfigEntry.
 *
 * Gets the name of @entry.
 *
 * Returns: the name of @entry:
 */
const gchar *
ggit_config_entry_get_name (GgitConfigEntry *entry)
{
	g_return_val_if_fail (entry != NULL, NULL);

	return entry->entry->name;
}

/**
 * ggit_config_entry_get_value:
 * @entry: a #GgitConfigEntry.
 *
 * Gets the value of @entry.
 *
 * Returns: the value of @entry.
 */
const gchar *
ggit_config_entry_get_value (GgitConfigEntry *entry)
{
	g_return_val_if_fail (entry != NULL, NULL);

	return entry->entry->value;
}

/**
 * ggit_config_entry_get_level:
 * @entry: a #GgitConfigEntry.
 *
 * Gets the #GgitConfigLevel of @entry.
 *
 * Returns: the #GgitConfigLevel of @entry.
 */
GgitConfigLevel
ggit_config_entry_get_level (GgitConfigEntry *entry)
{
	g_return_val_if_fail (entry != NULL, 0);

	return entry->entry->level;
}

/* ex:set ts=8 noet: */
