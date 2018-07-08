/*
 * ggit-config.c
 * This file is part of libggit
 *
 * Copyright (C) 2012 - Jesse van den Kieboom
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

#include "ggit-config.h"
#include "ggit-config-entry.h"

#include <git2.h>

#include "ggit-error.h"

/**
 * GgitConfig:
 *
 * Represents a git configuration.
 */
struct _GgitConfig
{
	GgitNative parent_instance;
};

G_DEFINE_TYPE (GgitConfig, ggit_config, GGIT_TYPE_NATIVE)

typedef struct
{
	gpointer user_data;

	GgitConfigCallback callback;
} CallbackWrapperData;

GgitConfig *
_ggit_config_wrap (git_config *config)
{
	GgitConfig *ret;

	g_return_val_if_fail (config != NULL, NULL);

	ret = g_object_new (GGIT_TYPE_CONFIG, "native", config, NULL);

	return ret;
}

static void
ggit_config_finalize (GObject *object)
{
	G_OBJECT_CLASS (ggit_config_parent_class)->finalize (object);
}

static void
ggit_config_class_init (GgitConfigClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->finalize = ggit_config_finalize;
}

static void
ggit_config_init (GgitConfig *config)
{
	_ggit_native_set_destroy_func (config, (GDestroyNotify) git_config_free);
}

/**
 * ggit_config_new:
 *
 * Create a new config. See also ggit_config_get_default() to get
 * a #GgitConfig representing the global, XDG and system configuration files.
 * To get a #GgitConfig for a repository use #ggit_repository_get_config
 * instead.
 *
 * Returns: (transfer full): a #GgitConfig.
 *
 **/
GgitConfig *
ggit_config_new (void)
{
	git_config *config;

	git_config_new (&config);

	return _ggit_config_wrap (config);
}

/**
 * ggit_config_new_from_file:
 * @file: the file to load.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Create a new config from a single on disk file. This is a convenience
 * API and is exactly the same as creating an empty #GgitConfig using
 * #ggit_config_new and adding the file with #ggit_config_add_file. The
 * level will be set to #GGIT_CONFIG_LEVEL_LOCAL. If the config could not be
 * loaded this function returns %NULL and @error will be set accordingly.
 *
 * Returns: (transfer full): a #GgitConfig.
 *
 **/
GgitConfig *
ggit_config_new_from_file (GFile   *file,
                           GError **error)
{
	git_config *config;
	gchar *path;
	gint ret;

	g_return_val_if_fail (G_IS_FILE (file), NULL);

	path = g_file_get_path (file);

	g_return_val_if_fail (path != NULL, NULL);

	ret = git_config_open_ondisk (&config, path);
	g_free (path);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return NULL;
	}
	else
	{
		return _ggit_config_wrap (config);
	}
}

/**
 * ggit_config_new_default:
 * @error: a #GError for error reporting, or %NULL.
 *
 * Get the global, XDG and system configuration files merged into one
 * #GgitConfig with their appropriate priority levels. If an error occured
 * trying to load the various configuration files, this function will return
 * %NULL and @error will be set accordingly.
 *
 * Returns: (transfer none): A #GgitConfig
 *
 **/
GgitConfig *
ggit_config_new_default (GError **error)
{
	git_config *config;
	gint ret;

	ret = git_config_open_default (&config);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return NULL;
	}
	else
	{
		return _ggit_config_wrap (config);
	}
}

/**
 * ggit_config_find_global:
 *
 * Find the file representing the users global git configuration. This file
 * is usually located at $HOME/.gitconfig. This function will try to guess
 * the full path to that file, if the file exists. The returned file may then
 * be used with #ggit_config_new_from_file or #ggit_config_add_file. This
 * function returns %NULL if the global config could not be found.
 *
 * Returns: (transfer full): a #GFile or %NULL if the global config could not be found.
 *
 **/
