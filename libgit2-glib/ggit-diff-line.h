/*
 * ggit-diff-line.h
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

#ifndef __GGIT_DIFF_LINE_H__
#define __GGIT_DIFF_LINE_H__

#include <glib-object.h>
#include <git2.h>
#include "ggit-types.h"

G_BEGIN_DECLS

#define GGIT_TYPE_DIFF_LINE       (ggit_diff_line_get_type ())
#define GGIT_DIFF_LINE(obj)       ((GgitDiffLine *)obj)

GType             ggit_diff_line_get_type           (void) G_GNUC_CONST;

GgitDiffLine     *_ggit_diff_line_wrap              (const git_diff_line *line,
                                                     const gchar         *encoding);

GgitDiffLine     *ggit_diff_line_ref                (GgitDiffLine        *line);
void              ggit_diff_line_unref              (GgitDiffLine        *line);

GgitDiffLineType  ggit_diff_line_get_origin         (GgitDiffLine        *line);
gint              ggit_diff_line_get_old_lineno     (GgitDiffLine        *line);
gint              ggit_diff_line_get_new_lineno     (GgitDiffLine        *line);
gint64            ggit_diff_line_get_content_offset (GgitDiffLine        *line);

const guint8     *ggit_diff_line_get_content        (GgitDiffLine        *line,
                                                     gsize               *length);

const gchar      *ggit_diff_line_get_text           (GgitDiffLine        *line);

G_DEFINE_AUTOPTR_CLEANUP_FUNC (GgitDiffLine, ggit_diff_line_unref)

G_END_DECLS

#endif /* __GGIT_DIFF_LINE_H__ */

/* ex:set ts=8 noet: */
