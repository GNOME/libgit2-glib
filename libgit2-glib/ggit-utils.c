/*
 * ggit-utils.c
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2011 - Ignacio Casal Quinteiro
 *
 * libgit2-glib is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * libgit2-glib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libgit2-glib; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA  02110-1301  USA
 */


#include "ggit-blob.h"
#include "ggit-commit.h"
#include "ggit-tag.h"
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
	GgitObject *object = NULL;
	git_otype otype;

	otype = git_object_type (obj);

	if (otype == GIT_OBJ_TAG)
	{
		object = GGIT_OBJECT (_ggit_tag_wrap ((git_tag *)obj, owned));
	}
	else if (otype == GGIT_TYPE_BLOB)
	{
		object = GGIT_OBJECT (_ggit_blob_wrap ((git_blob *)obj, owned));
	}
	else if (otype == GIT_OBJ_COMMIT)
	{
		object = GGIT_OBJECT (_ggit_commit_wrap ((git_commit *)obj, owned));
	}

	return object;
}

/* ex:set ts=8 noet: */
