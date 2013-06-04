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

#include <glib.h>

G_BEGIN_DECLS

/**
 * GgitBlob:
 *
 * Represents a blob object.
 */
typedef struct _GgitBlob GgitBlob;

/**
 * GgitBranch:
 *
 * Represents a branch object.
 */
typedef struct _GgitBranch GgitBranch;

/**
 * GgitCloneOptions:
 *
 * Represents the options used when cloning.
 */
typedef struct _GgitCloneOptions GgitCloneOptions;

/**
 * GgitCommit:
 *
 * Represents a commit object.
 */
typedef struct _GgitCommit GgitCommit;

/**
 * GgitCommitParents:
 *
 * Represents the parents of a commit object.
 */
typedef struct _GgitCommitParents GgitCommitParents;

/**
 * GgitConfig:
 *
 * Represents a git configuration.
 */
typedef struct _GgitConfig GgitConfig;

/**
 * GgitConfigEntry:
 *
 * Represents a git configuration entry.
 */
typedef struct _GgitConfigEntry GgitConfigEntry;

/**
 * GgitCred:
 *
 * Represents a git credential.
 */
typedef struct _GgitCred GgitCred;

/**
 * GgitCredPlaintext:
 *
 * Represents a plain text credential.
 */
typedef struct _GgitCredPlaintext GgitCredPlaintext;

/**
 * GgitDiff:
 *
 * Represents a diff.
 */
typedef struct _GgitDiff GgitDiff;

/**
 * GgitDiffDelta:
 *
 * Represents the changes done to one file.
 */
typedef struct _GgitDiffDelta GgitDiffDelta;

/**
 * GgitDiffFile:
 *
 * Represents a file in a #GgitDiff.
 */
typedef struct _GgitDiffFile GgitDiffFile;

/**
 * GgitDiffOptions:
 *
 * Represents the options used when creating a #GgitDiff.
 */
typedef struct _GgitDiffOptions GgitDiffOptions;

/**
 * GgitDiffRange:
 *
 * Represents the hunk of a diff.
 */
typedef struct _GgitDiffRange GgitDiffRange;

/**
 * GgitIndex:
 *
 * Represents an index object.
 */
typedef struct _GgitIndex GgitIndex;

/**
 * GgitIndexEntries:
 *
 * Represents the entries in an index object.
 */
typedef struct _GgitIndexEntries GgitIndexEntries;

/**
 * GgitIndexEntry:
 *
 * Represents an index entry object.
 */
typedef struct _GgitIndexEntry GgitIndexEntry;

/**
 * GgitIndexEntriesResolveUndo:
 *
 * Represents the resolve undo entries in an index object.
 */
typedef struct _GgitIndexEntriesResolveUndo GgitIndexEntriesResolveUndo;

/**
 * GgitIndexEntryResolveUndo:
 *
 * Represents an resolve undo index entry object.
 */
typedef struct _GgitIndexEntryResolveUndo GgitIndexEntryResolveUndo;

/**
 * GgitNative:
 *
 * Represents a generic native object.
 */
typedef struct _GgitNative GgitNative;

/**
 * GgitObject:
 *
 * Represents a generic object in a repository.
 */
typedef struct _GgitObject GgitObject;

/**
 * GgitObjectFactory:
 *
 * Represents an object factory.
 */
typedef struct _GgitObjectFactory GgitObjectFactory;

/**
 * GgitObjectFactoryBase:
 *
 * Represents the base type for objects created by an object factory.
 */
typedef struct _GgitObjectFactoryBase GgitObjectFactoryBase;

/**
 * GgitOId:
 *
 * Represents a unique ID of any object.
 */
typedef struct _GgitOId GgitOId;

/**
 * GgitPush:
 *
 * Represents a git push.
 */
typedef struct _GgitPush GgitPush;

/**
 * GgitRef:
 *
 * Reprensents a git reference.
 */
typedef struct _GgitRef GgitRef;

/**
 * GgitRefSpec:
 *
 * Reprensents a git reference specification.
 */
typedef struct _GgitRefSpec GgitRefSpec;

/**
 * GgitReflog:
 *
 * Represents a reflog.
 */
typedef struct _GgitReflog GgitReflog;

/**
 * GgitReflogEntry:
 *
 * Represents a reflog entry.
 */
typedef struct _GgitReflogEntry GgitReflogEntry;

