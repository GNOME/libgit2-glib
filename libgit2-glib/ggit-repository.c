/*
 * ggit-repository.c
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

#include <gio/gio.h>
#include <git2.h>
#include <git2/sys/commit.h>

#include "ggit-error.h"
#include "ggit-oid.h"
#include "ggit-ref.h"
#include "ggit-repository.h"
#include "ggit-utils.h"
#include "ggit-remote.h"
#include "ggit-submodule.h"
#include "ggit-signature.h"
#include "ggit-clone-options.h"
#include "ggit-status-options.h"
#include "ggit-tree-builder.h"
#include "ggit-branch-enumerator.h"
#include "ggit-blame.h"
#include "ggit-blame-options.h"
#include "ggit-commit.h"
#include "ggit-revert-options.h"
#include "ggit-cherry-pick-options.h"
#include "ggit-merge-options.h"
#include "ggit-index-entry.h"
#include "ggit-annotated-commit.h"
#include "ggit-rebase-options.h"
#include "ggit-blob.h"
#include "ggit-tag.h"


typedef struct _GgitRepositoryPrivate
{
	gchar *url;
	GFile *location;
	GFile *workdir;

	GgitCloneOptions *clone_options;

	guint is_bare : 1;
	guint init : 1;
} GgitRepositoryPrivate;

enum
{
	PROP_0,
	PROP_URL,
	PROP_LOCATION,
	PROP_IS_BARE,
	PROP_INIT,
	PROP_WORKDIR,
	PROP_HEAD,
	PROP_CLONE_OPTIONS
};

static void ggit_repository_initable_iface_init (GInitableIface  *iface);

G_DEFINE_TYPE_EXTENDED (GgitRepository, ggit_repository, GGIT_TYPE_NATIVE,
                        0,
                        G_ADD_PRIVATE (GgitRepository)
                        G_IMPLEMENT_INTERFACE (G_TYPE_INITABLE,
                                               ggit_repository_initable_iface_init))

static GHashTable *registry = NULL;

static GgitRepository *
repository_from_registry (git_repository *repository)
{
	if (registry == NULL)
	{
		return NULL;
	}

	return g_hash_table_lookup (registry, repository);
}

static void
register_repository (git_repository *repository,
                     GgitRepository *wrapper)
{
	if (registry == NULL)
	{
		registry = g_hash_table_new (g_direct_hash, g_direct_equal);
	}

	g_hash_table_insert (registry, repository, wrapper);
}

static void
unregister_repository (git_repository *repository)
{
	if (registry == NULL)
	{
		return;
	}

	g_hash_table_remove (registry, repository);

	if (g_hash_table_size (registry) == 0)
	{
		g_hash_table_destroy (registry);
		registry = NULL;
	}
}

/**
 * ggit_repository_path_is_ignored:
 * @repository: A #GgitRepository.
 * @path: A path within the repository.
 * @error: (allow-none): A location for a #GError, or %NULL.
 *
 * Tests if the ignore rules apply to the path provided.
 *
 * This acts similar to filtering performed when calling "git add ."
 * on the command line.
 *
 * Returns: %TRUE if @path should be ignored.
 */
gboolean
ggit_repository_path_is_ignored (GgitRepository  *repository,
                                 const gchar     *path,
				 GError         **error)
{
	int ignored = FALSE;
	int ret;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), FALSE);
	g_return_val_if_fail (path != NULL, FALSE);

	ret = git_ignore_path_is_ignored (&ignored, _ggit_native_get (repository), path);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return FALSE;
	}

	return !!ignored;
}

static void
ggit_repository_finalize (GObject *object)
{
	GgitRepository *repository = GGIT_REPOSITORY (object);
	GgitRepositoryPrivate *priv;
	git_repository *repo;

	priv = ggit_repository_get_instance_private (repository);

	g_free (priv->url);
	g_clear_object (&priv->location);
	g_clear_object (&priv->workdir);
	g_clear_object (&priv->clone_options);

	repo = _ggit_native_get (object);

	if (repo != NULL)
	{
		unregister_repository (repo);
	}

	G_OBJECT_CLASS (ggit_repository_parent_class)->finalize (object);
}

static void
ggit_repository_get_property (GObject    *object,
                              guint       prop_id,
                              GValue     *value,
                              GParamSpec *pspec)
{
	GgitRepository *repository = GGIT_REPOSITORY (object);
	GgitRepositoryPrivate *priv;

	priv = ggit_repository_get_instance_private (repository);

	switch (prop_id)
	{
		case PROP_URL:
			g_value_set_string (value, priv->url);
			break;
		case PROP_LOCATION:
			g_value_take_object (value,
			                     ggit_repository_get_location (repository));
			break;
		case PROP_IS_BARE:
			g_value_set_boolean (value,
			                     ggit_repository_is_bare (repository));
			break;
		case PROP_INIT:
			g_value_set_boolean (value,
			                     priv->init);
			break;
		case PROP_WORKDIR:
			g_value_take_object (value,
			                    ggit_repository_get_workdir (repository));
			break;
		case PROP_HEAD:
			g_value_set_object (value,
			                    ggit_repository_get_head (repository, NULL));
			break;
		case PROP_CLONE_OPTIONS:
			g_value_set_object (value, priv->clone_options);
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void
set_workdir (GgitRepository *repository,
             GFile          *workdir,
             gboolean        update_gitlink)
{
	GgitRepositoryPrivate *priv;

	priv = ggit_repository_get_instance_private (repository);

	g_clear_object (&priv->workdir);

	if (workdir)
	{
		priv->workdir = g_file_dup (workdir);

		if (_ggit_native_get (repository))
		{
			gchar *path;

			path = g_file_get_path (priv->workdir);

			git_repository_set_workdir (_ggit_native_get (repository),
			                            path, update_gitlink);

			g_free (path);
		}
	}
}

static void
ggit_repository_set_property (GObject      *object,
                              guint         prop_id,
                              const GValue *value,
                              GParamSpec   *pspec)
{
	GgitRepository *repository;
	GgitRepositoryPrivate *priv;

	repository = GGIT_REPOSITORY (object);
	priv = ggit_repository_get_instance_private (repository);

	switch (prop_id)
	{
		case PROP_URL:
			priv->url = g_value_dup_string (value);
			break;
		case PROP_LOCATION:
		{
			GFile *f;

			f = g_value_get_object (value);

			g_clear_object (&priv->location);

			if (f)
			{
				priv->location = g_file_dup (f);
			}

			break;
		}
		case PROP_WORKDIR:
			set_workdir (repository, g_value_get_object (value), FALSE);
			break;
		case PROP_IS_BARE:
			priv->is_bare = g_value_get_boolean (value);
			break;
		case PROP_INIT:
			priv->init = g_value_get_boolean (value);
			break;
		case PROP_CLONE_OPTIONS:
			priv->clone_options = g_value_dup_object (value);
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void
ggit_repository_class_init (GgitRepositoryClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->finalize = ggit_repository_finalize;
	object_class->get_property = ggit_repository_get_property;
	object_class->set_property = ggit_repository_set_property;

	g_object_class_install_property (object_class,
	                                 PROP_URL,
	                                 g_param_spec_string ("url",
	                                                      "URL for cloning a repository",
	                                                      "The URL for cloning a repository",
	                                                      NULL,
	                                                      G_PARAM_READWRITE |
	                                                      G_PARAM_CONSTRUCT_ONLY |
	                                                      G_PARAM_STATIC_STRINGS));

	g_object_class_install_property (object_class,
	                                 PROP_LOCATION,
	                                 g_param_spec_object ("location",
	                                                      "Location of repository",
	                                                      "The location of the repository",
	                                                      G_TYPE_FILE,
	                                                      G_PARAM_READWRITE |
	                                                      G_PARAM_CONSTRUCT_ONLY |
	                                                      G_PARAM_STATIC_STRINGS));

	g_object_class_install_property (object_class,
	                                 PROP_IS_BARE,
	                                 g_param_spec_boolean ("is-bare",
	                                                       "Is bare",
	                                                       "Is a bare repository",
	                                                       FALSE,
	                                                       G_PARAM_READWRITE |
	                                                       G_PARAM_CONSTRUCT_ONLY |
	                                                       G_PARAM_STATIC_STRINGS));

	g_object_class_install_property (object_class,
	                                 PROP_INIT,
	                                 g_param_spec_boolean ("init",
	                                                       "Init",
	                                                       "Whether to initialize a repository",
	                                                       FALSE,
	                                                       G_PARAM_READWRITE |
	                                                       G_PARAM_CONSTRUCT_ONLY |
	                                                       G_PARAM_STATIC_STRINGS));

	g_object_class_install_property (object_class,
	                                 PROP_WORKDIR,
	                                 g_param_spec_object ("workdir",
	                                                      "Path to repository working directory",
	                                                      "The path to the repository working directory",
	                                                      G_TYPE_FILE,
	                                                      G_PARAM_READWRITE |
	                                                      G_PARAM_CONSTRUCT |
	                                                      G_PARAM_STATIC_STRINGS));

	g_object_class_install_property (object_class,
	                                 PROP_HEAD,
	                                 g_param_spec_object ("head",
	                                                      "Head",
	                                                      "Head",
	                                                      GGIT_TYPE_REF,
	                                                      G_PARAM_READABLE));

	g_object_class_install_property (object_class,
	                                 PROP_CLONE_OPTIONS,
	                                 g_param_spec_object ("clone-options",
	                                                      "Clone options",
	                                                      "Clone options",
	                                                      GGIT_TYPE_CLONE_OPTIONS,
	                                                      G_PARAM_READWRITE |
	                                                      G_PARAM_CONSTRUCT_ONLY));
}

static void
ggit_repository_init (GgitRepository *repository)
{
}

static gboolean
ggit_repository_initable_init (GInitable    *initable,
                               GCancellable *cancellable,
                               GError      **error)
{
	GgitRepository *repository = GGIT_REPOSITORY (initable);
	GgitRepositoryPrivate *priv;
	gboolean success = TRUE;
	gint err;
	gchar *path = NULL;
	git_repository *repo = NULL;

	if (cancellable != NULL)
	{
		g_set_error_literal (error, G_IO_ERROR, G_IO_ERROR_NOT_SUPPORTED,
		                     "Cancellable initialization not supported");
		return FALSE;
	}

	priv = ggit_repository_get_instance_private (repository);

	if (priv->location != NULL)
	{
		path = g_file_get_path (priv->location);
	}

	if (path == NULL)
	{
		err = GGIT_ERROR_NOTFOUND;
	}
	else if (priv->init == TRUE)
	{
		err = git_repository_init (&repo,
		                           path,
		                           priv->is_bare);
	}
	else if (priv->url != NULL)
	{
		err = git_clone (&repo,
		                 priv->url,
		                 path,
		                 _ggit_clone_options_get_native (priv->clone_options));
	}
	else
	{
		err = git_repository_open (&repo,
		                           path);
	}

	g_free (path);

	if (err != GIT_OK)
	{
		_ggit_error_set (error, err);

		success = FALSE;
	}
	else if (priv->workdir)
	{
		path = g_file_get_path (priv->workdir);

		if (path)
		{
			git_repository_set_workdir (_ggit_native_get (initable),
			                            path, FALSE);
		}

		g_free (path);
	}

	_ggit_native_set (GGIT_REPOSITORY (initable),
	                  repo,
	                  (GDestroyNotify)git_repository_free);

	if (success && !priv->workdir && !priv->is_bare)
	{
		priv->workdir = ggit_repository_get_workdir (GGIT_REPOSITORY (initable));
	}

	return success;
}

static void
ggit_repository_initable_iface_init (GInitableIface *iface)
{
	iface->init = ggit_repository_initable_init;
}

GgitRepository *
_ggit_repository_wrap (git_repository *repository,
                       gboolean        owned)
{
	GgitRepository *ret;
	GgitRepositoryPrivate *priv;

	ret = repository_from_registry (repository);

	if (ret != NULL)
	{
		return g_object_ref (ret);
	}

	ret = g_object_new (GGIT_TYPE_REPOSITORY,
	                    "native", repository,
	                    NULL);

	priv = ggit_repository_get_instance_private (ret);

	priv->workdir = ggit_repository_get_workdir (ret);

	if (owned)
	{
		_ggit_native_set_destroy_func (ret,
		                               (GDestroyNotify)git_repository_free);
	}

	if (owned)
	{
		register_repository (repository, ret);
	}

	return ret;
}

git_repository *
_ggit_repository_get_repository (GgitRepository *repository)
{
	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), NULL);

	return _ggit_native_get (repository);
}

/**
 * ggit_repository_open:
 * @location: the location of the repository.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Open a git repository.
 *
 * The @path must point to an existing git repository folder, e.g.
 *
 *		/path/to/my_repo/.git/	(normal repository)
 *							objects/
 *							index
 *							HEAD
 *
 *		/path/to/bare_repo/		(bare repository)
 *						objects/
 *						index
 *						HEAD
 *
 *	The method will automatically detect if @path is a normal
 *	or bare repository or fail if it is neither.
 *
 * Returns: (transfer full) (nullable): a newly created #GgitRepository.
 */
GgitRepository *
ggit_repository_open (GFile   *location,
                      GError **error)
{
	g_return_val_if_fail (G_IS_FILE (location), NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	return g_initable_new (GGIT_TYPE_REPOSITORY, NULL, error,
	                       "location", location,
	                       NULL);
}

/**
 * ggit_repository_init_repository:
 * @location: the location of the repository.
 * @is_bare: if %TRUE, a git repository without a working directory is created
 *           at the pointed path. If %FALSE, provided path will be considered as the working
 *           directory into which the .git directory will be created.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Creates a new git repository in the given folder.
 *
 * Returns: (transfer full) (nullable): a newly created #GgitRepository.
 */
GgitRepository *
ggit_repository_init_repository (GFile     *location,
                                 gboolean   is_bare,
                                 GError   **error)
{
	g_return_val_if_fail (G_IS_FILE (location), NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	return g_initable_new (GGIT_TYPE_REPOSITORY, NULL, error,
	                       "location", location,
	                       "is-bare", is_bare,
	                       "init", TRUE,
	                       NULL);
}

/**
 * ggit_repository_clone:
 * @url: url to fetch the repository from.
 * @location: the location of the repository.
 * @options: (allow-none): a #GgitCloneOptions.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Clones a new git repository in the given folder.
 *
 * Returns: (transfer full) (nullable): a newly created #GgitRepository.
 */
GgitRepository *
ggit_repository_clone (const gchar       *url,
                       GFile             *location,
                       GgitCloneOptions  *options,
                       GError           **error)
{
	g_return_val_if_fail (url != NULL, NULL);
	g_return_val_if_fail (G_IS_FILE (location), NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	return g_initable_new (GGIT_TYPE_REPOSITORY, NULL, error,
	                       "url", url,
	                       "location", location,
	                       "clone-options", options,
	                       NULL);
}

/**
 * ggit_repository_lookup:
 * @repository: a #GgitRepository.
 * @oid: a #GgitOId.
 * @gtype: a #GType.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Lookups a reference to one of the objects in the @repository.
 *
 * The generated reference must be freed with g_object_unref().
 *
 * The @gtype must match the type of the object
 * in the odb; the method will fail otherwise.
 * The special value %G_TYPE_NONE may be passed to let
 * the method guess the object's type.
 *
 * Returns: (transfer full) (nullable): the found #GgitObject, or %NULL on error.
 */
GgitObject *
ggit_repository_lookup (GgitRepository  *repository,
                        GgitOId         *oid,
                        GType            gtype,
                        GError         **error)
{
	GgitObject *object = NULL;
	git_object *obj;
	const git_oid *id;
	git_otype otype;
	gint ret;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), NULL);

	id = (const git_oid *)_ggit_oid_get_oid (oid);
	otype = ggit_utils_get_otype_from_gtype (gtype);

	ret = git_object_lookup (&obj,
	                         _ggit_native_get (repository),
	                         id,
	                         otype);

	if (ret == GIT_OK)
	{
		object = ggit_utils_create_real_object (obj, TRUE);
	}
	else
	{
		_ggit_error_set (error, ret);
	}

	return object;
}

/**
 * ggit_repository_revparse:
 * @repository: a #GgitRepository.
 * @spec: the revision specification.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Find an object, as specified by a revision string. See `man gitrevisions`,
 * or the documentation for `git rev-parse` for information on the syntax
 * accepted.
 *
 * Returns: (transfer full) (nullable): a #GgitObject or %NULL if the revision could not be found.
 *
 **/
GgitObject *
ggit_repository_revparse (GgitRepository  *repository,
                          const gchar     *spec,
                          GError         **error)
{
	GgitObject *object = NULL;
	git_object *obj = NULL;
	gint ret;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), NULL);
	g_return_val_if_fail (spec != NULL, NULL);

	ret = git_revparse_single (&obj, _ggit_native_get (repository), spec);

	if (ret == GIT_OK)
	{
		object = ggit_utils_create_real_object (obj, TRUE);
	}
	else
	{
		_ggit_error_set (error, ret);
	}

	return object;
}

/**
 * ggit_repository_lookup_reference:
 * @repository: a #GgitRepository.
 * @name: the long name for the reference (e.g. HEAD, ref/heads/master, refs/tags/v0.1.0, ...).
 * @error: a #GError for error reporting, or %NULL.
 *
 * Lookups a reference by its name in @repository. The returned #GgitRef must
 * be freed with g_object_unref().
 *
 * Returns: (transfer full) (nullable): the searched reference.
 */
GgitRef *
ggit_repository_lookup_reference (GgitRepository  *repository,
                                  const gchar     *name,
                                  GError         **error)
{
	GgitRef *ref = NULL;
	git_reference *reference;
	gint ret;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), NULL);
	g_return_val_if_fail (name != NULL, NULL);

	ret = git_reference_lookup (&reference, _ggit_native_get (repository),
	                            name);

	if (ret == GIT_OK)
	{
		ref = _ggit_ref_wrap (reference, FALSE);
	}
	else
	{
		_ggit_error_set (error, ret);
	}

	return ref;
}