GFile *
ggit_config_find_global (void)
{
	git_buf buf = {0,};
	GFile *path = NULL;

	if (git_config_find_global (&buf) == GIT_OK)
	{
		path = g_file_new_for_path (buf.ptr);
		git_buf_free (&buf);
	}

	return path;
}

/**
 * ggit_config_find_system:
 *
 * Find the file representing the systems global git configuration. This file
 * is usually located at /etc/gitconfig on UNIX type systems or
 * %PROGRAMFILES%\Git\etc\gitconfig on windows. This function will try to guess
 * the full path to that file, if the file exists. The returned file may then
 * be used with #ggit_config_new_from_file or #ggit_config_add_file. This
 * function returns %NULL if the system config could not be found.
 *
 * Returns: (transfer full): a #GFile or %NULL if the system config could not
 *                           be found.
 *
 **/
GFile *
ggit_config_find_system (void)
{
	git_buf buf = {0,};
	GFile *path = NULL;

	if (git_config_find_system (&buf) == GIT_OK)
	{
		path = g_file_new_for_path (buf.ptr);
		git_buf_free (&buf);
	}

	return path;
}

/**
 * ggit_config_open_level:
 * @config: a #GgitConfig.
 * @level: the level to open.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Open a specific level config derived from a multi-level one.
 *
 * Returns: (transfer full): the configuration at @level, or %NULL.
 *
 **/
GgitConfig *
ggit_config_open_level (GgitConfig       *config,
                        GgitConfigLevel   level,
                        GError          **error)
{
	gint ret;
	git_config *out;

	g_return_val_if_fail (GGIT_IS_CONFIG (config), NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	ret = git_config_open_level (&out, _ggit_native_get (config), (git_config_level_t)level);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return NULL;
	}

	return _ggit_config_wrap (out);
}

/**
 * ggit_config_add_file:
 * @config: a #GgitConfig.
 * @file: a #GFile.
 * @level: a #GgitConfigLevel.
 * @force: if a config file already exists for the given priority level, replace it.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Add an on-disk config file instance to an existing config
 *
 * The on-disk file pointed at by @file will be opened and
 * parsed; it's expected to be a native Git config file following
 * the default Git config syntax (see man git-config).
 *
 * Further queries on this config object will access each
 * of the config file instances in order (instances with
 * a higher priority level will be accessed first).
 */
void
ggit_config_add_file (GgitConfig      *config,
                      GFile           *file,
                      GgitConfigLevel  level,
                      gboolean         force,
                      GError         **error)
{
	gint ret;
	gchar *path;

	g_return_if_fail (GGIT_IS_CONFIG (config));
	g_return_if_fail (G_IS_FILE (file));
	g_return_if_fail (error == NULL || *error == NULL);

	path = g_file_get_path (file);
	ret = git_config_add_file_ondisk (_ggit_native_get (config),
	                                  path,
	                                  (git_config_level_t)level,
#if LIBGIT2_SOVERSION >= 27
	                                  NULL,
#endif
	                                  force);
	g_free (path);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
	}
}

/**
 * ggit_config_get_int32:
 * @config: a #GgitConfig.
 * @name: the name of the configuration value.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Get a int32 configuration value.
 *
 * Returns: the value.
 *
 **/
gint32
ggit_config_get_int32 (GgitConfig   *config,
                       const gchar  *name,
                       GError      **error)
{
	gint ret;
	gint32 retval;

	g_return_val_if_fail (GGIT_IS_CONFIG (config), 0);
	g_return_val_if_fail (name != NULL, 0);
	g_return_val_if_fail (error == NULL || *error == NULL, 0);

	ret = git_config_get_int32 (&retval, _ggit_native_get (config), name);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return 0;
	}

	return retval;
}

/**
 * ggit_config_set_int32:
 * @config: a #GgitConfig.
 * @name: the name of the configuration value.
 * @value: the new value.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Set a int32 value.
 *
 * Returns: %TRUE if the value was successfully set, %FALSE otherwise.
 *
 **/
