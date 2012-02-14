/*
 * ggit-convert.c
 * This file is part of libgit2-glib
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

#include "ggit-convert.h"
#include <string.h>

static void
utf8_validate_fallback (gchar  *text,
                        gssize  size)
{
	gchar const *end;

	while (!g_utf8_validate (text, size, &end))
	{
		*((gchar *)end) = '?';
	}
}

static gchar *
convert_fallback (gchar const *text,
                  gssize       size,
                  gchar const *fallback)
{
	gchar *res;
	gsize read, written;
	GString *str = g_string_new ("");

	while ((res = g_convert(text,
	                        size,
	                        "UTF-8",
	                        "ASCII",
	                        &read,
	                        &written,
	                        NULL)) == NULL)
	{
		res = g_convert (text, read, "UTF-8", "ASCII", NULL, NULL, NULL);
		str = g_string_append (str, res);

		str = g_string_append (str, fallback);
		text = text + read + 1;
		size = size - read;
	}

	str = g_string_append (str, res);
	g_free (res);

	utf8_validate_fallback (str->str, str->len);
	return g_string_free (str, FALSE);
}

static gboolean
convert_and_check (const gchar  *str,
                   gssize        size,
                   const gchar  *from_charset,
                   gchar       **ret)
{
	gsize read;
	gsize written;

	*ret = g_convert (str,
	                  size,
	                  "UTF-8",
	                  from_charset,
	                  &read,
	                  &written,
	                  NULL);

	if (*ret && read == size)
	{
		utf8_validate_fallback (*ret, written);
		return TRUE;
	}
	else
	{
		g_free (*ret);
		return FALSE;
	}

}

gchar *
ggit_convert_utf8 (const gchar *str,
                   gssize       size,
                   const gchar *from_charset)
{
	const gchar *locale_charset;

	if (size == -1)
	{
		size = strlen (str);
	}

	if (from_charset == NULL)
	{
		if (g_utf8_validate (str, size, NULL))
		{
			return g_strndup (str, size);
		}
	}

	if (from_charset != NULL &&
	    g_ascii_strcasecmp (from_charset, "UTF-8"))
	{
		gchar *ret = g_strndup (str, size);
		utf8_validate_fallback (ret, size);

		return ret;
	}
	else if (from_charset)
	{
		gchar *ret;

		if (convert_and_check (str, size, from_charset, &ret))
		{
			return ret;
		}
	}

	if (!g_get_charset (&locale_charset))
	{
		gchar *ret;

		if (convert_and_check (str, size, locale_charset, &ret))
		{
			return ret;
		}
	}

	return convert_fallback (str, size, "?");
}
