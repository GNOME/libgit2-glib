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
typedef struct _GgitRef GgitRef;

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

typedef enum {
	GGIT_SUBMODULE_UPDATE_CHECKOUT = 0,
	GGIT_SUBMODULE_UPDATE_REBASE = 1,
	GGIT_SUBMODULE_UPDATE_MERGE = 2
} GgitSubmoduleUpdate;

/**
 * GgitSubmoduleIgnore:
 * @GGIT_SUBMODULE_IGNORE_ALL: never dirty.
 * @GGIT_SUBMODULE_IGNORE_DIRTY: only dirty if HEAD moved.
 * @GGIT_SUBMODULE_IGNORE_UNTRACKED: dirty if tracked files change.
 * @GGIT_SUBMODULE_IGNORE_NONE: any change or untracked == dirty.
 */
typedef enum {
	GGIT_SUBMODULE_IGNORE_ALL = 0,
	GGIT_SUBMODULE_IGNORE_DIRTY = 1,
	GGIT_SUBMODULE_IGNORE_UNTRACKED = 2,
	GGIT_SUBMODULE_IGNORE_NONE = 3
} GgitSubmoduleIgnore;

/**
 * GgitSubmodule:
 *
 * Represents a git submodule.
 */
typedef struct _GgitSubmodule GgitSubmodule;

/**
 * GitgRemote:
 *
 * Represents a git remote.
 */
typedef struct _GgitRemote GgitRemote;

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
	GGIT_CREATE_NONE = 0,
	GGIT_CREATE_FORCE = 1 << 0
} GgitCreateFlags;

/**
 * GgitBranchType:
 * @GGIT_BRANCH_LOCAL: specifies a local branch
 * @GGIT_BRANCH_REMOTE: specifies a remote branch
 *
 * The type of a branch.
 */
typedef enum {
	GGIT_BRANCH_LOCAL = 1,
	GGIT_BRANCH_REMOTE = 2,
} GgitBranchType;

/**
 * GgitSubmoduleCallback:
 * @name: the name of the submodule.
 * @data: (closure) (allow-none): user-supplied data.
 *
 * The type of the callback functions for retrieving the submodules
 * in a #GgitRepository. See ggit_repository_submodule_foreach().
 *
 * Returns: 0 to go for the next submodule or a #GgitError in case there was an error.
 */
typedef int (* GgitSubmoduleCallback) (const gchar     *name,
                                       gpointer         data);

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

/**
 * GgitDiff:
 *
 * Represents a diff.
 */
typedef struct _GgitDiff GgitDiff;

/**
 * GgitDiffOptions:
 *
 * Represents the options used when creating a #GgitDiff.
 */
typedef struct _GgitDiffOptions GgitDiffOptions;

/**
 * GgitDiffFile:
 *
 * Represents a file in a #GgitDiff.
 */
typedef struct _GgitDiffFile GgitDiffFile;

/**
 * GgitDiffDelta:
 *
 * Represents the changes done to one file.
 */
typedef struct _GgitDiffDelta GgitDiffDelta;

/**
 * GgitDiffRange:
 *
 * Represents the hunk of a diff.
 */
typedef struct _GgitDiffRange GgitDiffRange;

/**
 * GgitDiffFlags:
 * @GGIT_DIFF_NORMAL: normal.
 * @GGIT_DIFF_REVERSE: reverse.
 * @GGIT_DIFF_FORCE_TEXT: force text.
 * @GGIT_DIFF_IGNORE_WHITESPACE: ignore whitespace.
 * @GGIT_DIFF_IGNORE_WHITESPACE_CHANGE: ignore whitespace change.
 * @GGIT_DIFF_IGNORE_WHITESPACE_EOL: ignore whitespace at end-of-line.
 * @GGIT_DIFF_IGNORE_SUBMODULES: ignore submodules.
 * @GGIT_DIFF_PATIENCE: generate using the "patience diff" algorithm.
 * @GGIT_DIFF_INCLUDE_IGNORED: include ignored files.
 * @GGIT_DIFF_INCLUDE_UNTRACKED: include untracked files.
 * @GGIT_DIFF_INCLUDE_UNMODIFIED: include unmodified files.
 * @GGIT_DIFF_RECURSE_UNTRACKED_DIRS:
 *
 * How the diff should be generated.
 */
typedef enum {
	GGIT_DIFF_NORMAL                   = 0,
	GGIT_DIFF_REVERSE                  = 1 << 0,
	GGIT_DIFF_FORCE_TEXT               = 1 << 1,
	GGIT_DIFF_IGNORE_WHITESPACE        = 1 << 2,
	GGIT_DIFF_IGNORE_WHITESPACE_CHANGE = 1 << 3,
	GGIT_DIFF_IGNORE_WHITESPACE_EOL    = 1 << 4,
	GGIT_DIFF_IGNORE_SUBMODULES        = 1 << 5,
	GGIT_DIFF_PATIENCE                 = 1 << 6,
	GGIT_DIFF_INCLUDE_IGNORED          = 1 << 7,
	GGIT_DIFF_INCLUDE_UNTRACKED        = 1 << 8,
	GGIT_DIFF_INCLUDE_UNMODIFIED       = 1 << 9,
	GGIT_DIFF_RECURSE_UNTRACKED_DIRS   = 1 << 10
} GgitDiffFlags;

