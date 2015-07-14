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
 * GgitBlobOutputStream:
 *
 * Represents a blob stream object.
 */
typedef struct _GgitBlobOutputStream GgitBlobOutputStream;

/**
 * GgitBranch:
 *
 * Represents a branch object.
 */
typedef struct _GgitBranch GgitBranch;

/**
 * GgitBranchEnumerator:
 *
 * Represents a branch enumerator.
 */
typedef struct _GgitBranchEnumerator GgitBranchEnumerator;

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
 * GgitCredSshInteractive:
 *
 * Represents a ssh interactive credential.
 */
typedef struct _GgitCredSshInteractive GgitCredSshInteractive;

/**
 * GgitCredSshInteractivePrompt:
 *
 * Represents a ssh interactive prompt.
 */
typedef struct _GgitCredSshInteractivePrompt GgitCredSshInteractivePrompt;

/**
 * GgitCredSshKeyFromAgent:
 *
 * Represents a ssh key agent credential.
 */
typedef struct _GgitCredSshKeyFromAgent GgitCredSshKeyFromAgent;

/**
 * GgitCredPlaintext:
 *
 * Represents a plain text credential.
 */
typedef struct _GgitCredPlaintext GgitCredPlaintext;

/**
 * GgitDiff:
 *
 * Represents a diff list.
 */
typedef struct _GgitDiff GgitDiff;

/**
 * GgitDiffBinary:
 *
 * Represents a diff binary.
 */
typedef struct _GgitDiffBinary GgitDiffBinary;

/**
 * GgitDiffBinaryFile:
 *
 * Represents a diff binary file.
 */
typedef struct _GgitDiffBinaryFile GgitDiffBinaryFile;

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
 * GgitDiffFindOptions:
 *
 * Represents options for finding diff similarity.
 */
typedef struct _GgitDiffFindOptions GgitDiffFindOptions;

/**
 * GgitDiffHunk:
 *
 * Represents the hunk of a diff.
 */
typedef struct _GgitDiffHunk GgitDiffHunk;

/**
 * GgitDiffLine:
 *
 * Represents the line of a diff.
 */
typedef struct _GgitDiffLine GgitDiffLine;

/**
 * GgitDiffOptions:
 *
 * Represents the options used when creating a #GgitDiff.
 */
typedef struct _GgitDiffOptions GgitDiffOptions;

/**
 * GgitDiffSimilarityMetric:
 *
 * Represents a similarity metric.
 */
typedef struct _GgitDiffSimilarityMetric GgitDiffSimilarityMetric;

/**
 * GgitBlameHunk:
 *
 * Represents a blame hunk.
 */
typedef struct _GgitBlameHunk GgitBlameHunk;

/**
 * GgitFetchOptions:
 *
 * Represents a git fetch options.
 */
typedef struct _GgitFetchOptions GgitFetchOptions;

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
 * GgitMergeOptions:
 *
 * Represents the options used when merging.
 */
typedef struct _GgitMergeOptions GgitMergeOptions;

/**
 * GgitNote:
 *
 * Represents a note object.
 */
typedef struct _GgitNote GgitNote;

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
 * GgitPatch:
 *
 * Represents a patch object.
 */
typedef struct _GgitPatch GgitPatch;

/**
 * GgitPushOptions:
 *
 * Represents a git push options.
 */
typedef struct _GgitPushOptions GgitPushOptions;

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
 * GgitRemoteHead:
 *
 * Represents a git remote head.
 */
typedef struct _GgitRemoteHead GgitRemoteHead;

/**
 * GgitRemoteCallbacks:
 *
 * Represents a git remote callbacks.
 */
typedef struct _GgitRemoteCallbacks GgitRemoteCallbacks;

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
 * GgitSubmoduleUpdateOptions:
 *
 * Represents options for a submodule update.
 */
typedef struct _GgitSubmoduleUpdateOptions GgitSubmoduleUpdateOptions;

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
 * GgitBlameOptions:
 *
 * Represents blame options.
 */
typedef struct _GgitBlameOptions GgitBlameOptions;

