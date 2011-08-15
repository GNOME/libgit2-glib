/*
 * ggit-error.h
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2011 - Ignacio Casal Quinteiro
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
 * along with libgit2-glib; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA  02110-1301  USA
 */

#ifndef __GGIT_ERROR_H__
#define __GGIT_ERROR_H__

#include <glib.h>

#include "ggit-types.h"

G_BEGIN_DECLS

#define GGIT_ERROR ggit_error_quark ()

GQuark   ggit_error_quark (void);

void    _ggit_error_set   (GError **error,
                           gint     err);

G_END_DECLS

#endif /* __GGIT_ERROR_H__ */

/* ex:set ts=8 noet: */
