/*
 * ggit-push-options.h
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2017 - Ignacio Casal Quinteiro
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

#ifndef __GGIT_PROXY_OPTIONS_H__
#define __GGIT_PROXY_OPTIONS_H__

#include <glib-object.h>
#include <git2.h>

#include "ggit-types.h"

G_BEGIN_DECLS

#define GGIT_TYPE_PROXY_OPTIONS (ggit_proxy_options_get_type ())
G_DECLARE_DERIVABLE_TYPE (GgitProxyOptions, ggit_proxy_options, GGIT, PROXY_OPTIONS, GObject)

struct _GgitProxyOptionsClass
{
	GObjectClass parent_class;
};

const git_proxy_options *
                       _ggit_proxy_options_get_proxy_options (GgitProxyOptions *options);

GgitProxyOptions       *ggit_proxy_options_new               (void);

G_END_DECLS

#endif /* __GGIT_PROXY_OPTIONS_H__ */

/* ex:set ts=8 noet: */
