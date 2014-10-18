/*
 * ggit-note.h
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2014 - Ignacio Casal Quinteiro
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

#ifndef __GGIT_NOTE_H__
#define __GGIT_NOTE_H__

#include <glib-object.h>
#include <git2.h>

#include "ggit-types.h"

G_BEGIN_DECLS

#define GGIT_TYPE_NOTE       (ggit_note_get_type ())
#define GGIT_NOTE(obj)       ((GgitNote *)obj)

GType             ggit_note_get_type           (void) G_GNUC_CONST;

GgitNote        *_ggit_note_wrap               (git_note       *note);

GgitNote         *ggit_note_ref                (GgitNote       *note);
void              ggit_note_unref              (GgitNote       *note);

const gchar      *ggit_note_get_message        (GgitNote       *note);

GgitOId          *ggit_note_get_id             (GgitNote       *note);

G_END_DECLS

#endif /* __GGIT_NOTE_H__ */

/* ex:set ts=8 noet: */
