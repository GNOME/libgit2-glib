/*
 * ggit-mailmap.h
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2021 - Liliana Marie Prikler
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

#ifndef __GGIT_MAILMAP_H__
#define __GGIT_MAILMAP_H__

#include <libgit2-glib/ggit-native.h>
#include <git2.h>
#include <libgit2-glib/ggit-repository.h>
#include <libgit2-glib/ggit-signature.h>
#include <libgit2-glib/ggit-types.h>

G_BEGIN_DECLS

#define GGIT_TYPE_MAILMAP (ggit_mailmap_get_type ())
G_DECLARE_FINAL_TYPE (GgitMailmap, ggit_mailmap, GGIT, MAILMAP, GgitNative)

GgitMailmap          *_ggit_mailmap_wrap                  (const git_mailmap   *mailmap,
                                                           gboolean             owned);

GgitMailmap          *ggit_mailmap_new                    (GError             **error);
GgitMailmap          *ggit_mailmap_new_from_repository    (GgitRepository      *repository,
                                                           GError             **error);

void                  ggit_mailmap_add_entry              (GgitMailmap         *mailmap,
                                                           const gchar         *real_name,
                                                           const gchar         *real_email,
                                                           const gchar         *replace_name,
                                                           const gchar         *replace_email,
                                                           GError             **error);

void                  ggit_mailmap_resolve                (GgitMailmap   *mailmap,
                                                           const gchar        **real_name,
                                                           const gchar        **real_email,
                                                           const gchar         *replace_name,
                                                           const gchar         *replace_email,
                                                           GError             **error);


GgitSignature        *ggit_mailmap_resolve_signature      (GgitMailmap   *mailmap,
                                                           GgitSignature *signature,
                                                           GError             **error);

G_END_DECLS

#endif /* __GGIT_MAILMAP_H__ */

/* ex:set ts=8 noet: */
