/*
 * ggit-status-options.h
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


#ifndef __GGIT_STATUS_OPTIONS_H__
#define __GGIT_STATUS_OPTIONS_H__

#include <glib-object.h>
#include <git2/strarray.h>
#include <git2/status.h>

#include "ggit-types.h"

G_BEGIN_DECLS

#define GGIT_TYPE_STATUS_OPTIONS       (ggit_status_options_get_type ())
#define GGIT_STATUS_OPTIONS(obj)       ((GgitStatusOptions *)obj)

GType              ggit_status_options_get_type           (void) G_GNUC_CONST;

const git_status_options *
                  _ggit_status_options_get_status_options (GgitStatusOptions        *status_options);

GgitStatusOptions *ggit_status_options_copy               (GgitStatusOptions        *status_options);
void               ggit_status_options_free               (GgitStatusOptions        *status_options);

GgitStatusOptions *ggit_status_options_new                (GgitStatusFlag            flags,
                                                           GgitStatusShow            show,
                                                           const gchar             **pathspec);

G_END_DECLS

#endif /* __GGIT_STATUS_OPTIONS_H__ */

/* ex:set ts=8 noet: */
