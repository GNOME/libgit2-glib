/*
 * ggit-repository.h
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2011 - Ignacio Casal Quinteiro
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


#ifndef __GGIT_REPOSITORY_H__
#define __GGIT_REPOSITORY_H__

#include <glib-object.h>
#include <git2.h>
#include <gio/gio.h>
#include <libgit2-glib/ggit-config.h>
#include <libgit2-glib/ggit-index.h>
#include <libgit2-glib/ggit-types.h>
#include <libgit2-glib/ggit-native.h>
#include <libgit2-glib/ggit-object.h>
#include <libgit2-glib/ggit-tree.h>
#include <libgit2-glib/ggit-branch.h>
#include <libgit2-glib/ggit-blob-output-stream.h>
#include <libgit2-glib/ggit-checkout-options.h>
#include <libgit2-glib/ggit-note.h>

G_BEGIN_DECLS

#define GGIT_TYPE_REPOSITORY			(ggit_repository_get_type ())
#define GGIT_REPOSITORY(obj)			(G_TYPE_CHECK_INSTANCE_CAST ((obj), GGIT_TYPE_REPOSITORY, GgitRepository))
#define GGIT_REPOSITORY_CLASS(klass)		(G_TYPE_CHECK_CLASS_CAST ((klass), GGIT_TYPE_REPOSITORY, GgitRepositoryClass))
#define GGIT_IS_REPOSITORY(obj)			(G_TYPE_CHECK_INSTANCE_TYPE ((obj), GGIT_TYPE_REPOSITORY))
#define GGIT_IS_REPOSITORY_CLASS(klass)		(G_TYPE_CHECK_CLASS_TYPE ((klass), GGIT_TYPE_REPOSITORY))
#define GGIT_REPOSITORY_GET_CLASS(obj)		(G_TYPE_INSTANCE_GET_CLASS ((obj), GGIT_TYPE_REPOSITORY, GgitRepositoryClass))

typedef struct _GgitRepositoryClass	GgitRepositoryClass;
typedef struct _GgitRepositoryPrivate	GgitRepositoryPrivate;

struct _GgitRepository
{
	/*< private >*/
	GgitNative parent;

	GgitRepositoryPrivate *priv;
};

/**
 * GgitRepositoryClass:
 * @parent_class: The parent class.
 *
 * The class structure for #GgitRepositoryClass.
 */
struct _GgitRepositoryClass
{
	/*< private >*/
	GgitNativeClass parent_class;
};

GType               ggit_repository_get_type          (void) G_GNUC_CONST;

GgitRepository     *_ggit_repository_wrap             (git_repository        *repository,
                                                       gboolean               owned);

git_repository     *_ggit_repository_get_repository   (GgitRepository        *repository);

GgitRepository     *ggit_repository_open              (GFile                 *location,
                                                       GError               **error);

GgitRepository     *ggit_repository_init_repository   (GFile                 *location,
                                                       gboolean               is_bare,
                                                       GError               **error);

GgitRepository     *ggit_repository_clone             (const gchar           *url,
                                                       GFile                 *location,
                                                       GgitCloneOptions      *options,
                                                       GError               **error);

GgitObject         *ggit_repository_lookup            (GgitRepository        *repository,
                                                       GgitOId               *oid,
                                                       GType                  gtype,
                                                       GError               **error);

GgitRef            *ggit_repository_lookup_reference  (GgitRepository        *repository,
                                                       const gchar           *name,
                                                       GError               **error);

GgitRef            *ggit_repository_create_reference  (GgitRepository        *repository,
                                                       const gchar           *name,
                                                       GgitOId               *oid,
                                                       const gchar           *log_message,
                                                       GError               **error);

GgitRef            *ggit_repository_create_symbolic_reference
                                                      (GgitRepository        *repository,
                                                       const gchar           *name,
                                                       const gchar           *target,
                                                       const gchar           *log_message,
                                                       GError               **error);

