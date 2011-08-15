/*
 * ggit-types.h
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2011 - Garrett Regier
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


#ifndef __GGIT_TYPES_H__
#define __GGIT_TYPES_H__

G_BEGIN_DECLS

/**
 * GgitBlob:
 *
 * Represents a blob object.
 */
typedef struct _GgitBlob GgitBlob;

/**
 * GgitCommit:
 *
 * Represents a commit object.
 */
typedef struct _GgitCommit GgitCommit;

/**
 * GgitObject:
 *
 * Represents a generic object in a repository.
 */
typedef struct _GgitObject GgitObject;

/**
 * GgitOId:
 *
 * Represents a unique ID of any object.
 */
typedef struct _GgitOId GgitOId;

/**
 * GgitRef:
 *
 * Reprensents a git reference.
 */
typedef struct _GgitRef	GgitRef;

/**
 * GgitRefType:
 * @GGIT_REF_INVALID: An invalid reference.
 * @GGIT_REF_OID: A reference which points at an object id.
 * @GGIT_REF_SYMBOLIC: A reference which points at another reference.
 * @GGIT_REF_PACKED:
 * @GGIT_REF_HAS_PEEL:
 * @GGIT_REF_LISTALL: All references types.
 */
typedef enum
{
	GGIT_REF_INVALID = 0,
	GGIT_REF_OID = 1,
	GGIT_REF_SYMBOLIC = 2,
	GGIT_REF_PACKED = 4,
	GGIT_REF_HAS_PEEL = 8,
	GGIT_REF_LISTALL = GGIT_REF_OID | GGIT_REF_SYMBOLIC | GGIT_REF_PACKED
} GgitRefType;

/**
 * GgitRepository:
 *
 * Represents an existing git repository including all of it's
 * object contents.
 */
typedef struct _GgitRepository GgitRepository;

/**
 * GgitSignature:
 *
 * Represents an action signature.
 */
typedef struct _GgitSignature GgitSignature;

/**
 * GgitTag:
 *
 * Represents a tag object.
 */
typedef struct _GgitTag GgitTag;

/* NOTE: keep in sync with git2/repository.h */
/**
 * GgitRepositoryPathid:
 * @GGIT_REPO_PATH: The path to the repository.
 * @GGIT_REPO_PATH_INDEX: The path to the index.
 * @GGIT_REPO_PATH_ODB: The path to the ODB.
 * @GGIT_REPO_PATH_WORKDIR: The path to the working directory.
 *
 * These identify the various path ids.
 */
typedef enum
{
	GGIT_REPO_PATH,
	GGIT_REPO_PATH_INDEX,
	GGIT_REPO_PATH_ODB,
	GGIT_REPO_PATH_WORKDIR
} GgitRepositoryPathid;

