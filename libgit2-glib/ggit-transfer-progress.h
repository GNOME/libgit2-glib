/*
 * ggit-transfer-progress.h
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

#ifndef __GGIT_TRANSFER_PROGRESS_H__
#define __GGIT_TRANSFER_PROGRESS_H__

#include <glib-object.h>
#include <git2.h>

#include "ggit-types.h"

G_BEGIN_DECLS

#define GGIT_TYPE_TRANSFER_PROGRESS       (ggit_transfer_progress_get_type ())
#define GGIT_TRANSFER_PROGRESS(obj)       ((GgitTransferProgress *)obj)

GType                   ggit_transfer_progress_get_type             (void) G_GNUC_CONST;

GgitTransferProgress  *_ggit_transfer_progress_wrap                 (const git_transfer_progress *progress);

GgitTransferProgress   *ggit_transfer_progress_copy                 (GgitTransferProgress     *progress);
void                    ggit_transfer_progress_free                 (GgitTransferProgress     *progress);

guint                   ggit_transfer_progress_get_total_objects    (GgitTransferProgress     *progress);

guint                   ggit_transfer_progress_get_indexed_objects  (GgitTransferProgress     *progress);

guint                   ggit_transfer_progress_get_received_objects (GgitTransferProgress     *progress);

gsize                   ggit_transfer_progress_get_received_bytes   (GgitTransferProgress     *progress);

G_DEFINE_AUTOPTR_CLEANUP_FUNC (GgitTransferProgress, ggit_transfer_progress_free)

G_END_DECLS

#endif /* __GGIT_TRANSFER_PROGRESS_H__ */

/* ex:set ts=8 noet: */