gboolean
ggit_config_set_int32 (GgitConfig   *config,
                       const gchar  *name,
                       gint32        value,
                       GError      **error)
{
	gint ret;

	g_return_val_if_fail (GGIT_IS_CONFIG (config), FALSE);
	g_return_val_if_fail (name != NULL, FALSE);
	g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

	ret = git_config_set_int32 (_ggit_native_get (config), name, value);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return FALSE;
	}

	return TRUE;
}

/**
 * ggit_config_get_int64:
 * @config: a #GgitConfig.
 * @name: the name of the configuration value.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Get a int64 configuration value.
 *
 * Returns: the value.
 *
 **/
gint64
ggit_config_get_int64 (GgitConfig   *config,
                       const gchar  *name,
                       GError      **error)
{
	gint ret;
	int64_t retval;

	g_return_val_if_fail (GGIT_IS_CONFIG (config), 0);
	g_return_val_if_fail (name != NULL, 0);
	g_return_val_if_fail (error == NULL || *error == NULL, 0);

	ret = git_config_get_int64 (&retval, _ggit_native_get (config), name);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return 0;
	}

	return (gint64)retval;
}

/**
 * ggit_config_set_int64:
 * @config: a #GgitConfig.
 * @name: the name of the configuration value.
 * @value: the new value.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Set a int64 value.
 *
 * Returns: %TRUE if the value was successfully set, %FALSE otherwise.
 *
 **/
gboolean
ggit_config_set_int64 (GgitConfig   *config,
                       const gchar  *name,
                       gint64        value,
                       GError      **error)
{
	gint ret;

	g_return_val_if_fail (GGIT_IS_CONFIG (config), FALSE);
	g_return_val_if_fail (name != NULL, FALSE);
	g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

	ret = git_config_set_int64 (_ggit_native_get (config), name, value);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return FALSE;
	}

	return TRUE;
}

/**
 * ggit_config_get_bool:
 * @config: a #GgitConfig.
 * @name: the name of the configuration value.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Get a boolean configuration value.
 *
 * Returns: the value.
 *
 **/
gboolean
ggit_config_get_bool (GgitConfig   *config,
                      const gchar  *name,
                      GError      **error)
{
	gint ret;
	gint retval;

	g_return_val_if_fail (GGIT_IS_CONFIG (config), FALSE);
	g_return_val_if_fail (name != NULL, FALSE);
	g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

	ret = git_config_get_bool (&retval, _ggit_native_get (config), name);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return FALSE;
	}

	return (retval > 0);
}

/**
 * ggit_config_set_bool:
 * @config: a #GgitConfig.
 * @name: the name of the configuration value.
 * @value: the new value.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Set a boolean value.
 *
 * Returns: %TRUE if the value was successfully set, %FALSE otherwise.
 *
 **/
gboolean
ggit_config_set_bool (GgitConfig   *config,
                      const gchar  *name,
                      gboolean      value,
                      GError      **error)
{
	gint ret;

	g_return_val_if_fail (GGIT_IS_CONFIG (config), FALSE);
	g_return_val_if_fail (name != NULL, FALSE);
	g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

	ret = git_config_set_bool (_ggit_native_get (config), name, value ? 1 : 0);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return FALSE;
	}

	return TRUE;
}

/**
 * ggit_config_get_string:
 * @config: a #GgitConfig.
 * @name: the name of the configuration value.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Get the configuration value of @name as string.
 *
 * Returns: (allow-none): the string value of @name, or %NULL if such a value
 *                        does not exist
 *
 **/
const gchar *
ggit_config_get_string (GgitConfig   *config,
                        const gchar  *name,
                        GError      **error)
{
	gint ret;
	const gchar *retval;

	g_return_val_if_fail (GGIT_IS_CONFIG (config), FALSE);
	g_return_val_if_fail (name != NULL, FALSE);
	g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

	ret = git_config_get_string (&retval, _ggit_native_get (config), name);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return NULL;
	}

	return retval;
}