/**
 * GgitCheckoutOptions:
 *
 * Represents the options used when doign a checkout.
 */
typedef struct _GgitCheckoutOptions GgitCheckoutOptions;

/**
 * GgitCherryPickOptions:
 *
 * Represents the options used when doign a cherry-pick.
 */
typedef struct _GgitCherryPickOptions GgitCherryPickOptions;

/**
 * GgitRevertOptions:
 *
 * Represents the options used when reverting.
 */
typedef struct _GgitRevertOptions GgitRevertOptions;

/**
 * GgitFormatEmailOptions:
 *
 * Represents the options used when formatting a diff for e-mail.
 */
typedef struct _GgitDiffFormatEmailOptions GgitDiffFormatEmailOptions;

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
	GGIT_FEATURE_THREADS = 1 << 0,
	GGIT_FEATURE_HTTPS   = 1 << 1,
	GGIT_FEATURE_SSH     = (1 << 2)
} GgitFeatureFlags;

/**
 * GgitDiffBinaryType:
 * @GIT_DIFF_BINARY_NONE: There is no binary delta.
 * @GIT_DIFF_BINARY_LITERAL: The binary data is the literal contents of the file.
 * @GIT_DIFF_BINARY_DELTA: The binary data is the delta from one side to the other.
 *
 * When producing a binary diff, the binary data returned will be
 * either the deflated full ("literal") contents of the file, or
 * the deflated binary delta between the two sides (whichever is
 * smaller).
 */
typedef enum {
	GGIT_DIFF_BINARY_NONE,
	GGIT_DIFF_BINARY_LITERAL,
	GGIT_DIFF_BINARY_DELTA
} GgitDiffBinaryType;

/**
 * GgitBlameFlags:
 * @GGIT_BLAME_NORMAL: Normal blame, the default.
 * @GGIT_BLAME_TRACK_COPIES_SAME_FILE: Track lines that have moved within a file
 *                                     (like git blame -M)
 */
typedef enum
{
	GGIT_BLAME_NORMAL                 = 0,
	GGIT_BLAME_TRACK_COPIES_SAME_FILE = 1 << 0
} GgitBlameFlags;

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
	GGIT_CONFIG_LEVEL_SYSTEM  = 1,
	GGIT_CONFIG_LEVEL_XDG     = 2,
	GGIT_CONFIG_LEVEL_GLOBAL  = 3,
	GGIT_CONFIG_LEVEL_LOCAL   = 4,
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
 * GgitDiffFormatType:
 * @GGIT_DIFF_FORMAT_PATCH: full git diff.
 * @GGIT_DIFF_FORMAT_PATCH_HEADER: just the file headers of patch.
 * @GGIT_DIFF_FORMAT_RAW: like git diff --raw.
 * @GGIT_DIFF_FORMAT_NAME_ONLY: like git diff --name-only.
 * @GGIT_DIFF_FORMAT_NAME_STATUS: like git diff --name-status.
 *
 * Possible output formats for diff data.
 */
typedef enum {
	GGIT_DIFF_FORMAT_PATCH        = 1u,
	GGIT_DIFF_FORMAT_PATCH_HEADER = 2u,
	GGIT_DIFF_FORMAT_RAW          = 3u,
	GGIT_DIFF_FORMAT_NAME_ONLY    = 4u,
	GGIT_DIFF_FORMAT_NAME_STATUS  = 5u
} GgitDiffFormatType;

