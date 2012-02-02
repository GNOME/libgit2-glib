/*
 * ggit-config.c
 * This file is part of libggit
 *
 * Copyright (C) 2012 - Jesse van den Kieboom
 *
 * libggit is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * libggit is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libggit. If not, see <http://www.gnu.org/licenses/>.
 */

#include "ggit-config.h"

#include <git2/errors.h>
#include <git2/config.h>

#include "ggit-error.h"

#define GGIT_CONFIG_GET_PRIVATE(object)(G_TYPE_INSTANCE_GET_PRIVATE((object), GGIT_TYPE_CONFIG, GgitConfigPrivate))

struct _GgitConfigPrivate
{
	git_config *config;
	GFile *file;
};

G_DEFINE_TYPE (GgitConfig, ggit_config, G_TYPE_OBJECT)

enum
{
	PROP_0,
	PROP_FILE
};

static void
ggit_config_finalize (GObject *object)
{
	GgitConfig *config = GGIT_CONFIG (object);

	git_config_free (config->priv->config);

	if (config->priv->file)
	{
		g_object_unref (config->priv->file);
	}

	G_OBJECT_CLASS (ggit_config_parent_class)->finalize (object);
}

static void
ggit_config_constructed (GObject *object)
{
	GgitConfig *config;

	config = GGIT_CONFIG (object);

	if (config->priv->file)
	{
		gchar *path;

		path = g_file_get_path (config->priv->file);

		if (path != NULL)
		{
			git_config_add_file_ondisk (config->priv->config,
			                            path,
			                            0);
		}

		g_free (path);
	}
}