/**
 * GgitRemote:
 *
 * Represents a git remote.
 */
typedef struct _GgitRemote GgitRemote;

/**
 * GgitRepository:
 *
 * Represents an existing git repository including all of it's
 * object contents.
 */
typedef struct _GgitRepository GgitRepository;

/**
 * GgitRevisionWalker:
 *
 * Represents a revision walker.
 */
typedef struct _GgitRevisionWalker GgitRevisionWalker;

/**
 * GgitSignature:
 *
 * Represents an action signature.
 */
typedef struct _GgitSignature GgitSignature;

/**
 * GgitStatusOptions:
 *
 * Represents the options used when creating getting file status.
 */
typedef struct _GgitStatusOptions GgitStatusOptions;

/**
 * GgitSubmodule:
 *
 * Represents a git submodule.
 */
typedef struct _GgitSubmodule GgitSubmodule;

/**
 * GgitTag:
 *
 * Represents a tag object.
 */
typedef struct _GgitTag GgitTag;

/**
 * GgitTransferProgress:
 *
 * Represents transfering progress.
 */
typedef struct _GgitTransferProgress GgitTransferProgress;

/**
 * GgitTree:
 *
 * Represents a tree object.
 */
typedef struct _GgitTree GgitTree;

/**
 * GgitTreeEntry:
 *
 * Represents an entry in a tree.
 */
typedef struct _GgitTreeEntry GgitTreeEntry;

/**
 * GgitTreeBuilder:
 *
 * Represents a tree object.
 */
typedef struct _GgitTreeBuilder GgitTreeBuilder;

/**
 * GgitBranchType:
 * @GGIT_BRANCH_LOCAL: specifies a local branch.
 * @GGIT_BRANCH_REMOTE: specifies a remote branch.
 *
 * The type of a branch.
 */
typedef enum
{
	GGIT_BRANCH_LOCAL  = 1,
	GGIT_BRANCH_REMOTE = 2
} GgitBranchType;

typedef enum
{
	GGIT_CAP_THREADS = 1 << 0,
	GGIT_CAP_HTTPS   = 1 << 1
} GgitCapFlags;

/**
 * GgitConfigLevel:
 * @GGIT_CONFIG_LEVEL_SYSTEM: System-wide configuration file.
 * @GGIT_CONFIG_LEVEL_XDG: XDG compatible configuration file (.config/git/config).
 * @GGIT_CONFIG_LEVEL_GLOBAL: User-specific configuration file, also called Global configuration file.
 * @GGIT_CONFIG_LEVEL_LOCAL: Repository specific configuration file.
 * @GGIT_CONFIG_LEVEL_HIGHEST: Represents the highest level of a config file.
 *
 * Priority level of a config file.
 * These priority levels correspond to the natural escalation logic
 * (from higher to lower) when searching for config entries in git.git.
 */
typedef enum
{
	GGIT_CONFIG_LEVEL_SYSTEM = 1,
	GGIT_CONFIG_LEVEL_XDG = 2,
	GGIT_CONFIG_LEVEL_GLOBAL = 3,
	GGIT_CONFIG_LEVEL_LOCAL = 4,
	GGIT_CONFIG_LEVEL_HIGHEST = -1
} GgitConfigLevel;

/**
 * GgitCreateFlags:
 * @GGIT_CREATE_NONE: attempt to create.
 * @GGIT_CREATE_FORCE: force creation.
 *
 * Describes how something should be created.
 */
typedef enum
{
	GGIT_CREATE_NONE  = 0,
	GGIT_CREATE_FORCE = 1 << 0
} GgitCreateFlags;

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
 * GgitDiffOption:
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
 * @GGIT_DIFF_RECURSE_UNTRACKED_DIRS: recurse to untracked directories.
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
} GgitDiffOption;

/**
 * GgitDiffFlag:
 * @GGIT_DIFF_FLAG_BINARY: if the file is binary.
 * @GGIT_DIFF_FLAG_NOT_BINARY: if the file is not binary.
 * @GGIT_DIFF_FLAG_VALID_OID: if the OID is valid.
 *
 * Describes the diff file and/or delta flags
 */
typedef enum {
	GGIT_DIFF_FLAG_BINARY     = 1 << 0,
	GGIT_DIFF_FLAG_NOT_BINARY = 1 << 1,
	GGIT_DIFF_FLAG_VALID_OID  = 1 << 2
} GgitDiffFlag;