/**
 * ggit_repository_lookup_reference_dwim:
 * @repository: a #GgitRepository.
 * @short_name: the short name for the reference (e.g. master, v0.1.0, ...).
 * @error: a #GError for error reporting, or %NULL.
 *
 * Lookups a reference by its short name in @repository applying the git precendence
 * rules to the given shorthand to determine which reference the user is referring to.
 * The returned #GgitRef must be freed with g_object_unref().
 *
 * Returns: (transfer full) (nullable): the searched reference.
 */
GgitRef *
ggit_repository_lookup_reference_dwim (GgitRepository  *repository,
                                       const gchar     *short_name,
                                       GError         **error)
{
	GgitRef *ref = NULL;
	git_reference *reference;
	gint ret;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), NULL);
	g_return_val_if_fail (short_name != NULL, NULL);

	ret = git_reference_dwim (&reference, _ggit_native_get (repository),
	                          short_name);

	if (ret == GIT_OK)
	{
		ref = _ggit_ref_wrap (reference, FALSE);
	}
	else
	{
		_ggit_error_set (error, ret);
	}

	return ref;
}

/**
 * ggit_repository_create_reference:
 * @repository: a #GgitRepository.
 * @name: the name for the new #GgitRef.
 * @oid: the #GgitOId pointed to by the reference.
 * @log_message: The one line long message to be appended to the reflog.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Creates a new object id reference.
 *
 * The reference will be created in the repository and written
 * to the disk. The returned value must be freed with g_object_unref().
 *
 * Returns: (transfer full) (nullable): the newly created reference.
 */
GgitRef *
ggit_repository_create_reference (GgitRepository  *repository,
                                  const gchar     *name,
                                  GgitOId         *oid,
                                  const gchar     *log_message,
                                  GError         **error)
{
	GgitRef *ref = NULL;
	git_reference *reference;
	gint ret;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), NULL);
	g_return_val_if_fail (name != NULL, NULL);
	g_return_val_if_fail (oid != NULL, NULL);
	g_return_val_if_fail (log_message != NULL, NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	ret = git_reference_create (&reference, _ggit_native_get (repository),
	                            name, _ggit_oid_get_oid (oid), FALSE,
	                            log_message);

	if (ret == GIT_OK)
	{
		ref = _ggit_ref_wrap (reference, FALSE);
	}
	else
	{
		_ggit_error_set (error, ret);
	}

	return ref;
}

/**
 * ggit_repository_create_symbolic_reference:
 * @repository: a #GgitRepository.
 * @name: the name for the new #GgitRef.
 * @target: the full name to the reference.
 * @log_message: The one line long message to be appended to the reflog.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Creates a new symbolic reference.
 *
 * The reference will be created in the repository and written
 * to the disk. The returned value must be freed with g_object_unref().
 *
 * Returns: (transfer full) (nullable): the newly created reference.
 */
GgitRef *
ggit_repository_create_symbolic_reference (GgitRepository  *repository,
                                           const gchar     *name,
                                           const gchar     *target,
                                           const gchar     *log_message,
                                           GError         **error)
{
	GgitRef *ref = NULL;
	git_reference *reference;
	gint ret;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), NULL);
	g_return_val_if_fail (name != NULL, NULL);
	g_return_val_if_fail (target != NULL, NULL);
	g_return_val_if_fail (log_message != NULL, NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	ret = git_reference_symbolic_create (&reference, _ggit_native_get (repository),
	                                     name, target, FALSE,
	                                     log_message);

	if (ret == GIT_OK)
	{
		ref = _ggit_ref_wrap (reference, FALSE);
	}
	else
	{
		_ggit_error_set (error, ret);
	}

	return ref;
}

/**
 * ggit_repository_get_head:
 * @repository: a #GgitRepository.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Get and resolves the current HEAD reference of the repository. Note that the
 * returned ref is already resolved (if HEAD is symbolic). If you want to
 * retrieve the symbolic ref called HEAD, then use #ggit_repository_lookup_reference
 * instead.
 *
 * Returns: (transfer full) (nullable): a #GgitRef
 *
 **/
GgitRef *
ggit_repository_get_head (GgitRepository  *repository,
                          GError         **error)
{
	GgitRef *ref = NULL;
	git_reference *reference;
	gint ret;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	ret = git_repository_head (&reference, _ggit_native_get (repository));

	if (ret == GIT_OK)
	{
		ref = _ggit_ref_wrap (reference, TRUE);
	}
	else
	{
		_ggit_error_set (error, ret);
	}

	return ref;
}

/**
 * ggit_repository_set_head:
 * @repository: a #GgitRepository.
 * @ref_name: canonical name of the reference HEAD should point to.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Returns: %TRUE if head was successfully set, %FALSE otherwise.
 *
 **/
gboolean
ggit_repository_set_head (GgitRepository   *repository,
                          const gchar      *ref_name,
                          GError          **error)
{
	gint ret;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), FALSE);
	g_return_val_if_fail (ref_name != NULL, FALSE);
	g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

	ret = git_repository_set_head (_ggit_native_get (repository), ref_name);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return FALSE;
	}

	return TRUE;
}

/**
 * ggit_repository_discover:
 * @location: the base location where the lookup starts.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Looks for a git repository.
 *
 * The lookup starts from @path and walks up the parent directories
 * and stops when a repository is found.
 *
 * Returns: (transfer full) (nullable): the repository location.
 */
GFile *
ggit_repository_discover (GFile   *location,
                          GError **error)
{
	g_return_val_if_fail (G_IS_FILE (location), NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	return ggit_repository_discover_full (location, FALSE, NULL, error);
}

/**
 * ggit_repository_discover_full:
 * @location: the base location where the lookup starts.
 * @across_fs: indictaes whether lookup will work across filesystem devices.
 * @ceiling_dirs: (array zero-terminated=1) (allow-none): a list of absolute paths
 *   at which lookup will stop when reached, or %NULL.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Looks for a git repository.
 *
 * The lookup starts from @path and walks up the parent directories
 * and stops when a repository is found.
 *
 * Returns: (transfer full) (nullable): the repository location.
 */
GFile *
ggit_repository_discover_full (GFile        *location,
                               gboolean      across_fs,
                               const gchar **ceiling_dirs,
                               GError      **error)
{
	gchar *path;
	gint ret;
	git_buf buf = {0,};
	GFile *rep = NULL;
	gchar *dirs_path = NULL;
	gchar path_sep[2] = { GIT_PATH_LIST_SEPARATOR, 0 };

	g_return_val_if_fail (G_IS_FILE (location), NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	path = g_file_get_path (location);
	g_return_val_if_fail (path != NULL, NULL);

	if (ceiling_dirs != NULL) {
		dirs_path = g_strjoinv (path_sep, (gchar **)ceiling_dirs);
	}

	ret = git_repository_discover (&buf,
	                               path,
	                               across_fs ? 1 : 0,
	                               dirs_path);

	g_free (path);
	g_free (dirs_path);

	if (ret == GIT_OK)
	{
		rep = g_file_new_for_path (buf.ptr);
#if LIBGIT2_SOVERSION >= 28
		git_buf_dispose (&buf);
#else
		git_buf_free (&buf);
#endif
	}
	else
	{
		_ggit_error_set (error, ret);
	}

	return rep;
}

/**
 * ggit_repository_is_head_detached:
 * @repository: a #GgitRepository.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Checks if @repository's HEAD is detached.
 *
 * A repository's HEAD is detached when it points directly to a commit
 * instead of a branch.
 *
 * Returns: %TRUE if HEAD is detached.
 */
gboolean
ggit_repository_is_head_detached (GgitRepository  *repository,
                                  GError         **error)
{
	gint ret;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), FALSE);
	g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

	ret = git_repository_head_detached (_ggit_native_get (repository));

	if (ret < 0)
	{
		_ggit_error_set (error, ret);
	}

	return ret == 1;
}

