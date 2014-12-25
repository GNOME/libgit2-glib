/*
 * ggit-push-options.h
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

#ifndef __GGIT_PUSH_OPTIONS_H__
#define __GGIT_PUSH_OPTIONS_H__

#include <glib-object.h>
#include <git2.h>

#include "ggit-types.h"

G_BEGIN_DECLS

#define GGIT_TYPE_PUSH_OPTIONS			(ggit_push_options_get_type ())
#define GGIT_PUSH_OPTIONS(obj)			(G_TYPE_CHECK_INSTANCE_CAST ((obj), GGIT_TYPE_PUSH_OPTIONS, GgitPushOptions))
#define GGIT_PUSH_OPTIONS_CONST(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), GGIT_TYPE_PUSH_OPTIONS, GgitPushOptions const))
#define GGIT_PUSH_OPTIONS_CLASS(klass)		(G_TYPE_CHECK_CLASS_CAST ((klass), GGIT_TYPE_PUSH_OPTIONS, GgitPushOptionsClass))
#define GGIT_IS_PUSH_OPTIONS(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), GGIT_TYPE_PUSH_OPTIONS))
#define GGIT_IS_PUSH_OPTIONS_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), GGIT_TYPE_PUSH_OPTIONS))
#define GGIT_PUSH_OPTIONS_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS ((obj), GGIT_TYPE_PUSH_OPTIONS, GgitPushOptionsClass))

typedef struct _GgitPushOptionsClass	GgitPushOptionsClass;
typedef struct _GgitPushOptionsPrivate	GgitPushOptionsPrivate;

struct _GgitPushOptions
{
	GObject parent;

	GgitPushOptionsPrivate *priv;
};

struct _GgitPushOptionsClass
{
	GObjectClass parent_class;
};

GType                  ggit_push_options_get_type (void) G_GNUC_CONST;

const git_push_options *
                      _ggit_push_options_get_push_options (
                                                          GgitPushOptions *options);

GgitPushOptions       *ggit_push_options_new              (void);

gint                   ggit_push_options_get_parallelism (GgitPushOptions *options);
void                   ggit_push_options_set_parallelism (GgitPushOptions *options,
                                                          gint             parallelism);


G_END_DECLS

#endif /* __GGIT_PUSH_OPTIONS_H__ */

/* ex:set ts=8 noet: */
