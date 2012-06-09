/*
 * ggit-ref.c
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2011 - Ignacio Casal Quinteiro
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

#include "ggit-ref.h"
#include "ggit-oid.h"
#include "ggit-error.h"
#include "ggit-reflog.h"
#include "ggit-repository.h"
#include "ggit-signature.h"
#include "ggit-utils.h"

#include <git2/errors.h>

G_DEFINE_TYPE (GgitRef, ggit_ref, GGIT_TYPE_NATIVE)

static void
ggit_ref_class_init (GgitRefClass *klass)
{
}

static void
ggit_ref_init (GgitRef *self)
{
}

GgitRef *
_ggit_ref_wrap (git_reference *ref)
{
	return g_object_new (GGIT_TYPE_REF,
	                     "native", ref,
	                     NULL);
}

/**
 * ggit_ref_get_id:
 * @ref: a #GgitRef.
 *
 * Get the OID pointed to by a reference.
 * Only available if the reference is direct (i.e. not symbolic).
 *
 * Returns: (transfer full): a new oid if available, %NULL otherwise.
 */
GgitOId *
ggit_ref_get_id (GgitRef *ref)
{
	GgitOId *goid = NULL;
	const git_oid *oid;

	g_return_val_if_fail (ref != NULL, NULL);

	oid = git_reference_oid (_ggit_native_get (ref));

	if (oid != NULL)
	{
		goid = _ggit_oid_new ((git_oid *)oid);
	}

	return goid;
}

/**
 * ggit_ref_get_target:
 * @ref: a #GgitRef.
 *
 * Gets the full name to the reference pointed by this reference.
 * Only available if the reference is symbolic.
 *
 * Returns: the name if available, %NULL otherwise.
 */
const gchar *
ggit_ref_get_target (GgitRef *ref)
{
	g_return_val_if_fail (ref != NULL, NULL);

	return git_reference_target (_ggit_native_get (ref));
}

/**
 * ggit_ref_get_reference_type:
 * @ref: a #GgitRef.
 *
 * Gets the type of @ref. Either direct (#GGIT_REF_OID) or
 * symbolic (#GGIT_REF_SYMBOLIC).
 *
 * Returns: the type of a reference.
 */
GgitRefType
ggit_ref_get_reference_type (GgitRef *ref)
{
	g_return_val_if_fail (ref != NULL, GGIT_REF_INVALID);

	return (GgitRefType)git_reference_type (_ggit_native_get (ref));
}

/**
 * ggit_ref_get_name:
 * @ref: a #GgitRef.
 *
 * Gets the full name of @ref.
 *
 * Returns: the full name of a reference.
 */
const gchar *
ggit_ref_get_name (GgitRef *ref)
{
	g_return_val_if_fail (ref != NULL, NULL);

	return git_reference_name (_ggit_native_get (ref));
}

/**
 * ggit_ref_to_string:
 * @ref: a #GgitRef.
 *
 * Get a string representation of the ref.
 *
 * Returns: a string representation of the ref.
 *
 **/
const gchar *
ggit_ref_to_string (GgitRef *ref)
{
	return ggit_ref_get_name (ref);
}

/**
 * ggit_ref_resolve:
 * @ref: a #GgitRef.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Resolves a symbolic reference.
 *
 * This method iteratively peels a symbolic reference
 * until it resolves to a direct reference to an OID.
 *
 * If a direct reference is passed as an argument,
 * that reference is returned immediately.
 *
 * Returns: (transfer full): the resolved reference to the peeled one.
 */
GgitRef *
ggit_ref_resolve (GgitRef  *ref,
                  GError  **error)
{
	GgitRef *rev_ref = NULL;
	git_reference *reference;
	gint ret;

	g_return_val_if_fail (ref != NULL, NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	ret = git_reference_resolve (&reference, _ggit_native_get (ref));

	if (ret == GIT_OK)
	{
		rev_ref = _ggit_ref_wrap (reference);
	}
	else
	{
		_ggit_error_set (error, ret);
	}

	return rev_ref;
}

/**
 * ggit_ref_get_owner:
 * @ref: a #GgitRef.
 *
 * Gets the repository where @ref resides.
 *
 * Returns: (transfer full): the repository where a reference resides.
 */
GgitRepository *
ggit_ref_get_owner (GgitRef *ref)
{
	g_return_val_if_fail (ref != NULL, NULL);

	return _ggit_repository_wrap (git_reference_owner (_ggit_native_get (ref)),
	                              FALSE);
}

/**
 * ggit_ref_set_target:
 * @ref: a #GgitRef.
 * @target: The new target for the reference.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Sets the symbolic target of @ref.
 *
 * The reference must be a symbolic reference, otherwise
 * this method will fail.
 *
 * The reference will be automatically updated in
 * memory and on disk.
 */
void
ggit_ref_set_target (GgitRef      *ref,
                     const gchar  *target,
                     GError      **error)
{
	gint ret;

	g_return_if_fail (ref != NULL);
	g_return_if_fail (target != NULL);
	g_return_if_fail (error == NULL || *error == NULL);

	ret = git_reference_set_target (_ggit_native_get (ref), target);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
	}
}

/**
 * ggit_ref_set_oid:
 * @ref: a #GgitRef.
 * @oid: a #GgitOId.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Sets the OID target of @ref.
 *
 * The reference must be a direct reference, otherwise
 * this method will fail.
 *
 * The reference will be automatically updated in
 * memory and on disk.
 */
