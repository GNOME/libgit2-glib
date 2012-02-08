/*
 * ggit-tag.h
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2011 - Ignacio Casal Quinteiro
 *
 * libgit2-glib is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * libgit2-glib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libgit2-glib; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA  02110-1301  USA
 */


#ifndef __GGIT_TAG_H__
#define __GGIT_TAG_H__

#include <glib-object.h>
#include <git2/tag.h>

#include "ggit-object.h"
#include "ggit-types.h"

G_BEGIN_DECLS

#define GGIT_TYPE_TAG			(ggit_tag_get_type ())
#define GGIT_TAG(obj)			(G_TYPE_CHECK_INSTANCE_CAST ((obj), GGIT_TYPE_TAG, GgitTag))
#define GGIT_TAG_CONST(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), GGIT_TYPE_TAG, GgitTag const))
#define GGIT_TAG_CLASS(klass)		(G_TYPE_CHECK_CLASS_CAST ((klass), GGIT_TYPE_TAG, GgitTagClass))
#define GGIT_IS_TAG(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), GGIT_TYPE_TAG))
#define GGIT_IS_TAG_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), GGIT_TYPE_TAG))
#define GGIT_TAG_GET_CLASS(obj)		(G_TYPE_INSTANCE_GET_CLASS ((obj), GGIT_TYPE_TAG, GgitTagClass))

typedef struct _GgitTagClass	GgitTagClass;

struct _GgitTag
{
	GgitObject parent;

	/* priv padding */
	gpointer _priv;
};

struct _GgitTagClass
{
	GgitObjectClass parent_class;
};

GType             ggit_tag_get_type             (void) G_GNUC_CONST;

GgitTag         *_ggit_tag_wrap                 (git_tag  *tag,
                                                 gboolean owned);

GgitObject       *ggit_tag_get_target           (GgitTag  *tag,
                                                 GError  **error);

GgitOId          *ggit_tag_get_target_id        (GgitTag  *tag);

const gchar      *ggit_tag_get_name             (GgitTag  *tag);

GgitSignature    *ggit_tag_get_tagger           (GgitTag *tag);

const gchar      *ggit_tag_get_message          (GgitTag  *tag);

G_END_DECLS

#endif /* __GGIT_TAG_H__ */

/* ex:set ts=8 noet: */
