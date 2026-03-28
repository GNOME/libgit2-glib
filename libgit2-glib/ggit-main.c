/*
 * ggit-main.c
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

#include <git2.h>

#include "ggit-main.h"

/**
 * ggit_get_features:
 *
 * Gets the features of the library.
 *
 * Returns: a #GgitFeatureFlags.
 */
GgitFeatureFlags
ggit_get_features (void)
{
	return git_libgit2_features ();
}

/**
 * ggit_init:
 *
 * Call this function before using any other libgit2-glib function.
 *
 * It is safe to call this function as many times as you need it but each call
 * must be followed by a corresponding call to @ggit_shutdown.
 */
void
ggit_init (void)
{
	git_libgit2_init ();
}

/**
 * ggit_shutdown:
 *
 * Call this function to reduce the ref-count incremented by a call to
 * @ggit_init.  Will free all shared resources when the count returns to zero.
 */
void
ggit_shutdown (void)
{
	git_libgit2_shutdown ();
}


/* ex:set ts=8 noet: */