static void
ggit_config_set_property (GObject      *object,
                          guint         prop_id,
                          const GValue *value,
                          GParamSpec   *pspec)
{
	GgitConfig *self = GGIT_CONFIG (object);

	switch (prop_id)
	{
		case PROP_FILE:
		{
			GFile *f;

			f = g_value_get_object (value);

			if (f)
			{
				self->priv->file = g_file_dup (f);
			}

			break;
		}
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
ggit_config_get_property (GObject    *object,
                          guint       prop_id,
                          GValue     *value,
                          GParamSpec *pspec)
{
	GgitConfig *self = GGIT_CONFIG (object);

	switch (prop_id)
	{
		case PROP_FILE:
			g_value_set_object (value, self->priv->file);
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
ggit_config_class_init (GgitConfigClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->finalize = ggit_config_finalize;

	object_class->get_property = ggit_config_get_property;
	object_class->set_property = ggit_config_set_property;

	object_class->constructed = ggit_config_constructed;

	g_type_class_add_private (object_class, sizeof (GgitConfigPrivate));

	g_object_class_install_property (object_class,
	                                 PROP_FILE,
	                                 g_param_spec_object ("file",
	                                                      "File",
	                                                      "File",
	                                                      G_TYPE_FILE,
	                                                      G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY | G_PARAM_STATIC_STRINGS));
}

static void
ggit_config_init (GgitConfig *self)
{
	self->priv = GGIT_CONFIG_GET_PRIVATE (self);
	git_config_new (&self->priv->config);
}

/**
 * ggit_config_new:
 * @file: (allow-none): a #GFile
 *
 * Create a new config for a given file. If %NULL is specified for @file, an
 * an empty configuration is created. See also #ggit_config_get_global to get
 * a config representing the global user configuration.
 *
 * Returns: (transfer full): a #GgitConfig
 *
 **/
GgitConfig *
ggit_config_new (GFile *file)
{
	return g_object_new (GGIT_TYPE_CONFIG,
	                     "file", file,
	                     NULL);
}

/**
 * ggit_config_get_global:
 *
 * Get the global config.
 *
 * Returns: (transfer none): A #GgitConfig
 *
 **/
GgitConfig *
ggit_config_get_global ()
{
	static GgitConfig *config = NULL;

	if (!config)
	{
		gchar global_path[GIT_PATH_MAX];

		if (git_config_find_global (global_path) == GIT_SUCCESS)
		{
			GFile *f;

			f = g_file_new_for_path (global_path);
			config = ggit_config_new (f);
			g_object_unref (f);
		}
		else
		{
			config = ggit_config_new (NULL);
		}

		g_object_add_weak_pointer (G_OBJECT (config),
		                           (gpointer *)&config);
	}

	return config;
}

/**
 * ggit_config_get_bool:
 * @config: a #GgitConfig.
 * @name: the name of the configuration value.
 * @error: A #GError
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

	ret = git_config_get_bool (config->priv->config, name, &retval);

	if (ret != GIT_SUCCESS)
	{
		_ggit_error_set (error, ret);
		return FALSE;
	}
	else
	{
		return retval ? TRUE : FALSE;
	}
}

/**
 * ggit_config_set_bool:
 * @config: a #GgitConfig.
 * @name: the name of the configuration value
 * @value: the new value
 * @error: A #GError
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

	ret = git_config_set_bool (config->priv->config, name, value ? 1 : 0);

	if (ret != GIT_SUCCESS)
	{
		_ggit_error_set (error, ret);
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

/**
 * ggit_config_get_string:
 * @config: a #GgitConfig.
 * @name: the name of the configuration value.
 * @error: a #GError
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

	ret = git_config_get_string (config->priv->config, name, &retval);

	if (ret != GIT_SUCCESS)
	{
		_ggit_error_set (error, ret);
		return NULL;
	}
	else
	{
		return retval;
	}
}

/**
 * ggit_config_set_string:
 * @config: a #GgitConfig.
 * @name: the name of the configuration value.
 * @value: the new value
 * @error: A #GError
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
		ret = git_config_delete (config->priv->config, name);
	}
	else
	{
		ret = git_config_set_string (config->priv->config, name, value);
	}

	if (ret != GIT_SUCCESS)
	{
		_ggit_error_set (error, ret);
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

/**
 * ggit_config_delete:
 * @config: a #GgitConfig.
 * @name: the configuration value.
 * @error: A #GError
 *
 * Delete a configuration value.
 *
 * Returns: %TRUE if the configuration value was deleted, %FALSE otherwise.
 *
 **/
gboolean
ggit_config_delete (GgitConfig   *config,
                    const gchar  *name,
                    GError      **error)
{
	gint ret;

	g_return_val_if_fail (GGIT_IS_CONFIG (config), FALSE);
	g_return_val_if_fail (name != NULL, FALSE);
	g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

	ret = git_config_delete (config->priv->config, name);

	if (ret != GIT_SUCCESS)
	{
		_ggit_error_set (error, ret);
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

/**
 * ggit_config_foreach:
 * @config: a #GgitConfig.
 * @callback: (scope call): a #GgitConfigCallback.
 * @user_data: the user data for @callback
 * @error: a #GError.
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

	g_return_val_if_fail (GGIT_IS_CONFIG (config), FALSE);
	g_return_val_if_fail (callback != NULL, FALSE);
	g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

	ret = git_config_foreach (config->priv->config, callback, user_data);

	if (ret != GIT_SUCCESS)
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
	GRegex *regex;
	GMatchInfo *ret;

	const gchar *value;

	GgitConfigMatchCallback callback;
	gpointer user_data;
} MatchInfo;

static gint
match_foreach (const gchar *name,
               const gchar *value,
               MatchInfo   *info)
{
	if (g_regex_match (info->regex, name, (GRegexMatchFlags)0, &info->ret))
	{
		if (info->callback)
		{
			gint ret;

			ret = info->callback (info->ret, value, info->user_data);
			g_match_info_free (info->ret);

			return ret;
		}
		else
		{
			info->value = value;

			/* Semi arbitrary error */
			return GGIT_ERROR_EXISTS;
		}
	}

	return GIT_SUCCESS;
}

/**
 * ggit_config_match:
 * @config: a #GgitConfig.
 * @regex: a #GRegex.
 * @match_info: (out) (allow-none): a #GMatchInfo.
 * @error: a #GError.
 *
 * Matches a configuration against a regular expression. @match_info will
 * contain the match information if the return value is not %NULL, otherwise
 * @error will be set.
 *
 * Returns: (allow-none): the value of that matched configuration
 *
 **/
gchar const *
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
		_ggit_error_set (error, GGIT_ERROR_NOMATCH);
	}

	return info.value;
}

/**
 * ggit_config_match_foreach:
 * @config: a #GgitConfig.
 * @regex: a #GRegex.
 * @callback: (scope call): a #GgitConfigMatchCallback.
 * @user_data: the user data for @callback
 * @error: a #GError
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
	gint ret;

	g_return_val_if_fail (GGIT_IS_CONFIG (config), FALSE);
	g_return_val_if_fail (regex != NULL, FALSE);
	g_return_val_if_fail (callback != NULL, FALSE);
	g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

	info.regex = regex;
	info.callback = callback;
	info.user_data = user_data;

	ret = ggit_config_foreach (config,
	                           (GgitConfigCallback)match_foreach,
	                           &info,
	                           error);

	if (ret != GIT_SUCCESS)
	{
		return FALSE;
	}

	return TRUE;
}

GgitConfig *
_ggit_config_wrap (git_config *config)
{
	GgitConfig *ret;

	g_return_val_if_fail (config != NULL, NULL);

	ret = g_object_new (GGIT_TYPE_CONFIG, NULL);
	git_config_free (ret->priv->config);
	ret->priv->config = config;

	return ret;
}