/**
 * ggit_repository_is_head_unborn:
 * @repository: a #GgitRepository.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Checks if @repository's HEAD is an orphan.
 *
 * An orphan branch is one named from HEAD but doesn't exist in
 * the refs namespace, because it doesn't have any commit to point to.
 *
 * Returns: %TRUE if the current branch is an orphan.
 */
gboolean
ggit_repository_is_head_unborn (GgitRepository  *repository,
                                GError         **error)
{
	gint ret;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), FALSE);
	g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

	ret = git_repository_head_unborn (_ggit_native_get (repository));

	if (ret < 0)
	{
		_ggit_error_set (error, ret);
	}

	return ret == 1;
}

/**
 * ggit_repository_is_empty:
 * @repository: a #GgitRepository.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Checks if @repository is empty.
 *
 * An empty repository has just been initialized and contains
 * no commits.
 *
 * Returns: %TRUE if the repository is empty.
 */
gboolean
ggit_repository_is_empty (GgitRepository  *repository,
                          GError         **error)
{
	gint ret;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), FALSE);
	g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

	ret = git_repository_is_empty (_ggit_native_get (repository));

	if (ret < 0)
	{
		_ggit_error_set (error, ret);
	}

	return ret == 1;
}

/**
 * ggit_repository_get_location:
 * @repository: a #GgitRepository.
 *
 * Get the gitdir location of the repository.
 *
 * Returns: (transfer full) (nullable): the location of the gitdir of the repository.
 */
GFile *
ggit_repository_get_location (GgitRepository *repository)
{
	const gchar *path;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), NULL);

	path = git_repository_path (_ggit_native_get (repository));

	return path ? g_file_new_for_path (path) : NULL;
}

/**
 * ggit_repository_get_workdir:
 * @repository: a #GgitRepository.
 *
 * Gets the working directory of the repository.
 *
 * Returns: (transfer full) (nullable): the location of the working directory of the repository.
 */
GFile *
ggit_repository_get_workdir (GgitRepository *repository)
{
	const gchar *path;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), NULL);

	path = git_repository_workdir (_ggit_native_get (repository));

	return path ? g_file_new_for_path (path) : NULL;
}

/**
 * ggit_repository_set_workdir:
 * @repository: a #GgitRepository.
 * @workdir: the working directory.
 * @update_gitlink: create/update gitlink in workdir.
 *
 * Sets the working directory of the repository. If @update_gitlink is set to
 * %TRUE "core.worktree" will be set in the config if workdir is not the parent
 * of the .git directory).
 */
void
ggit_repository_set_workdir (GgitRepository *repository,
                             GFile          *workdir,
                             gboolean        update_gitlink)
{
	g_return_if_fail (GGIT_IS_REPOSITORY (repository));
	g_return_if_fail (G_IS_FILE (workdir));

	set_workdir (repository, workdir, update_gitlink);
}

/**
 * ggit_repository_is_bare:
 * @repository: a #GgitRepository.
 *
 * Checks if @repository is bare.
 *
 * Returns: %TRUE if the repository is empty.
 */
gboolean
ggit_repository_is_bare (GgitRepository *repository)
{
	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), FALSE);

	return git_repository_is_bare (_ggit_native_get (repository));
}

/**
 * ggit_repository_file_status:
 * @repository: a #GgitRepository.
 * @location: the file to retrieve status for, rooted at the repository working dir.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Gets the file status for a single file.
 *
 * Returns: the status for a single file.
 */
GgitStatusFlags
ggit_repository_file_status (GgitRepository  *repository,
                             GFile           *location,
                             GError         **error)
{
	GgitRepositoryPrivate *priv;
	GgitStatusFlags status_flags;
	gint ret;
	gchar *path;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), GGIT_STATUS_IGNORED);
	g_return_val_if_fail (G_IS_FILE (location), GGIT_STATUS_IGNORED);
	g_return_val_if_fail (error == NULL || *error == NULL, GGIT_STATUS_IGNORED);

	priv = ggit_repository_get_instance_private (repository);

	path = g_file_get_relative_path (priv->workdir, location);

	g_return_val_if_fail (path != NULL, GGIT_STATUS_IGNORED);

	ret = git_status_file (&status_flags,
	                       _ggit_native_get (repository),
	                       path);

	g_free (path);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
	}

	return status_flags;
}

/**
 * ggit_repository_file_status_foreach:
 * @repository: a #GgitRepository.
 * @options: (allow-none): status options, or %NULL.
 * @callback: (scope call): a #GgitStatusCallback.
 * @user_data: callback user data.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Gathers file statuses and run a callback for each one.
 *
 * To the callback is passed the path of the file, the status and the data pointer
 * passed to this function. If the callback returns something other than
 * 0, the iteration will stop and @error will be set.
 *
 * Set @options to %NULL to get the default status options.
 *
 * Returns: %TRUE if there was no error, %FALSE otherwise
 *
 */
gboolean
ggit_repository_file_status_foreach (GgitRepository     *repository,
                                     GgitStatusOptions  *options,
                                     GgitStatusCallback  callback,
                                     gpointer            user_data,
                                     GError            **error)
{
	gint ret;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), FALSE);
	g_return_val_if_fail (callback != NULL, FALSE);
	g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

	ret = git_status_foreach_ext (_ggit_native_get (repository),
	                              _ggit_status_options_get_status_options (options),
	                              callback,
	                              user_data);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return FALSE;
	}

	return TRUE;
}

typedef struct
{
	GgitReferencesCallback callback;
	gpointer user_data;
} ReferenceForeachInfo;

static gint
references_foreach_wrapper (git_reference *reference,
                            gpointer       payload)
{
	ReferenceForeachInfo *info = (ReferenceForeachInfo *)payload;

	return info->callback(_ggit_ref_wrap (reference, TRUE), info->user_data);
}

/**
 * ggit_repository_references_foreach:
 * @repository: a #GgitRepository.
 * @callback: (scope call): a #GgitReferencesCallback.
 * @user_data: callback user data.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Gathers references and run a callback for each one.
 *
 * To the callback is passed the reference and the data pointer
 * passed to this function. If the callback returns something other than
 * 0, the iteration will stop and @error will be set.
 *
 * Returns: %TRUE if there was no error, %FALSE otherwise
 *
 */
gboolean
ggit_repository_references_foreach (GgitRepository          *repository,
                                    GgitReferencesCallback   callback,
                                    gpointer                 user_data,
                                    GError                 **error)
{
	gint ret;
	ReferenceForeachInfo info;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), FALSE);
	g_return_val_if_fail (callback != NULL, FALSE);
	g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

	info.callback = callback;
	info.user_data = user_data;

	ret = git_reference_foreach (_ggit_native_get (repository),
	                             references_foreach_wrapper,
	                             &info);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return FALSE;
	}

	return TRUE;
}

/**
 * ggit_repository_references_foreach_name:
 * @repository: a #GgitRepository.
 * @callback: (scope call): a #GgitReferencesNameCallback.
 * @user_data: callback user data.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Gathers reference names and run a callback for each one.
 *
 * To the callback is passed the name of the reference and the data pointer
 * passed to this function. If the callback returns something other than
 * 0, the iteration will stop and @error will be set.
 *
 * Returns: %TRUE if there was no error, %FALSE otherwise
 *
 */
gboolean
ggit_repository_references_foreach_name (GgitRepository             *repository,
                                         GgitReferencesNameCallback  callback,
                                         gpointer                    user_data,
                                         GError                    **error)
{
	gint ret;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), FALSE);
	g_return_val_if_fail (callback != NULL, FALSE);
	g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

	ret = git_reference_foreach_name (_ggit_native_get (repository),
	                                  callback,
	                                  user_data);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return FALSE;
	}

	return TRUE;
}

/**
 * ggit_repository_get_config:
 * @repository: a #GgitRepository.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Get the config for a specific repository.
 *
 * Returns: (transfer full) (nullable): a #GgitConfig.
 *
 **/
GgitConfig *
ggit_repository_get_config (GgitRepository  *repository,
                            GError         **error)
{
	git_config *config;
	gint ret;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	ret = git_repository_config (&config,
	                             _ggit_native_get (repository));

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return NULL;
	}

	return _ggit_config_wrap (config);
}

/**
 * ggit_repository_get_index:
 * @repository: a #GgitRepository.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Get the index for a specific repository.
 *
 * Returns: (transfer full) (nullable): a #GgitIndex.
 *
 **/
GgitIndex *
ggit_repository_get_index (GgitRepository  *repository,
                           GError         **error)
{
	git_index *idx;
	gint ret;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	ret = git_repository_index (&idx,
	                            _ggit_native_get (repository));

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return NULL;
	}

	return _ggit_index_wrap (idx);
}

/**
 * ggit_repository_create_tag:
 * @repository: a #GgitRepository.
 * @tag_name: the tag name.
 * @target: a #GgitObject.
 * @tagger: a #GgitSignature.
 * @message: the tag message.
 * @flags: a #GgitCreateFlags.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Create a new tag object.
 *
 * Returns: (transfer full) (nullable): the id to which the tag points, or
 *                                        %NULL in case of an error.
 *
 **/
GgitOId *
ggit_repository_create_tag (GgitRepository   *repository,
                            gchar const      *tag_name,
                            GgitObject       *target,
                            GgitSignature    *tagger,
                            gchar const      *message,
                            GgitCreateFlags   flags,
                            GError          **error)
{
	git_oid oid;
	gboolean force;
	gint ret;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), NULL);
	g_return_val_if_fail (tag_name != NULL, NULL);
	g_return_val_if_fail (GGIT_IS_OBJECT (target), NULL);
	g_return_val_if_fail (tagger != NULL, NULL);
	g_return_val_if_fail (message != NULL, NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	force = flags & GGIT_CREATE_FORCE;

	ret = git_tag_create (&oid,
	                     _ggit_native_get (repository),
	                     tag_name,
	                     _ggit_native_get (target),
	                     _ggit_native_get (tagger),
	                     message,
	                     force ? 1 : 0);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return NULL;
	}

	return _ggit_oid_wrap (&oid);
}

/**
 * ggit_repository_create_tag_from_buffer:
 * @repository: a #GgitRepository.
 * @tag: the tag buffer.
 * @flags: a #GgitCreateFlags.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Create a new tag from a buffer describing the tag object. The buffer must
 * be correctly formatted.
 *
 * Returns: (transfer full) (nullable): the id to which the tag points, or
 *                                        %NULL in case of an error.
 *
 **/
GgitOId *
ggit_repository_create_tag_from_buffer (GgitRepository   *repository,
                                        const gchar      *tag,
                                        GgitCreateFlags   flags,
                                        GError          **error)
{
	git_oid oid;
	gboolean force;
	gint ret;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), NULL);
	g_return_val_if_fail (tag != NULL, NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	force = flags & GGIT_CREATE_FORCE;

	ret = git_tag_create_frombuffer (&oid,
	                                 _ggit_native_get (repository),
	                                 tag,
	                                 force ? 1 : 0);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return NULL;
	}

	return _ggit_oid_wrap (&oid);
}

/**
 * ggit_repository_create_tag_lightweight:
 * @repository: a #GgitRepository.
 * @tag_name: the name of the tag.
 * @target: a #GgitObject.
 * @flags: a #GgitCreateFlags.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Creates a new lightweight tag.
 *
 * Returns: (transfer full) (nullable): the id to which the tag points, or
 *                                        %NULL in case of an error.
 *
 **/