/**
 * GgitDiffLineType:
 * @GGIT_DIFF_LINE_CONTEXT: line is part of the context.
 * @GGIT_DIFF_LINE_ADDITION: line that was added.
 * @GGIT_DIFF_LINE_DELETION: line that was removed.
 * @GGIT_DIFF_LINE_CONTEXT_EOFNL: Both files have no LF at end.
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
	GGIT_DIFF_LINE_CONTEXT       = ' ',
	GGIT_DIFF_LINE_ADDITION      = '+',
	GGIT_DIFF_LINE_DELETION      = '-',
	GGIT_DIFF_LINE_CONTEXT_EOFNL = '=',
	GGIT_DIFF_LINE_ADD_EOFNL     = '>',
	GGIT_DIFF_LINE_DEL_EOFNL     = '<',
	GGIT_DIFF_LINE_FILE_HDR      = 'F',
	GGIT_DIFF_LINE_HUNK_HDR      = 'H',
	GGIT_DIFF_LINE_BINARY        = 'B'
} GgitDiffLineType;

/* NOTE: keep in sync with git2/errors.h */
/**
 * GgitError:
 * @GGIT_ERROR_GIT_ERROR: A generic error for when an operation fails.
 * @GGIT_ERROR_NOTFOUND: Input does not exist in the scope searched.
 * @GGIT_ERROR_EXISTS: A reference with this name already exists.
 * @GGIT_ERROR_AMBIGUOUS: The given error is ambiguous.
 * @GGIT_ERROR_BUFS: The buffer is too short.
 * @GGIT_ERROR_PASSTHROUGH: Skip and passthrough the given ODB backend.
 * @GGIT_ERROR_ITEROVER: The iteration has finished.
 *
 * Error codes for the %GGIT_ERROR error domain.
 */
typedef enum {
	GGIT_ERROR_GIT_ERROR   = -1,
	GGIT_ERROR_NOTFOUND    = -3,
	GGIT_ERROR_EXISTS      = -4,
	GGIT_ERROR_AMBIGUOUS   = -5,
	GGIT_ERROR_BUFS        = -6,
	GGIT_ERROR_PASSTHROUGH = -30,
	GGIT_ERROR_ITEROVER    = -31
} GgitError;

/* NOTE: keep in sync with git2/refs.h */
typedef enum {
	GGIT_FILEMODE_NEW             = 0000000,
	GGIT_FILEMODE_TREE            = 0040000,
	GGIT_FILEMODE_BLOB            = 0100644,
	GGIT_FILEMODE_BLOB_EXECUTABLE = 0100755,
	GGIT_FILEMODE_LINK            = 0120000,
	GGIT_FILEMODE_COMMIT          = 0160000
} GgitFileMode;

/* NOTE: keep in sync with git2/refs.h */
/**
 * GgitRemoteDownloadTagsType:
 * @GGIT_REMOTE_DOWNLOAD_TAGS_AUTO:
 * @GGIT_REMOTE_DOWNLOAD_TAGS_NONE:
 * @GGIT_REMOTE_DOWNLOAD_TAGS_ALL:
 */
typedef enum {
	GGIT_REMOTE_DOWNLOAD_TAGS_AUTO = 0,
	GGIT_REMOTE_DOWNLOAD_TAGS_NONE = 1,
	GGIT_REMOTE_DOWNLOAD_TAGS_ALL  = 2
} GgitRemoteDownloadTagsType;

/* NOTE: keep in sync with git2/refs.h */
/**
 * GgitRefType:
 * @GGIT_REF_INVALID: An invalid reference.
 * @GGIT_REF_OID: A reference which points at an object id.
 * @GGIT_REF_SYMBOLIC: A reference which points at another reference.
 * @GGIT_REF_LISTALL: All reference types.
 *
 * Describes the type a reference is.
 */
typedef enum {
	GGIT_REF_INVALID  = 0,
	GGIT_REF_OID      = 1,
	GGIT_REF_SYMBOLIC = 2,
	GGIT_REF_LISTALL  = GGIT_REF_OID | GGIT_REF_SYMBOLIC
} GgitRefType;

