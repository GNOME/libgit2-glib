/*
 * ggit-tag.h
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


#ifndef __GGIT_TAG_H__
#define __GGIT_TAG_H__

#include <glib-object.h>
#include <git2.h>

#include "ggit-object.h"
#include "ggit-types.h"

G_BEGIN_DECLS

#define GGIT_TYPE_TAG (ggit_tag_get_type ())
G_DECLARE_DERIVABLE_TYPE (GgitTag, ggit_tag, GGIT, TAG, GgitObject)

/**
 * GgitTagClass:
 * @parent_class: The parent class.
 *
 * The class structure for #GgitTagClass.
 */
struct _GgitTagClass
{
	/*< private >*/
	GgitObjectClass parent_class;
};

GgitTag         *_ggit_tag_wrap                 (git_tag  *tag,
                                                 gboolean owned);

GgitObject       *ggit_tag_get_target           (GgitTag  *tag,
                                                 GError  **error);

GgitOId          *ggit_tag_get_target_id        (GgitTag  *tag);

const gchar      *ggit_tag_get_name             (GgitTag  *tag);

GgitSignature    *ggit_tag_get_tagger           (GgitTag  *tag);

const gchar      *ggit_tag_get_message          (GgitTag  *tag);

GType             ggit_tag_get_target_type      (GgitTag  *tag);

GgitObject       *ggit_tag_peel                 (GgitTag  *tag,
                                                 GError  **error);

G_END_DECLS

#endif /* __GGIT_TAG_H__ */

/* ex:set ts=8 noet: */
