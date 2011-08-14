/*
 * ggit-ref.h
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


#ifndef __GGIT_REF_H__
#define __GGIT_REF_H__

#include <glib-object.h>
#include <git2/refs.h>

#include "ggit-types.h"

G_BEGIN_DECLS

#define GGIT_TYPE_REF		(ggit_ref_get_type ())
#define GGIT_REF(obj)		((GgitRef *)obj)
#define GGIT_REF_CONST(obj)	((GgitRef const *)obj)

GType            ggit_ref_get_type           (void) G_GNUC_CONST;

GgitRef        *_ggit_ref_wrap               (git_reference *ref);

GgitRef         *ggit_ref_copy               (GgitRef *ref);

void             ggit_ref_free               (GgitRef *ref);

GgitOId         *ggit_ref_get_oid            (GgitRef *ref);

const gchar     *ggit_ref_get_target         (GgitRef *ref);

GgitRefType      ggit_ref_get_reference_type (GgitRef *ref);

const gchar     *ggit_ref_get_name           (GgitRef *ref);

GgitRef         *ggit_ref_resolve            (GgitRef  *ref,
                                              GError  **error);

GgitRepository  *ggit_ref_get_owner          (GgitRef *ref);

void             ggit_ref_set_target         (GgitRef      *ref,
                                              const gchar  *target,
                                              GError      **error);

void             ggit_ref_set_oid            (GgitRef  *ref,
                                              GgitOId  *oid,
                                              GError  **error);

void             ggit_ref_rename             (GgitRef     *ref,
                                              const gchar *new_name,
                                              gboolean     force,
                                              GError     **error);

void             ggit_ref_delete             (GgitRef  *ref,
                                              GError  **error);

G_END_DECLS

#endif /* __GGIT_REF_H__ */
