#include "ggit-branch-enumerator.h"

struct _GgitBranchEnumerator
{
	git_branch_iterator *iterator;
	GgitRef *ref;
	gint ref_count;
};

G_DEFINE_BOXED_TYPE (GgitBranchEnumerator, ggit_branch_enumerator, ggit_branch_enumerator_ref, ggit_branch_enumerator_unref)

GgitBranchEnumerator *
_ggit_branch_enumerator_wrap (git_branch_iterator *iter)
{
	GgitBranchEnumerator *ret;

	g_return_val_if_fail (iter != NULL, NULL);

	ret = g_slice_new (GgitBranchEnumerator);
	ret->ref_count = 1;
	ret->iterator = iter;

	return ret;
}

/**
 * ggit_branch_enumerator_ref:
 * @enumerator: a #GgitBranchEnumerator.
 *
 * Returns: (transfer full): @enumerator
 */
GgitBranchEnumerator *
ggit_branch_enumerator_ref (GgitBranchEnumerator *enumerator)
{
	g_atomic_int_inc (&enumerator->ref_count);
	return enumerator;
}

/**
 * ggit_branch_enumerator_unref:
 * @enumerator: a #GgitBranchEnumerator.
 */
void
ggit_branch_enumerator_unref (GgitBranchEnumerator *enumerator)
{
	if (g_atomic_int_dec_and_test (&enumerator->ref_count))
	{
		g_clear_object (&enumerator->ref);

		git_branch_iterator_free (enumerator->iterator);
		g_slice_free (GgitBranchEnumerator, enumerator);
	}
}

/**
 * ggit_branch_enumerator_iterator:
 * @enumerator: a #GgitBranchEnumerator.
 *
 * Get an iterator for the enumerator.
 *
 * Returns: (transfer none) (allow-none): a #GgitBranchEnumerator.
 *
 **/
GgitBranchEnumerator *
ggit_branch_enumerator_iterator (GgitBranchEnumerator *enumerator)
{
	g_return_val_if_fail (enumerator != NULL, NULL);

	return enumerator;
}

/**
 * ggit_branch_enumerator_next:
 * @enumerator: a #GgitBranchEnumerator.
 *
 * Move the enumerator to the next branch.
 *
 * Returns: %TRUE if there is a next branch, %FALSE otherwise.
 */
gboolean
ggit_branch_enumerator_next (GgitBranchEnumerator *enumerator)
{
	git_reference *ref;
	git_branch_t branch_type;

	g_return_val_if_fail (enumerator != NULL, FALSE);

	g_clear_object (&enumerator->ref);

	if (git_branch_next (&ref, &branch_type, enumerator->iterator) != GIT_OK)
	{
		return FALSE;
	}

	if (branch_type == GIT_BRANCH_LOCAL)
	{
		enumerator->ref = GGIT_REF (_ggit_branch_wrap (ref));
	}
	else
	{
		enumerator->ref = _ggit_ref_wrap (ref, FALSE);
	}

	return TRUE;
}

/**
 * ggit_branch_enumerator_get:
 * @enumerator: a #GgitBranchEnumerator.
 *
 * Returns the currently being enumerated branch, or %NULL. Note that the
 * returned GgitRef will be of type GgitBranch if the ref represents a local
 * branch.
 *
 * Returns: (transfer full) (allow-none): the branch ref.
 */
GgitRef *
ggit_branch_enumerator_get (GgitBranchEnumerator *enumerator)
{
	g_return_val_if_fail (enumerator != NULL, NULL);

	if (enumerator->ref != NULL)
	{
		return g_object_ref (enumerator->ref);
	}

	return NULL;
}