GgitOId *
ggit_repository_create_tag_lightweight (GgitRepository   *repository,
                                        const gchar      *tag_name,
                                        GgitObject       *target,
                                        GgitCreateFlags   flags,
                                        GError          **error)
{
	gboolean force;
	git_oid oid;
	gint ret;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), NULL);
	g_return_val_if_fail (tag_name != NULL, NULL);
	g_return_val_if_fail (GGIT_IS_OBJECT (target), NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	force = flags & GGIT_CREATE_FORCE;

	ret = git_tag_create_lightweight (&oid,
	                                  _ggit_native_get (repository),
	                                  tag_name,
	                                  _ggit_native_get (target),
	                                  force ? 1 : 0);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return NULL;
	}

	return _ggit_oid_wrap (&oid);
}

/**
 * ggit_repository_create_tag_annotation:
 * @repository: a #GgitRepository.
 * @tag_name: the name of the tag.
 * @target: a #GgitObject.
 * @signature: a #GgitObject.
 * @message: the tag message.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Creates a new annotated tag.
 *
 * Returns: (transfer full) (nullable): the id to which the tag points, or
 *                                        %NULL in case of an error.
 *
 **/
GgitOId *
ggit_repository_create_tag_annotation (GgitRepository  *repository,
                                       const gchar     *tag_name,
                                       GgitObject      *target,
                                       GgitSignature   *signature,
                                       const gchar     *message,
                                       GError         **error)
{
	git_oid oid;
	gint ret;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), NULL);
	g_return_val_if_fail (tag_name != NULL, NULL);
	g_return_val_if_fail (GGIT_IS_OBJECT (target), NULL);
	g_return_val_if_fail (GGIT_IS_SIGNATURE (signature), NULL);
	g_return_val_if_fail (message != NULL, NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	ret = git_tag_annotation_create (&oid,
	                                 _ggit_native_get (repository),
	                                 tag_name,
	                                 _ggit_native_get (target),
	                                 _ggit_native_get (signature),
	                                 message);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return NULL;
	}

	return _ggit_oid_wrap (&oid);
}

/**
 * ggit_repository_list_tags:
 * @repository: a #GgitRepository.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Fill a list with all the tags in the @repository.
 *
 * Returns: (transfer full) (nullable): a list with the tags in @repository.
 **/
gchar **
ggit_repository_list_tags (GgitRepository  *repository,
                           GError         **error)
{
	gint ret;
	git_strarray tag_names;
	gchar **tags;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	ret = git_tag_list (&tag_names,
	                    _ggit_native_get (repository));

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		tags = NULL;
	}
	else
	{
		tags = ggit_utils_get_str_array_from_git_strarray (&tag_names);
	}

	return tags;
}

/**
 * ggit_repository_list_tags_match:
 * @repository: a #GgitRepository.
 * @pattern: (allow-none): a pattern to match.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Fill a list with all the tags in the @repository matching the provided
 * pattern. The pattern can use standard fnmatch syntax.
 *
 * Returns: (transfer full) (nullable): a list with matching tags in @repository.
 **/
gchar **
ggit_repository_list_tags_match (GgitRepository  *repository,
                                 const gchar     *pattern,
                                 GError         **error)
{
	gint ret;
	git_strarray tag_names;
	gchar **tags;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	ret = git_tag_list_match (&tag_names,
	                          pattern ? pattern : "",
	                          _ggit_native_get (repository));

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		tags = NULL;
	}
	else
	{
		tags = ggit_utils_get_str_array_from_git_strarray (&tag_names);
	}

	return tags;
}

/**
 * ggit_repository_delete_tag:
 * @repository: a #GgitRepository.
 * @name: the name of the tag.
 * @error: a #GError.
 *
 * Delete an existing tag reference by name.
 *
 * Returns: %TRUE if the tag was deleted successfully, %FALSE otherwise.
 **/
gboolean
ggit_repository_delete_tag (GgitRepository  *repository,
                            gchar const     *name,
                            GError         **error)
{
	gint ret;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), FALSE);
	g_return_val_if_fail (name != NULL, FALSE);
	g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

	ret = git_tag_delete (_ggit_native_get (repository), name);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

typedef struct
{
	gpointer user_data;

	GgitTagCallback callback;
} TagCallbackWrapperData;

static gint
tag_callback_wrapper (const gchar *name,
                      git_oid     *tag_oid,
                      gpointer     user_data)
{
	TagCallbackWrapperData *wrapper_data = (TagCallbackWrapperData *)user_data;
	GgitOId *oid;
	gint ret;

	oid = _ggit_oid_wrap (tag_oid);

	ret = wrapper_data->callback (name, oid, wrapper_data->user_data);
	ggit_oid_free (oid);

	return ret;
}

/**
 * ggit_repository_tag_foreach:
 * @repository: a #GgitRepository.
 * @callback: (scope call): a #GgitTagCallback.
 * @user_data: callback user data.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Get all the tags in the repository and run the provided callback on each. If
 * the callback returns something other than 0, the iteration will stop and
 * @error will be set.
 *
 * Returns: %TRUE if there was no error, %FALSE otherwise
 **/
gboolean
ggit_repository_tag_foreach (GgitRepository   *repository,
                             GgitTagCallback   callback,
                             gpointer          user_data,
                             GError          **error)
{
	TagCallbackWrapperData wrapper_data;
	gint ret;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), FALSE);
	g_return_val_if_fail (callback != NULL, FALSE);
	g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

	wrapper_data.user_data = user_data;
	wrapper_data.callback = callback;

	ret = git_tag_foreach (_ggit_native_get (repository),
	                       tag_callback_wrapper,
	                       &wrapper_data);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return FALSE;
	}

	return TRUE;
}

/**
 * ggit_repository_create_branch:
 * @repository: a #GgitRepository.
 * @branch_name: the name of the branch.
 * @target: a #GgitObject.
 * @flags: a #GgitCreateFlags.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Creates a new branch pointing at a target commit.
 *
 * Returns: (transfer full) (nullable): the reference to which the branch
 *                                        points, or %NULL in case of an error.
 **/
GgitBranch *
ggit_repository_create_branch (GgitRepository   *repository,
                               const gchar      *branch_name,
                               GgitObject       *target,
                               GgitCreateFlags   flags,
                               GError          **error)
{
	gboolean force;
	git_reference *reference;
	gint ret;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), NULL);
	g_return_val_if_fail (branch_name != NULL, NULL);
	g_return_val_if_fail (GGIT_IS_OBJECT (target), NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	force = flags & GGIT_CREATE_FORCE;

	ret = git_branch_create (&reference,
	                         _ggit_native_get (repository),
	                         branch_name,
	                         _ggit_native_get (target),
	                         force ? 1 : 0);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return NULL;
	}

	return _ggit_branch_wrap (reference);
}

/**
 * ggit_repository_enumerate_branches:
 * @repository: a #GgitRepository.
 * @list_type: a #GgitBranchType.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Get a branch enumerator to enumerate over all branches of the specified
 * @list_type in @repository.
 *
 * Returns: (transfer full) (nullable): a branch enumerator.
 **/
GgitBranchEnumerator *
ggit_repository_enumerate_branches (GgitRepository  *repository,
                                    GgitBranchType   list_type,
                                    GError         **error)
{
	git_branch_iterator *iter;
	gint ret;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	ret = git_branch_iterator_new (&iter,
	                               _ggit_native_get (repository),
	                               (git_branch_t)list_type);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return NULL;
	}

	return _ggit_branch_enumerator_wrap (iter);
}

/**
 * ggit_repository_lookup_branch:
 * @repository: a #GgitRepository.
 * @branch_name: the name of the branch.
 * @branch_type: a #GgitBranchType.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Lookups a branch by its name in a repository.
 *
 * Returns: (transfer full) (nullable): a branch by its name in a repository.
 */
GgitBranch *
ggit_repository_lookup_branch (GgitRepository *repository,
                               const gchar    *branch_name,
                               GgitBranchType  branch_type,
                               GError         **error)
{
	gint ret;
	git_reference *branch;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), NULL);
	g_return_val_if_fail (branch_name != NULL, NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	ret = git_branch_lookup (&branch,
	                         _ggit_native_get (repository),
	                         branch_name,
	                         (git_branch_t)branch_type);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return NULL;
	}

	return _ggit_branch_wrap (branch);
}

/**
 * ggit_repository_lookup_blob:
 * @repository: a #GgitRepository.
 * @oid: a #GgitOId.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Lookups a branch by its name in a repository.
 *
 * Returns: (transfer full) (nullable): a #GgitBlog pointer.
 */
GgitBlob *
ggit_repository_lookup_blob (GgitRepository *repository,
                             GgitOId        *oid,
                             GError         **error)
{
	gint ret;
	git_blob *blob;
	const git_oid *id;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	id = (const git_oid *)_ggit_oid_get_oid (oid);
	ret = git_blob_lookup (&blob,
	                       _ggit_native_get (repository),
	                       id);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return NULL;
	}

	return _ggit_blob_wrap (blob, TRUE);
}

/**
 * ggit_repository_lookup_commit:
 * @repository: a #GgitRepository.
 * @oid: a #GgitOId.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Lookups a branch by its name in a repository.
 *
 * Returns: (transfer full) (nullable): a #GgitCommit pointer.
 */
GgitCommit *
ggit_repository_lookup_commit (GgitRepository *repository,
                               GgitOId        *oid,
                               GError         **error)
{
	gint ret;
	git_commit *commit;
	const git_oid *id;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	id = (const git_oid *)_ggit_oid_get_oid (oid);
	ret = git_commit_lookup (&commit,
	                         _ggit_native_get (repository),
	                         id);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return NULL;
	}

	return _ggit_commit_wrap (commit, TRUE);
}

/**
 * ggit_repository_lookup_tag:
 * @repository: a #GgitRepository.
 * @oid: a #GgitOId.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Lookups a branch by its name in a repository.
 *
 * Returns: (transfer full) (nullable): a #GgitTag pointer.
 */
GgitTag *
ggit_repository_lookup_tag (GgitRepository *repository,
                            GgitOId        *oid,
                            GError         **error)
{
	gint ret;
	git_tag *tag;
	const git_oid *id;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	id = (const git_oid *)_ggit_oid_get_oid (oid);
	ret = git_tag_lookup (&tag,
	                      _ggit_native_get (repository),
	                      id);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return NULL;
	}

	return _ggit_tag_wrap (tag, TRUE);
}

/**
 * ggit_repository_lookup_tree:
 * @repository: a #GgitRepository.
 * @oid: a #GgitOId.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Lookups a branch by its name in a repository.
 *
 * Returns: (transfer full) (nullable): a #GgitTree pointer.
 */
GgitTree *
ggit_repository_lookup_tree (GgitRepository *repository,
                             GgitOId        *oid,
                             GError         **error)
{
	gint ret;
	git_tree *tree;
	const git_oid *id;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	id = (const git_oid *)_ggit_oid_get_oid (oid);
	ret = git_tree_lookup (&tree,
	                       _ggit_native_get (repository),
	                       id);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return NULL;
	}

	return _ggit_tree_wrap (tree, TRUE);
}

/**
 * ggit_repository_lookup_remote:
 * @repository: a #GgitRepository.
 * @name: the remote's name.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Gets the remote called @name.
 *
 * Returns: (transfer full) (nullable): a new #GgitRemote or %NULL if there is an error.
 */
GgitRemote *
ggit_repository_lookup_remote (GgitRepository  *repository,
                               const gchar     *name,
                               GError         **error)
{
	gint ret;
	git_remote *remote;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), NULL);
	g_return_val_if_fail (name != NULL, NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	ret = git_remote_lookup (&remote,
	                       _ggit_native_get (repository),
	                       name);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return NULL;
	}

	return _ggit_remote_wrap (remote);
}

/**
 * ggit_repository_create_remote:
 * @repository: a #GgitRepository.
 * @name: the name of the new remote.
 * @url: the url of the remote.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Adds a remote with the default fetch refspec to the repository's configuration.
 *
 * Returns: (transfer full) (nullable): a new #GgitRemote or %NULL if there is an error.
 */
GgitRemote *
ggit_repository_create_remote (GgitRepository  *repository,
                               const gchar     *name,
                               const gchar     *url,
                               GError         **error)
{
	gint ret;
	git_remote *remote;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), NULL);
	g_return_val_if_fail (name != NULL, NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	ret = git_remote_create (&remote,
	                         _ggit_native_get (repository),
	                         name,
	                         url);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return NULL;
	}

	return _ggit_remote_wrap (remote);
}