GgitBlobOutputStream *
                    ggit_repository_create_blob       (GgitRepository        *repository);


GgitOId            *ggit_repository_create_blob_from_buffer (
                                                       GgitRepository        *repository,
                                                       gconstpointer          buffer,
                                                       gsize                  size,
                                                       GError               **error);

GgitOId            *ggit_repository_create_blob_from_file (
                                                       GgitRepository        *repository,
                                                       GFile                 *file,
                                                       GError               **error);

GgitOId            *ggit_repository_create_blob_from_path (
                                                       GgitRepository        *repository,
                                                       const gchar           *path,
                                                       GError               **error);

GgitOId            *ggit_repository_create_commit     (GgitRepository        *repository,
                                                       const gchar           *update_ref,
                                                       GgitSignature         *author,
                                                       GgitSignature         *committer,
                                                       const gchar           *message_encoding,
                                                       const gchar           *message,
                                                       GgitTree              *tree,
                                                       GgitCommit           **parents,
                                                       gint                   parent_count,
                                                       GError               **error);

GgitOId            *ggit_repository_create_commit_from_ids (
                                                       GgitRepository        *repository,
                                                       const gchar           *update_ref,
                                                       GgitSignature         *author,
                                                       GgitSignature         *committer,
                                                       const gchar           *message_encoding,
                                                       const gchar           *message,
                                                       GgitOId               *tree,
                                                       GgitOId              **parents,
                                                       gint                   parent_count,
                                                       GError               **error);

GgitOId            *ggit_repository_create_tag        (GgitRepository        *repository,
                                                       const gchar           *tag_name,
                                                       GgitObject            *target,
                                                       GgitSignature         *tagger,
                                                       const gchar           *message,
                                                       GgitCreateFlags        flags,
                                                       GError               **error);

GgitOId            *ggit_repository_create_tag_from_buffer (
                                                       GgitRepository        *repository,
                                                       const gchar           *tag,
                                                       GgitCreateFlags        flags,
                                                       GError               **error);

GgitOId            *ggit_repository_create_tag_lightweight (
                                                       GgitRepository        *repository,
                                                       const gchar           *tag_name,
                                                       GgitObject            *target,
                                                       GgitCreateFlags        flags,
                                                       GError               **error);

GgitOId            *ggit_repository_create_tag_annotation (
                                                       GgitRepository        *repository,
                                                       const gchar           *tag_name,
                                                       GgitObject            *target,
                                                       GgitSignature         *signature,
                                                       const gchar           *message,
                                                       GError               **error);

gboolean            ggit_repository_delete_tag        (GgitRepository        *repository,
                                                       const gchar           *name,
                                                       GError               **error);

gboolean            ggit_repository_tag_foreach       (GgitRepository        *repository,
                                                       GgitTagCallback        callback,
                                                       gpointer               user_data,
                                                       GError               **error);

GgitTreeBuilder    *ggit_repository_create_tree_builder (
                                                       GgitRepository        *repository,
                                                       GError               **error);

GgitTreeBuilder    *ggit_repository_create_tree_builder_from_tree (
                                                       GgitRepository        *repository,
                                                       GgitTree              *tree,
                                                       GError               **error);

GgitIndexEntry     *ggit_repository_create_index_entry_for_file (
                                                       GgitRepository        *repository,
                                                       GFile                 *file,
                                                       GgitOId               *id,
                                                       GError               **error);

GgitIndexEntry     *ggit_repository_create_index_entry_for_path (
                                                       GgitRepository        *repository,
                                                       const gchar           *path,
                                                       GgitOId               *id,
                                                       GError               **error);

gchar             **ggit_repository_list_tags         (GgitRepository        *repository,
                                                       GError               **error);

gchar             **ggit_repository_list_tags_match   (GgitRepository        *repository,
                                                       const gchar           *pattern,
                                                       GError               **error);

