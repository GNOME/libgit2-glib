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

/*
 * BLOB_CHUNKING_STATE_IDLE: stream's thread is waiting for data
 * BLOB_CHUNKING_STATE_SENDING: have data, outside thread is waiting for result
 * BLOB_CHUNKING_STATE_CANCELLED: cancelled, stream's thread to exit and set error result
 * BLOB_CHUNKING_STATE_CLOSED: closed by outside thread or error, exit and set result
 */
typedef enum
{
	BLOB_CHUNKING_STATE_IDLE,
	BLOB_CHUNKING_STATE_SENDING,
	BLOB_CHUNKING_STATE_CANCELLED,
	BLOB_CHUNKING_STATE_CLOSED
} BlobChunkingState;

/**
 * GgitBlobOutputStream:
 *
 * Represents a blob stream object.
 */

typedef struct _GgitBlobOutputStreamPrivate
{
	GgitRepository *repository;
	GThread *thread;

	GMutex mutex;
	GCond cond;

	gssize written;

	const gchar *writebuf;
	gsize bufsize;

	gint ret;
	GgitOId *oid;

	BlobChunkingState state;
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
		g_mutex_lock (&priv->mutex);

		if (priv->state != BLOB_CHUNKING_STATE_CANCELLED)
		{
			priv->state = BLOB_CHUNKING_STATE_CANCELLED;
			g_cond_signal (&priv->cond);
		}

		g_mutex_unlock (&priv->mutex);
		return FALSE;
	}

	g_mutex_lock (&priv->mutex);
	priv->state = BLOB_CHUNKING_STATE_CLOSED;
	g_cond_signal (&priv->cond);
	g_mutex_unlock (&priv->mutex);

	g_thread_join (priv->thread);
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
	gssize written;

	priv = ggit_blob_output_stream_get_instance_private (stream);

	g_mutex_lock (&priv->mutex);

	while (priv->state == BLOB_CHUNKING_STATE_SENDING)
	{
		g_cond_wait (&priv->cond, &priv->mutex);
	}

	if (priv->state == BLOB_CHUNKING_STATE_CLOSED)
	{
		g_mutex_unlock (&priv->mutex);
		return 0;
	}

	if (g_cancellable_is_cancelled (cancellable))
	{
		priv->state = BLOB_CHUNKING_STATE_CANCELLED;
	}

	if (priv->state == BLOB_CHUNKING_STATE_IDLE)
	{
		priv->writebuf = buffer;
		priv->bufsize = count;
		priv->written = 0;
		priv->state = BLOB_CHUNKING_STATE_SENDING;
	}

	g_cond_signal (&priv->cond);

	while (priv->state == BLOB_CHUNKING_STATE_SENDING)
	{
		g_cond_wait (&priv->cond, &priv->mutex);
	}

	if (priv->state == BLOB_CHUNKING_STATE_CANCELLED)
	{
		g_cancellable_set_error_if_cancelled (cancellable, error);
		written = -1;
	}
	else
	{
		written = priv->written;
	}

	g_mutex_unlock (&priv->mutex);

	return written;
}

static void
ggit_blob_output_stream_finalize (GObject *object)
{
	GgitBlobOutputStream *stream;
	GgitBlobOutputStreamPrivate *priv;

	stream = GGIT_BLOB_OUTPUT_STREAM (object);
	priv = ggit_blob_output_stream_get_instance_private (stream);

	g_mutex_clear (&priv->mutex);
	g_cond_clear (&priv->cond);

	if (priv->oid)
	{
		ggit_oid_free (priv->oid);
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
ggit_blob_output_stream_class_init (GgitBlobOutputStreamClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	GOutputStreamClass *stream_class = G_OUTPUT_STREAM_CLASS (klass);

	object_class->finalize = ggit_blob_output_stream_finalize;
	object_class->get_property = ggit_blob_output_stream_get_property;
	object_class->set_property = ggit_blob_output_stream_set_property;

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

static int
blob_chunk_cb (char   *content,
               size_t  maxlen,
               void   *payload)
{
	GgitBlobOutputStream *stream = payload;
	GgitBlobOutputStreamPrivate *priv;
	int written = 0;

	priv = ggit_blob_output_stream_get_instance_private (stream);

	g_mutex_lock (&priv->mutex);

	while (priv->state == BLOB_CHUNKING_STATE_IDLE)
	{
		g_cond_wait (&priv->cond, &priv->mutex);
	}

	if (priv->state == BLOB_CHUNKING_STATE_CLOSED)
	{
		g_mutex_unlock (&priv->mutex);
		return 0;
	}

	if (priv->state == BLOB_CHUNKING_STATE_CANCELLED)
	{
		g_mutex_unlock (&priv->mutex);
		return -1;
	}

	/* state must be BLOB_CHUNKING_STATE_SENDING */

	if (priv->bufsize > maxlen)
	{
		priv->written = maxlen;
	}
	else
	{
		priv->written = priv->bufsize;
	}

	if (priv->written > 0)
	{
		memcpy (content, priv->writebuf, priv->written);
		written = priv->written;
	}

	priv->state = BLOB_CHUNKING_STATE_IDLE;
	g_cond_signal (&priv->cond);
	g_mutex_unlock (&priv->mutex);

	return written;
}

static gpointer
chunk_blob_in_thread (gpointer data)
{
	GgitBlobOutputStream *stream = data;
	GgitBlobOutputStreamPrivate *priv;
	git_oid oid;
	gint ret;

	priv = ggit_blob_output_stream_get_instance_private (stream);

	ret = git_blob_create_fromchunks (&oid,
	                                  _ggit_native_get (priv->repository),
	                                  NULL,
	                                  blob_chunk_cb,
	                                  data);

	g_mutex_lock (&priv->mutex);
	priv->ret = ret;

	if (ret == GIT_OK)
	{
		priv->state = BLOB_CHUNKING_STATE_CLOSED;
		priv->oid = _ggit_oid_wrap (&oid);
	}
	else
	{
		priv->state = BLOB_CHUNKING_STATE_CANCELLED;
	}

	g_cond_signal (&priv->cond);

	g_mutex_unlock (&priv->mutex);

	return NULL;
}

static void
ggit_blob_output_stream_init (GgitBlobOutputStream *stream)
{
	GgitBlobOutputStreamPrivate *priv;

	priv = ggit_blob_output_stream_get_instance_private (stream);

	g_mutex_init (&priv->mutex);
	g_cond_init (&priv->cond);

	priv->state = BLOB_CHUNKING_STATE_IDLE;

	priv->thread = g_thread_new ("ggit-blob-output-stream",
	                             chunk_blob_in_thread,
	                             stream);
}

GgitBlobOutputStream *
_ggit_blob_output_stream_new (GgitRepository  *repository)
{
	GgitBlobOutputStream *s;

	s = g_object_new (GGIT_TYPE_BLOB_OUTPUT_STREAM,
	                  "repository", repository,
	                  NULL);

	return s;
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
 * Returns: (transfer full): a #GgitOId or %NULL.
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
