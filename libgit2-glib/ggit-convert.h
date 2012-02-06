/*
 * ggit-convert.h
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2012 - Jesse van den Kieboom
 *
 * libgit2-glib is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * libgit2-glib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libgit2-glib. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __GGIT_CONVERT_H__
#define __GGIT_CONVERT_H__

#include <glib-object.h>

G_BEGIN_DECLS

gchar *ggit_convert_utf8 (const gchar *str,
                          gssize       size,
                          const gchar *from_charset);

G_END_DECLS

#endif /* __GGIT_CONVERT_H__ */