/**
 * ggit_config_set_string:
 * @config: a #GgitConfig.
 * @name: the name of the configuration value.
 * @value: the new value.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Set a new string value of a configuration.
 *
 * Returns: %TRUE if the value was successfully set, %FALSE otherwise.
 *
 **/
gboolean
ggit_config_set_string (GgitConfig   *config,
                        const gchar  *name,
                        const gchar  *value,
                        GError      **error)
{
	gint ret;

	g_return_val_if_fail (GGIT_IS_CONFIG (config), FALSE);
	g_return_val_if_fail (name != NULL, FALSE);
	g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

	if (value == NULL)
	{
		ret = git_config_delete_entry (_ggit_native_get (config), name);
	}
	else
	{
		ret = git_config_set_string (_ggit_native_get (config),
		                             name, value);
	}

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return FALSE;
	}

	return TRUE;
}

/**
 * ggit_config_get_entry:
 * @config: a #GgitConfig.
 * @name: the configuration name.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Get #GgitConfigEntry of a config variable.
 *
 * Returns: (transfer full): the entry of @name, or %NULL if such a value
 *                           does not exist.
 *
 **/
GgitConfigEntry *
ggit_config_get_entry (GgitConfig   *config,
                       const gchar  *name,
                       GError      **error)
{
	git_config_entry *entry;
	gint ret;

	g_return_val_if_fail (GGIT_IS_CONFIG (config), NULL);
	g_return_val_if_fail (name != NULL, NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	ret = git_config_get_entry (&entry, _ggit_native_get (config), name);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return NULL;
	}

	return _ggit_config_entry_wrap (entry);
}

/**
 * ggit_config_delete_entry:
 * @config: a #GgitConfig.
 * @name: the configuration value.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Delete a config variable from the config file.
 *
 * Returns: %TRUE if the configuration value was deleted, %FALSE otherwise.
 *
 **/
gboolean
ggit_config_delete_entry (GgitConfig   *config,
                          const gchar  *name,
                          GError      **error)
{
	gint ret;

	g_return_val_if_fail (GGIT_IS_CONFIG (config), FALSE);
	g_return_val_if_fail (name != NULL, FALSE);
	g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

	ret = git_config_delete_entry (_ggit_native_get (config), name);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return FALSE;
	}

	return TRUE;
}

static gint
callback_wrapper (const git_config_entry *config_entry,
                  gpointer                payload)
{
	CallbackWrapperData *wrapper_data = (CallbackWrapperData *)payload;
	GgitConfigEntry *entry;
	gint ret = 0;

	entry = _ggit_config_entry_wrap (config_entry);

	if (wrapper_data->callback != NULL)
	{
		ret = wrapper_data->callback (entry, wrapper_data->user_data);
	}

	ggit_config_entry_unref (entry);

	return ret;
}

/**
 * ggit_config_foreach:
 * @config: a #GgitConfig.
 * @callback: (scope call): a #GgitConfigCallback.
 * @user_data: (allow-none): the user data for @callback.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Call @callback for each configuration value.
 *
 * Returns: %TRUE if successfull, %FALSE otherwise
 *
 **/
gboolean
ggit_config_foreach (GgitConfig          *config,
                     GgitConfigCallback   callback,
                     gpointer             user_data,
                     GError             **error)
{
	gint ret;
	CallbackWrapperData wrapper_data;

	g_return_val_if_fail (GGIT_IS_CONFIG (config), FALSE);
	g_return_val_if_fail (callback != NULL, FALSE);
	g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

	wrapper_data.user_data = user_data;
	wrapper_data.callback = callback;

	ret = git_config_foreach (_ggit_native_get (config), callback_wrapper, &wrapper_data);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return FALSE;
	}

	return TRUE;
}

