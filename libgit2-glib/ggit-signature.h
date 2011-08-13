/*
 * ggit-signature.h
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


#ifndef __GGIT_SIGNATURE_H__
#define __GGIT_SIGNATURE_H__

#include <glib-object.h>
#include <git2/signature.h>

#include "ggit-types.h"

G_BEGIN_DECLS

#define GGIT_TYPE_SIGNATURE		(ggit_signature_get_type ())
#define GGIT_SIGNATURE(obj)		((GgitSignature *)obj)
#define GGIT_SIGNATURE_CONST(obj)	((GgitSignature const *)obj)

GType                 ggit_signature_get_type          (void) G_GNUC_CONST;

GgitSignature        *_ggit_signature_wrap             (git_signature *signature);

GgitSignature        *ggit_signature_new               (const gchar    *name,
                                                        const gchar    *email,
                                                        gint64          signature_time,
                                                        gint            signature_offset,
                                                        GError        **error);

GgitSignature        *ggit_signature_new_now           (const gchar    *name,
                                                        const gchar    *email,
                                                        GError        **error);

GgitSignature        *ggit_signature_copy              (GgitSignature  *signature);

void                  ggit_signature_free              (GgitSignature  *signature);

const gchar          *ggit_signature_get_name          (GgitSignature  *signature);

const gchar          *ggit_signature_get_email         (GgitSignature  *signature);

gint64                ggit_signature_get_time          (GgitSignature  *signature);

gint                  ggit_signature_get_time_offset   (GgitSignature  *signature);

G_END_DECLS

#endif /* __GGIT_SIGNATURE_H__ */
