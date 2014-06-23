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

#include <git2.h>

/* FIXME: we should have a ref superclass and direct and symbolic subclasses */

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
 * ggit_ref_is_valid_name:
 * @name: the name to validate.
 *
 * Check if the given @name is a valid name for a reference. Note that @name
 * should be the full ref name (including prefixes).
 *
 * Valid toplevel names can contain only capital letters and underscores
 * and must start and end with a letter (e.g. HEAD, ORIG_HEAD).
 *
 * Valid refs/ names may contain any characters, except '~', '^', ':', '\', '?',
 * '[', '*', ".." and "@{", because they are interpreted by revparse.
 *
 * Returns: %TRUE if @name is valid, %FALSE otherwise.
 *
 **/
gboolean
ggit_ref_is_valid_name (const gchar *name)
{
	g_return_val_if_fail (name != NULL, FALSE);

	return git_reference_is_valid_name (name) == 1;
}

/**
 * ggit_ref_get_target:
 * @ref: a #GgitRef.
 *
 * Get the OID pointed to by a direct reference.
 * Only available if the reference is direct (i.e. an object id reference,
 * not a symbolic one).
 *
 * Returns: (transfer full): a new oid if available, %NULL otherwise.
 */
GgitOId *
ggit_ref_get_target (GgitRef *ref)
{
	GgitOId *goid = NULL;
	const git_oid *oid;

	g_return_val_if_fail (ref != NULL, NULL);

	oid = git_reference_target (_ggit_native_get (ref));

	if (oid != NULL)
	{
		goid = _ggit_oid_wrap ((git_oid *)oid);
	}

	return goid;
}

/**
 * ggit_ref_get_symbolic_target:
 * @ref: a #GgitRef.
 *
 * Get full name to the reference pointed to by a symbolic reference.
 * Only available if the reference is symbolic.
 *
 * Returns: the name if available, %NULL otherwise.
 */
const gchar *
ggit_ref_get_symbolic_target (GgitRef *ref)
{
	g_return_val_if_fail (ref != NULL, NULL);

	return git_reference_symbolic_target (_ggit_native_get (ref));
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
 * ggit_ref_get_shorthand:
 * @ref: a #GgitRef
 *
 * Gets the shorthand name of @ref.
 *
 * Returns: the shorthand name of a reference.
 */
const gchar *
ggit_ref_get_shorthand (GgitRef *ref)
{
	g_return_val_if_fail (ref != NULL, NULL);

	return git_reference_shorthand (_ggit_native_get (ref));
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
 * ggit_ref_set_symbolic_target:
 * @ref: a #GgitRef.
 * @target: The new target for the reference.
 * @signature: a #GgitSignature that will used to populate the reflog entry.
 * @log_message: The one line long message to be appended to the reflog.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Create a new reference with the same name as the given reference but a
 * different symbolic target. The reference must be a symbolic reference,
 * otherwise this will fail.
 *
 * The new reference will be written to disk, overwriting the given reference.
 *
 * The target name will be checked for validity.
 * See `ggit_ref_create_symbolic()` for rules about valid names.
 *
 * Returns: (transfer full): the newly created #GgitRef.
 */
GgitRef *
ggit_ref_set_symbolic_target (GgitRef       *ref,
                              const gchar   *target,
                              GgitSignature *signature,
                              const gchar   *log_message,
                              GError       **error)
{
	git_reference *out;
	gint ret;

	g_return_val_if_fail (ref != NULL, NULL);
	g_return_val_if_fail (target != NULL, NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	ret = git_reference_symbolic_set_target (&out,
	                                         _ggit_native_get (ref),
	                                         target,
	                                         _ggit_native_get (signature),
	                                         log_message);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return NULL;
	}

	return _ggit_ref_wrap (out);
}

/**
 * ggit_ref_set_target:
 * @ref: a #GgitRef.
 * @oid: a #GgitOId.
 * @signature: a #GgitSignature that will used to populate the reflog entry.
 * @log_message: The one line long message to be appended to the reflog.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Create a new reference with the same name as the given reference but a
 * different OID target. The reference must be a direct reference, otherwise
 * this will fail.
 *
 * The new reference will be written to disk, overwriting the given reference.
 *
 * Returns: (transfer full): the newly created #GgitRef.
 */
GgitRef *
ggit_ref_set_target (GgitRef       *ref,
                     GgitOId       *oid,
                     GgitSignature *signature,
                     const gchar   *log_message,
                     GError       **error)
{
	git_reference *out;
	gint ret;

	g_return_val_if_fail (ref != NULL, NULL);
	g_return_val_if_fail (oid != NULL, NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	ret = git_reference_set_target (&out,
	                                _ggit_native_get (ref),
	                                _ggit_oid_get_oid (oid),
	                                _ggit_native_get (signature),
	                                log_message);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return NULL;
	}

	return _ggit_ref_wrap (out);
}

/**
 * ggit_ref_rename:
 * @ref: a #GgitRef.
 * @new_name: the new name.
 * @force: %TRUE to force the renaming.
 * @signature: a #GgitSignature that will used to populate the reflog entry.
 * @log_message: The one line long message to be appended to the reflog.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Rename an existing reference.
 *
 * This method works for both direct and symbolic references.
 *
 * The new name will be checked for validity.
 * See `ggit_ref_create_symbolic()` for rules about valid names.
 *
 * If not error, @ref will be deleted from disk and a
 * new #GgitRef will be returned.
 *
 * The reference will be immediately renamed in-memory and on disk.
 *
 * If the `force` flag is not enabled, and there's already
 * a reference with the given name, the renaming will fail.
 *
 * IMPORTANT:
 * The user needs to write a proper reflog entry if the
 * reflog is enabled for the repository. We only rename
 * the reflog if it exists.
 *
 * Returns: (transfer full): a newly created #GgitRef.
 */
GgitRef *
ggit_ref_rename (GgitRef       *ref,
                 const gchar   *new_name,
                 gboolean       force,
                 GgitSignature *signature,
                 const gchar   *log_message,
                 GError       **error)
{
	git_reference *out;
	gint ret;

	g_return_val_if_fail (ref != NULL, NULL);
	g_return_val_if_fail (new_name != NULL, NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	force = (force != FALSE);

	ret = git_reference_rename (&out,
	                            _ggit_native_get (ref),
	                            new_name,
	                            force,
	                            _ggit_native_get (signature),
	                            log_message);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return NULL;
	}

	return _ggit_ref_wrap (out);
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
	                         git_reference_target (r),
	                         GIT_OBJ_ANY);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return NULL;
	}

	return ggit_utils_create_real_object (obj, TRUE);
}