/**
 * GgitResetType:
 * @GGIT_RESET_SOFT: The head will be moved to the commit.
 * @GGIT_RESET_MIXED: the head will be moved to the commit and the index
 *                    will be replaced with the content of the commit tree.
 * @GGIT_RESET_HARD: MIXED plus changes in working tree discarded.
 *
 * Describes the type of reset to perform.
 */
typedef enum {
	GGIT_RESET_SOFT  = 1,
	GGIT_RESET_MIXED = 2,
	GGIT_RESET_HARD  = 3
} GgitResetType;

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
typedef enum {
	GGIT_SORT_NONE        = 0,
	GGIT_SORT_TOPOLOGICAL = 1 << 0,
	GGIT_SORT_TIME        = 1 << 1,
	GGIT_SORT_REVERSE     = 1 << 2
} GgitSortMode;

/**
 * GgitStashFlags:
 * @GGIT_STASH_DEFAULT: default stash.
 * @GGIT_STASH_KEEP_INDEX: All changes already added to the index
 * are left intact in the working directory.
 * @GGIT_STASH_INCLUDE_UNTRACKED: All untracked files are also stashed and then
 * cleaned up from the working directory.
 * @GGIT_STASH_INCLUDE_IGNORED: All ignored files are also stashed and then
 * cleaned up from the working directory.
 *
 * Describes how a stash should be applied.
 */
typedef enum {
	GGIT_STASH_DEFAULT           = 0,
	GGIT_STASH_KEEP_INDEX        = 1 << 0,
	GGIT_STASH_INCLUDE_UNTRACKED = 1 << 1,
	GGIT_STASH_INCLUDE_IGNORED   = 1 << 2
} GgitStashFlags;

/* NOTE: keep in sync with git2/status.h */
/**
 * GgitStatusFlags:
 * @GGIT_STATUS_CURRENT: file is current.
 * @GGIT_STATUS_INDEX_NEW: file in index is new.
 * @GGIT_STATUS_INDEX_MODIFIED: file in index is modified.
 * @GGIT_STATUS_INDEX_DELETED: file in index is deleted.
 * @GGIT_STATUS_WORKING_TREE_NEW: file in working tree is new.
 * @GGIT_STATUS_WORKING_TREE_MODIFIED: file in working tree is modified.
 * @GGIT_STATUS_WORKING_TREE_DELETED: file in working tree is deleted.
 * @GGIT_STATUS_IGNORED: file is ignored.
 *
 * Describes a file's status.
 */
typedef enum {
	GGIT_STATUS_CURRENT                 = 0,
	GGIT_STATUS_INDEX_NEW               = 1 << 0,
	GGIT_STATUS_INDEX_MODIFIED          = 1 << 1,
	GGIT_STATUS_INDEX_DELETED           = 1 << 2,
	GGIT_STATUS_INDEX_RENAMED           = 1 << 3,
	GGIT_STATUS_INDEX_TYPECHANGE        = 1 << 4,
	GGIT_STATUS_WORKING_TREE_NEW        = 1 << 7,
	GGIT_STATUS_WORKING_TREE_MODIFIED   = 1 << 8,
	GGIT_STATUS_WORKING_TREE_DELETED    = 1 << 9,
	GGIT_STATUS_WORKING_TREE_TYPECHANGE = 1 << 10,
	GGIT_STATUS_IGNORED                 = 1 << 14
} GgitStatusFlags;

/**
 * GgitSubmoduleIgnore:
 * @GGIT_SUBMODULE_IGNORE_DEFAULT: reset to default.
 * @GGIT_SUBMODULE_IGNORE_NONE: any change or untracked == dirty.
 * @GGIT_SUBMODULE_IGNORE_UNTRACKED: dirty if tracked files change.
 * @GGIT_SUBMODULE_IGNORE_DIRTY: only dirty if HEAD moved.
 * @GGIT_SUBMODULE_IGNORE_ALL: never dirty
 *
 * Describes which submodules should be ignored.
 */
typedef enum {
	GGIT_SUBMODULE_IGNORE_DEFAULT   = -1,
	GGIT_SUBMODULE_IGNORE_NONE      = 0,
	GGIT_SUBMODULE_IGNORE_UNTRACKED = 1,
	GGIT_SUBMODULE_IGNORE_DIRTY     = 2,
	GGIT_SUBMODULE_IGNORE_ALL       = 3
} GgitSubmoduleIgnore;

/**
 * GgitSubmoduleStatus:
 * FIXME
 */
