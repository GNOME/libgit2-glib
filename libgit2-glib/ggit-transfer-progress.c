/*
 * ggit-transfer-progress.c
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2013 - Ignacio Casal Quinteiro
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

#include "ggit-transfer-progress.h"
#include <git2.h>
#include <string.h>

struct _GgitTransferProgress
{
	git_transfer_progress progress;
};

G_DEFINE_BOXED_TYPE (GgitTransferProgress, ggit_transfer_progress,
                     ggit_transfer_progress_copy,
                     ggit_transfer_progress_free)

GgitTransferProgress *
_ggit_transfer_progress_wrap (const git_transfer_progress *progress)
{
	GgitTransferProgress *gprogress;

	g_return_val_if_fail (progress != NULL, NULL);

	gprogress = g_slice_new (GgitTransferProgress);
	memcpy (&gprogress->progress, progress, sizeof (git_transfer_progress));

	return gprogress;
}

/**
 * ggit_transfer_progress_copy:
 * @progress: a #GgitTransferProgress.
 *
 * Copies @progress into a newly allocated #GgitTransferProgress.
 *
 * Returns: (transfer full) (allow-none): a newly allocated #GgitTransferProgress or %NULL.
 */
GgitTransferProgress *
ggit_transfer_progress_copy (GgitTransferProgress *progress)
{
	g_return_val_if_fail (progress != NULL, NULL);

	return _ggit_transfer_progress_wrap (&progress->progress);
}

/**
 * ggit_transfer_progress_free:
 * @progress: a #GgitTransferProgress.
 *
 * Frees @progress.
 */
void
ggit_transfer_progress_free (GgitTransferProgress *progress)
{
	g_return_if_fail (progress != NULL);

	g_slice_free (GgitTransferProgress, progress);
}

/**
 * ggit_transfer_progress_get_total_objects:
 * @progress: a #GgitTransferProgress.
 *
 * Gets the total objects of the transfer.
 *
 * Returns: the total objects of the transfer.
 */
guint
ggit_transfer_progress_get_total_objects (GgitTransferProgress *progress)
{
	g_return_val_if_fail (progress != NULL, 0);

	return progress->progress.total_objects;
}

/**
 * ggit_transfer_progress_get_indexed_objects:
 * @progress: a #GgitTransferProgress.
 *
 * Gets the indexed objects of the transfer.
 *
 * Returns: the indexed objects of the transfer.
 */
guint
ggit_transfer_progress_get_indexed_objects (GgitTransferProgress *progress)
{
	g_return_val_if_fail (progress != NULL, 0);

	return progress->progress.indexed_objects;
}

/**
 * ggit_transfer_progress_get_received_objects:
 * @progress: a #GgitTransferProgress.
 *
 * Gets the received objects of the transfer.
 *
 * Returns: the received objects of the transfer.
 */
guint
ggit_transfer_progress_get_received_objects (GgitTransferProgress *progress)
{
	g_return_val_if_fail (progress != NULL, 0);

	return progress->progress.received_objects;
}

/**
 * ggit_transfer_progress_get_received_bytes:
 * @progress: a #GgitTransferProgress.
 *
 * Gets the received bytes of the transfer.
 *
 * Returns: the received bytes of the transfer.
 */
gsize
ggit_transfer_progress_get_received_bytes (GgitTransferProgress *progress)
{
	g_return_val_if_fail (progress != NULL, 0);

	return progress->progress.received_bytes;
}

/* ex:set ts=8 noet: */
