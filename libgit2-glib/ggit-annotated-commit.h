/*
 * ggit-annotated-commit.h
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


#ifndef __GGIT_ANNOTATED_COMMIT_H__
#define __GGIT_ANNOTATED_COMMIT_H__

#include <glib-object.h>
#include <git2.h>

#include "ggit-types.h"

G_BEGIN_DECLS

#define GGIT_TYPE_ANNOTATED_COMMIT       (ggit_annotated_commit_get_type ())
#define GGIT_ANNOTATED_COMMIT(obj)       ((GgitAnnotatedCommit *)obj)

GType                        ggit_annotated_commit_get_type                    (void) G_GNUC_CONST;

GgitAnnotatedCommit        *_ggit_annotated_commit_wrap                        (git_annotated_commit *annotated_commit);

git_annotated_commit       *_ggit_annotated_commit_get_annotated_commit        (GgitAnnotatedCommit  *annotated_commit);

GgitAnnotatedCommit         *ggit_annotated_commit_ref                         (GgitAnnotatedCommit  *annotated_commit);
void                         ggit_annotated_commit_unref                       (GgitAnnotatedCommit  *annotated_commit);

GgitOId                     *ggit_annotated_commit_get_id                      (GgitAnnotatedCommit  *annotated_commit);

G_DEFINE_AUTOPTR_CLEANUP_FUNC (GgitAnnotatedCommit, ggit_annotated_commit_unref)

G_END_DECLS

#endif /* __GGIT_ANNOTATED_COMMIT_H__ */

/* ex:set ts=8 noet: */
