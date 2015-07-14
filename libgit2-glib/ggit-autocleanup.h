/*
 * ggit-autocleanup.h
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2015 - Garrett Regier
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

#ifndef __GGIT_AUTOCLEANUP_H__
#define __GGIT_AUTOCLEANUP_H__

#include <glib-object.h>

#include <libgit2-glib/ggit-types.h>

#include <libgit2-glib/ggit-blame.h>
#include <libgit2-glib/ggit-blame-options.h>
#include <libgit2-glib/ggit-branch-enumerator.h>
#include <libgit2-glib/ggit-clone-options.h>
#include <libgit2-glib/ggit-config-entry.h>
#include <libgit2-glib/ggit-cred-ssh-interactive.h>
#include <libgit2-glib/ggit-diff-delta.h>
#include <libgit2-glib/ggit-diff-file.h>
#include <libgit2-glib/ggit-diff-hunk.h>
#include <libgit2-glib/ggit-diff-line.h>
#include <libgit2-glib/ggit-diff-similarity-metric.h>
#include <libgit2-glib/ggit-index-entry.h>
#include <libgit2-glib/ggit-index-entry-resolve-undo.h>
#include <libgit2-glib/ggit-merge-options.h>
#include <libgit2-glib/ggit-note.h>
#include <libgit2-glib/ggit-oid.h>
#include <libgit2-glib/ggit-patch.h>
#include <libgit2-glib/ggit-reflog.h>
#include <libgit2-glib/ggit-reflog-entry.h>
#include <libgit2-glib/ggit-ref-spec.h>
#include <libgit2-glib/ggit-remote.h>
#include <libgit2-glib/ggit-revert-options.h>
#include <libgit2-glib/ggit-status-options.h>
#include <libgit2-glib/ggit-submodule.h>
#include <libgit2-glib/ggit-transfer-progress.h>
#include <libgit2-glib/ggit-tree-entry.h>

G_BEGIN_DECLS

#ifndef __GI_SCANNER__
#if GLIB_CHECK_VERSION (2, 44, 0)

G_DEFINE_AUTOPTR_CLEANUP_FUNC (GgitBlameHunk, ggit_blame_hunk_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (GgitBlameOptions, ggit_blame_options_free)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (GgitBlobOutputStream, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (GgitBranch, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (GgitBranchEnumerator, ggit_branch_enumerator_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (GgitCheckoutOptions, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (GgitCherryPickOptions, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (GgitCloneOptions, ggit_clone_options_free)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (GgitCommit, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (GgitCommitParents, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (GgitConfig, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (GgitConfigEntry, ggit_config_entry_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (GgitCred, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (GgitCredPlaintext, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (GgitCredSshInteractive, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (GgitCredSshInteractivePrompt, ggit_cred_ssh_interactive_prompt_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (GgitCredSshKeyFromAgent, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (GgitDiffDelta, ggit_diff_delta_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (GgitDiffFile, ggit_diff_file_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (GgitDiffFindOptions, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (GgitDiffFormatEmailOptions, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (GgitDiff, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (GgitDiffHunk, ggit_diff_hunk_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (GgitDiffLine, ggit_diff_line_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (GgitDiffOptions, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (GgitDiffSimilarityMetric, ggit_diff_similarity_metric_free)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (GgitIndexEntries, ggit_index_entries_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (GgitIndexEntriesResolveUndo, ggit_index_entries_resolve_undo_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (GgitIndexEntry, ggit_index_entry_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (GgitIndexEntryResolveUndo, ggit_index_entry_resolve_undo_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (GgitIndex, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (GgitMergeOptions, ggit_merge_options_free)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (GgitNote, ggit_note_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (GgitOId, ggit_oid_free)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (GgitPatch, ggit_patch_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (GgitPushOptions, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (GgitReflog, ggit_reflog_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (GgitReflogEntry, ggit_reflog_entry_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (GgitRefSpec, ggit_ref_spec_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (GgitRemote, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (GgitRemoteCallbacks, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (GgitRemoteHead, ggit_remote_head_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (GgitRepository, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (GgitRevertOptions, ggit_revert_options_free)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (GgitRevisionWalker, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (GgitSignature, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (GgitStatusOptions, ggit_status_options_free)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (GgitSubmodule, ggit_submodule_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (GgitSubmoduleUpdateOptions, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (GgitTag, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (GgitTransferProgress, ggit_transfer_progress_free)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (GgitTree, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (GgitTreeBuilder, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (GgitTreeEntry, ggit_tree_entry_unref)

#endif /* GLIB_CHECK_VERSION (2, 44, 0) */
#endif /* __GI_SCANNER__ */

G_END_DECLS

#endif /* __GGIT_AUTOCLEANUP_H__ */