/**
 * ggit_repository_list_remotes:
 * @repository: a #GgitRepository.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Fill a list with all the remotes in @repository.
 *
 * Returns: (transfer full) (nullable): a list with the remotes.
 **/
gchar **
ggit_repository_list_remotes (GgitRepository  *repository,
                              GError         **error)
{
	gint ret;
	git_strarray remote_names;
	gchar **remotes;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	ret = git_remote_list (&remote_names,
	                       _ggit_native_get (repository));

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		remotes = NULL;
	}
	else
	{
		remotes = ggit_utils_get_str_array_from_git_strarray (&remote_names);
	}

	return remotes;
}

/**
 * ggit_repository_add_remote_push:
 * @repository: a #GgitRepository.
 * @remote: the remote name.
 * @refspec: the push refspec.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Adds a push refspec to @remote's configuration.
 * Adds the given refspec to the push list in the configuration. No
 * loaded remote instances will be affected.
 */
void
ggit_repository_add_remote_push (GgitRepository  *repository,
                                 GgitRemote      *remote,
                                 const gchar     *refspec,
                                 GError         **error)
{
	gint ret;

	g_return_if_fail (GGIT_IS_REMOTE(remote));
	g_return_if_fail (refspec != NULL && refspec[0] != '\0');
	g_return_if_fail (error == NULL || *error == NULL); 

	ret = git_remote_add_push (_ggit_native_get (repository),
	                           ggit_remote_get_name (_ggit_native_get (remote)),
	                           refspec);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
	}
}

/**
 * ggit_repository_add_remote_fetch:
 * @repository: a #GgitRepository.
 * @remote: a #GgitRemote.
 * @refspec: the fetch refspec.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Adds a fetch refspec to the @remote's configuration.
 * Adds @refspec to the fetch list in the configuration. No
 * loaded remote instances will be affected.
 */
void
ggit_repository_add_remote_fetch (GgitRepository  *repository,
                                  GgitRemote      *remote,
                                  const gchar     *refspec,
                                  GError         **error)
{
	gint ret;

	g_return_if_fail (GGIT_IS_REMOTE (remote));
	g_return_if_fail (refspec != NULL && refspec[0] != '\0');
	g_return_if_fail (error == NULL || *error == NULL);

	ret = git_remote_add_fetch (_ggit_native_get (repository),
	                            _ggit_native_get (remote),
	                            refspec);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
	}
}

/**
 * ggit_repository_lookup_submodule:
 * @repository: a #GgitRepository.
 * @name: the name of the submodule.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Lookups a submodule information by name or path. If the submodule
 * does not exist, %NULL is returned and a GGIT_ERROR_NOTFOUND error set.
 *
 * Returns: (transfer full) (nullable): a newly-allocated #GgitSubmodule.
 */
GgitSubmodule *
ggit_repository_lookup_submodule (GgitRepository  *repository,
                                  const gchar     *name,
                                  GError         **error)
{
	GgitSubmodule *gsubmodule = NULL;
	gint ret;
	git_submodule *submodule;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), NULL);
	g_return_val_if_fail (name != NULL, NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	ret = git_submodule_lookup (&submodule,
	                            _ggit_native_get (repository),
	                            name);

	if (ret == GIT_OK)
	{
		gsubmodule = _ggit_submodule_wrap (submodule);
	}
	else
	{
		_ggit_error_set (error, ret);
	}

	return gsubmodule;
}

typedef struct
{
	gpointer user_data;

	GgitSubmoduleCallback callback;
} SubmoduleCallbackWrapperData;

static gint
submodule_wrapper_callback (git_submodule *submodule,
                            const gchar   *name,
                            gpointer       user_data)
{
	SubmoduleCallbackWrapperData *wrapper_data = (SubmoduleCallbackWrapperData *)user_data;
	GgitSubmodule *gsubmodule;
	gint ret;

	gsubmodule = _ggit_submodule_wrap (submodule);

	ret = wrapper_data->callback (gsubmodule, name, wrapper_data->user_data);

	_ggit_submodule_invalidate (gsubmodule);
	ggit_submodule_unref (gsubmodule);

	return ret;
}

/**
 * ggit_repository_submodule_foreach:
 * @repository: a #GgitRepository.
 * @callback: (scope call): a #GgitStatusCallback.
 * @user_data: callback user data.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Gathers submodules and run a callback for each one.
 *
 * To the callback is passed the submodule instance and name. If the callback
 * returns something other than 0, the iteration will stop and @error
 * will be set.
 *
 * NOTE: the submodule instance passed to the callback is only valid during
 * the call to ggit_repository_submodule_foreach and may not be used after the
 * call has finished.
 *
 * Returns: %TRUE if there was no error, %FALSE otherwise
 */
gboolean
ggit_repository_submodule_foreach (GgitRepository        *repository,
                                   GgitSubmoduleCallback  callback,
                                   gpointer               user_data,
                                   GError               **error)
{
	SubmoduleCallbackWrapperData wrapper_data;
	gint ret;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), FALSE);
	g_return_val_if_fail (callback != NULL, FALSE);
	g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

	wrapper_data.user_data = user_data;
	wrapper_data.callback = callback;

	ret = git_submodule_foreach (_ggit_native_get (repository),
	                             submodule_wrapper_callback,
	                             &wrapper_data);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return FALSE;
	}

	return TRUE;
}

/**
 * ggit_repository_set_submodule_ignore:
 * @repository: a #GgitRepository.
 * @name: the name of the submodule.
 * @ignore: a #GgitSubmoduleIgnore.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Sets the ignore rule for the submodule in the configuration.
 * This does not affect any currently-loaded instances..
 */
void
ggit_repository_set_submodule_ignore (GgitRepository       *repository,
                                      const gchar          *name,
                                      GgitSubmoduleIgnore   ignore,
                                      GError              **error)
{
	gint ret;

	g_return_if_fail (GGIT_IS_REPOSITORY (repository));
	g_return_if_fail (name != NULL);
	g_return_if_fail (error == NULL || *error == NULL);

	ret = git_submodule_set_ignore (_ggit_native_get (repository),
	                                name,
	                                (git_submodule_ignore_t)ignore);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
	}
}

/**
 * ggit_repository_set_submodule_update:
 * @repository: a #GgitRepository.
 * @name: the name of the submodule.
 * @update: a #GgitSubmoduleUpdate.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Sets the update rule for the submodule in the configuration.
 * This setting won't affect any existing instances.
 */
void
ggit_repository_set_submodule_update (GgitRepository       *repository,
                                      const gchar          *name,
                                      GgitSubmoduleUpdate   update,
                                      GError              **error)
{
	gint ret;

	g_return_if_fail (GGIT_IS_REPOSITORY (repository));
	g_return_if_fail (name != NULL);
	g_return_if_fail (error == NULL || *error == NULL);

	ret = git_submodule_set_update (_ggit_native_get (repository),
	                                name,
	                                (git_submodule_update_t)update);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
	}
}

/**
 * ggit_repository_set_submodule_url:
 * @repository: a #GgitRepository.
 * @name: the name of the submodule to configure.
 * @url: URL that should be used for the submodule.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Sets the URL for the submodule in the configuration.
 *
 * After calling this, you may wish to call ggit_submodule_sync() to
 * write the changes to the checked out submodule repository.
 */
void
ggit_repository_set_submodule_url (GgitRepository  *repository,
                                   const gchar     *name,
                                   const gchar     *url,
                                   GError         **error)
{
	gint ret;

	g_return_if_fail (GGIT_IS_REPOSITORY (repository));
	g_return_if_fail (name != NULL);
	g_return_if_fail (url != NULL);
	g_return_if_fail (error == NULL || *error == NULL);

	ret = git_submodule_set_url (_ggit_native_get (repository),
	                             name,
	                             url);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
	}
}

/**
 * ggit_repository_set_submodule_fetch_recurse:
 * @repository: a #GgitRepository.
 * @name: the name of the submodule.
 * @fetch_recurse_submodules: a #GgitSubmoduleRecurse.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Sets the submodule.'name'.fetchRecurseSubmodules value for
 * the submodule. This setting won't affect any existing instances..
 */
void
ggit_repository_set_submodule_fetch_recurse (GgitRepository        *repository,
                                             const gchar           *name,
                                             GgitSubmoduleRecurse   fetch_recurse_submodules,
                                             GError               **error)
{
	gint ret;

	g_return_if_fail (GGIT_IS_REPOSITORY (repository));
	g_return_if_fail (name != NULL);
	g_return_if_fail (error == NULL || *error == NULL);

	ret = git_submodule_set_fetch_recurse_submodules (_ggit_native_get (repository),
	                                                  name,
	                                                  (git_submodule_recurse_t)fetch_recurse_submodules);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
	}
}

/**
 * ggit_repository_get_submodule_status:
 * @repository: a #GgitRepository.
 * @name: the name of the submodule.
 * @ignore: the ignore rules to follow.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Gets the status for a submodule.
 * This looks at a submodule and tries to determine the status.  It
 * will return a combination of the %GGIT_SUBMODULE_STATUS values.
 * How deeply it examines the working directory to do this will depend
 * on @ignore.
 *
 * Returns: the #GgitSubmoduleStatus for @submodule.
 */
GgitSubmoduleStatus
ggit_repository_get_submodule_status (GgitRepository      *repository,
                                      const gchar         *name,
                                      GgitSubmoduleIgnore  ignore,
                                      GError             **error)
{
	gint ret;
	GgitSubmoduleStatus status;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), 0);
	g_return_val_if_fail (name != NULL, 0);
	g_return_val_if_fail (error == NULL || *error == NULL, 0);

	ret = git_submodule_status (&status,
	                            _ggit_native_get (repository),
	                            name,
	                            (git_submodule_ignore_t)ignore);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
	}

	return status;
}

/**
 * ggit_repository_reset:
 * @repository: a #GgitRepository.
 * @target: the target #GgitObject which is a commit or a tag.
 * @reset_type: the #GgitResetType to perform.
 * @checkout_options: the #GgitCheckoutOptions to be used for a HARD reset.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Performs a reset of type @reset_type on @repository to @target,
 * or @error will be set.
 */
void
ggit_repository_reset (GgitRepository       *repository,
                       GgitObject           *target,
                       GgitResetType         reset_type,
                       GgitCheckoutOptions  *checkout_options,
                       GError              **error)
{
	gint ret;

	g_return_if_fail (GGIT_IS_REPOSITORY (repository));
	g_return_if_fail (GGIT_IS_OBJECT (target));
	g_return_if_fail (GGIT_IS_CHECKOUT_OPTIONS (checkout_options));
	g_return_if_fail (error == NULL || *error == NULL);

	ret = git_reset (_ggit_native_get (repository),
	                 _ggit_native_get (target),
	                 (git_reset_t)reset_type,
	                 (git_checkout_options *)_ggit_checkout_options_get_checkout_options (checkout_options));

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
	}
}

/**
 * ggit_repository_reset_default:
 * @repository: a #GgitRepository.
 * @target: (allow-none): the target #GgitObject which is a commit or a tag.
 * @pathspecs: (array zero-terminated=1): a list of file paths to reset.
 * @error: a #GError for error reporting or %NULL.
 *
 * Update some entries in the index from the target commit tree. The scope of
 * the updated entries is determined by the paths specified in @pathspecs.
 * Passing %NULL in @target will result in removing entries in the index
 * matching the provided pathspecs.
 *
 **/
void
ggit_repository_reset_default (GgitRepository       *repository,
                               GgitObject           *target,
                               const gchar * const  *pathspecs,
                               GError              **error)
{
	gint ret;
	git_strarray pp;

	g_return_if_fail (GGIT_IS_REPOSITORY (repository));
	g_return_if_fail (target == NULL || GGIT_IS_OBJECT (target));
	g_return_if_fail (pathspecs != NULL && *pathspecs != NULL);
	g_return_if_fail (error == NULL || *error == NULL);

	ggit_utils_get_git_strarray_from_str_array (pathspecs, &pp);

	ret = git_reset_default (_ggit_native_get (repository),
	                         _ggit_native_get (target),
	                         &pp);

	git_strarray_free (&pp);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
	}
}

/**
 * ggit_repository_save_stash:
 * @repository: a #GgitRepository.
 * @stasher: a #GgitSignature.
 * @message: description along with the stashed state or %NULL to be autogenerated.
 * @flags: a #GgitStashFlags to control the stashing process.
 * @error: a #GError for error reporting, #GGIT_ENOTFOUND if there's nothing to stash or %NULL.
 *
 * Saves the local modifications to a new stash.
 * It returns the commit containing the stashed state.
 * This commit is also the target of the direct reference refs/stash.
 *
 * Returns: (transfer full) (nullable): a new object id of the commit containing the stashed state.
 */
