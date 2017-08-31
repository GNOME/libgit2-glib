/*
 * ggit-rebase.c
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2015 - Ignacio Casal Quinteiro
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

#include "ggit-rebase.h"
#include "ggit-rebase-operation.h"
#include "ggit-error.h"
#include "ggit-oid.h"

G_STATIC_ASSERT (GGIT_REBASE_NO_OPERATION == GIT_REBASE_NO_OPERATION);

/**
 * GgitRebase:
 *
 * Represents a rebase object.
 */
struct _GgitRebase
{
	GgitNative parent_instance;
};

G_DEFINE_TYPE (GgitRebase, ggit_rebase, GGIT_TYPE_NATIVE)

static void
ggit_rebase_class_init (GgitRebaseClass *klass)
{
}

static void
ggit_rebase_init (GgitRebase *self)
{
}

GgitRebase *
_ggit_rebase_wrap (git_rebase *rebase)
{
	GgitRebase *grebase;

	grebase = g_object_new (GGIT_TYPE_REBASE,
	                        "native", rebase,
	                        NULL);

	_ggit_native_set_destroy_func (grebase,
	                               (GDestroyNotify)git_rebase_free);

	return grebase;
}

/**
 * ggit_rebase_get_operation_entry_count:
 * @rebase: a #GgitRebase.
 *
 * Gets the count of rebase operations that are to be applied.
 *
 * Returns: the count of rebase operations that are to be applied.
 */
gsize
ggit_rebase_get_operation_entry_count (GgitRebase *rebase)
{
	g_return_val_if_fail (GGIT_IS_REBASE (rebase), 0);

	return git_rebase_operation_entrycount (_ggit_native_get (rebase));
}

/**
 * ggit_rebase_get_operation_index:
 * @rebase: a #GgitRebase.
 *
 * Gets the index of the rebase operation that is currently being applied.
 * If the first operation has not yet been applied (because you have
 * called ggit_repository_rebase_init() but not yet ggit_rebase_next())
 * then this returns @GGIT_REBASE_NO_OPERATION.
 *
 * Returns: The index of the rebase operation currently being applied.
 */
gsize
ggit_rebase_get_operation_index (GgitRebase *rebase)
{
	g_return_val_if_fail (GGIT_IS_REBASE (rebase), 0);

	return git_rebase_operation_current (_ggit_native_get (rebase));
}

/**
 * ggit_rebase_get_operation_by_index:
 * @rebase: a #GgitRebase.
 * @idx: The index of the rebase operation to retrieve.
 *
 * Gets the rebase operation specified by @idx.
 *
 * Returns: (transfer full) (nullable): The rebase operation or %NULL if @idx was out of bounds.
 */
GgitRebaseOperation *
ggit_rebase_get_operation_by_index (GgitRebase *rebase,
                                    gsize       idx)
{
	git_rebase_operation *operation;

	g_return_val_if_fail (GGIT_IS_REBASE (rebase), NULL);

	operation = git_rebase_operation_byindex (_ggit_native_get (rebase), idx);

	return operation != NULL ? _ggit_rebase_operation_wrap (operation) : NULL;
}

/**
 * ggit_rebase_next:
 * @rebase: a #GgitRebase.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Performs the next rebase operation and returns the information about it.
 * If the operation is one that applies a patch (which is any operation except
 * @GGIT_REBASE_OPERATION_EXEC) then the patch will be applied and the index and
 * working directory will be updated with the changes. If there are conflicts,
 * you will need to address those before committing the changes.
 *
 * Returns: (transfer full) (nullable): the rebase operation that is to be performed next or %NULL.
 */
GgitRebaseOperation *
ggit_rebase_next (GgitRebase  *rebase,
                  GError     **error)
{
	git_rebase_operation *operation;
	gint ret;

	g_return_val_if_fail (GGIT_IS_REBASE (rebase), NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	ret = git_rebase_next (&operation,
	                       _ggit_native_get (rebase));

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return NULL;
	}

	return _ggit_rebase_operation_wrap (operation);
}

/**
 * ggit_rebase_commit:
 * @rebase: a #GgitRebase.
 * @author: (allow-none): a #GgitSignature or %NULL to keep the author from
 * the original commit.
 * @committer: a #GgitSignature.
 * @message: (allow-none): the message for this commit, or %NULL to use
 * the message from the original commit.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Commits the current patch.  You must have resolved any conflicts that
 * were introduced during the patch application from the ggit_rebase_next()
 * invocation.
 *
 * Returns: (transfer full) (nullable): a #GgitOId of the newly created commit or %NULL.
 */
GgitOId *
ggit_rebase_commit (GgitRebase     *rebase,
                    GgitSignature  *author,
                    GgitSignature  *committer,
                    const gchar    *message,
                    GError        **error)
{
	git_oid oid;
	gint ret;

	g_return_val_if_fail (GGIT_IS_REBASE (rebase), NULL);
	g_return_val_if_fail (GGIT_IS_SIGNATURE (author) || author == NULL, NULL);
	g_return_val_if_fail (GGIT_IS_SIGNATURE (committer), NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	ret = git_rebase_commit (&oid,
	                         _ggit_native_get (rebase),
	                         author ? _ggit_native_get (author) : NULL,
	                         _ggit_native_get (committer),
	                         NULL,
	                         message);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return NULL;
	}

	return _ggit_oid_wrap (&oid);
}

/**
 * ggit_rebase_abort:
 * @rebase: a #GgitRebase.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Aborts a rebase that is currently in progress, resetting the repository
 * and working directory to their state before rebase began.
 */
void
ggit_rebase_abort (GgitRebase  *rebase,
                   GError     **error)
{
	gint ret;

	g_return_if_fail (GGIT_IS_REBASE (rebase));
	g_return_if_fail (error == NULL || *error == NULL);

	ret = git_rebase_abort (_ggit_native_get (rebase));

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
	}
}

/**
 * ggit_rebase_finish:
 * @rebase: a #GgitRebase.
 * @signature: (allow-none): the identity that is finishing the rebase or %NULL.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Finishes a rebase that is currently in progress once all patches have
 * been applied.
 */
void
ggit_rebase_finish (GgitRebase     *rebase,
                    GgitSignature  *signature,
                    GError        **error)
{
	gint ret;

	g_return_if_fail (GGIT_IS_REBASE (rebase));
	g_return_if_fail (GGIT_IS_SIGNATURE (signature) || signature == NULL);
	g_return_if_fail (error == NULL || *error == NULL);

	ret = git_rebase_finish (_ggit_native_get (rebase),
	                         signature != NULL ? _ggit_native_get (signature) : NULL);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
	}
}

/* ex:set ts=8 noet: */
