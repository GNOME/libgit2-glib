/*
 * ggit-push-progress.h
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

#ifndef __GGIT_PUSH_PROGRESS_H__
#define __GGIT_PUSH_PROGRESS_H__

#include <glib-object.h>
#include <libgit2-glib/ggit-types.h>

G_BEGIN_DECLS

/*
 * Type checking and casting macros
 */
#define GGIT_TYPE_PUSH_PROGRESS		(ggit_push_progress_get_type ())
#define GGIT_PUSH_PROGRESS(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), GGIT_TYPE_PUSH_PROGRESS, GgitPushProgress))
#define GGIT_PUSH_PROGRESS_IFACE(obj)	(G_TYPE_CHECK_CLASS_CAST ((obj), GGIT_TYPE_PUSH_PROGRESS, GgitPushProgressInterface))
#define GGIT_IS_PUSH_PROGRESS(obj)	(G_TYPE_CHECK_INSTANCE_TYPE ((obj), GGIT_TYPE_PUSH_PROGRESS))
#define GGIT_PUSH_PROGRESS_GET_IFACE(obj)	(G_TYPE_INSTANCE_GET_INTERFACE ((obj), GGIT_TYPE_PUSH_PROGRESS, GgitPushProgressInterface))

typedef struct _GgitPushProgress           GgitPushProgress; /* dummy typedef */
typedef struct _GgitPushProgressInterface  GgitPushProgressInterface;

struct _GgitPushProgressInterface
{
	GTypeInterface g_iface;

	/* Virtual public methods */
	gboolean (*packbuilder_progress) (GgitPushProgress      *push_progress,
	                                  GgitPackbuilderStage   stage,
	                                  guint                  current,
	                                  guint                  total,
	                                  GError               **error);

	gboolean (*transfer_progress)    (GgitPushProgress      *push_progress,
	                                  guint                  current,
	                                  guint                  total,
	                                  gsize                  bytes,
	                                  GError               **error);
};

/*
 * Public methods
 */
GType	 ggit_push_progress_get_type	(void)  G_GNUC_CONST;

gboolean ggit_push_progress_packbuilder_progress (GgitPushProgress      *push_progress,
                                                  GgitPackbuilderStage   stage,
                                                  guint                  current,
                                                  guint                  total,
                                                  GError               **error);

gboolean ggit_push_progress_transfer_progress    (GgitPushProgress      *push_progress,
                                                  guint                  current,
                                                  guint                  total,
                                                  gsize                  bytes,
                                                  GError               **error);

G_END_DECLS

#endif /* __GGIT_PUSH_PROGRESS_H__ */

/* ex:set ts=8 noet: */