/**
 * GgitDiffOption:
 * @GGIT_DIFF_NORMAL: normal.
 * @GGIT_DIFF_REVERSE: reverse the sides of the diff.
 * @GGIT_DIFF_INCLUDE_IGNORED: include ignored files.
 * @GGIT_DIFF_RECURSE_IGNORED_DIRS: also add all files under ignored dirs.
 * @GGIT_DIFF_INCLUDE_UNTRACKED: include untracked files.
 * @GGIT_DIFF_RECURSE_UNTRACKED_DIRS: recurse to untracked directories.
 * @GGIT_DIFF_INCLUDE_UNMODIFIED: include unmodified files.
 * @GGIT_DIFF_INCLUDE_TYPECHANGE: enable generation of typechange detal records.
 * @GGIT_DIFF_INCLUDE_TYPECHANGE_TREES: try to label tree transitions as type changes.
 * @GGIT_DIFF_IGNORE_FILE_MODE: ignore file mode changes.
 * @GGIT_DIFF_IGNORE_SUBMODULES: ignore submodules.
 * @GGIT_DIFF_IGNORE_CASE: use case insensitive filename comparison.
 * @GGIT_DIFF_DISABLE_PATHSPEC_MATCH: use exact path matching.
 * @GGIT_DIFF_SKIP_BINARY_CHECK: disable updating the binary flag in delta records.
 * @GGIT_DIFF_ENABLE_FAST_UNTRACKED_DIRS: immediately label untracked directories as
 * untracked, without checking inside.
 * @GGIT_DIFF_FORCE_TEXT: force text.
 * @GGIT_DIFF_FORCE_BINARY: treat all files as binary, disabling text diffs.
 * @GGIT_DIFF_IGNORE_WHITESPACE: ignore whitespace.
 * @GGIT_DIFF_IGNORE_WHITESPACE_CHANGE: ignore whitespace change.
 * @GGIT_DIFF_IGNORE_WHITESPACE_EOL: ignore whitespace at end-of-line.
 * @GGIT_DIFF_SHOW_UNTRACKED_CONTENT: include content of untracked files.
 * this implies GGIT_DIFF_INCLUDE_UNTRACKED but not GGIT_DIFF_RECURSE_UNTRACKED_DIRS.
 * @GGIT_DIFF_SHOW_UNMODIFIED: show unmodified files.
 * @GGIT_DIFF_PATIENCE: generate using the "patience diff" algorithm.
 * @GGIT_DIFF_MINIMAL: take extra time to find minimal diff.
 *
 * How the diff should be generated.
 */
typedef enum {
	GGIT_DIFF_NORMAL                     = 0,
	GGIT_DIFF_REVERSE                    = 1u << 0,
	GGIT_DIFF_INCLUDE_IGNORED            = 1u << 1,
	GGIT_DIFF_RECURSE_IGNORED_DIRS       = 1u << 2,
	GGIT_DIFF_INCLUDE_UNTRACKED          = 1u << 3,
	GGIT_DIFF_RECURSE_UNTRACKED_DIRS     = 1u << 4,
	GGIT_DIFF_INCLUDE_UNMODIFIED         = 1u << 5,
	GGIT_DIFF_INCLUDE_TYPECHANGE         = 1u << 6,
	GGIT_DIFF_INCLUDE_TYPECHANGE_TREES   = 1u << 7,
	GGIT_DIFF_IGNORE_FILE_MODE           = 1u << 8,
	GGIT_DIFF_IGNORE_SUBMODULES          = 1u << 9,
	GGIT_DIFF_IGNORE_CASE                = 1u << 10,

	GGIT_DIFF_DISABLE_PATHSPEC_MATCH     = 1u << 12,
	GGIT_DIFF_SKIP_BINARY_CHECK          = 1u << 13,
	GGIT_DIFF_ENABLE_FAST_UNTRACKED_DIRS = 1u << 14,

	GGIT_DIFF_FORCE_TEXT                 = 1u << 20,
	GGIT_DIFF_FORCE_BINARY               = 1u << 21,
	GGIT_DIFF_IGNORE_WHITESPACE          = 1u << 22,
	GGIT_DIFF_IGNORE_WHITESPACE_CHANGE   = 1u << 23,
	GGIT_DIFF_IGNORE_WHITESPACE_EOL      = 1u << 24,

	GGIT_DIFF_SHOW_UNTRACKED_CONTENT     = 1u << 25,
	GGIT_DIFF_SHOW_UNMODIFIED            = 1u << 26,

	GGIT_DIFF_PATIENCE                   = 1u << 28,
	GGIT_DIFF_MINIMAL                    = 1u << 29
} GgitDiffOption;

