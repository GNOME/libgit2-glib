/*
 * ggit-fetch-options.h
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


#ifndef __GGIT_FETCH_OPTIONS_H__
#define __GGIT_FETCH_OPTIONS_H__

#include <glib-object.h>
#include <git2.h>
#include <libgit2-glib/ggit-remote-callbacks.h>

G_BEGIN_DECLS

#define GGIT_TYPE_FETCH_OPTIONS       (ggit_fetch_options_get_type ())
#define GGIT_FETCH_OPTIONS(obj)       ((GgitFetchOptions *)obj)

GType                      ggit_fetch_options_get_type            (void) G_GNUC_CONST;

const git_fetch_options  *_ggit_fetch_options_get_fetch_options   (GgitFetchOptions        *fetch_options);

GgitFetchOptions          *ggit_fetch_options_copy                (GgitFetchOptions        *fetch_options);
void                       ggit_fetch_options_free                (GgitFetchOptions        *fetch_options);

GgitFetchOptions          *ggit_fetch_options_new                 (void);

GgitRemoteCallbacks       *ggit_fetch_options_get_remote_callbacks (GgitFetchOptions       *options);
void                       ggit_fetch_options_set_remote_callbacks (GgitFetchOptions       *options,
                                                                    GgitRemoteCallbacks    *callbacks);

GgitRemoteDownloadTagsType ggit_fetch_options_get_download_tags    (GgitFetchOptions           *options);
void                       ggit_fetch_options_set_download_tags    (GgitFetchOptions           *options,
                                                                    GgitRemoteDownloadTagsType  download_tags);

G_END_DECLS

#endif /* __GGIT_FETCH_OPTIONS_H__ */

/* ex:set ts=8 noet: */