typedef enum {
	GGIT_SUBMODULE_STATUS_IN_HEAD           = 1 << 0,
	GGIT_SUBMODULE_STATUS_IN_INDEX          = 1 << 1,
	GGIT_SUBMODULE_STATUS_IN_CONFIG         = 1 << 2,
	GGIT_SUBMODULE_STATUS_IN_WD             = 1 << 3,
	GGIT_SUBMODULE_STATUS_INDEX_ADDED       = 1 << 4,
	GGIT_SUBMODULE_STATUS_INDEX_DELETED     = 1 << 5,
	GGIT_SUBMODULE_STATUS_INDEX_MODIFIED    = 1 << 6,
	GGIT_SUBMODULE_STATUS_WD_UNINITIALIZED  = 1 << 7,
	GGIT_SUBMODULE_STATUS_WD_ADDED          = 1 << 8,
	GGIT_SUBMODULE_STATUS_WD_DELETED        = 1 << 9,
	GGIT_SUBMODULE_STATUS_WD_MODIFIED       = 1 << 10,
	GGIT_SUBMODULE_STATUS_WD_INDEX_MODIFIED = 1 << 11,
	GGIT_SUBMODULE_STATUS_WD_WD_MODIFIED    = 1 << 12,
	GGIT_SUBMODULE_STATUS_WD_UNTRACKED      = 1 << 13
} GgitSubmoduleStatus;

/**
 * GgitSubmoduleUpdate:
 * @GGIT_SUBMODULE_UPDATE_CHECKOUT: checkout the submodule.
 * @GGIT_SUBMODULE_UPDATE_REBASE: rebase the current branch onto the
 *                                commit recorded in the superproject.
 * @GGIT_SUBMODULE_UPDATE_MERGE: merge the commit recorded in the
 *                               superproject into the current branch.
 *
 * Describes how a submodule should be updated.
 */
typedef enum {
	GGIT_SUBMODULE_UPDATE_CHECKOUT = 0,
	GGIT_SUBMODULE_UPDATE_REBASE   = 1,
	GGIT_SUBMODULE_UPDATE_MERGE    = 2
} GgitSubmoduleUpdate;

/**
 * GgitTreeWalkMode:
 * @GGIT_TREE_WALK_MODE_PRE: walk tree in pre-order
 * @GGIT_TREE_WALK_MODE_POST: walk tree in post-order
 *
 * Describes in which order a tree walk should be performed.
 */
typedef enum {
	GGIT_TREE_WALK_MODE_PRE = 0,
	GGIT_TREE_WALK_MODE_POST = 1,
} GgitTreeWalkMode;

/**
 * GgitStatusOption:
 * GGIT_STATUS_OPTION_INCLUDE_UNTRACKED: include untracked files (default).
 * GGIT_STATUS_OPTION_INCLUDE_IGNORED: include ignored files (default).
 * GGIT_STATUS_OPTION_INCLUDE_UNMODIFIED: include unmodified files.
 * GGIT_STATUS_OPTION_EXCLUDE_SUBMODULES: exclude submodules.
 * GGIT_STATUS_OPTION_RECURSE_UNTRACKED_DIRS: search untracked directories recursively (default).
 * GGIT_STATUS_OPTION_DISABLE_PATHSPEC_MATCH: do not match path specifications.
 * GGIT_STATUS_OPTION_RECURSE_IGNORED_DIRS: search ignored directories recursively.
 * GGIT_STATUS_OPTION_SORT_CASE_SENSITIVELY: sort case sensitive.
 * GGIT_STATUS_OPTION_SORT_CASE_INSENSITIVELY: sort case insensitive.
 * GGIT_STATUS_OPTION_DEFAULT: default flags.
 *
 * Status options specified when using #ggit_repository_file_status_foreach.
 *
 */
