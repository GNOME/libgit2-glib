/*
 * ggit-blame-options.h
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2013 - Jesse van den Kieboom
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

#ifndef __GGIT_BLAME_OPTIONS_H__
#define __GGIT_BLAME_OPTIONS_H__

#include <glib-object.h>
#include <git2.h>

#include <libgit2-glib/ggit-types.h>

G_BEGIN_DECLS

#define GGIT_TYPE_BLAME_OPTIONS       (ggit_blame_options_get_type ())
#define GGIT_BLAME_OPTIONS(obj)       ((GgitBlameOptions *)obj)

GType              ggit_blame_options_get_type           (void) G_GNUC_CONST;

git_blame_options *
                  _ggit_blame_options_get_native        (GgitBlameOptions *blame_options);

GgitBlameOptions *ggit_blame_options_copy               (GgitBlameOptions *blame_options);
void              ggit_blame_options_free               (GgitBlameOptions *blame_options);

GgitBlameOptions *ggit_blame_options_new                (void);

GgitBlameFlags    ggit_blame_get_flags                  (GgitBlameOptions *blame_options);
void              ggit_blame_set_flags                  (GgitBlameOptions *blame_options,
                                                         GgitBlameFlags    flags);

GgitOId          *ggit_blame_options_get_newest_commit  (GgitBlameOptions *blame_options);
void              ggit_blame_options_set_newest_commit  (GgitBlameOptions *blame_options,
                                                         GgitOId          *oid);

GgitOId          *ggit_blame_options_get_oldest_commit  (GgitBlameOptions *blame_options);
void              ggit_blame_options_set_oldest_commit  (GgitBlameOptions *blame_options,
                                                         GgitOId          *oid);

guint32           ggit_blame_options_get_minimum_line   (GgitBlameOptions *blame_options);
void              ggit_blame_options_set_minimum_line   (GgitBlameOptions *blame_options,
                                                         guint32           line);

guint32           ggit_blame_options_get_maximum_line   (GgitBlameOptions *blame_options);
void              ggit_blame_options_set_maximum_line   (GgitBlameOptions *blame_options,
                                                         guint32           line);

guint16           ggit_blame_options_get_minimum_match_characters
                                                        (GgitBlameOptions *blame_options);

void              ggit_blame_options_set_minimum_match_characters
                                                        (GgitBlameOptions *blame_options,
                                                         guint16           characters);

G_DEFINE_AUTOPTR_CLEANUP_FUNC (GgitBlameOptions, ggit_blame_options_free)

G_END_DECLS

#endif /* __GGIT_BLAME_OPTIONS_H__ */

/* ex:set ts=8 noet: */
