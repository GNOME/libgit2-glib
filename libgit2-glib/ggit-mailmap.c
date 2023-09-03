/*
 * ggit-mailmap.c
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2021 - Liliana Marie Prikler
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

#include <git2.h>

#include "ggit-error.h"
#include "ggit-mailmap.h"
#include "ggit-signature.h"

/**
 * GgitMailmap:
 *
 * Represents a mailmap.
 */
struct _GgitMailmap
{
	GgitNative parent_instance;
};

G_DEFINE_TYPE(GgitMailmap, ggit_mailmap, GGIT_TYPE_NATIVE)

static void
ggit_mailmap_finalize (GObject *object)
{
	G_OBJECT_CLASS (ggit_mailmap_parent_class)->finalize (object);
}

static void
ggit_mailmap_set_property (GObject      *object,
                           guint         prop_id,
                           const GValue *value,
                           GParamSpec   *pspec)
{
	GgitMailmap *mailmap = GGIT_MAILMAP (object);

	switch (prop_id)
	{
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void
ggit_mailmap_get_property (GObject    *object,
                           guint       prop_id,
                           GValue     *value,
                           GParamSpec *pspec)
{
	GgitMailmap *mailmap = GGIT_MAILMAP (object);

	switch (prop_id)
	{
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void
ggit_mailmap_class_init (GgitMailmapClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->finalize = ggit_mailmap_finalize;

	object_class->get_property = ggit_mailmap_get_property;
	object_class->set_property = ggit_mailmap_set_property;
}

static void
ggit_mailmap_init (GgitMailmap *signature)
{
}

GgitMailmap *
_ggit_mailmap_wrap (const git_mailmap *mailmap,
                    gboolean           owned)
{
	GgitMailmap *ret;

	ret = g_object_new (GGIT_TYPE_MAILMAP, NULL);

	if (owned)
	{
		_ggit_native_set (ret,
		                  (gpointer)mailmap,
		                  (GDestroyNotify)git_mailmap_free);
	}
	else
	{
		_ggit_native_set (ret, (gpointer)mailmap, NULL);
	}

	return ret;
}

/**
 * ggit_mailmap_new:
 * @error: a #GError for error reporting, or %NULL.
 *
 * Creates a new #GgitMailmap.
 *
 * Returns: (transfer full) (nullable): a newly allocated #GgitMailmap or %NULL.
 *
 **/
GgitMailmap *
ggit_mailmap_new (GError **error)
{
	GgitMailmap *mailmap = NULL;
	git_mailmap *map;
	int ret;

	ret = git_mailmap_new (&map);

	if (ret == GIT_OK)
	{
		mailmap = _ggit_mailmap_wrap (map, TRUE);
	}
	else
	{
		_ggit_error_set (error, ret);
	}

	return mailmap;
}

/**
 * ggit_mailmap_new_from_repository:
 * @repository: a #GgitRepository.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Creates a new #GgitMailmap and loads mailmap files according to the
 * configuration of @repository.
 *
 * Returns: (transfer full) (nullable): a newly allocated #GgitMailmap or %NULL.
 *
 **/
GgitMailmap *
ggit_mailmap_new_from_repository (GgitRepository *repository,
                                  GError        **error)
{
	GgitMailmap *mailmap = NULL;
	git_repository *repo;
	git_mailmap *map;
	int ret;

	repo = _ggit_native_get (repository);
	ret = git_mailmap_from_repository (&map, repo);

	if (ret == GIT_OK)
	{
		mailmap = _ggit_mailmap_wrap (map, TRUE);
	}
	else
	{
		_ggit_error_set (error, ret);
	}

	return mailmap;
}

/**
 * ggit_mailmap_add_entry:
 * @mailmap: the mailmap to add the entry in.
 * @real_name: (nullable): the real name of the person.
 * @real_email: (nullable): the real email of the person.
 * @replace_name: (nullable): the name to replace.
 * @replace_email: the email to replace.
 *
 * Adds a new entry to @mailmap, replacing @replace_name and @replace_email
 * with @real_name and @real_email respectively.  If @replace_name is %NULL,
 * it is ignored during matching.  If either @real_name or @real_email is %NULL,
 * the respective field is not replaced.
 *
 **/
void
ggit_mailmap_add_entry (GgitMailmap *mailmap,
                        const gchar *real_name,
                        const gchar *real_email,
                        const gchar *replace_name,
                        const gchar *replace_email,
                        GError **error)
{
	git_mailmap *map;
	int ret;

	map = _ggit_native_get (mailmap);
	ret = git_mailmap_add_entry (map, real_name, real_email, replace_name, replace_email);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
	}
}

/**
 * ggit_mailmap_resolve:
 * @mailmap: (nullable): the mailmap to search for replacements in.
 * @real_name: (out) (transfer none): the real name of the person.
 * @real_email: (out) (transfer none): the real email of the person.
 * @replace_name: the name to replace.
 * @replace_email: the email to replace.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Resolves @replace_name and @replace_email to @real_name and @real_email.
 * If @mailmap is #NULL, no substitution is performed.
 *
 **/
void
ggit_mailmap_resolve (GgitMailmap  *mailmap,
		      const gchar **real_name,
		      const gchar **real_email,
		      const gchar  *replace_name,
		      const gchar  *replace_email,
		      GError **error)
{
	git_mailmap *map;
	int ret;

	map = mailmap ? _ggit_native_get (mailmap) : NULL;
	ret = git_mailmap_resolve (real_name, real_email, map, replace_name, replace_email);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
	}
}

/**
 * ggit_mailmap_resolve_signature:
 * @mailmap: (nullable): the mailmap to resolve names and emails with.
 * @signature: the commit signature as recorded.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Resolves @signature to use the real name and email according to @mailmap.
 * If @mailmap is #NULL, no substitution is performed, but a new signature is
 * still allocated and returned.
 *
 * Returns: (transfer full) (nullable): The corrected signature or %NULL.
 **/
GgitSignature *
ggit_mailmap_resolve_signature (GgitMailmap   *mailmap,
                                GgitSignature *signature,
                                GError       **error)
{
	GgitSignature *resolved = NULL;
	git_mailmap *map;
	git_signature *sig, *res;
	int ret;

	map = mailmap ? _ggit_native_get (mailmap) : NULL;
	sig = _ggit_native_get (signature);
	ret = git_mailmap_resolve_signature (&res, map, sig);

	if (ret == GIT_OK)
	{
		resolved = _ggit_signature_wrap (res, "UTF-8", TRUE);
	}
	else
	{
		_ggit_error_set (error, ret);
	}

	return resolved;
}