/**
 * GgitDiffFileFlags:
 * @GGIT_DIFF_FILE_VALID_OID: if the OID is valid.
 * @GGIT_DIFF_FILE_FREE_PATH: if the path must be freed (libgit2 internal).
 * @GGIT_DIFF_FILE_BINARY: if the file is binary.
 * @GGIT_DIFF_FILE_NOT_BINARY: if the file is not binary.
 * @GGIT_DIFF_FILE_FREE_DATA: if the data must be freed (libgit2 internal).
 * @GGIT_DIFF_FILE_UNMAP_DATA: if the data must be unmapped (libgit2 internal).
 */
typedef enum {
	GGIT_DIFF_FILE_VALID_OID  = 1 << 0,
	GGIT_DIFF_FILE_FREE_PATH  = 1 << 1,
	GGIT_DIFF_FILE_BINARY     = 1 << 2,
	GGIT_DIFF_FILE_NOT_BINARY = 1 << 3,
	GGIT_DIFF_FILE_FREE_DATA  = 1 << 4,
	GGIT_DIFF_FILE_UNMAP_DATA = 1 << 5
} GgitDiffFileFlags;


/**
 * GgitDeltaType:
 * @GGIT_DELTA_UNMODIFIED: unmodified.
 * @GGIT_DELTA_ADDED: added.
 * @GGIT_DELTA_DELETED: deleted.
 * @GGIT_DELTA_MODIFIED: modified.
 * @GGIT_DELTA_RENAMED: renamed.
 * @GGIT_DELTA_COPIED: copied.
 * @GGIT_DELTA_IGNORED: ignored.
 * @GGIT_DELTA_UNTRACKED: untracked.
 *
 * Describes the type of change the delta is.
 */
typedef enum {
	GGIT_DELTA_UNMODIFIED = 0,
	GGIT_DELTA_ADDED      = 1,
	GGIT_DELTA_DELETED    = 2,
	GGIT_DELTA_MODIFIED   = 3,
	GGIT_DELTA_RENAMED    = 4,
	GGIT_DELTA_COPIED     = 5,
	GGIT_DELTA_IGNORED    = 6,
	GGIT_DELTA_UNTRACKED  = 7
} GgitDeltaType;

/**
 * GgitDiffLineType:
 * @GGIT_DIFF_LINE_CONTEXT: line is part of the context.
 * @GGIT_DIFF_LINE_ADDITION: line that was added.
 * @GGIT_DIFF_LINE_DELETION: line that was removed.
 * @GGIT_DIFF_LINE_ADD_EOFNL: LF was added at end of file.
 * @GGIT_DIFF_LINE_DEL_EOFNL: LF was removed at end of file.
 * @GGIT_DIFF_LINE_FILE_HDR: the file header.
 * @GGIT_DIFF_LINE_HUNK_HDR: the hunk header.
 * @GGIT_DIFF_LINE_BINARY: is binary.
 *
 * These values describe where a line came from and will be passed to
 * the #GgitDiffLineCallback when iterating over a diff.
 *
 * The #GGIT_DIFF_LINE_FILE_HDR, #GGIT_DIFF_LINE_HUNK_HDR and
 * #GGIT_DIFF_LINE_BINARY values are only sent when the diff is being printed.
 */
typedef enum {
	GGIT_DIFF_LINE_CONTEXT   = ' ',
	GGIT_DIFF_LINE_ADDITION  = '+',
	GGIT_DIFF_LINE_DELETION  = '-',
	GGIT_DIFF_LINE_ADD_EOFNL = '\n',
	GGIT_DIFF_LINE_DEL_EOFNL = '\0',
	GGIT_DIFF_LINE_FILE_HDR  = 'F',
	GGIT_DIFF_LINE_HUNK_HDR  = 'H',
	GGIT_DIFF_LINE_BINARY    = 'B'
} GgitDiffLineType;

/**
 * GgitDiffFileCallback:
 * @delta: a #GgitDiffDelta.
 * @progress: the progress.
 * @user_data: (closure): user-supplied data.
 *
 * Called for each file.
 *
 * Returns: 0 to go continue or a #GgitError in case there was an error.
 */
typedef gint (* GgitDiffFileCallback) (GgitDiffDelta *delta,
                                       gfloat         progress,
                                       gpointer       user_data);

/**
 * GgitDiffHunkCallback:
 * @delta: a #GgitDiffDelta.
 * @range: a #GgitDiffRange.
 * @header: the header.
 * @header_len: the header length.
 * @user_data: (closure): user-supplied data.
 *
 * Called for each hunk.
 *
 * Returns: 0 to go continue or a #GgitError in case there was an error.
 */
typedef gint (* GgitDiffHunkCallback) (GgitDiffDelta *delta,
                                       GgitDiffRange *range,
                                       const gchar   *header,
                                       gsize          header_len,
                                       gpointer       user_data);

/**
 * GgitDiffLineCallback:
 * @delta: a #GgitDiffDelta.
 * @range: a #GgitDiffRange.
 * @line_type: a #GgitDiffLineType.
 * @content: the content.
 * @content_len: the content length.
 * @user_data: (closure): user-supplied data.
 *
 * Called for each line.
 *
 * Returns: 0 to go continue or a #GgitError in case there was an error.
 */
typedef gint (*GgitDiffLineCallback) (GgitDiffDelta    *delta,
                                      GgitDiffRange    *range,
                                      GgitDiffLineType  line_type,
                                      const gchar      *content,
                                      gsize             content_len,
                                      gpointer          user_data);

G_END_DECLS

#endif /* __GGIT_TYPES_H__ */

/* ex:set ts=8 noet: */