/**
 * GgitDiffFlag:
 * @GGIT_DIFF_FLAG_BINARY: if the file is binary.
 * @GGIT_DIFF_FLAG_NOT_BINARY: if the file is not binary.
 * @GGIT_DIFF_FLAG_VALID_ID: if the ID is valid.
 *
 * Describes the diff file and/or delta flags
 */
typedef enum {
	GGIT_DIFF_FLAG_BINARY     = 1 << 0,
	GGIT_DIFF_FLAG_NOT_BINARY = 1 << 1,
	GGIT_DIFF_FLAG_VALID_ID   = 1 << 2
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
	GGIT_FILE_MODE_UNREADABLE      = 0000000,
	GGIT_FILE_MODE_TREE            = 0040000,
	GGIT_FILE_MODE_BLOB            = 0100644,
	GGIT_FILE_MODE_BLOB_EXECUTABLE = 0100755,
	GGIT_FILE_MODE_LINK            = 0120000,
	GGIT_FILE_MODE_COMMIT          = 0160000
} GgitFileMode;

/* NOTE: keep in sync with git2/merge.h */
/**
 * GgitMergeAutomergeMode:
 * @GGIT_MERGE_FILE_FAVOR_NORMAL: When a region of a file is changed in both branches,
 * a conflict will be recorded in the index so that `git_checkout` can produce
 * a merge file with conflict markers in the working directory. This is the default.
 * @GGIT_MERGE_FILE_FAVOR_OURS: When a region of a file is changed in both branches, the file
 * created in the index will contain the "ours" side of any conflicting
 * region. The index will not record a conflict.
 * @GGIT_MERGE_FILE_FAVOR_THEIRS: When a region of a file is changed in both branches, the file
 * created in the index will contain the "theirs" side of any conflicting
 * region. The index will not record a conflict.
 * @GGIT_MERGE_FILE_FAVOR_UNION: When a region of a file is changed in both branches, the file
 * created in the index will contain each unique line from each side,
 * which has the result of combining both files.  The index will not
 * record a conflict.
 */
typedef enum {
	GGIT_MERGE_FILE_FAVOR_NORMAL = 0,
	GGIT_MERGE_FILE_FAVOR_OURS   = 1,
	GGIT_MERGE_FILE_FAVOR_THEIRS = 2,
	GGIT_MERGE_FILE_FAVOR_UNION  = 3
} GgitMergeFileFavor;

/**
 * GgitMergeTreeFlags:
 * @GGIT_MERGE_TREE_FIND_RENAMES: detect renames.
 */
typedef enum {
	GGIT_MERGE_TREE_FIND_RENAMES = (1 << 0)
} GgitMergeTreeFlags;

/* NOTE: keep in sync with git2/refs.h */
/**
 * GgitRemoteDownloadTagsType:
 * @GGIT_REMOTE_DOWNLOAD_TAGS_UNSPECIFIED: Use the setting from the configuration.
 * @GGIT_REMOTE_DOWNLOAD_TAGS_AUTO: Ask the server for tags pointing to objects we're already
 * downloading.
 * @GGIT_REMOTE_DOWNLOAD_TAGS_NONE: Don't ask for any tags beyond the refspecs.
 * @GGIT_REMOTE_DOWNLOAD_TAGS_ALL: Ask for the all the tags.
 *
 * Automatic tag following option
 *
 * Lets us select the --tags option to use.
 */
typedef enum {
	GGIT_REMOTE_DOWNLOAD_TAGS_UNSPECIFIED = 0,
	GGIT_REMOTE_DOWNLOAD_TAGS_AUTO,
	GGIT_REMOTE_DOWNLOAD_TAGS_NONE,
	GGIT_REMOTE_DOWNLOAD_TAGS_ALL
} GgitRemoteDownloadTagsType;

/* NOTE: keep in sync with git2/remote.h */
/**
 * GgitRemoteCompletionType:
 * @GGIT_REMOTE_COMPLETION_DOWNLOAD: download.
 * @GGIT_REMOTE_COMPLETION_INDEXING: indexing.
 * @GGIT_REMOTE_COMPLETION_ERROR: error.
 *
 * Argument to the completion callback which tells it which operation finished.
 */
