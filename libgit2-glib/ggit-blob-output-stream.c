/*
 * ggit-blob-output-stream.c
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2013 - Jesse van den Kieboom
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

#include "ggit-blob-output-stream.h"
#include "ggit-repository.h"
#include "ggit-oid.h"
#include "ggit-error.h"

#include <git2.h>
#include <string.h>

/**
 * GgitBlobOutputStream:
 *
 * Represents a blob stream object.
 */

typedef struct _GgitBlobOutputStreamPrivate
{
	GgitRepository *repository;

	git_writestream *stream;
	gboolean something_written;

	gint ret;
	GgitOId *oid;
} GgitBlobOutputStreamPrivate;

G_DEFINE_TYPE_WITH_PRIVATE (GgitBlobOutputStream, ggit_blob_output_stream, G_TYPE_OUTPUT_STREAM)

enum
{
	PROP_0,
	PROP_REPOSITORY
};

static gboolean
ggit_blob_output_stream_close (GOutputStream  *object,
                               GCancellable   *cancellable,
                               GError        **error)
{
	GgitBlobOutputStream *stream = GGIT_BLOB_OUTPUT_STREAM (object);
	GgitBlobOutputStreamPrivate *priv;

	priv = ggit_blob_output_stream_get_instance_private (stream);

	if (g_cancellable_set_error_if_cancelled (cancellable, error))
	{
		return FALSE;
	}

	if (priv->ret != GIT_OK)
	{
		return TRUE;
	}

	if (priv->something_written)
	{
		git_oid oid;

		if (git_blob_create_fromstream_commit (&oid, priv->stream) == GIT_OK)
		{
			priv->oid = _ggit_oid_wrap (&oid);
		}
		else
		{
			g_set_error (error, G_IO_ERROR, G_IO_ERROR_FAILED,
			             "Could not create object id");
			return FALSE;
		}
	}
	else
	{
		if (priv->stream->close (priv->stream) != GIT_OK)
		{
			g_set_error (error, G_IO_ERROR, G_IO_ERROR_FAILED,
			             "Could not close write stream");
			return FALSE;
		}
	}

	return TRUE;
}

static gboolean
ggit_blob_output_stream_flush (GOutputStream  *stream,
                               GCancellable   *cancellable,
                               GError        **error)
{
	if (g_cancellable_set_error_if_cancelled (cancellable, error))
	{
		return FALSE;
	}

	return TRUE;
}

static gssize
ggit_blob_output_stream_write (GOutputStream  *object,
                               const void     *buffer,
                               gsize           count,
                               GCancellable   *cancellable,
                               GError        **error)
{
	GgitBlobOutputStream *stream = GGIT_BLOB_OUTPUT_STREAM (object);
	GgitBlobOutputStreamPrivate *priv;

	priv = ggit_blob_output_stream_get_instance_private (stream);

	if (g_cancellable_set_error_if_cancelled (cancellable, error))
	{
		return -1;
	}

	if (priv->ret != GIT_OK)
	{
		g_set_error (error, G_IO_ERROR, G_IO_ERROR_FAILED,
		             "Could not create write stream");
		return -1;
	}

	if (count == 0)
	{
		return 0;
	}

	if (count > 0)
	{
		gint ret = 0;

		ret = priv->stream->write (priv->stream, buffer, count);

		if (ret == GIT_OK)
		{
			priv->something_written = TRUE;
			return count;
		}
	}

	g_set_error (error, G_IO_ERROR, G_IO_ERROR_FAILED,
	             "Could not write the blob");

	return -1;
}

static void
ggit_blob_output_stream_finalize (GObject *object)
{
	GgitBlobOutputStream *stream;
	GgitBlobOutputStreamPrivate *priv;

	stream = GGIT_BLOB_OUTPUT_STREAM (object);
	priv = ggit_blob_output_stream_get_instance_private (stream);


	if (priv->oid)
	{
		ggit_oid_free (priv->oid);
	}
	else
	{
		/* NOTE: if we have an oid the stream is already freed */
		priv->stream->free (priv->stream);
	}

	g_clear_object (&priv->repository);

	G_OBJECT_CLASS (ggit_blob_output_stream_parent_class)->finalize (object);
}