/* NOTE: keep in sync with git2/errors.h */
/**
 * GgitError:
 * @GGIT_ERROR_GIT_ERROR: A generic error for when an operation fails.
 * @GGIT_ERROR_NOTOID: Input was not a properly formatted Git object id.
 * @GGIT_ERROR_NOTFOUND: Input does not exist in the scope searched.
 * @GGIT_ERROR_NOMEM: Not enough space available.
 * @GGIT_ERROR_OSERR: Consult the OS error information.
 * @GGIT_ERROR_OBJTYPE: The specified object is of invalid type.
 * @GGIT_ERROR_NOTAREPO: The specified repository is invalid.
 * @GGIT_ERROR_INVALIDTYPE: The object type is invalid or doesn't match.
 * @GGIT_ERROR_MISSINGOBJDATA: The object cannot be written because it's
 *                             missing internal data.
 * @GGIT_ERROR_PACKCORRUPTED: The packfile for the ODB is corrupted.
 * @GGIT_ERROR_FLOCKFAIL: Failed to acquire or release a file lock.
 * @GGIT_ERROR_ZLIB: The Z library failed to inflate/deflate an object's data.
 * @GGIT_ERROR_BUSY: The queried object is currently busy.
 * @GGIT_ERROR_BAREINDEX: The index file is not backed up by an
 *                        existing repository.
 * @GGIT_ERROR_INVALIDREFNAME: The name of the reference is not valid.
 * @GGIT_ERROR_REFCORRUPTED: The specified reference has its data corrupted.
 * @GGIT_ERROR_TOONESTEDSYMREF: The specified symbolic reference is
 *                              too deeply nested.
 * @GGIT_ERROR_PACKEDREFSCORRUPTED: The pack-refs file is either corrupted or
 *                                  its format is not currently supported.
 * @GGIT_ERROR_INVALIDPATH: The path is invalid.
 * @GGIT_ERROR_REVWALKOVER: The revision walker is empty; there
 *                          are no more commits left to iterate.
 * @GGIT_ERROR_INVALIDREFSTATE: The state of the reference is not valid.
 * @GGIT_ERROR_NOTIMPLEMENTED: This feature has not been implemented yet.
 * @GGIT_ERROR_EXISTS: A reference with this name already exists.
 * @GGIT_ERROR_OVERFLOW: The given integer literal is too large to be parsed.
 * @GGIT_ERROR_NOTNUM: The given literal is not a valid number.
 * @GGIT_ERROR_STREAM: Streaming error.
 * @GGIT_ERROR_INVALIDARGS: invalid arguments to function.
 * @GGIT_ERROR_OBJCORRUPTED: The specified object has its data corrupted.
 * @GGIT_ERROR_AMBIGUOUSOIDPREFIX: The given short oid is ambiguous.
 * @GGIT_ERROR_PASSTHROUGH: Skip and passthrough the given ODB backend
 * @GGIT_ERROR_NOMATCH: The path pattern and string did not match.
 * @GGIT_ERROR_SHORTBUFFER: The buffer is too short to satisfy the request.
 *
 * Error codes for the %GGIT_ERROR error domain.
 */
typedef enum {
	GGIT_ERROR_GIT_ERROR = -1,
	GGIT_ERROR_NOTOID = -2,
	GGIT_ERROR_NOTFOUND = -3,
	GGIT_ERROR_NOMEM = -4,
	GGIT_ERROR_OSERR = -5,
	GGIT_ERROR_OBJTYPE = -6,
	GGIT_ERROR_NOTAREPO = -7,
	GGIT_ERROR_INVALIDTYPE = -8,
	GGIT_ERROR_MISSINGOBJDATA = -9,
	GGIT_ERROR_PACKCORRUPTED = -10,
	GGIT_ERROR_FLOCKFAIL = -11,
	GGIT_ERROR_ZLIB = -12,
	GGIT_ERROR_BUSY = -13,
	GGIT_ERROR_BAREINDEX = -14,
	GGIT_ERROR_INVALIDREFNAME = -15,
	GGIT_ERROR_REFCORRUPTED  = -16,
	GGIT_ERROR_TOONESTEDSYMREF = -17,
	GGIT_ERROR_PACKEDREFSCORRUPTED = -18,
	GGIT_ERROR_INVALIDPATH = -19,
	GGIT_ERROR_REVWALKOVER = -20,
	GGIT_ERROR_INVALIDREFSTATE = -21,
	GGIT_ERROR_NOTIMPLEMENTED = -22,
	GGIT_ERROR_EXISTS = -23,
	GGIT_ERROR_OVERFLOW = -24,
	GGIT_ERROR_NOTNUM = -25,
	GGIT_ERROR_STREAM = -26,
	GGIT_ERROR_INVALIDARGS = -27,
	GGIT_ERROR_OBJCORRUPTED = -28,
	GGIT_ERROR_AMBIGUOUSOIDPREFIX = -29,
	GGIT_ERROR_PASSTHROUGH = -30,
	GGIT_ERROR_NOMATCH = -31,
	GGIT_ERROR_SHORTBUFFER = -32
} GgitError;

G_END_DECLS

#endif /* __GGIT_TYPES_H__ */

/* ex:set ts=8 noet: */