typedef struct
{
	GRegex *regex;
	GMatchInfo *ret;

	gchar *value;

	GgitConfigMatchCallback callback;
	gpointer user_data;
} MatchInfo;

static gint
match_foreach (GgitConfigEntry *entry,
               MatchInfo       *info)
{
	const gchar *name;

	name = ggit_config_entry_get_name (entry);

	if (g_regex_match (info->regex, name, (GRegexMatchFlags)0, &info->ret))
	{
		const gchar *value;

		value = ggit_config_entry_get_value (entry);

		if (info->callback)
		{
			gint ret;

			ret = info->callback (info->ret, value, info->user_data);
			g_match_info_free (info->ret);

			return ret;
		}
		else
		{
			g_free (info->value);
			info->value = g_strdup (value);

			/* Semi arbitrary error */
			return GGIT_ERROR_EXISTS;
		}
	}

	return GIT_OK;
}

/**
 * ggit_config_match:
 * @config: a #GgitConfig.
 * @regex: a #GRegex.
 * @match_info: (out) (allow-none): a #GMatchInfo.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Matches a configuration against a regular expression. @match_info will
 * contain the match information if the return value is not %NULL, otherwise
 * @error will be set.
 *
 * Returns: (allow-none) (transfer full): the value of that matched configuration
 *
 **/
gchar *
ggit_config_match (GgitConfig  *config,
                   GRegex      *regex,
                   GMatchInfo **match_info,
                   GError     **error)
{
	MatchInfo info = {0,};

	g_return_val_if_fail (GGIT_IS_CONFIG (config), FALSE);
	g_return_val_if_fail (regex != NULL, FALSE);
	g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

	info.regex = regex;

	ggit_config_foreach (config, (GgitConfigCallback)match_foreach, &info, NULL);

	if (info.ret)
	{
		if (match_info)
		{
			*match_info = info.ret;
		}
		else
		{
			g_match_info_free (info.ret);
		}
	}

	if (!info.value)
	{
		_ggit_error_set (error, GGIT_ERROR_NOTFOUND);
	}

	return info.value;
}

/**
 * ggit_config_match_foreach:
 * @config: a #GgitConfig.
 * @regex: a #GRegex.
 * @callback: (scope call) (closure user_data): a #GgitConfigMatchCallback.
 * @user_data: (closure): the user data for @callback.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Call @callback for all configurations matching @regex.
 *
 * Returns: %TRUE if matches were successfull, %FALSE otherwise
 *
 **/
gboolean
ggit_config_match_foreach (GgitConfig               *config,
                           GRegex                   *regex,
                           GgitConfigMatchCallback   callback,
                           gpointer                  user_data,
                           GError                  **error)
{
	MatchInfo info = {0,};

	g_return_val_if_fail (GGIT_IS_CONFIG (config), FALSE);
	g_return_val_if_fail (regex != NULL, FALSE);
	g_return_val_if_fail (callback != NULL, FALSE);
	g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

	info.regex = regex;
	info.callback = callback;
	info.user_data = user_data;

	return ggit_config_foreach (config,
	                            (GgitConfigCallback)match_foreach,
	                             &info,
	                             error);
}

/**
 * ggit_config_snapshot:
 * @config: a #GgitConfig.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Create a snapshot of the current state of the configuration,
 * which allows you to look into a consistent view of the configuration
 * for looking up complex values (e.g. a remote, submodule).
 *
 * Returns: (transfer full): a new #GgitConfig, or %NULL if an error occurred.
 *
 **/
GgitConfig *
ggit_config_snapshot (GgitConfig  *config,
                      GError     **error)
{
	git_config *retconf;
	gint ret;

	g_return_val_if_fail (GGIT_IS_CONFIG (config), NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

	ret = git_config_snapshot (&retconf, _ggit_native_get (config));

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return NULL;
	}

	return _ggit_config_wrap (retconf);
}

/* ex:set ts=8 noet: */