GgitBranch         *ggit_repository_create_branch     (GgitRepository        *repository,
                                                       const gchar           *branch_name,
                                                       GgitObject            *target,
                                                       GgitCreateFlags        flags,
                                                       GError               **error);

GgitBranchEnumerator *ggit_repository_enumerate_branches (GgitRepository        *repository,
                                                          GgitBranchType         list_type,
                                                          GError               **error);

GgitBranch         *ggit_repository_lookup_branch     (GgitRepository        *repository,
                                                       const gchar           *branch_name,
                                                       GgitBranchType         branch_type,
                                                       GError               **error);

GgitRemote         *ggit_repository_lookup_remote     (GgitRepository        *repository,
                                                       const gchar           *name,
                                                       GError               **error);

GgitRemote         *ggit_repository_create_remote     (GgitRepository        *repository,
                                                       const gchar           *name,
                                                       const gchar           *url,
                                                       GError               **error);

gchar             **ggit_repository_list_remotes      (GgitRepository        *repository,
                                                       GError               **error);

void                ggit_repository_add_remote_push   (GgitRepository        *repository,
                                                       GgitRemote            *remote,
                                                       const gchar           *push_spec,
                                                       GError               **error);

GgitRef            *ggit_repository_get_head          (GgitRepository        *repository,
                                                       GError               **error);

GFile              *ggit_repository_discover          (GFile                 *location,
                                                       GError               **error);

gboolean            ggit_repository_is_head_detached  (GgitRepository        *repository,
                                                       GError               **error);

gboolean            ggit_repository_is_head_unborn    (GgitRepository        *repository,
                                                       GError               **error);

gboolean            ggit_repository_is_empty          (GgitRepository        *repository,
                                                       GError               **error);

GFile              *ggit_repository_get_location      (GgitRepository        *repository);
GFile              *ggit_repository_get_workdir       (GgitRepository        *repository);

void                ggit_repository_set_workdir       (GgitRepository        *repository,
                                                       GFile                 *workdir,
                                                       gboolean               update_gitlink);

gboolean            ggit_repository_is_bare           (GgitRepository        *repository);

GgitStatusFlags     ggit_repository_file_status       (GgitRepository        *repository,
                                                       GFile                 *location,
                                                       GError               **error);

gboolean            ggit_repository_file_status_foreach
                                                     (GgitRepository         *repository,
                                                      GgitStatusOptions      *options,
                                                      GgitStatusCallback      callback,
                                                      gpointer                user_data,
                                                      GError                **error);

gboolean            ggit_repository_references_foreach_name (GgitRepository             *repository,
                                                             GgitReferencesNameCallback  callback,
                                                             gpointer                    user_data,
                                                             GError                    **error);

GgitConfig         *ggit_repository_get_config         (GgitRepository          *repository,
                                                        GError                 **error);

GgitIndex          *ggit_repository_get_index          (GgitRepository          *repository,
                                                        GError                 **error);

GgitSubmodule      *ggit_repository_lookup_submodule   (GgitRepository          *repository,
                                                        const gchar             *name,
                                                        GError                 **error);

gboolean            ggit_repository_submodule_foreach  (GgitRepository          *repository,
                                                        GgitSubmoduleCallback    callback,
                                                        gpointer                 user_data,
                                                        GError                 **error);

void                ggit_repository_reset              (GgitRepository          *repository,
                                                        GgitObject              *target,
                                                        GgitResetType            reset_type,
                                                        GgitCheckoutOptions     *checkout_options,
                                                        GError                 **error);

void                ggit_repository_reset_default      (GgitRepository          *repository,
                                                        GgitObject              *target,
                                                        const gchar * const     *pathspecs,
                                                        GError                 **error);

GgitObject         *ggit_repository_revparse          (GgitRepository        *repository,
                                                       const gchar           *spec,
                                                       GError               **error);

GgitOId            *ggit_repository_save_stash        (GgitRepository        *repository,
                                                       GgitSignature         *stasher,
                                                       const gchar           *message,
                                                       GgitStashFlags         flags,
                                                       GError               **error);

