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
#include "ggit-native.h"
#include "ggit-error.h"
#include "ggit-signature.h"
#include "ggit-oid.h"

struct _GgitReflog
{
	GgitRef *ref;
	git_reflog *reflog;
	gint ref_count;
};

G_DEFINE_BOXED_TYPE (GgitReflog, ggit_reflog, ggit_reflog_ref, ggit_reflog_unref)

GgitReflog *
_ggit_reflog_wrap (GgitRef    *ref,
                   git_reflog *reflog)
{
	GgitReflog *greflog;

	greflog = g_slice_new (GgitReflog);
	greflog->reflog = reflog;
	greflog->ref = g_object_ref (ref);
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
 * Returns: (transfer none) (allow-none): the passed in #GgitReflog or %NULL.
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
		g_object_unref (reflog->ref);

		git_reflog_free (reflog->reflog);
		g_slice_free (GgitReflog, reflog);
	}
}

/**
 * ggit_reflog_write:
 * @reflog: a #GgitReflog.
 * @error: a #GError.
 *
 * Write the reflog to disk.
 *
 * Returns: %TRUE if the reflog was successfully written, or %FALSE on error.
 *
 **/
gboolean
ggit_reflog_write (GgitReflog  *reflog,
                   GError     **error)
{
	gint ret;

	g_return_val_if_fail (reflog != NULL, FALSE);
	g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

	ret = git_reflog_write (reflog->reflog);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return FALSE;
	}

	return TRUE;
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
 * Returns: (transfer full) (allow-none): the reflog entry at the index, or %NULL if not found.
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


/**
 * ggit_reflog_rename:
 * @reflog: a #GgitReflog.
 * @new_name: the new name of the reference.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Renames the reflog for to @new_name, on error @error is set.
 */
gboolean
ggit_reflog_rename (GgitReflog  *reflog,
                    const gchar *new_name,
                    GError      **error)
{
	git_reference *nref;
	gint ret;

	g_return_val_if_fail (reflog != NULL, FALSE);
	g_return_val_if_fail (new_name != NULL && *new_name != '\0', FALSE);
	g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

	nref = _ggit_native_get (reflog->ref);

	ret = git_reflog_rename (git_reference_owner (nref),
	                         git_reference_name (nref),
	                         new_name);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return FALSE;
	}

	return TRUE;
}

/**
 * ggit_reflog_append:
 * @reflog: a #GgitReflog.
 * @oid: a #GgitOId.
 * @committer: a #GgitSignature.
 * @message: the message.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Creates a reflog entry.
 *
 * Returns: %TRUE if the reflog was successfully created, or %FALSE if error is set.
 */
gboolean
ggit_reflog_append (GgitReflog     *reflog,
                    GgitOId        *oid,
                    GgitSignature  *committer,
                    const gchar    *message,
                    GError        **error)
{
	gint ret;

	g_return_val_if_fail (reflog != NULL, FALSE);
	g_return_val_if_fail (oid != NULL, FALSE);
	g_return_val_if_fail (GGIT_IS_SIGNATURE (committer), FALSE);
	g_return_val_if_fail (message != NULL && *message != '\0', FALSE);
	g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

	ret = git_reflog_append (reflog->reflog,
	                         _ggit_oid_get_oid (oid),
	                         _ggit_native_get (committer),
	                         message);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return FALSE;
	}

	return TRUE;
}

/* ex:set ts=8 noet: */
