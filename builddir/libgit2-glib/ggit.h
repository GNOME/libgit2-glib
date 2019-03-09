/*
 * ggit.h
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

#ifndef __GGIT_H__
#define __GGIT_H__

#include <libgit2-glib/ggit-annotated-commit.h>
#include <libgit2-glib/ggit-blob.h>
#include <libgit2-glib/ggit-blob-output-stream.h>
#include <libgit2-glib/ggit-branch-enumerator.h>
#include <libgit2-glib/ggit-branch.h>
#include <libgit2-glib/ggit-clone-options.h>
#include <libgit2-glib/ggit-commit.h>
#include <libgit2-glib/ggit-commit-parents.h>
#include <libgit2-glib/ggit-config-entry.h>
#include <libgit2-glib/ggit-config.h>
#include <libgit2-glib/ggit-cred.h>
#include <libgit2-glib/ggit-cred-plaintext.h>
#include <libgit2-glib/ggit-diff-delta.h>
#include <libgit2-glib/ggit-diff-file.h>
#include <libgit2-glib/ggit-diff-format-email-options.h>
#include <libgit2-glib/ggit-diff.h>
#include <libgit2-glib/ggit-diff-hunk.h>
#include <libgit2-glib/ggit-diff-line.h>
#include <libgit2-glib/ggit-diff-options.h>
#include <libgit2-glib/ggit-diff-similarity-metric.h>
#include <libgit2-glib/ggit-enum-types.h>
#include <libgit2-glib/ggit-error.h>
#include <libgit2-glib/ggit-fetch-options.h>
#include <libgit2-glib/ggit-index-entry.h>
#include <libgit2-glib/ggit-index-entry-resolve-undo.h>
#include <libgit2-glib/ggit-index.h>
#include <libgit2-glib/ggit-main.h>
#include <libgit2-glib/ggit-merge-options.h>
#include <libgit2-glib/ggit-message.h>
#include <libgit2-glib/ggit-native.h>
#include <libgit2-glib/ggit-object-factory-base.h>
#include <libgit2-glib/ggit-object-factory.h>
#include <libgit2-glib/ggit-object.h>
#include <libgit2-glib/ggit-oid.h>
#include <libgit2-glib/ggit-patch.h>
#include <libgit2-glib/ggit-rebase-operation.h>
#include <libgit2-glib/ggit-rebase-options.h>
#include <libgit2-glib/ggit-rebase.h>
#include <libgit2-glib/ggit-ref.h>
#include <libgit2-glib/ggit-reflog-entry.h>
#include <libgit2-glib/ggit-reflog.h>
#include <libgit2-glib/ggit-ref-spec.h>
#include <libgit2-glib/ggit-remote-callbacks.h>
#include <libgit2-glib/ggit-remote.h>
#include <libgit2-glib/ggit-repository.h>
#include <libgit2-glib/ggit-revision-walker.h>
#include <libgit2-glib/ggit-signature.h>
#include <libgit2-glib/ggit-status-options.h>
#include <libgit2-glib/ggit-submodule.h>
#include <libgit2-glib/ggit-submodule-update-options.h>
#include <libgit2-glib/ggit-tag.h>
#include <libgit2-glib/ggit-transfer-progress.h>
#include <libgit2-glib/ggit-tree-builder.h>
#include <libgit2-glib/ggit-tree-entry.h>
#include <libgit2-glib/ggit-tree.h>
#include <libgit2-glib/ggit-types.h>

#include <libgit2-glib/ggit-cred-ssh-key-from-agent.h>
#include <libgit2-glib/ggit-cred-ssh-interactive.h>

#endif

/* ex:set ts=8 noet: */