typedef enum {
	GGIT_STATUS_OPTION_INCLUDE_UNTRACKED        = (1 << 0),
	GGIT_STATUS_OPTION_INCLUDE_IGNORED          = (1 << 1),
	GGIT_STATUS_OPTION_INCLUDE_UNMODIFIED       = (1 << 2),
	GGIT_STATUS_OPTION_EXCLUDE_SUBMODULES       = (1 << 3),
	GGIT_STATUS_OPTION_RECURSE_UNTRACKED_DIRS   = (1 << 4),
	GGIT_STATUS_OPTION_DISABLE_PATHSPEC_MATCH   = (1 << 5),
	GGIT_STATUS_OPTION_RECURSE_IGNORED_DIRS     = (1 << 6),
	GGIT_STATUS_OPTION_SORT_CASE_SENSITIVELY    = (1 << 9),
	GGIT_STATUS_OPTION_SORT_CASE_INSENSITIVELY  = (1 << 10),
	GGIT_STATUS_OPTION_DEFAULT                  = GGIT_STATUS_OPTION_INCLUDE_IGNORED |
	                                              GGIT_STATUS_OPTION_INCLUDE_UNTRACKED |
	                                              GGIT_STATUS_OPTION_RECURSE_UNTRACKED_DIRS
} GgitStatusOption;

/**
 * GgitStatusShow:
 * @GGIT_STATUS_SHOW_INDEX_AND_WORKDIR: receive one callback for each file,
 * even if the file is in both the index and the workdir (combining #GgitStatusFlags).
 * @GGIT_STATUS_SHOW_INDEX_ONLY: only receive callbacks for files in the index.
 * @GGIT_STATUS_SHOW_WORKDIR_ONLY: only receive callbacks for files in the workdir.
 * @GGIT_STATUS_SHOW_INDEX_THEN_WORKDIR: receive callbacks first for files in
 * the index, and then in the workdir. This is equivalent to first specifying
 * #GGIT_STATUS_SHOW_INDEX_ONLY and then #GGIT_STATUS_SHOW_WORKDIR_ONLY.
 *
 * Show options for #ggit_repository_file_status_foreach. Determines which
 * files are included in the status.
 *
 */
typedef enum {
	GGIT_STATUS_SHOW_INDEX_AND_WORKDIR = 0,
	GGIT_STATUS_SHOW_INDEX_ONLY = 1,
	GGIT_STATUS_SHOW_WORKDIR_ONLY = 2,
	GGIT_STATUS_SHOW_INDEX_THEN_WORKDIR = 3,
} GgitStatusShow;

/**
 * GgitBranchesCallback:
 * @branch_name: the branch name.
 * @branch_type: a #GgitBranchType.
 * @user_data: (closure): user-supplied data.
 *
 * The type of the callback functions for retrieving the branches
 * in a #GgitRepository. See ggit_repository_branches_foreach().
 *
 * Returns: 0 to go for the next branch or a #GgitError in case there was an error.
 *
 */
typedef gint (* GgitBranchesCallback) (const gchar    *branch_name,
                                       GgitBranchType  branch_type,
                                       gpointer        user_data);

/**
 * GgitConfigCallback:
 * @entry: a #GgitConfigEntry.
 * @user_data: (closure): user-supplied data.
 *
 * The type of the callback functions for retrieving values from a #GgitConfig.
 * See ggit_config_foreach().
 *
 * Returns: 0 to go for the next config value or a #GgitError in case there was
 *          an error.
 */
typedef gint (* GgitConfigCallback) (GgitConfigEntry *entry,
                                     gpointer         user_data);

/**
 * GgitConfigMatchCallback:
 * @match_info: the match
 * @value: the value
 * @user_data: (closure): user-supplied data.
 *
 * The type of the callback functions for retrieving values from a #GgitConfig
 * using a regular expression. See ggit_config_match_foreach().
 *
 * Returns: 0 to go for the next config value or a #GgitError in case there was
 *          an error.
 */
typedef gint (* GgitConfigMatchCallback) (GMatchInfo  *match_info,
                                          const gchar *value,
                                          gpointer     user_data);

/**
 * GgitCredAcquireCallback:
 * @url: the resource for which we are demanding a credential.
 * @username_from_url: (allow-none): The username that was embedded in a "user@host"
 *                                   remote url, or NULL if not included.
 * @allowed_types: a bitmask stating which cred types are OK to return.
 * @cred: (transfer full): newly created credential object.
 * @user_data: (closure): user-supplied data.
 *
 * Signature of a function which acquires a credential object.
 */
typedef gint (* GgitCredAcquireCallback) (const gchar *url,
                                          const gchar *username_from_url,
                                          guint        allowed_types,
                                          GgitCred   **cred,
                                          gpointer     user_data);

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
 * @header: (array length=header_len) (element-type guint8): the header.
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
 * @content: (array length=content_len) (element-type guint8): the content.
 * @content_len: the content length.
 * @user_data: (closure): user-supplied data.
 *
 * Called for each line.
 *
 * Returns: 0 to go continue or a #GgitError in case there was an error.
 */