/**
 * ggit_ref_get_log:
 * @ref: a #GgitRef.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Gets the #GgitReflog for @ref. The reflog will be created if it doesn't exist
 * yet.
 *
 * Returns: (transfer full): the reflog.
 */
GgitReflog *
ggit_ref_get_log (GgitRef  *ref,
                  GError  **error)
{
	git_reflog *reflog;
	git_reference *nref;
	gint ret;

	g_return_val_if_fail (GGIT_IS_REF (ref), NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	nref = _ggit_native_get (ref);

	ret = git_reflog_read (&reflog,
	                       git_reference_owner (nref),
	                       git_reference_name (nref));

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return NULL;
	}

	return _ggit_reflog_wrap (ref, reflog);
}

/**
 * ggit_ref_delete_log:
 * @ref: a #GgitRef.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Deletes the log for @ref, on error @error is set.
 */
void
ggit_ref_delete_log (GgitRef  *ref,
                     GError  **error)
{
	git_reference *nref;
	gint ret;

	g_return_if_fail (GGIT_IS_REF (ref));
	g_return_if_fail (error == NULL || *error == NULL);

	nref = _ggit_native_get (ref);

	ret = git_reflog_delete (git_reference_owner (nref),
	                         git_reference_name (nref));

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
	}
}

/**
 * ggit_ref_is_tag:
 * @ref: a #GgitRef.
 *
 * Check whether the reference is a tag.
 *
 * Returns: %TRUE if the reference is a tag, %FALSE otherwise.
 *
 **/
gboolean
ggit_ref_is_tag (GgitRef *ref)
{
	g_return_val_if_fail (ref != NULL, FALSE);

	return git_reference_is_tag (_ggit_native_get (ref));
}

/**
 * ggit_ref_is_remote:
 * @ref: a #GgitRef.
 *
 * Check whether the reference is a remote.
 *
 * Returns: %TRUE if the reference is a remote, %FALSE otherwise.
 *
 **/
gboolean
ggit_ref_is_remote (GgitRef *ref)
{
	g_return_val_if_fail (ref != NULL, FALSE);

	return git_reference_is_remote (_ggit_native_get (ref));
}

/**
 * ggit_ref_is_branch:
 * @ref: a #GgitRef.
 *
 * Check whether the reference is a branch.
 *
 * Returns: %TRUE if the reference is a branch, %FALSE otherwise.
 *
 **/
gboolean
ggit_ref_is_branch (GgitRef *ref)
{
	g_return_val_if_fail (ref != NULL, FALSE);

	return git_reference_is_branch (_ggit_native_get (ref));
}

/* ex:set ts=8 noet: */
