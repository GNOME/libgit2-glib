/*
 * ggit-push-progress.c
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2014 - Jesse van den Kieboom
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

#include "ggit-push-progress.h"

G_DEFINE_INTERFACE (GgitPushProgress, ggit_push_progress, G_TYPE_OBJECT)

void
ggit_push_progress_default_init (GgitPushProgressInterface *iface)
{
}

gboolean
ggit_push_progress_packbuilder_progress (GgitPushProgress      *push_progress,
                                         GgitPackbuilderStage   stage,
                                         guint                  current,
                                         guint                  total,
                                         GError               **error)
{
	GgitPushProgressInterface *iface;

	g_return_val_if_fail (GGIT_IS_PUSH_PROGRESS (push_progress), TRUE);
	g_return_val_if_fail (error == NULL || *error == NULL, TRUE);

	iface = GGIT_PUSH_PROGRESS_GET_IFACE (push_progress);

	if (iface->packbuilder_progress != NULL)
	{
		return iface->packbuilder_progress (push_progress,
		                                    stage,
		                                    current,
		                                    total,
		                                    error);
	}

	return TRUE;
}

gboolean
ggit_push_progress_transfer_progress (GgitPushProgress  *push_progress,
                                      guint              current,
                                      guint              total,
                                      gsize              bytes,
                                      GError           **error)
{
	GgitPushProgressInterface *iface;

	g_return_val_if_fail (GGIT_IS_PUSH_PROGRESS (push_progress), TRUE);
	g_return_val_if_fail (error == NULL || *error == NULL, TRUE);

	iface = GGIT_PUSH_PROGRESS_GET_IFACE (push_progress);

	if (iface->transfer_progress != NULL)
	{
		return iface->transfer_progress (push_progress,
		                                 current,
		                                 total,
		                                 bytes,
		                                 error);
	}

	return TRUE;
}

/* ex:set ts=8 noet: */
