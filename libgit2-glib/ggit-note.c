/*
 * ggit-note.c
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2014 - Ignacio Casal Quinteiro
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

#include "ggit-note.h"
#include "ggit-oid.h"

struct _GgitNote
{
	gint ref_count;

	git_note *note;
	GgitOId *id;
};

G_DEFINE_BOXED_TYPE (GgitNote, ggit_note,
                     ggit_note_ref, ggit_note_unref)

GgitNote *
_ggit_note_wrap (git_note *note)
{
	GgitNote *gnote;

	g_return_val_if_fail (note != NULL, NULL);

	gnote = g_slice_new (GgitNote);
	gnote->ref_count = 1;
	gnote->note = note;
	gnote->id = _ggit_oid_wrap (git_note_id (note));

	return gnote;
}

/**
 * ggit_note_ref:
 * @note: a #GgitNote.
 *
 * Atomically increments the reference count of @note by one.
 * This function is MT-safe and may be called from any thread.
 *
 * Returns: (transfer none) (allow-none): a #GgitNote or %NULL.
 **/
GgitNote *
ggit_note_ref (GgitNote *note)
{
	g_return_val_if_fail (note != NULL, NULL);

	g_atomic_int_inc (&note->ref_count);

	return note;
}

/**
 * ggit_note_unref:
 * @note: a #GgitNote.
 *
 * Atomically decrements the reference count of @note by one.
 * If the reference count drops to 0, @note is freed.
 **/
void
ggit_note_unref (GgitNote *note)
{
	g_return_if_fail (note != NULL);

	if (g_atomic_int_dec_and_test (&note->ref_count))
	{
		git_note_free (note->note);
		ggit_oid_free (note->id);
		g_slice_free (GgitNote, note);
	}
}

/**
 * ggit_note_get_message:
 * @note: a #GgitNote.
 *
 * Gets the note message.
 *
 * Returns: (transfer none) (allow-none): the note message or %NULL.
 */
const gchar *
ggit_note_get_message (GgitNote *note)
{
	g_return_val_if_fail (note != NULL, NULL);

	return git_note_message (note->note);
}

/**
 * ggit_note_get_id:
 * @note: a #GgitNote.
 *
 * Gets the note object's id.
 *
 * Returns: (transfer none) (allow-none): the object's id or %NULL.
 */
GgitOId *
ggit_note_get_id (GgitNote *note)
{
	g_return_val_if_fail (note != NULL, NULL);

	return note->id;
}

/* ex:set ts=8 noet: */
