/*
 * ggit-ref-spec.h
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2012 - Garrett Regier
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

#ifndef __GGIT_REF_SPEC_H__
#define __GGIT_REF_SPEC_H__

#include <git2.h>

#include "ggit-types.h"

G_BEGIN_DECLS

#define GGIT_TYPE_REF_SPEC       (ggit_ref_spec_get_type ())
#define GGIT_REF_SPEC(obj)       ((GgitRefSpec *)obj)

GType           ggit_ref_spec_get_type         (void) G_GNUC_CONST;

GgitRefSpec    *_ggit_ref_spec_wrap            (const git_refspec *refspec);

GgitRefSpec    *ggit_ref_spec_ref              (GgitRefSpec       *refspec);
void            ggit_ref_spec_unref            (GgitRefSpec       *refspec);

const gchar    *ggit_ref_spec_get_source       (GgitRefSpec       *refspec);
const gchar    *ggit_ref_spec_get_destination  (GgitRefSpec       *refspec);
gboolean        ggit_ref_spec_is_forced        (GgitRefSpec       *refspec);

G_END_DECLS

#endif /* __GGIT_REF_SPEC_H__ */

/* ex:set ts=8 noet: */