typedef enum {
	GGIT_REMOTE_COMPLETION_DOWNLOAD = 0,
	GGIT_REMOTE_COMPLETION_INDEXING = 1,
	GGIT_REMOTE_COMPLETION_ERROR    = 2
} GgitRemoteCompletionType;

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
 * @GGIT_SUBMODULE_IGNORE_UNSPECIFIED: reset to on-disk value.
 * @GGIT_SUBMODULE_IGNORE_NONE: don't ignore any change.
 * @GGIT_SUBMODULE_IGNORE_UNTRACKED: ignore untracked files.
 * @GGIT_SUBMODULE_IGNORE_DIRTY: ignore changes in the working directory.
 * @GGIT_SUBMODULE_IGNORE_ALL: never check if the submodule is dirty.
 *
 * Describes which submodules should be ignored.
 */
typedef enum {
	GGIT_SUBMODULE_IGNORE_UNSPECIFIED = -1,

	GGIT_SUBMODULE_IGNORE_NONE = 1,
	GGIT_SUBMODULE_IGNORE_UNTRACKED = 2,
	GGIT_SUBMODULE_IGNORE_DIRTY = 3,
	GGIT_SUBMODULE_IGNORE_ALL = 4
} GgitSubmoduleIgnore;

/**
 * GgitSubmoduleRecurse:
 * @GGIT_SUBMODULE_RECURSE_NO: do no recurse into submodules.
 * @GGIT_SUBMODULE_RECURSE_YES: recurse into submodules.
 * @GGIT_SUBMODULE_RECURSE_ONDEMAND: recurse into submodules only when
 * commit not already in local clone.
 *
 * Options for submodule recurse.
 * Represents the value of `submodule.$name.fetchRecurseSubmodules`
 */
typedef enum {
	GGIT_SUBMODULE_RECURSE_NO = 0,
	GGIT_SUBMODULE_RECURSE_YES = 1,
	GGIT_SUBMODULE_RECURSE_ONDEMAND = 2
} GgitSubmoduleRecurse;

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
 * @GGIT_SUBMODULE_UPDATE_NONE: do not update this submodule.
 * @GGIT_SUBMODULE_UPDATE_DEFAULT: not used except as static initializer
 * when we don't want any particular update rule to be specified.
 *
 * Describes how a submodule should be updated.
 */
typedef enum {
	GGIT_SUBMODULE_UPDATE_CHECKOUT = 1,
	GGIT_SUBMODULE_UPDATE_REBASE   = 2,
	GGIT_SUBMODULE_UPDATE_MERGE    = 3,
	GGIT_SUBMODULE_UPDATE_NONE     = 4,

	GGIT_SUBMODULE_UPDATE_DEFAULT = 0
} GgitSubmoduleUpdate;

/**
 * GgitTreeWalkMode:
 * @GGIT_TREE_WALK_MODE_PRE: walk tree in pre-order
 * @GGIT_TREE_WALK_MODE_POST: walk tree in post-order
 *
 * Describes in which order a tree walk should be performed.
 */
typedef enum {
	GGIT_TREE_WALK_MODE_PRE  = 0,
	GGIT_TREE_WALK_MODE_POST = 1
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
 * GGIT_STATUS_OPTION_RENAMES_HEAD_TO_INDEX: indicates that rename detection
 * should be processed between the head and the index and enables
 * the GIT_STATUS_INDEX_RENAMED as a possible status flag.
 * GGIT_STATUS_OPTION_RENAMES_INDEX_TO_WORKDIR: indicates tha rename
 * detection should be run between the index and the working directory
 * and enabled GIT_STATUS_WT_RENAMED as a possible status flag.
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
	GGIT_STATUS_OPTION_RENAMES_HEAD_TO_INDEX    = (1 << 7),
	GGIT_STATUS_OPTION_RENAMES_INDEX_TO_WORKDIR = (1 << 8),
	GGIT_STATUS_OPTION_SORT_CASE_SENSITIVELY    = (1 << 9),
	GGIT_STATUS_OPTION_SORT_CASE_INSENSITIVELY  = (1 << 10),
	GGIT_STATUS_OPTION_DEFAULT                  = GGIT_STATUS_OPTION_INCLUDE_IGNORED | GGIT_STATUS_OPTION_INCLUDE_UNTRACKED | GGIT_STATUS_OPTION_RECURSE_UNTRACKED_DIRS
} GgitStatusOption;

