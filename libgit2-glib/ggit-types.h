/*
 * ggit-types.h
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2011 - Garrett Regier
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
 * GgitTree:
 *
 * Represents a tree object.
 */
typedef struct _GgitTree GgitTree;

/**
 * GitgIndex:
 *
 * Represents an index object.
 */
typedef struct _GgitIndex GgitIndex;

/* NOTE: keep in sync with git2/errors.h */
/**
 * GgitError:
 * @GGIT_ERROR_GIT_ERROR: A generic error for when an operation fails.
 * @GGIT_ERROR_NOTFOUND: Input does not exist in the scope searched.
 * @GGIT_ERROR_EXISTS: A reference with this name already exists.
 * @GGIT_ERROR_AMBIGUOUS: The given error is ambiguous.
 * @GGIT_ERROR_BUFS: FIXME
 * @GGIT_ERROR_PASSTHROUGH: Skip and passthrough the given ODB backend
 * @GGIT_ERROR_REVWALKOVER: The revision walker is empty; there
 *                          are no more commits left to iterate.
 *
 * Error codes for the %GGIT_ERROR error domain.
 */
typedef enum
{
	GGIT_ERROR_GIT_ERROR = -1,
	GGIT_ERROR_NOTFOUND = -3,
	GGIT_ERROR_EXISTS = -4,
	GGIT_ERROR_AMBIGUOUS = -5,
	GGIT_ERROR_BUFS = -6,
	GGIT_ERROR_PASSTHROUGH = -30,
	GGIT_ERROR_REVWALKOVER = -31
} GgitError;

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

/* NOTE: keep in sync with git2/refs.h */
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
 * GgitRevisionWalker:
 *
 * Represents a revision walker.
 */
typedef struct _GgitRevisionWalker GgitRevisionWalker;

/**
 * GgitSortMode:
 * @GGIT_SORT_NONE: Sorts the repository contents in no particular ordering;
 *                  this sorting is arbitrary, implementation-specific
 *                  and subject to change at any time.
 * @GGIT_SORT_TOPOLOGICAL: Sorts the repository contents in topological order
 *                         (parents before children); this sorting mode
 *                         can be combined with time sorting.
 * @GGIT_SORT_TIME: Sort the repository contents by commit time;
 *                  this sorting mode can be combined with
 *                  topological sorting.
 * @GGIT_SORT_REVERSE: Iterate through the repository contents in reverse
 *                     order; this sorting mode can be combined with
 *                     any of the above.
 *
 * The type of sorting mode for the revision walker.
 */
typedef enum
{
	GGIT_SORT_NONE = 0,
	GGIT_SORT_TOPOLOGICAL = 1 << 0,
	GGIT_SORT_TIME = 1 << 1,
	GGIT_SORT_REVERSE = 1 << 2
} GgitSortMode;

/* FIXME */
/* NOTE: keep in sync with git2/status.h */
/**
 * GgitStatusFlags:
 * @GGIT_STATUS_CURRENT:
 * @GGIT_STATUS_INDEX_NEW:
 * @GGIT_STATUS_INDEX_MODIFIED:
 * @GGIT_STATUS_INDEX_DELETED:
 * @GGIT_STATUS_WORKING_TREE_NEW:
 * @GGIT_STATUS_WORKING_TREE_MODIFIED:
 * @GGIT_STATUS_WORKING_TREE_DELETED:
 * @GGIT_STATUS_IGNORED:
 */
typedef enum
{
	GGIT_STATUS_CURRENT = 0,
	GGIT_STATUS_INDEX_NEW = 1 << 0,
	GGIT_STATUS_INDEX_MODIFIED = 1 << 1,
	GGIT_STATUS_INDEX_DELETED = 1 << 2,
	GGIT_STATUS_WORKING_TREE_NEW = 1 << 3,
	GGIT_STATUS_WORKING_TREE_MODIFIED = 1 << 4,
	GGIT_STATUS_WORKING_TREE_DELETED = 1 << 5,
	GGIT_STATUS_IGNORED = 1 << 6
} GgitStatusFlags;

typedef enum
{
	GGIT_CREATE_TAG_NONE = 0,
	GGIT_CREATE_TAG_FORCE = 1 << 0
} GgitCreateTagFlags;

/**
 * GgitStatusCallback:
 * @path: the file to retrieve status for, rooted at the repository working dir.
 * @status_flags: the status value.
 * @data: (closure): user-supplied data.
 *
 * The type of the callback functions for retrieving the status of the files
 * in a #GgitRepository. See ggit_repository_file_status_foreach().
 *
 * Returns: 0 to go for the next file or a #GgitError in case there was an error.
 */
typedef int (* GgitStatusCallback) (const gchar     *path,
                                    GgitStatusFlags  status_flags,
                                    gpointer         data);

/**
 * GgitReferencesCallback:
 * @name: the name of the reference
 * @data: (closure) (allow-none): user-supplied data.
 *
 * The type of the callback functions for retrieving the references
 * in a #GgitRepository. See ggit_repository_references_foreach().
 *
 * Returns: 0 to go for the next references or a #GgitError in case there was an error.
 */
typedef int (* GgitReferencesCallback) (const gchar     *name,
                                        gpointer         data);

/**
 * GgitConfigCallback:
 * @name: the name of the configuration value
 * @value: the value
 * @data: (closure) (allow-none): user-supplied data.
 *
 * The type of the callback functions for retrieving values from a #GgitConfig.
 * See ggit_config_foreach().
 *
 * Returns: 0 to go for the next config value or a #GgitError in case there was
 *          an error.
 */
typedef int (* GgitConfigCallback) (const gchar     *name,
                                    const gchar     *value,
                                    gpointer         data);

/**
 * GgitConfigMatchCallback:
 * @match_info: the match
 * @value: the value
 * @data: (closure) (allow-none): user-supplied data.
 *
 * The type of the callback functions for retrieving values from a #GgitConfig
 * using a regular expression. See ggit_config_match_foreach().
 *
 * Returns: 0 to go for the next config value or a #GgitError in case there was
 *          an error.
 */
typedef int (* GgitConfigMatchCallback) (GMatchInfo  *match_info,
                                         const gchar *value,
                                         gpointer     data);

/**
 * GgitTag:
 *
 * Represents a tag object.
 */
typedef struct _GgitTag GgitTag;

G_END_DECLS

#endif /* __GGIT_TYPES_H__ */

/* ex:set ts=8 noet: */