GgitOId *
ggit_repository_save_stash (GgitRepository  *repository,
                            GgitSignature   *stasher,
                            const gchar     *message,
                            GgitStashFlags   flags,
                            GError         **error)
{
	gint ret;
	git_oid oid;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), NULL);
	g_return_val_if_fail (GGIT_IS_SIGNATURE (stasher), NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	ret = git_stash_save (&oid,
	                      _ggit_native_get (repository),
	                      _ggit_native_get (stasher),
	                      message,
	                      flags);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return NULL;
	}

	return _ggit_oid_wrap (&oid);
}

/**
 * ggit_repository_drop_stash:
 * @repository: a #GgitRepository.
 * @index: the position within the stash list. 0 points to the.
 * most recent stashed state.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Removes a single stashed state from the stash list.
 */
void
ggit_repository_drop_stash (GgitRepository  *repository,
                            gsize            index,
                            GError         **error)
{
	gint ret;

	g_return_if_fail (GGIT_IS_REPOSITORY (repository));
	g_return_if_fail (error == NULL || *error == NULL);

	ret = git_stash_drop (_ggit_native_get (repository),
	                      index);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
	}
}

typedef struct
{
	gpointer user_data;

	GgitStashCallback callback;
} StashCallbackWrapperData;

static gint
stash_callback_wrapper (gsize          index,
                        const gchar   *message,
                        const git_oid *stash_oid,
                        gpointer       user_data)
{
	StashCallbackWrapperData *wrapper_data = (StashCallbackWrapperData *)user_data;
	GgitOId *oid;
	gint ret;

	oid = _ggit_oid_wrap (stash_oid);

	ret = wrapper_data->callback (index, message, oid, wrapper_data->user_data);
	ggit_oid_free (oid);

	return ret;
}

/**
 * ggit_repository_stash_foreach:
 * @repository: a #GgitRepository.
 * @callback: (scope call): a #GgitStashCallback.
 * @user_data: callback user data.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Loops over all the stashed states and issue a callback for each one.
 * If @callback returns a non-zero value, this will stop looping.
 *
 * Returns: %TRUE if there was no error, %FALSE otherwise.
 */
gboolean
ggit_repository_stash_foreach (GgitRepository     *repository,
                               GgitStashCallback   callback,
                               gpointer            user_data,
                               GError            **error)
{
	StashCallbackWrapperData wrapper_data;
	gint ret;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), FALSE);
	g_return_val_if_fail (callback != NULL, FALSE);
	g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

	wrapper_data.user_data = user_data;
	wrapper_data.callback = callback;

	ret = git_stash_foreach (_ggit_native_get (repository),
	                         stash_callback_wrapper,
	                         &wrapper_data);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return FALSE;
	}

	return TRUE;
}

/**
 * ggit_repository_get_ahead_behind:
 * @repository: a #GgitRepository.
 * @local: the commit for local.
 * @upstream: the commit for upstream.
 * @ahead: (out): number of unique from commits in @upstream.
 * @behind: (out): number of unique from commits in @local.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Count the number of unique commits between two commit objects.
 *
 * There is no need for branches containing the commits to have any
 * upstream relationship, but it helps to think of one as a branch and
 * the other as its upstream, the @ahead and @behind values will be
 * what git would report for the branches.
 */
void
ggit_repository_get_ahead_behind (GgitRepository  *repository,
                                  GgitOId         *local,
                                  GgitOId         *upstream,
                                  gsize           *ahead,
                                  gsize           *behind,
                                  GError         **error)
{
	gint ret;

	g_return_if_fail (GGIT_IS_REPOSITORY (repository));
	g_return_if_fail (local != NULL);
	g_return_if_fail (upstream != NULL);
	g_return_if_fail (ahead != NULL);
	g_return_if_fail (behind != NULL);
	g_return_if_fail (error == NULL || *error == NULL);

	ret = git_graph_ahead_behind (ahead, behind,
	                              _ggit_native_get (repository),
	                              _ggit_oid_get_oid (local),
	                              _ggit_oid_get_oid (upstream));

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
	}
}

/**
 * ggit_repository_get_descendant_of:
 * @repository: a #GgitRepository.
 * @commit: the commit.
 * @ancestor: the ancestor.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Check whether @com mit is a descendant of @ancestor. Note that if this
 * function returns %FALSE, an error might have occurred. If so, @error will
 * be set appropriately.
 *
 * Returns: %TRUE if @commit is a descendant of @ancestor, or %FALSE otherwise.
 *
 */
gboolean
ggit_repository_get_descendant_of (GgitRepository  *repository,
                                   GgitOId         *commit,
                                   GgitOId         *ancestor,
                                   GError         **error)
{
	gint ret;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), FALSE);
	g_return_val_if_fail (commit != NULL, FALSE);
	g_return_val_if_fail (ancestor != NULL, FALSE);
	g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

	ret = git_graph_descendant_of (_ggit_native_get (repository),
	                               _ggit_oid_get_oid (commit),
	                               _ggit_oid_get_oid (ancestor));

	if (ret == 1)
	{
		return TRUE;
	}

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);

	}

	return FALSE;
}

/**
 * ggit_repository_create_blob:
 * @repository: a #GgitRepository.
 *
 * Create a new blob and return a #GOutputStream to write contents to the blob.
 * This is an efficient way to create new blobs without copying data. The
 * blob id can be obtained from the blob output stream using
 * #ggit_blob_output_stream_get_id, after you close the stream.
 *
 * Returns: (transfer full) (nullable): a #GgitBlobOutputStream.
 *
 **/
GgitBlobOutputStream *
ggit_repository_create_blob (GgitRepository *repository)
{
	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), NULL);
	return _ggit_blob_output_stream_new (repository);
}

/**
 * ggit_repository_create_blob_from_buffer:
 * @repository: a #GgitRepository.
 * @buffer: (array length=size) (element-type guint8): the data.
 * @size: the length (in bytes) of the data.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Write an in-memory buffer to the object database as a blob.
 *
 * Returns: (transfer full) (nullable): the new #GgitOid of the written blob,
 * or %NULL if writing the blob failed.
 */
GgitOId *
ggit_repository_create_blob_from_buffer (GgitRepository  *repository,
                                         gconstpointer    buffer,
                                         gsize            size,
                                         GError         **error)
{
	gint ret;
	git_oid oid;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), NULL);
	g_return_val_if_fail (buffer != NULL, NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	ret = git_blob_create_frombuffer (&oid,
	                                  _ggit_native_get (repository),
	                                  buffer,
	                                  size);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return NULL;
	}

	return _ggit_oid_wrap (&oid);
}

/**
 * ggit_repository_create_blob_from_file:
 * @repository: a #GgitRepository.
 * @file: a #GFile.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Write a file to the object database as a blob.
 *
 * Returns (transfer full) (nullable): the new #GgitOid of the written blob,
 * or %NULL if writing the blob failed.
 */
GgitOId *
ggit_repository_create_blob_from_file (GgitRepository  *repository,
                                       GFile           *file,
                                       GError         **error)
{
	gint ret;
	git_oid oid;
	gchar *path;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), NULL);
	g_return_val_if_fail (G_IS_FILE (file), NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	path = g_file_get_path (file);

	ret = git_blob_create_fromdisk (&oid,
	                                _ggit_native_get (repository),
	                                path);

	g_free (path);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return NULL;
	}

	return _ggit_oid_wrap (&oid);
}

/**
 * ggit_repository_create_blob_from_path:
 * @repository: a #GgitRepository.
 * @path: the file path.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Write a path relative to the repository working directory to the object
 * database as a blob.
 *
 * Returns: (transfer full) (nullable): the new #GgitOid of the written blob,
 * or %NULL if writing the blob failed.
 */
GgitOId *
ggit_repository_create_blob_from_path (GgitRepository  *repository,
                                       const gchar     *path,
                                       GError         **error)
{
	gint ret;
	git_oid oid;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), NULL);
	g_return_val_if_fail (path != NULL, NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	ret = git_blob_create_fromworkdir (&oid,
	                                _ggit_native_get (repository),
	                                path);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return NULL;
	}

	return _ggit_oid_wrap (&oid);
}

/**
 * ggit_repository_create_commit:
 * @repository: a #GgitRepository.
 * @update_ref: (allow-none): name of the reference to update.
 * @author: author signature.
 * @committer: committer signature (and time of commit).
 * @message_encoding: (allow-none): message encoding.
 * @message: commit message.
 * @tree: the tree of objects to commit.
 * @parents: (array length=parent_count): parent commits.
 * @parent_count: number of parent commits in @parents.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Create a new commit. If @update_ref is not %NULL, the given reference will
 * be updated to point to the newly created commit. Use "HEAD" to update the
 * HEAD of the current branch and make it point to this commit.
 *
 * If @message_encoding is set to %NULL, "UTF-8" encoding is assumed for the
 * provided @message. Note that @message will not be cleaned up automatically.
 * You can use #ggit_message_prettify to do this yourself if needed.
 *
 * Returns: (transfer full) (nullable): the #GgitOId of the created commit object,
 * or %NULL in case of an error.
 *
 */
GgitOId *
ggit_repository_create_commit (GgitRepository  *repository,
                               const gchar     *update_ref,
                               GgitSignature   *author,
                               GgitSignature   *committer,
                               const gchar     *message_encoding,
                               const gchar     *message,
                               GgitTree        *tree,
                               GgitCommit     **parents,
                               gint             parent_count,
                               GError         **error)
{
	gint ret;
	git_oid oid;
	git_commit **parents_native;
	gint i;

	parents_native = g_new0 (git_commit *, parent_count);

	for (i = 0; i < parent_count; ++i)
	{
		parents_native[i] = _ggit_native_get (parents[i]);
	}

	ret = git_commit_create (&oid,
	                         _ggit_native_get (repository),
	                         update_ref,
	                         _ggit_native_get (author),
	                         _ggit_native_get (committer),
	                         message_encoding,
	                         message,
	                         _ggit_native_get (tree),
	                         parent_count,
	                         (const git_commit **)parents_native);

	g_free (parents_native);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return NULL;
	}

	return _ggit_oid_wrap (&oid);
}

/**
 * ggit_repository_create_commit_from_ids:
 * @repository: a #GgitRepository.
 * @update_ref: (allow-none): name of the reference to update.
 * @author: author signature.
 * @committer: committer signature (and time of commit).
 * @message_encoding: (allow-none): message encoding.
 * @message: commit message.
 * @tree: the tree of objects to commit.
 * @parents: (array length=parent_count): parent commits.
 * @parent_count: number of parent commits in @parents.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Create a new commit. If @update_ref is not %NULL, the given reference will
 * be updated to point to the newly created commit. Use "HEAD" to update the
 * HEAD of the current branch and make it point to this commit.
 *
 * If @message_encoding is set to %NULL, "UTF-8" encoding is assumed for the
 * provided @message. Note that @message will not be cleaned up automatically.
 * You can use #ggit_message_prettify to do this yourself if needed.
 *
 * Returns: (transfer full) (nullable): the #GgitOId of the created commit object,
 * or %NULL in case of an error.
 *
 */
GgitOId *
ggit_repository_create_commit_from_ids (GgitRepository  *repository,
                                        const gchar     *update_ref,
                                        GgitSignature   *author,
                                        GgitSignature   *committer,
                                        const gchar     *message_encoding,
                                        const gchar     *message,
                                        GgitOId         *tree,
                                        GgitOId        **parents,
                                        gint             parent_count,
                                        GError         **error)
{
	gint ret;
	git_oid id;
	git_oid **parents_native;
	gint i;

	parents_native = g_new0 (git_oid *, parent_count);

	for (i = 0; i < parent_count; ++i)
	{
		parents_native[i] = (git_oid *)_ggit_oid_get_oid (parents[i]);
	}

	ret = git_commit_create_from_ids (&id,
	                                  _ggit_native_get (repository),
	                                  update_ref,
	                                  _ggit_native_get (author),
	                                  _ggit_native_get (committer),
	                                  message_encoding,
	                                  message,
	                                  _ggit_oid_get_oid (tree),
	                                  parent_count,
	                                  (git_oid const **)parents_native);

	g_free (parents_native);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return NULL;
	}

	return _ggit_oid_wrap (&id);
}

