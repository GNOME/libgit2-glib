/*
 * git-glib-error.h
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

#ifndef __GIT_GLIB_ERROR_H__
#define __GIT_GLIB_ERROR_H__

#include <glib.h>

G_BEGIN_DECLS

/* NOTE: keep in sync with git2/errors.h */

enum {
	GIT_GLIB_ERROR_GIT_ERROR = -1,

	/** Input was not a properly formatted Git object id. */
	GIT_GLIB_ERROR_NOTOID = -2,

	/** Input does not exist in the scope searched. */
	GIT_GLIB_ERROR_NOTFOUND = -3,

	/** Not enough space available. */
	GIT_GLIB_ERROR_NOMEM = -4,

	/** Consult the OS error information. */
	GIT_GLIB_ERROR_OSERR = -5,

	/** The specified object is of invalid type */
	GIT_GLIB_ERROR_OBJTYPE = -6,

	/** The specified repository is invalid */
	GIT_GLIB_ERROR_NOTAREPO = -7,

	/** The object type is invalid or doesn't match */
	GIT_GLIB_ERROR_INVALIDTYPE = -8,

	/** The object cannot be written because it's missing internal data */
	GIT_GLIB_ERROR_MISSINGOBJDATA = -9,

	/** The packfile for the ODB is corrupted */
	GIT_GLIB_ERROR_PACKCORRUPTED = -10,

	/** Failed to acquire or release a file lock */
	GIT_GLIB_ERROR_FLOCKFAIL = -11,

	/** The Z library failed to inflate/deflate an object's data */
	GIT_GLIB_ERROR_ZLIB = -12,

	/** The queried object is currently busy */
	GIT_GLIB_ERROR_BUSY = -13,

	/** The index file is not backed up by an existing repository */
	GIT_GLIB_ERROR_BAREINDEX = -14,

	/** The name of the reference is not valid */
	GIT_GLIB_ERROR_INVALIDREFNAME = -15,

	/** The specified reference has its data corrupted */
	GIT_GLIB_ERROR_REFCORRUPTED  = -16,

	/** The specified symbolic reference is too deeply nested */
	GIT_GLIB_ERROR_TOONESTEDSYMREF = -17,

	/** The pack-refs file is either corrupted or its format is not currently supported */
	GIT_GLIB_ERROR_PACKEDREFSCORRUPTED = -18,

	/** The path is invalid */
	GIT_GLIB_ERROR_INVALIDPATH = -19,

	/** The revision walker is empty; there are no more commits left to iterate */
	GIT_GLIB_ERROR_REVWALKOVER = -20,

	/** The state of the reference is not valid */
	GIT_GLIB_ERROR_INVALIDREFSTATE = -21,

	/** This feature has not been implemented yet */
	GIT_GLIB_ERROR_NOTIMPLEMENTED = -22,

	/** A reference with this name already exists */
	GIT_GLIB_ERROR_EXISTS = -23,

	/** The given integer literal is too large to be parsed */
	GIT_GLIB_ERROR_OVERFLOW = -24,

	/** The given literal is not a valid number */
	GIT_GLIB_ERROR_NOTNUM = -25,

	/** Streaming error */
	GIT_GLIB_ERROR_STREAM = -26,

	/** invalid arguments to function */
	GIT_GLIB_ERROR_INVALIDARGS = -27,

	/** The specified object has its data corrupted */
	GIT_GLIB_ERROR_OBJCORRUPTED = -28,

	/** The given short oid is ambiguous */
	GIT_GLIB_ERROR_AMBIGUOUSOIDPREFIX = -29,

	/** Skip and passthrough the given ODB backend */
	GIT_GLIB_ERROR_PASSTHROUGH = -30,

	/** The path pattern and string did not match */
	GIT_GLIB_ERROR_NOMATCH = -31,

	/**  The buffer is too short to satisfy the request */
	GIT_GLIB_ERROR_SHORTBUFFER = -32
};

#define GIT_GLIB_ERROR git_glib_error_quark ()

GQuark   git_glib_error_quark (void);

void    _git_glib_error_set   (GError **error,
                               gint     err);

G_END_DECLS

#endif /* __GIT_GLIB_ERROR_H__ */
