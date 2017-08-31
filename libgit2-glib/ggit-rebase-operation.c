/*
 * ggit-rebase-operation.c
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

#include "ggit-rebase-operation.h"
#include "ggit-oid.h"

struct _GgitRebaseOperation
{
	gint ref_count;

	git_rebase_operation *rebase_operation;
};

G_DEFINE_BOXED_TYPE (GgitRebaseOperation, ggit_rebase_operation,
                     ggit_rebase_operation_ref, ggit_rebase_operation_unref)

GgitRebaseOperation *
_ggit_rebase_operation_wrap (git_rebase_operation *rebase_operation)
{
	GgitRebaseOperation *glib_rebase_operation;

	glib_rebase_operation = g_slice_new (GgitRebaseOperation);
	glib_rebase_operation->ref_count = 1;
	glib_rebase_operation->rebase_operation = rebase_operation;

	return glib_rebase_operation;
}

git_rebase_operation *
_ggit_rebase_operation_get_rebase_operation (GgitRebaseOperation *rebase_operation)
{
	return rebase_operation->rebase_operation;
}

/**
 * ggit_rebase_operation_ref:
 * @rebase_operation: a #GgitRebaseOperation.
 *
 * Atomically increments the reference count of @rebase_operation by one.
 * This function is MT-safe and may be called from any thread.
 *
 * Returns: (transfer none) (nullable): a newly allocated #GgitRebaseOperation or %NULL.
 */
GgitRebaseOperation *
ggit_rebase_operation_ref (GgitRebaseOperation *rebase_operation)
{
	g_return_val_if_fail (rebase_operation != NULL, NULL);

	g_atomic_int_inc (&rebase_operation->ref_count);

	return rebase_operation;
}

/**
 * ggit_rebase_operation_unref:
 * @rebase_operation: a #GgitRebaseOperation.
 *
 * Atomically decrements the reference count of @rebase_operation by one.
 * If the reference count drops to 0, @rebase_operation is freed.
 */
void
ggit_rebase_operation_unref (GgitRebaseOperation *rebase_operation)
{
	g_return_if_fail (rebase_operation != NULL);

	if (g_atomic_int_dec_and_test (&rebase_operation->ref_count))
	{
		g_slice_free (GgitRebaseOperation, rebase_operation);
	}
}

/**
 * ggit_rebase_operation_get_operation_type:
 * @rebase_operation: a #GgitRebaseOperation.
 *
 * Gets the type of rebase operation.
 *
 * Returns: a #GgitRebaseOperationType.
 */
GgitRebaseOperationType
ggit_rebase_operation_get_operation_type (GgitRebaseOperation  *rebase_operation)
{
	g_return_val_if_fail (rebase_operation != NULL, 0);

	return (GgitRebaseOperationType)rebase_operation->rebase_operation->type;
}

/**
 * ggit_rebase_operation_get_id:
 * @rebase_operation: a #GgitRebaseOperation.
 *
 * Gets the commit ID being cherry-picked. This will be populated for
 * all operations except those of type @GGIT_REBASE_OPERATION_EXEC.
 *
 * Returns: (transfer full) (nullable): the commit ID being cherry-picked or %NULL.
 */
GgitOId *
ggit_rebase_operation_get_id (GgitRebaseOperation *rebase_operation)
{
	g_return_val_if_fail (rebase_operation != NULL, NULL);

	return _ggit_oid_wrap ((git_oid *)&rebase_operation->rebase_operation->id);
}

/**
 * ggit_rebase_operation_get_exec:
 * @rebase_operation: a #GgitRebaseOperation.
 *
 * Gets the executable the user has requested be run.  This will only
 * be populated for operations of type @GGIT_REBASE_OPERATION_EXEC.
 *
 * Returns: (transfer none) (nullable): the executable the user has requested be run or %NULL.
 */
const gchar *
ggit_rebase_operation_get_exec (GgitRebaseOperation  *rebase_operation)
{
	g_return_val_if_fail (rebase_operation != NULL, NULL);

	return rebase_operation->rebase_operation->exec;
}

/* ex:set ts=8 noet: */
