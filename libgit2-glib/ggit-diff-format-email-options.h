/*
 * ggit-diff_format_email-options.h
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2013 - Ignacio Casal Quinteiro
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

#ifndef __GGIT_DIFF_FORMAT_EMAIL_OPTIONS_H__
#define __GGIT_DIFF_FORMAT_EMAIL_OPTIONS_H__

#include <glib-object.h>
#include <git2.h>

#include "ggit-types.h"
#include "ggit-signature.h"

G_BEGIN_DECLS

#define GGIT_TYPE_DIFF_FORMAT_EMAIL_OPTIONS		(ggit_diff_format_email_options_get_type ())
#define GGIT_DIFF_FORMAT_EMAIL_OPTIONS(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), GGIT_TYPE_DIFF_FORMAT_EMAIL_OPTIONS, GgitDiffFormatEmailOptions))
#define GGIT_DIFF_FORMAT_EMAIL_OPTIONS_CONST(obj)	(G_TYPE_CHECK_INSTANCE_CAST ((obj), GGIT_TYPE_DIFF_FORMAT_EMAIL_OPTIONS, GgitDiffFormatEmailOptions const))
#define GGIT_DIFF_FORMAT_EMAIL_OPTIONS_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST ((klass), GGIT_TYPE_DIFF_FORMAT_EMAIL_OPTIONS, GgitDiffFormatEmailOptionsClass))
#define GGIT_IS_DIFF_FORMAT_EMAIL_OPTIONS(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), GGIT_TYPE_DIFF_FORMAT_EMAIL_OPTIONS))
#define GGIT_IS_DIFF_FORMAT_EMAIL_OPTIONS_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), GGIT_TYPE_DIFF_FORMAT_EMAIL_OPTIONS))
#define GGIT_DIFF_FORMAT_EMAIL_OPTIONS_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS ((obj), GGIT_TYPE_DIFF_FORMAT_EMAIL_OPTIONS, GgitDiffFormatEmailOptionsClass))

typedef struct _GgitDiffFormatEmailOptionsClass	GgitDiffFormatEmailOptionsClass;
typedef struct _GgitDiffFormatEmailOptionsPrivate	GgitDiffFormatEmailOptionsPrivate;

struct _GgitDiffFormatEmailOptions
{
	GObject parent;

	GgitDiffFormatEmailOptionsPrivate *priv;
};

struct _GgitDiffFormatEmailOptionsClass
{
	GObjectClass parent_class;
};

GType                  ggit_diff_format_email_options_get_type (void) G_GNUC_CONST;

const git_diff_format_email_options *
              _ggit_diff_format_email_options_get_diff_format_email_options (
                                         GgitDiffFormatEmailOptions *options);

GgitDiffFormatEmailOptions *
               ggit_diff_format_email_options_new        (void);

GgitDiffFormatEmailFlags
               ggit_diff_format_email_options_get_flags  (
                                         GgitDiffFormatEmailOptions  *options);
void           ggit_diff_format_email_options_set_flags  (
                                         GgitDiffFormatEmailOptions  *options,
                                         GgitDiffFormatEmailFlags     flags);

gsize          ggit_diff_format_email_options_get_patch_number (
                                         GgitDiffFormatEmailOptions  *options);
void           ggit_diff_format_email_options_set_patch_number (
                                         GgitDiffFormatEmailOptions  *options,
                                         gsize                        number);

gsize          ggit_diff_format_email_options_get_total_patches (
                                         GgitDiffFormatEmailOptions  *options);
void           ggit_diff_format_email_options_set_total_patches (
                                         GgitDiffFormatEmailOptions  *options,
                                         gsize                        patches);

GgitOId       *ggit_diff_format_email_options_get_id (
                                         GgitDiffFormatEmailOptions  *options);
void           ggit_diff_format_email_options_set_id (
                                         GgitDiffFormatEmailOptions  *options,
                                         GgitOId                     *id);

const gchar   *ggit_diff_format_email_options_get_summary (
                                         GgitDiffFormatEmailOptions  *options);
void           ggit_diff_format_email_options_set_summary (
                                         GgitDiffFormatEmailOptions  *options,
                                         const gchar                 *summary);

GgitSignature *ggit_diff_format_email_options_get_author (
                                         GgitDiffFormatEmailOptions  *options);
void           ggit_diff_format_email_options_set_author (
                                         GgitDiffFormatEmailOptions  *options,
                                         GgitSignature               *author);

G_END_DECLS

#endif /* __GGIT_DIFF_FORMAT_EMAIL_OPTIONS_H__ */

/* ex:set ts=8 noet: */
