/*
 * ggit-cred.c
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2013 - Ignacio Casal Quinteiro
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


#include "ggit-cred.h"

// NOTE: this is a simple wrap around GgitNative to have a common point for git_cred types

G_DEFINE_TYPE (GgitCred, ggit_cred, GGIT_TYPE_NATIVE)

static void
ggit_cred_class_init (GgitCredClass *klass)
{
}

static void
ggit_cred_init (GgitCred *cred)
{
}

/* ex:set ts=8 noet: */
