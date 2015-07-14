/*
 * ggit-ref.h
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

#ifndef __GGIT_REF_H__
#define __GGIT_REF_H__

#include <glib-object.h>
#include <git2.h>
#include <libgit2-glib/ggit-oid.h>
#include <libgit2-glib/ggit-native.h>
#include <libgit2-glib/ggit-object.h>

G_BEGIN_DECLS

#define GGIT_TYPE_REF (ggit_ref_get_type ())
G_DECLARE_DERIVABLE_TYPE (GgitRef, ggit_ref, GGIT, REF, GgitNative)

/**
 * GgitRefClass:
 * @parent_class: The parent class.
 *
 * The class structure for #GgitRefClass.
 */
struct _GgitRefClass
{
	/*< private >*/
	GgitNativeClass parent_class;
};

GgitRef        *_ggit_ref_wrap              (git_reference  *ref,
                                             gboolean        owned);

gboolean        ggit_ref_is_valid_name      (const gchar    *name);

GgitOId        *ggit_ref_get_target         (GgitRef        *ref);

const gchar    *ggit_ref_get_symbolic_target(GgitRef        *ref);

GgitRefType     ggit_ref_get_reference_type (GgitRef        *ref);

const gchar    *ggit_ref_get_name           (GgitRef        *ref);
const gchar    *ggit_ref_get_shorthand      (GgitRef        *ref);
const gchar    *ggit_ref_to_string          (GgitRef        *ref);

GgitRef        *ggit_ref_resolve            (GgitRef        *ref,
                                             GError        **error);

GgitRepository *ggit_ref_get_owner          (GgitRef        *ref);

GgitRef        *ggit_ref_set_symbolic_target(GgitRef        *ref,
                                             const gchar    *target,
                                             const gchar    *log_message,
                                             GError        **error);

GgitRef        *ggit_ref_set_target         (GgitRef        *ref,
                                             GgitOId        *oid,
                                             const gchar    *log_message,
                                             GError        **error);

GgitRef        *ggit_ref_rename             (GgitRef        *ref,
                                             const gchar    *new_name,
                                             gboolean        force,
                                             const gchar    *log_message,
                                             GError        **error);

void            ggit_ref_delete             (GgitRef        *ref,
                                             GError        **error);

GgitObject     *ggit_ref_lookup             (GgitRef        *ref,
                                             GError        **error);

gboolean        ggit_ref_has_log            (GgitRef        *ref);

GgitReflog     *ggit_ref_get_log            (GgitRef        *ref,
                                             GError        **error);

void            ggit_ref_delete_log         (GgitRef        *ref,
                                             GError        **error);

gboolean        ggit_ref_is_tag             (GgitRef        *ref);
gboolean        ggit_ref_is_remote          (GgitRef        *ref);
gboolean        ggit_ref_is_branch          (GgitRef        *ref);
gboolean        ggit_ref_is_note            (GgitRef        *ref);

G_END_DECLS

#endif /* __GGIT_REF_H__ */

/* ex:set ts=8 noet: */