void                ggit_repository_drop_stash        (GgitRepository        *repository,
                                                       gsize                  index,
                                                       GError               **error);

gboolean            ggit_repository_stash_foreach     (GgitRepository        *repository,
                                                       GgitStashCallback      callback,
                                                       gpointer               user_data,
                                                       GError               **error);

void                ggit_repository_get_ahead_behind  (GgitRepository        *repository,
                                                       GgitOId               *local,
                                                       GgitOId               *upstream,
                                                       gsize                 *ahead,
                                                       gsize                 *behind,
                                                       GError               **error);

gboolean            ggit_repository_get_descendant_of (GgitRepository        *repository,
                                                       GgitOId               *commit,
                                                       GgitOId               *ancestor,
                                                       GError               **error);

GgitBlame          *ggit_repository_blame_file        (GgitRepository        *repository,
                                                       GFile                 *file,
                                                       GgitBlameOptions      *blame_options,
                                                       GError               **error);

const gchar        *ggit_repository_get_attribute     (GgitRepository           *repository,
                                                       const gchar              *path,
                                                       const gchar              *name,
                                                       GgitAttributeCheckFlags   flags,
                                                       GError                  **error);

gboolean            ggit_repository_checkout_head     (GgitRepository           *repository,
                                                       GgitCheckoutOptions      *options,
                                                       GError                  **error);

gboolean            ggit_repository_checkout_index    (GgitRepository           *repository,
                                                       GgitIndex                *index,
                                                       GgitCheckoutOptions      *options,
                                                       GError                  **error);

gboolean            ggit_repository_checkout_tree     (GgitRepository           *repository,
                                                       GgitObject               *tree,
                                                       GgitCheckoutOptions      *options,
                                                       GError                  **error);

gboolean            ggit_repository_revert            (GgitRepository           *repository,
                                                       GgitCommit               *commit,
                                                       GgitRevertOptions        *options,
                                                       GError                  **error);

gboolean            ggit_repository_cherry_pick       (GgitRepository           *repository,
                                                       GgitCommit               *commit,
                                                       GgitCherryPickOptions    *options,
                                                       GError                  **error);

GgitIndex          *ggit_repository_cherry_pick_commit (GgitRepository           *repository,
                                                        GgitCommit               *commit,
                                                        GgitCommit               *our_commit,
                                                        guint                     mainline,
                                                        GgitMergeOptions         *merge_options,
                                                        GError                  **error);

const gchar        *ggit_repository_get_default_notes_ref (GgitRepository           *repository,
                                                           GError                  **error);

GgitOId            *ggit_repository_create_note        (GgitRepository          *repository,
                                                        const gchar             *notes_ref,
                                                        GgitSignature           *author,
                                                        GgitSignature           *committer,
                                                        GgitOId                 *id,
                                                        const gchar             *note,
                                                        gboolean                 force,
                                                        GError                 **error);

gboolean            ggit_repository_remove_note        (GgitRepository          *repository,
                                                        const gchar             *notes_ref,
                                                        GgitSignature           *author,
                                                        GgitSignature           *committer,
                                                        GgitOId                 *id,
                                                        GError                 **error);

GgitNote           *ggit_repository_read_note          (GgitRepository          *repository,
                                                        const gchar             *notes_ref,
                                                        GgitOId                 *id,
                                                        GError                 **error);

gboolean            ggit_repository_note_foreach       (GgitRepository          *repository,
                                                        const gchar             *notes_ref,
                                                        GgitNoteCallback         callback,
                                                        gpointer                 user_data,
                                                        GError                 **error);
gboolean           ggit_repository_path_is_ignored     (GgitRepository          *repository,
                                                        const gchar             *path,
                                                        GError                 **error);

G_END_DECLS

#endif /* __GGIT_REPOSITORY_H__ */

/* ex:set ts=8 noet: */