/**
 * GgitStatusShow:
 * @GGIT_STATUS_SHOW_INDEX_AND_WORKDIR: receive one callback for each file,
 * even if the file is in both the index and the workdir (combining #GgitStatusFlags).
 * @GGIT_STATUS_SHOW_INDEX_ONLY: only receive callbacks for files in the index.
 * @GGIT_STATUS_SHOW_WORKDIR_ONLY: only receive callbacks for files in the workdir.
 *
 * Show options for #ggit_repository_file_status_foreach. Determines which
 * files are included in the status.
 *
 */
typedef enum {
	GGIT_STATUS_SHOW_INDEX_AND_WORKDIR = 0,
	GGIT_STATUS_SHOW_INDEX_ONLY        = 1,
	GGIT_STATUS_SHOW_WORKDIR_ONLY      = 2
} GgitStatusShow;

/**
 * GgitAttributeCheckFlags:
 * @GGIT_ATTRIBUTE_CHECK_FILE_THEN_INDEX: check working directory, then index.
 * @GGIT_ATTRIBUTE_CHECK_INDEX_THEN_FILE: check index, then working directory.
 * @GGIT_ATTRIBUTE_CHECK_INDEX_ONLY: check only index.
 * @GGIT_ATTRIBUTE_CHECK_NO_SYSTEM: ignore system wide attributes.
 *
 * Attribute check flags indicate the order in which to check for gitattributes.
 * git core uses @GGIT_ATTRIBUTE_CHECK_FILE_THEN_INDEX for all operations,
 * except on checkout, where it uses @GGIT_ATTRIBUTE_CHECK_INDEX_THEN_FILE.
 *
 */
typedef enum
{
	GGIT_ATTRIBUTE_CHECK_FILE_THEN_INDEX = 0,
	GGIT_ATTRIBUTE_CHECK_INDEX_THEN_FILE = 1 << 0,
	GGIT_ATTRIBUTE_CHECK_INDEX_ONLY      = 1 << 1,
	GGIT_ATTRIBUTE_CHECK_NO_SYSTEM       = 1 << 2
} GgitAttributeCheckFlags;

/**
 * GgitPackbuilderStage:
 * @GGIT_PACKBUILDER_STAGE_ADDING_OBJECTS: adding objects.
 * @GGIT_PACKBUILDER_STAGE_DELTAFICATION: deltafication.
 *
 * Valid stages for pack building.
 */
typedef enum
{
	GGIT_PACKBUILDER_STAGE_ADDING_OBJECTS = 0,
	GGIT_PACKBUILDER_STAGE_DELTAFICATION  = 1
} GgitPackbuilderStage;