/**
 * ggit_repository_create_tree_builder:
 * @repository: a #GgitRepository.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Create a new tree builder.
 *
 * Returns: (transfer full) (nullable): a new #GgitTreeBuilder,
 * or %NULL if there was an error.
 **/
GgitTreeBuilder *
ggit_repository_create_tree_builder (GgitRepository  *repository,
                                     GError         **error)
{
	gint ret;
	git_treebuilder *builder;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), NULL);

	ret = git_treebuilder_new (&builder,
	                           _ggit_native_get (repository),
	                           NULL);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return NULL;
	}

	return _ggit_tree_builder_wrap (builder, repository, TRUE);
}

/**
 * ggit_repository_create_tree_builder_from_tree:
 * @tree: a #GgitTree.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Create a tree builder for initialized with @tree. To create an empty
 * tree builder, use #ggit_repository_create_tree_builder instead.
 *
 * Returns: (transfer full) (nullable): a new #GgitTreeBuilder object,
 * or %NULL if there was an error.
 **/
GgitTreeBuilder *
ggit_repository_create_tree_builder_from_tree (GgitRepository  *repository,
                                               GgitTree        *tree,
                                               GError         **error)
{
	gint ret;
	git_treebuilder *builder;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), NULL);
	g_return_val_if_fail (GGIT_IS_TREE (tree), NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	ret = git_treebuilder_new (&builder,
	                           _ggit_native_get (repository),
	                           _ggit_native_get (tree));

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return NULL;
	}

	return _ggit_tree_builder_wrap (builder, repository, TRUE);
}

/**
 * ggit_repository_create_index_entry_for_file:
 * @repository: a #GgitRepository.
 * @file: (allow-none): a #GFile.
 * @id: (allow-none): a #GgitOId.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Create a new index entry. When @file is not %NULL, the path of the returned
 * entry (#ggit_index_entry_get_path) is set to the path of @file relative to
 * the working directory of @repository. The file must reside in the working
 * directory of @repository. The file related
 * fields of the returned entry are also queried from this file (if the file exists).
 *
 * If @id is not %NULL, then the id of the returned entry is set to @id
 * (see #ggit_index_entry_get_id) which could point to a blob (for a file)
 * or a tree (for a directory).
 *
 * Returns: (transfer full) (nullable): a #GgitIndexEntry or %NULL when an error occurred.
 *
 **/
GgitIndexEntry *
ggit_repository_create_index_entry_for_file (GgitRepository  *repository,
                                             GFile           *file,
                                             GgitOId         *id,
                                             GError         **error)
{
	GgitRepositoryPrivate *priv;
	gchar *path = NULL;
	GgitIndexEntry *ret;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), NULL);
	g_return_val_if_fail (file == NULL || G_IS_FILE (file), NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	priv = ggit_repository_get_instance_private (repository);

	if (file != NULL)
	{
		path = g_file_get_relative_path (priv->workdir, file);

		if (!path)
		{
			g_set_error_literal (error,
			                     G_IO_ERROR,
			                     G_IO_ERROR_NOT_FOUND,
			                     "File is not in the working directory");

			return NULL;
		}
	}

	ret = _ggit_index_entry_new (path, id);
	g_free (path);

	if (file && !id)
	{
		ggit_index_entry_stat (ret, file, NULL);
	}

	return ret;
}

/**
 * ggit_repository_create_index_entry_for_path:
 * @repository: a #GgitRepository.
 * @path: (allow-none): a path.
 * @id: (allow-none): a #GgitOId.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Create a new index entry. When @path is not %NULL, the path of the returned
 * entry (#ggit_index_entry_get_path) is set to @path. The specified path can be
 * either absolute or relative. In the case of
 * an absolute path, the path must reside within the working directory of
 * @repository. The file related fields of the returned entry are also queried
 * from this path (if the file exists).
 *
 * If @id is not %NULL, then the id of the returned entry is set to @id
 * (see #ggit_index_entry_get_id) which could point to a blob (for a file)
 * or a tree (for a directory).
 *
 * Returns: (transfer full) (nullable): a #GgitIndexEntry or %NULL when an error occurred.
 *
 **/
GgitIndexEntry *
ggit_repository_create_index_entry_for_path (GgitRepository  *repository,
                                             const gchar     *path,
                                             GgitOId         *id,
                                             GError         **error)
{
	GgitRepositoryPrivate *priv;
	GFile *f;
	GgitIndexEntry *ret;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	priv = ggit_repository_get_instance_private (repository);

	if (path)
	{
		if (!g_path_is_absolute (path))
		{
			f = g_file_resolve_relative_path (priv->workdir, path);
		}
		else
		{
			f = g_file_new_for_path (path);
		}
	}
	else
	{
		f = NULL;
	}

	ret = ggit_repository_create_index_entry_for_file (repository,
	                                                   f,
	                                                   id,
	                                                   error);

	g_object_unref (f);

	return ret;
}

/**
 * ggit_repository_blame_file:
 * @repository: a #GgitRepository.
 * @file: the file to blame.
 * @blame_options: (allow-none): blame options.
 * @error: a #GError.
 *
 * Get a blame for a single file.
 *
 * Returns: (transfer full) (nullable): a #GgitBlame.
 *
 **/
GgitBlame *
ggit_repository_blame_file (GgitRepository    *repository,
                            GFile             *file,
                            GgitBlameOptions  *blame_options,
                            GError           **error)
{
	GgitRepositoryPrivate *priv;
	git_blame *blame;
	gchar *path;
	int ret;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), NULL);
	g_return_val_if_fail (G_IS_FILE (file), NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	priv = ggit_repository_get_instance_private (repository);

	path = g_file_get_relative_path (priv->workdir, file);

	ret = git_blame_file (&blame,
	                      _ggit_native_get (repository),
	                      path,
	                      _ggit_blame_options_get_blame_options (blame_options));

	g_free (path);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return NULL;
	}

	return _ggit_blame_wrap (blame);
}

/**
 * ggit_repository_get_attribute:
 * @repository: a #GgitRepository.
 * @path: the relative path to the file.
 * @name: the name of the attribute.
 * @flags: a #GgitAttributeCheckFlags.
 * @error: a #GError.
 *
 * Get the attribute value of the specified attribute for the given file.
 *
 * Returns: (transfer none) (nullable): the attribute value, or %NULL.
 *
 **/
const gchar *
ggit_repository_get_attribute (GgitRepository           *repository,
                               const gchar              *path,
                               const gchar              *name,
                               GgitAttributeCheckFlags   flags,
                               GError                  **error)
{
	const char *value;
	int ret;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), NULL);
	g_return_val_if_fail (path != NULL, NULL);
	g_return_val_if_fail (name != NULL, NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	ret = git_attr_get (&value,
	                    _ggit_native_get (repository),
	                    flags,
	                    path,
	                    name);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return NULL;
	}

	return value;
}

/**
 * ggit_repository_checkout_head:
 * @repository: a #GgitRepository.
 * @options: (allow-none): a #GgitCheckoutOptions or %NULL.
 * @error: a #GError for error reporting or %NULL.
 *
 * Update files in the working tree to reflect the contents of current HEAD. If
 * @options is %NULL, then the default checkout options will be used.
 *
 * If the checkout was not successfull, then @error will be set.
 *
 * Returns: %TRUE if the checkout was successfull, %FALSE otherwise.
 *
 **/
gboolean
ggit_repository_checkout_head (GgitRepository       *repository,
                               GgitCheckoutOptions  *options,
                               GError              **error)
{
	gint ret;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), FALSE);
	g_return_val_if_fail (GGIT_IS_CHECKOUT_OPTIONS (options), FALSE);
	g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

	ret = git_checkout_head (_ggit_native_get (repository),
	                         _ggit_checkout_options_get_checkout_options (options));

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return FALSE;
	}

	return TRUE;
}

/**
 * ggit_repository_checkout_index:
 * @repository: a #GgitRepository.
 * @index: (allow-none): a #GgitIndex or %NULL.
 * @options: (allow-none): a #GgitCheckoutOptions or %NULL.
 * @error: a #GError for error reporting or %NULL.
 *
 * Update files in the working tree to reflect the contents of the index. If
 * @index is %NULL, then the current index of the repository will be used. If
 * @options is %NULL, then the default checkout options will be used.
 *
 * If the checkout was not successfull, then @error will be set.
 *
 * Returns: %TRUE if the checkout was successfull, %FALSE otherwise.
 *
 **/
gboolean
ggit_repository_checkout_index (GgitRepository       *repository,
                                GgitIndex            *index,
                                GgitCheckoutOptions  *options,
                                GError              **error)
{
	gint ret;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), FALSE);
	g_return_val_if_fail (index == NULL || GGIT_IS_INDEX (index), FALSE);
	g_return_val_if_fail (GGIT_IS_CHECKOUT_OPTIONS (options), FALSE);
	g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

	ret = git_checkout_index (_ggit_native_get (repository),
	                          index != NULL ? _ggit_index_get_index (index) : NULL,
	                         _ggit_checkout_options_get_checkout_options (options));

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return FALSE;
	}

	return TRUE;
}
/**
 * ggit_repository_checkout_tree:
 * @repository: a #GgitRepository.
 * @tree: (allow-none): a #GgitObject or %NULL.
 * @options: (allow-none): a #GgitCheckoutOptions or %NULL.
 * @error: a #GError for error reporting or %NULL.
 *
 * Update files in the working tree to reflect the contents of the specified
 * commit, tag or tree object. If @tree is %NULL, then the current HEAD of the
 * repository will be used. If @options is %NULL, then the default checkout
 * options will be used.
 *
 * If the checkout was not successfull, then @error will be set.
 *
 * Returns: %TRUE if the checkout was successfull, %FALSE otherwise.
 *
 **/
gboolean
ggit_repository_checkout_tree (GgitRepository       *repository,
                               GgitObject           *tree,
                               GgitCheckoutOptions  *options,
                               GError              **error)
{
	gint ret;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), FALSE);
	g_return_val_if_fail (tree == NULL || GGIT_IS_OBJECT (tree), FALSE);
	g_return_val_if_fail (GGIT_IS_CHECKOUT_OPTIONS (options), FALSE);
	g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

	ret = git_checkout_tree (_ggit_native_get (repository),
	                         tree != NULL ? _ggit_native_get (tree) : NULL,
	                         _ggit_checkout_options_get_checkout_options (options));

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return FALSE;
	}

	return TRUE;
}

/**
 * ggit_repository_revert:
 * @repository: a #GgitRepository.
 * @commit: a #GgitCommit.
 * @options: a #GgitRevertOptions.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Revert the given @commit on top of the current working directory.
 *
 * Returns: %TRUE if the revert was successfull, %FALSE otherwise.
 *
 **/
gboolean
ggit_repository_revert (GgitRepository     *repository,
                        GgitCommit         *commit,
                        GgitRevertOptions  *options,
                        GError            **error)
{
	gint ret;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), FALSE);
	g_return_val_if_fail (GGIT_IS_COMMIT (commit), FALSE);
	g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

	ret = git_revert (_ggit_native_get (repository),
	                  _ggit_native_get (commit),
	                  options ? _ggit_revert_options_get_revert_options (options) : NULL);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return FALSE;
	}

	return TRUE;
}

/**
 * ggit_repository_cherry_pick:
 * @repository: a #GgitRepository.
 * @commit: a #GgitCommit.
 * @options: a #GgitCherryPickOptions.
 * @error: a #GError for error reporting or %NULL.
 *
 * Cherry pick the specified commit, making changes in the index and the working
 * directory.
 *
 * Returns: %TRUE if the commit was cherry-picked successfully, %FALSE otherwise.
 *
 **/
gboolean
ggit_repository_cherry_pick (GgitRepository         *repository,
                             GgitCommit             *commit,
                             GgitCherryPickOptions  *options,
                             GError                **error)
{
	gint ret;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), FALSE);
	g_return_val_if_fail (GGIT_IS_COMMIT (commit), FALSE);
	g_return_val_if_fail (options == NULL || GGIT_IS_CHERRY_PICK_OPTIONS (options), FALSE);
	g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

	ret = git_cherrypick (_ggit_native_get (repository),
	                      _ggit_native_get (commit),
	                      _ggit_cherry_pick_options_get_cherry_pick_options (options));

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return FALSE;
	}

	return TRUE;
}