typedef gint (* GgitDiffLineCallback) (GgitDiffDelta    *delta,
                                       GgitDiffRange    *range,
                                       GgitDiffLineType  line_type,
                                       const gchar      *content,
                                       gsize             content_len,
                                       gpointer          user_data);

/**
 * GgitReferencesNameCallback:
 * @name: the name of the reference
 * @user_data: (closure): user-supplied data.
 *
 * The type of the callback functions for retrieving the references
 * in a #GgitRepository. See ggit_repository_references_foreach_name().
 *
 * Returns: 0 to go for the next references or a #GgitError in case there was an error.
 */
typedef gint (* GgitReferencesNameCallback) (const gchar *name,
                                             gpointer     user_data);

/**
 * GgitRemoteListCallback:
 * @name: the name of the reference.
 * @oid: the reference's oid.
 * @loid: the reference's loid.
 * @local: if available locally.
 * @user_data: (closure): user-supplied data.
 *
 * The type of the callback functions for listing the references of a
 * #GgitRemote. See ggit_remote_list().
 *
 * Returns: 0 to go continue or a #GgitError in case there was an error.
 */
typedef gint (* GgitRemoteListCallback) (const gchar *name,
                                         GgitOId     *oid,
                                         GgitOId     *loid,
                                         gboolean     local,
                                         gpointer     user_data);

/**
 * GgitStashCallback:
 * @index: the position within the stash list. 0 points to the
 * most recent stashed state.
 * @message: the stash message.
 * @stash_oid: the commit oid of the stashed state.
 * @user_data: (closure): user-suplied data.
 *
 * When iterating over all the stashed states, callback that will be
 * issued per entry. See ggit_repository_stash_foreach().
 *
 * Returns: 0 to go continue or a #GgitError in case there was an error.
 */
typedef gint (* GgitStashCallback) (gsize        index,
                                    const gchar *message,
                                    GgitOId     *stash_oid,
                                    gpointer    *user_data);

/**
 * GgitStatusCallback:
 * @path: the file to retrieve status for, rooted at the repository working dir.
 * @status_flags: the status value.
 * @user_data: (closure): user-supplied data.
 *
 * The type of the callback functions for retrieving the status of the files
 * in a #GgitRepository. See ggit_repository_file_status_foreach().
 *
 * Returns: 0 to go for the next file or a #GgitError in case there was an error.
 */
typedef gint (* GgitStatusCallback) (const gchar     *path,
                                     GgitStatusFlags  status_flags,
                                     gpointer         user_data);

/**
 * GgitSubmoduleCallback:
 * @submodule: a #GgitSubmodule.
 * @name: the name of the submodule.
 * @user_data: (closure): user-supplied data.
 *
 * The type of the callback functions for retrieving the submodules
 * in a #GgitRepository. See ggit_repository_submodule_foreach().
 *
 * Returns: 0 to go for the next submodule or a #GgitError in case there was an error.
 */
typedef gint (* GgitSubmoduleCallback) (GgitSubmodule *submodule,
                                        const gchar   *name,
                                        gpointer       user_data);

/**
 * GgitTransferProgressCallback:
 * @stats: a #GgitTransferProgress.
 * @user_data: (closure): user-supplied data.
 *
 * Progress callbacks during indexing.
 *
 * Returns: a value less than zero to cancel the transfer.
 */
typedef gint (* GgitTransferProgressCallback) (const GgitTransferProgress *stats,
                                               gpointer                    user_data);

/**
 * GgitTreeWalkCallback:
 * @root: the current (relative) root of the entry
 * @entry: the tree entry
 * @user_data: (closure): user-supplied data.
 *
 * The type of the callback functions for walking a tree.
 * See ggit_tree_walk().
 *
 * Returns: 0 to go for the next entry or a #GgitError in case there was an error.
 *
 */
typedef gint (* GgitTreeWalkCallback) (const gchar         *root,
                                       const GgitTreeEntry *entry,
                                       gpointer             user_data);

G_END_DECLS

#endif /* __GGIT_TYPES_H__ */

/* ex:set ts=8 noet: */
