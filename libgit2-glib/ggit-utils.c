/*
 * ggit-utils.c
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

#include <git2/errors.h>

#include "ggit-blob.h"
#include "ggit-commit.h"
#include "ggit-tag.h"
#include "ggit-tree.h"
#include "ggit-utils.h"

git_otype
ggit_utils_get_otype_from_gtype (GType gtype)
{
	git_otype otype;

	if (g_type_is_a (gtype, GGIT_TYPE_TAG))
	{
		otype = GIT_OBJ_TAG;
	}
	else if (g_type_is_a (gtype, GGIT_TYPE_BLOB))
	{
		otype = GIT_OBJ_BLOB;
	}
	else if (g_type_is_a (gtype, GGIT_TYPE_COMMIT))
	{
		otype = GIT_OBJ_COMMIT;
	}
	else if (g_type_is_a (gtype, GGIT_TYPE_TREE))
	{
		otype = GIT_OBJ_TREE;
	}
	else if (g_type_is_a (gtype, G_TYPE_NONE))
	{
		otype = GIT_OBJ_ANY;
	}
	else
	{
		otype = GIT_OBJ_BAD;
	}

	return otype;
}

GType
ggit_utils_get_gtype_from_otype (git_otype type)
{
	switch (type)
	{
		case GIT_OBJ_TAG:
			return GGIT_TYPE_TAG;
		case GIT_OBJ_BLOB:
			return GGIT_TYPE_BLOB;
		case GIT_OBJ_TREE:
			return GGIT_TYPE_TREE;
		case GIT_OBJ_COMMIT:
			return GGIT_TYPE_COMMIT;
		default:
			return G_TYPE_NONE;
	}
}

GgitObject *
ggit_utils_create_real_object (git_object *obj,
                               gboolean    owned)
{
	git_otype otype;

	otype = git_object_type (obj);

	switch (otype)
	{
		case GIT_OBJ_TAG:
			return GGIT_OBJECT (_ggit_tag_wrap ((git_tag *)obj, owned));
		case GIT_OBJ_BLOB:
			return GGIT_OBJECT (_ggit_blob_wrap ((git_blob *)obj, owned));
		case GIT_OBJ_COMMIT:
			return GGIT_OBJECT (_ggit_commit_wrap ((git_commit *)obj, owned));
		case GIT_OBJ_TREE:
			return GGIT_OBJECT (_ggit_tree_wrap ((git_tree *)obj, owned));
		default:
			return NULL;
	}
}

gchar **
ggit_utils_get_str_array_from_git_strarray (git_strarray *gitarray)
{
	gchar **array;
	gint i;

	array = g_new (gchar *, gitarray->count + 1);

	for (i = 0; i < gitarray->count; i++)
	{
		array[i] = gitarray->strings[i];
	}
	array[i] = NULL;

	return array;
}

void
ggit_utils_get_git_strarray_from_str_array (const gchar  **array,
                                            git_strarray  *gitarray)
{
	git_strarray fake_gitarray;

	if (array == NULL)
	{
		gitarray->count = 0;
		gitarray->strings = NULL;
		return;
	}

	fake_gitarray.count = g_strv_length ((gchar **) array);
	fake_gitarray.strings = (gchar **) array;

	if (git_strarray_copy (gitarray, &fake_gitarray) != GIT_OK)
	{
		/* out of memory */
	}
}

/* ex:set ts=8 noet: */