/**
 * ggit_repository_cherry_pick_commit:
 * @repository: a #GgitRepository.
 * @commit: a #GgitCommit to cherry-pick.
 * @our_commit: a #GgitCommit to cherry-pick on.
 * @mainline: the parent of the commit, in case of a merge commit.
 * @merge_options: (allow-none): a #GgitMergeOptions.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Cherry-picks the given @commit against the provided @our_commit, producing
 * and index that reflects the result of the cherry-pick.
 *
 * Returns: (transfer full) (nullable): a #GgitIndex.
 *
 **/
GgitIndex *
ggit_repository_cherry_pick_commit (GgitRepository    *repository,
                                    GgitCommit        *commit,
                                    GgitCommit        *our_commit,
                                    guint              mainline,
                                    GgitMergeOptions  *merge_options,
                                    GError           **error)
{
	gint ret;
	git_index *idx;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), NULL);
	g_return_val_if_fail (GGIT_IS_COMMIT (commit), NULL);
	g_return_val_if_fail (GGIT_IS_COMMIT (our_commit), NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	ret = git_cherrypick_commit (&idx,
	                             _ggit_native_get (repository),
	                             _ggit_native_get (commit),
	                             _ggit_native_get (our_commit),
	                             mainline,
	                             _ggit_merge_options_get_merge_options (merge_options));

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return NULL;
	}

	return _ggit_index_wrap (idx);
}

/**
 * ggit_repository_get_default_notes_ref:
 * @repository: a #GgitRepository.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Gets the default notes reference for @repository. It defaults to
 * "refs/notes/commits".
 *
 * Returns: (transfer full) (nullable): the default notes reference for @repository.
 */
gchar *
ggit_repository_get_default_notes_ref (GgitRepository  *repository,
                                       GError         **error)
{
	git_buf buf = {0,};
	gchar *ref;
	gint ret;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	ret = git_note_default_ref (&buf, _ggit_native_get (repository));

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return NULL;
	}

	ref = g_strdup (buf.ptr);
#if LIBGIT2_SOVERSION >= 28
	git_buf_dispose (&buf);
#else
	git_buf_free (&buf);
#endif

	return ref;
}

/**
 * ggit_repository_create_note:
 * @repository: a #GgitRepository.
 * @notes_ref: (allow-none): canonical name of the reference to use, or %NULL to use the default ref.
 * @author: author signature.
 * @committer: committer signature.
 * @id: OID of the git object to decorate.
 * @note: content of the note to add for object oid.
 * @force: whether to overwrite existing note.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Adds a note for an object.
 *
 * Returns: (transfer full) (nullable): the OID for the note or %NULL in case of error.
 */
GgitOId *
ggit_repository_create_note (GgitRepository  *repository,
                             const gchar     *notes_ref,
                             GgitSignature   *author,
                             GgitSignature   *committer,
                             GgitOId         *id,
                             const gchar     *note,
                             gboolean         force,
                             GError         **error)
{
	gint ret;
	git_oid oid;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), NULL);
	g_return_val_if_fail (GGIT_IS_SIGNATURE (author), NULL);
	g_return_val_if_fail (GGIT_IS_SIGNATURE (committer), NULL);
	g_return_val_if_fail (id != NULL, NULL);
	g_return_val_if_fail (note != NULL, NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	ret = git_note_create (&oid,
	                       _ggit_native_get (repository),
	                       notes_ref,
	                       _ggit_native_get (author),
	                       _ggit_native_get (committer),
	                       _ggit_oid_get_oid (id),
	                       note,
	                       force);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return NULL;
	}

	return _ggit_oid_wrap (&oid);
}

/**
 * ggit_repository_remove_note:
 * @repository: a #GgitRepository.
 * @notes_ref: (allow-none): canonical name of the reference to use, or %NULL to use the default ref.
 * @author: author signature.
 * @committer: committer signature.
 * @id: OID of the git object to decorate.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Removes the note for an object.
 *
 * Returns: %TRUE if the note was removed from @id.
 */
gboolean
ggit_repository_remove_note (GgitRepository  *repository,
                             const gchar     *notes_ref,
                             GgitSignature   *author,
                             GgitSignature   *committer,
                             GgitOId         *id,
                             GError         **error)
{
	gint ret;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), FALSE);
	g_return_val_if_fail (GGIT_IS_SIGNATURE (author), FALSE);
	g_return_val_if_fail (GGIT_IS_SIGNATURE (committer), FALSE);
	g_return_val_if_fail (id != NULL, FALSE);
	g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

	ret = git_note_remove (_ggit_native_get (repository),
	                       notes_ref,
	                       _ggit_native_get (author),
	                       _ggit_native_get (committer),
	                       _ggit_oid_get_oid (id));

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return FALSE;
	}

	return TRUE;
}

/**
 * ggit_repository_read_note:
 * @repository: a #GgitRepository.
 * @notes_ref: (allow-none): canonical name of the reference to use, or %NULL to use the default ref.
 * @id: OID of the git object to decorate.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Reads the note for an object.
 *
 * Returns: (transfer full) (nullable): the read note or %NULL in case of an error.
 */
GgitNote *
ggit_repository_read_note (GgitRepository  *repository,
                           const gchar     *notes_ref,
                           GgitOId         *id,
                           GError         **error)
{
	gint ret;
	git_note *note;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), NULL);
	g_return_val_if_fail (id != NULL, NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	ret = git_note_read (&note,
	                     _ggit_native_get (repository),
	                     notes_ref,
	                     _ggit_oid_get_oid (id));

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return NULL;
	}

	return _ggit_note_wrap (note);
}

typedef struct
{
	gpointer user_data;

	GgitNoteCallback callback;
} NoteCallbackWrapperData;

static gint
note_callback_wrapper (const git_oid *blob_id,
                       const git_oid *annotated_object_id,
                       gpointer       user_data)
{
	NoteCallbackWrapperData *wrapper_data = (NoteCallbackWrapperData *)user_data;
	GgitOId *gblob_id;
	GgitOId *gannotated_object_id;
	gint ret;

	gblob_id = _ggit_oid_wrap (blob_id);
	gannotated_object_id = _ggit_oid_wrap (annotated_object_id);

	ret = wrapper_data->callback (gblob_id, gannotated_object_id,
	                              wrapper_data->user_data);
	ggit_oid_free (gblob_id);
	ggit_oid_free (gannotated_object_id);

	return ret;
}

/**
 * ggit_repository_note_foreach:
 * @repository: a #GgitRepository.
 * @notes_ref: (allow-none): canonical name of the reference to use, or %NULL to use the default ref.
 * @callback: (scope call): a #GgitNoteCallback.
 * @user_data: callback user data.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Loop over all the notes within a specified namespace
 * and issue a callback for each one.
 * If @callback returns a non-zero value, this will stop looping.

 * Returns: %TRUE if there was no error, %FALSE otherwise.
 */
gboolean
ggit_repository_note_foreach (GgitRepository    *repository,
                              const gchar       *notes_ref,
                              GgitNoteCallback   callback,
                              gpointer           user_data,
                              GError           **error)
{
	NoteCallbackWrapperData wrapper_data;
	gint ret;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), FALSE);
	g_return_val_if_fail (callback != NULL, FALSE);
	g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

	wrapper_data.user_data = user_data;
	wrapper_data.callback = callback;

	ret = git_note_foreach (_ggit_native_get (repository),
	                        notes_ref,
	                        note_callback_wrapper,
	                        &wrapper_data);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return FALSE;
	}

	return TRUE;
}

/**
 * ggit_repository_merge_trees:
 * @repository: a #GgitRepository.
 * @ancestor_tree: the common ancestor between the trees, or %NULL.
 * @our_tree: the tree that reflects the destination tree.
 * @their_tree: the tree that reflects the source tree.
 * @merge_options: the merge options.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Merge two trees creating a #GgitIndex reflecting the result of the merge.
 *
 * Returns: (transfer full) (nullable): a new #GgitIndex or %NULL if an error occurred.
 *
 **/
GgitIndex *
ggit_repository_merge_trees (GgitRepository    *repository,
                             GgitTree          *ancestor_tree,
                             GgitTree          *our_tree,
                             GgitTree          *their_tree,
                             GgitMergeOptions  *merge_options,
                             GError           **error)
{
	git_index *out;
	gint ret;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), NULL);
	g_return_val_if_fail (ancestor_tree == NULL || GGIT_IS_TREE (ancestor_tree), NULL);
	g_return_val_if_fail (GGIT_IS_TREE (our_tree), NULL);
	g_return_val_if_fail (GGIT_IS_TREE (their_tree), NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	ret = git_merge_trees (&out,
	                       _ggit_native_get (repository),
	                       _ggit_native_get (ancestor_tree),
	                       _ggit_native_get (our_tree),
	                       _ggit_native_get (their_tree),
	                       _ggit_merge_options_get_merge_options (merge_options));

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return NULL;
	}

	return _ggit_index_wrap (out);
}

/**
 * ggit_repository_merge_commits:
 * @repository: a #GgitRepository.
 * @our_commit: the commit that reflects the destination tree.
 * @their_commit: the commit that reflects the source tree.
 * @merge_options: the merge options.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Merge two commits creating a #GgitIndex reflecting the result of the merge.
 *
 * Returns: (transfer full) (nullable): a new #GgitIndex or %NULL if an error occurred.
 *
 **/
GgitIndex *
ggit_repository_merge_commits (GgitRepository    *repository,
                               GgitCommit        *our_commit,
                               GgitCommit        *their_commit,
                               GgitMergeOptions  *merge_options,
                               GError           **error)
{
	git_index *out;
	gint ret;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), NULL);
	g_return_val_if_fail (GGIT_IS_COMMIT (our_commit), NULL);
	g_return_val_if_fail (GGIT_IS_COMMIT (their_commit), NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	ret = git_merge_commits (&out,
	                         _ggit_native_get (repository),
	                         _ggit_native_get (our_commit),
	                         _ggit_native_get (their_commit),
	                         _ggit_merge_options_get_merge_options (merge_options));

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return NULL;
	}

	return _ggit_index_wrap (out);
}

/**
 * ggit_repository_rebase_init:
 * @repository: a #GgitRepository.
 * @branch: (allow-none): the terminal commit to rebase, or %NULL to rebase the
 * current branch.
 * @upstream: (allow-none): the commit to begin rebasing from, or %NULL to
 * rebase all reachable commits.
 * @onto: (allow-none): the branch to rebase onto, or %NULL to rebase onto
 * the given upstream.
 * @options: a #GgitRebaseOptions to specify how rebase is performed, or %NULL.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Initializes a rebase operation to rebase the changes in @branch
 * relative to @upstream onto another branch. To begin the rebase
 * process, call git_rebase_next(). When you have finished with this
 * object, call g_object_unref().
 *
 * Returns: (transfer full) (nullable): a newly allocated #GgitRebase.
 */
GgitRebase *
ggit_repository_rebase_init (GgitRepository       *repository,
                             GgitAnnotatedCommit  *branch,
                             GgitAnnotatedCommit  *upstream,
                             GgitAnnotatedCommit  *onto,
                             GgitRebaseOptions    *options,
                             GError              **error)
{
	git_rebase *rebase;
	gint ret;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	ret = git_rebase_init (&rebase,
	                       _ggit_native_get (repository),
	                       branch != NULL ? _ggit_annotated_commit_get_annotated_commit (branch) : NULL,
	                       upstream != NULL ? _ggit_annotated_commit_get_annotated_commit (upstream) : NULL,
	                       onto != NULL ? _ggit_annotated_commit_get_annotated_commit (onto) : NULL,
	                       options != NULL ? _ggit_rebase_options_get_rebase_options (options) : NULL);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return NULL;
	}

	return _ggit_rebase_wrap (rebase);
}

/**
 * ggit_repository_rebase_open:
 * @repository: a #GgitRepository.
 * @options: a #GgitRebaseOptions to specify how rebase is performed, or %NULL.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Opens an existing rebase that was previously started by either an
 * invocation of ggit_rebase_init() or by another client.
 *
 * Returns: (transfer full) (nullable): a newly allocated #GgitRebase.
 */
GgitRebase *
ggit_repository_rebase_open (GgitRepository     *repository,
                             GgitRebaseOptions  *options,
                             GError            **error)
{
	git_rebase *rebase;
	gint ret;

	g_return_val_if_fail (GGIT_IS_REPOSITORY (repository), NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	ret = git_rebase_open (&rebase,
	                       _ggit_native_get (repository),
	                       options != NULL ? _ggit_rebase_options_get_rebase_options (options) : NULL);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return NULL;
	}

	return _ggit_rebase_wrap (rebase);
}

/* ex:set ts=8 noet: */