typedef enum
{
	GGIT_CHECKOUT_NONE                    = 0,
	GGIT_CHECKOUT_SAFE                    = (1u << 0),
	GGIT_CHECKOUT_FORCE                   = (1u << 1),
	GGIT_CHECKOUT_RECREATE_MISSING        = (1u << 2),
	GGIT_CHECKOUT_ALLOW_CONFLICTS         = (1u << 4),
	GGIT_CHECKOUT_REMOVE_UNTRACKED        = (1u << 5),
	GGIT_CHECKOUT_REMOVE_IGNORED          = (1u << 6),
	GGIT_CHECKOUT_UPDATE_ONLY             = (1u << 7),
	GGIT_CHECKOUT_DONT_UPDATE_INDEX       = (1u << 8),
	GGIT_CHECKOUT_NO_REFRESH              = (1u << 9),
	GGIT_CHECKOUT_SKIP_UNMERGED           = (1u << 10),
	GGIT_CHECKOUT_USE_OURS                = (1u << 11),
	GGIT_CHECKOUT_USE_THEIRS              = (1u << 12),
	GGIT_CHECKOUT_DISABLE_PATHSPEC_MATCH  = (1u << 13),
	GGIT_CHECKOUT_SKIP_LOCKED_DIRECTORIES = (1u << 18),
	GGIT_CHECKOUT_DONT_OVERWRITE_IGNORED  = (1u << 19),
	GGIT_CHECKOUT_CONFLICT_STYLE_MERGE    = (1u << 20),
	GGIT_CHECKOUT_CONFLICT_STYLE_DIFF3    = (1u << 21),
	GGIT_CHECKOUT_DONT_REMOVE_EXISTING    = (1u << 22),
	GGIT_CHECKOUT_DONT_WRITE_INDEX        = (1u << 23),
	GGIT_CHECKOUT_UPDATE_SUBMODULES       = (1u << 16),
	GGIT_CHECKOUT_UPDATE_SUBMODULES_IF_CHANGED = (1u << 17)
} GgitCheckoutStrategy;

typedef enum {
	GGIT_CHECKOUT_NOTIFY_NONE      = 0,
	GGIT_CHECKOUT_NOTIFY_CONFLICT  = (1u << 0),
	GGIT_CHECKOUT_NOTIFY_DIRTY     = (1u << 1),
	GGIT_CHECKOUT_NOTIFY_UPDATED   = (1u << 2),
	GGIT_CHECKOUT_NOTIFY_UNTRACKED = (1u << 3),
	GGIT_CHECKOUT_NOTIFY_IGNORED   = (1u << 4),

	GGIT_CHECKOUT_NOTIFY_ALL       = 0x0FFFFu
} GgitCheckoutNotifyFlags;

typedef enum {
	GGIT_DIFF_FIND_BY_CONFIG                  = 0,
	GGIT_DIFF_FIND_RENAMES                    = (1u << 0),
	GGIT_DIFF_FIND_RENAMES_FROM_REWRITES      = (1u << 1),
	GGIT_DIFF_FIND_COPIES                     = (1u << 2),
	GGIT_DIFF_FIND_COPIES_FROM_UNMODIFIED     = (1u << 3),
	GGIT_DIFF_FIND_REWRITES                   = (1u << 4),
	GGIT_DIFF_BREAK_REWRITES                  = (1u << 5),
	GGIT_DIFF_FIND_AND_BREAK_REWRITES         = (GGIT_DIFF_FIND_REWRITES |
	                                             GGIT_DIFF_BREAK_REWRITES),
	GGIT_DIFF_FIND_FOR_UNTRACKED              = (1u << 6),
	GGIT_DIFF_FIND_ALL                        = (0x0ff),
	GGIT_DIFF_FIND_IGNORE_LEADING_WHITESPACE  = 0,
	GGIT_DIFF_FIND_IGNORE_WHITESPACE          = (1u << 12),
	GGIT_DIFF_FIND_DONT_IGNORE_WHITESPACE     = (1u << 13),
	GGIT_DIFF_FIND_EXACT_MATCH_ONLY           = (1u << 14),
	GGIT_DIFF_BREAK_REWRITES_FOR_RENAMES_ONLY = (1u << 15),
	GGIT_DIFF_FIND_REMOVE_UNMODIFIED          = (1u << 16)
} GgitDiffFindFlags;

typedef enum
{
	GGIT_DIFF_FORMAT_EMAIL_NONE                         = 0,
	GGIT_DIFF_FORMAT_EMAIL_EXCLUDE_SUBJECT_PATCH_MARKER = (1 << 0)
} GgitDiffFormatEmailFlags;

typedef enum
{
	GGIT_CREDTYPE_USERPASS_PLAINTEXT = (1u << 0),
	GGIT_CREDTYPE_SSH_KEY            = (1u << 1),
	GGIT_CREDTYPE_SSH_CUSTOM         = (1u << 2),
	GGIT_CREDTYPE_DEFAULT            = (1u << 3),
	GGIT_CREDTYPE_SSH_INTERACTIVE    = (1u << 4),
} GgitCredtype;

