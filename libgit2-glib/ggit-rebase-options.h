/*
 * ggit-rebase-options.h
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2015 - Ignacio Casal Quinteiro
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


#ifndef __GGIT_REBASE_OPTIONS_H__
#define __GGIT_REBASE_OPTIONS_H__

#include <glib-object.h>
#include <git2.h>

#include "ggit-types.h"
#include "ggit-checkout-options.h"

G_BEGIN_DECLS

#define GGIT_TYPE_REBASE_OPTIONS       (ggit_rebase_options_get_type ())
#define GGIT_REBASE_OPTIONS(obj)       ((GgitRebaseOptions *)obj)

GType                   ggit_rebase_options_get_type                (void) G_GNUC_CONST;

const git_rebase_options *
                       _ggit_rebase_options_get_rebase_options      (GgitRebaseOptions         *rebase_options);

GgitRebaseOptions      *ggit_rebase_options_copy                    (GgitRebaseOptions         *rebase_options);
void                    ggit_rebase_options_free                    (GgitRebaseOptions         *rebase_options);

GgitRebaseOptions      *ggit_rebase_options_new                     (void);

gboolean                ggit_rebase_options_get_quiet               (GgitRebaseOptions         *rebase_options);
void                    ggit_rebase_options_set_quiet               (GgitRebaseOptions         *rebase_options,
                                                                     gboolean                   quiet);

const gchar            *ggit_rebase_options_get_rewrite_notes_ref   (GgitRebaseOptions         *rebase_options);
void                    ggit_rebase_options_set_rewrite_notes_ref   (GgitRebaseOptions         *rebase_options,
                                                                     const gchar               *rewrite_notes_ref);

GgitCheckoutOptions    *ggit_rebase_options_get_checkout_options    (GgitRebaseOptions         *rebase_options);
void                    ggit_rebase_options_set_checkout_options    (GgitRebaseOptions         *rebase_options,
                                                                     GgitCheckoutOptions       *checkout_options);

G_DEFINE_AUTOPTR_CLEANUP_FUNC (GgitRebaseOptions, ggit_rebase_options_free)

G_END_DECLS

#endif /* __GGIT_REBASE_OPTIONS_H__ */

/* ex:set ts=8 noet: */