static void
ggit_blob_output_stream_set_property (GObject      *object,
                                      guint         prop_id,
                                      const GValue *value,
                                      GParamSpec   *pspec)
{
	GgitBlobOutputStream *stream = GGIT_BLOB_OUTPUT_STREAM (object);
	GgitBlobOutputStreamPrivate *priv;

	priv = ggit_blob_output_stream_get_instance_private (stream);

	switch (prop_id)
	{
	case PROP_REPOSITORY:
		g_clear_object (&priv->repository);
		priv->repository = g_value_dup_object (value);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
ggit_blob_output_stream_get_property (GObject    *object,
                                      guint       prop_id,
                                      GValue     *value,
                                      GParamSpec *pspec)
{
	switch (prop_id)
	{
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
ggit_blob_output_stream_constructed (GObject *object)
{
	GgitBlobOutputStream *stream;
	GgitBlobOutputStreamPrivate *priv;

	stream = GGIT_BLOB_OUTPUT_STREAM (object);
	priv = ggit_blob_output_stream_get_instance_private (stream);

	priv->ret = git_blob_create_fromstream (&priv->stream,
	                                        _ggit_native_get (priv->repository),
	                                       NULL);

	G_OBJECT_CLASS (ggit_blob_output_stream_parent_class)->constructed (object);
}

static void
ggit_blob_output_stream_class_init (GgitBlobOutputStreamClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	GOutputStreamClass *stream_class = G_OUTPUT_STREAM_CLASS (klass);

	object_class->finalize = ggit_blob_output_stream_finalize;
	object_class->get_property = ggit_blob_output_stream_get_property;
	object_class->set_property = ggit_blob_output_stream_set_property;
	object_class->constructed = ggit_blob_output_stream_constructed;

	stream_class->write_fn = ggit_blob_output_stream_write;
	stream_class->close_fn = ggit_blob_output_stream_close;
	stream_class->flush = ggit_blob_output_stream_flush;

	g_object_class_install_property (object_class,
	                                 PROP_REPOSITORY,
	                                 g_param_spec_object ("repository",
	                                                      "Repository",
	                                                      "Repository",
	                                                      GGIT_TYPE_REPOSITORY,
	                                                      G_PARAM_WRITABLE |
	                                                      G_PARAM_CONSTRUCT_ONLY |
	                                                      G_PARAM_STATIC_STRINGS));
}

static void
ggit_blob_output_stream_init (GgitBlobOutputStream *stream)
{
}

GgitBlobOutputStream *
_ggit_blob_output_stream_new (GgitRepository  *repository)
{
	return g_object_new (GGIT_TYPE_BLOB_OUTPUT_STREAM,
	                     "repository", repository,
	                     NULL);
}

/**
 * ggit_blob_output_stream_get_id:
 * @stream: a #GgitBlobOutputStream.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Get the id of the written blob. The blob id is only available after the
 * stream has been properly closed. If an error occurred while writing the blob,
 * the %NULL is returned and @error is set accordingly.
 *
 * Returns: (transfer full) (allow-none): a #GgitOId or %NULL.
 *
 **/
GgitOId *
ggit_blob_output_stream_get_id (GgitBlobOutputStream  *stream,
                                GError               **error)
{
	GgitBlobOutputStreamPrivate *priv;

	g_return_val_if_fail (GGIT_IS_BLOB_OUTPUT_STREAM (stream), NULL);

	priv = ggit_blob_output_stream_get_instance_private (stream);

	if (priv->ret != GIT_OK)
	{
		_ggit_error_set (error, priv->ret);
		return NULL;
	}

	return ggit_oid_copy (priv->oid);
}

/* ex:set ts=8 noet: */