typedef enum
{
	GGIT_DIRECTION_FETCH = 0,
	GGIT_DIRECTION_PUSH  = 1
} GgitDirection;

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
 * GgitDiffBinaryCallback:
 * @delta: a #GgitDiffDelta.
 * @binary: a #GgitDiffBinary.
 * @user_data: (closure): user-supplied data.
 *
 * Called for each file.
 *
 * Returns: 0 to go continue or a #GgitError in case there was an error.
 */
typedef gint (* GgitDiffBinaryCallback) (GgitDiffDelta  *delta,
                                         GgitDiffBinary *binary,
                                         gpointer        user_data);

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
 * @hunk: a #GgitDiffHunk.
 * @user_data: (closure): user-supplied data.
 *
 * Called for each hunk.
 *
 * Returns: 0 to go continue or a #GgitError in case there was an error.
 */
typedef gint (* GgitDiffHunkCallback) (GgitDiffDelta *delta,
                                       GgitDiffHunk  *hunk,
                                       gpointer       user_data);

/**
 * GgitDiffLineCallback:
 * @delta: a #GgitDiffDelta.
 * @hunk: a #GgitDiffHunk.
 * @line: a #GgitDiffLine.
 * @user_data: (closure): user-supplied data.
 *
 * Called for each line.
 *
 * Returns: 0 to go continue or a #GgitError in case there was an error.
 */
typedef gint (* GgitDiffLineCallback) (GgitDiffDelta    *delta,
                                       GgitDiffHunk     *hunk,
                                       GgitDiffLine     *line,
                                       gpointer          user_data);

/*
 * FIXME: request docs for this to libgit2
 */
typedef gint (* GgitDiffSimilarityMetricFileSignatureCallback) (GgitDiffFile *file,
                                                                const gchar  *fullpath,
                                                                gpointer     *out,
                                                                gpointer      user_data);


typedef gint (* GgitDiffSimilarityMetricBufferSignatureCallback) (GgitDiffFile *file,
                                                                  const gchar  *buf,
                                                                  gsize         buflen,
                                                                  gpointer     *out,
                                                                  gpointer      user_data);

typedef void (* GgitDiffSimilarityMetricFreeSignatureCallback) (gpointer signature,
                                                                gpointer user_data);

typedef gint (* GgitDiffSimilarityMetricSimilarityCallback) (gint     *score,
                                                             gpointer  signature_a,
                                                             gpointer  signature_b,
                                                             gpointer  user_data);

/**
 * GgitNoteCallback:
 * @blob_id: id of the blob containing the message.
 * @annotated_object_id: id of the git object being annotated.
 * @user_data: (closure): user-suplied data.
 *
 * When iterating over all the notes, callback that will be
 * issued per note. See ggit_repository_note_foreach().
 *
 * Returns: 0 to go continue or a #GgitError in case there was an error.
 */
typedef gint (* GgitNoteCallback) (GgitOId *blob_id,
                                   GgitOId *annotated_object_id,
                                   gpointer user_data);

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
                                    gpointer     user_data);

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
 * GgitTagCallback:
 * @name: the tag name.
 * @tag_oid: the tag oid.
 * @user_data: (closure): user-suplied data.
 *
 * When iterating over all the tags, callback that will be
 * issued per entry. See ggit_repository_tag_foreach().
 *
 * Returns: 0 to go continue or a #GgitError in case there was an error.
 */
typedef gint (* GgitTagCallback) (const gchar *name,
                                  GgitOId     *tag_oid,
                                  gpointer     user_data);

/**
 * GgitTransferProgressCallback:
 * @stats: a #GgitTransferProgress.
 * @user_data: (closure): user-supplied data.
 *
 * Progress callbacks during indexing.
 *
 * Returns: a value less than zero to cancel the transfer.
 */
typedef gint (* GgitTransferProgressCallback) (GgitTransferProgress *stats,
                                               gpointer              user_data);

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