void
ggit_ref_set_oid (GgitRef  *ref,
                  GgitOId  *oid,
                  GError  **error)
{
	gint ret;

	g_return_if_fail (ref != NULL);
	g_return_if_fail (oid != NULL);
	g_return_if_fail (error == NULL || *error == NULL);

	ret = git_reference_set_oid (_ggit_native_get (ref), _ggit_oid_get_oid (oid));

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
	}
}

/**
 * ggit_ref_rename:
 * @ref: a #GgitRef.
 * @new_name: the new name.
 * @force: %TRUE to force the renaming.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Renames @ref.
 *
 * This method works for both direct and symbolic references.
 * The new name will be checked for validity and may be
 * modified into a normalized form.
 *
 * The refernece will be immediately renamed in-memory
 * and on disk.
 */
void
ggit_ref_rename (GgitRef     *ref,
                 const gchar *new_name,
                 gboolean     force,
                 GError     **error)
{
	gint ret;

	g_return_if_fail (ref != NULL);
	g_return_if_fail (new_name != NULL);
	g_return_if_fail (error == NULL || *error == NULL);

	force = (force != FALSE);

	ret = git_reference_rename (_ggit_native_get (ref), new_name, force);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
	}
}

/**
 * ggit_ref_delete:
 * @ref: a #GgitRef.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Deletes @ref.
 *
 * This method works for both direct and symbolic references.
 *
 * The reference will be immediately removed on disk and from
 * memory. The given reference pointer will no longer be valid.
 */
void
ggit_ref_delete (GgitRef  *ref,
                 GError  **error)
{
	gint ret;

	g_return_if_fail (ref != NULL);
	g_return_if_fail (error == NULL || *error == NULL);

	ret = git_reference_delete (_ggit_native_get (ref));

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
	}
}

/**
 * ggit_ref_lookup:
 * @ref: a #GgitRef.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Convenient method to resolve a reference to an object.
 *
 * Returns: (transfer full): a #GgitObject.
 *
 **/
GgitObject *
ggit_ref_lookup (GgitRef  *ref,
                 GError  **error)
{
	git_object *obj;
	git_reference *r;
	gint ret;

	g_return_val_if_fail (GGIT_IS_REF (ref), NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	r = _ggit_native_get (ref);

	ret = git_object_lookup (&obj,
	                         git_reference_owner (r),
	                         git_reference_oid (r),
	                         GIT_OBJ_ANY);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return NULL;
	}
	else
	{
		return ggit_utils_create_real_object (obj, TRUE);
	}
}

/**
 * ggit_ref_get_reflog:
 * @ref: a #GgitRef.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Gets the #GgitReflog for @ref.
 *
 * Returns: the reflog.
 */
GgitReflog *
ggit_ref_get_reflog (GgitRef  *ref,
                     GError  **error)
{
	git_reflog *reflog;
	gint ret;

	g_return_val_if_fail (GGIT_IS_REF (ref), NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	ret = git_reflog_read (&reflog, _ggit_native_get (ref));

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return NULL;
	}

	return _ggit_reflog_new (reflog);
}

/**
 * ggit_ref_create_reflog:
 * @ref: a #GgitRef.
 * @oid: a #GgitOId.
 * @committer: a #GgitSignature.
 * @message: the message.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Creates a #GgitReflog with the given properties.
 *
 * Returns: the created reflog, or %NULL if error is set.
 */
GgitReflog *
ggit_ref_create_reflog (GgitRef        *ref,
                        GgitOId        *oid,
                        GgitSignature  *committer,
                        const gchar    *message,
                        GError        **error)
{
	gint ret;

	g_return_val_if_fail (GGIT_IS_REF (ref), NULL);
	g_return_val_if_fail (oid != NULL, NULL);
	g_return_val_if_fail (GGIT_IS_SIGNATURE (committer), NULL);
	g_return_val_if_fail (message != NULL && *message != '\0', NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	ret = git_reflog_write (_ggit_native_get (ref),
	                        _ggit_native_get (oid),
	                        _ggit_native_get (committer),
	                        message);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return NULL;
	}

	return ggit_ref_get_reflog (ref, error);
}

/**
 * ggit_ref_rename_reflog:
 * @ref: a #GgitRef.
 * @new_name: the new name of the reference.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Renames the reflog for @ref to @new_name, on error @error is set.
 */
void
ggit_ref_rename_reflog (GgitRef      *ref,
                        const gchar  *new_name,
                        GError      **error)
{
	gint ret;

	g_return_if_fail (GGIT_IS_REF (ref));
	g_return_if_fail (new_name != NULL && *new_name != '\0');
	g_return_if_fail (error == NULL || *error == NULL);

	ret = git_reflog_rename (_ggit_native_get (ref), new_name);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
	}
}

/**
 * ggit_ref_delete_reflog:
 * @ref: a #GgitRef.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Deletes the reflog for @ref, on error @error is set.
 */
void
ggit_ref_delete_reflog (GgitRef  *ref,
                        GError  **error)
{
	gint ret;

	g_return_if_fail (GGIT_IS_REF (ref));
	g_return_if_fail (error == NULL || *error == NULL);

	ret = git_reflog_delete (_ggit_native_get (ref));

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
	}
}

/* ex:set ts=8 noet: */
