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

#define GGIT_BLOB_OUTPUT_STREAM_GET_PRIVATE(object)(G_TYPE_INSTANCE_GET_PRIVATE((object), GGIT_TYPE_BLOB_OUTPUT_STREAM, GgitBlobOutputStreamPrivate))

struct _GgitBlobOutputStreamPrivate
{
	GgitRepository *repository;
	GThread *thread;

	GMutex mutex;
	GCond cond;

	GMutex reply_mutex;
	GCond reply_cond;

	gint written;

	const gchar *writebuf;
	gsize bufsize;

	gint ret;
	GgitOId *oid;

	guint closing : 1;
	guint cancelled : 1;
};

G_DEFINE_TYPE (GgitBlobOutputStream, ggit_blob_output_stream, G_TYPE_OUTPUT_STREAM)

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

	if (g_cancellable_set_error_if_cancelled (cancellable, error))
	{
		return FALSE;
	}

	g_mutex_lock (&stream->priv->mutex);
	stream->priv->closing = TRUE;

	g_cond_signal (&stream->priv->cond);
	g_mutex_lock (&stream->priv->reply_mutex);
	g_mutex_unlock (&stream->priv->mutex);
	g_cond_wait (&stream->priv->reply_cond, &stream->priv->reply_mutex);
	g_mutex_unlock (&stream->priv->reply_mutex);

	g_thread_join (stream->priv->thread);
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

	if (stream->priv->closing)
	{
		return 0;
	}

	while (TRUE)
	{
		g_mutex_lock (&stream->priv->mutex);

		if (g_cancellable_is_cancelled (cancellable))
		{
			stream->priv->cancelled = TRUE;
		}

		stream->priv->writebuf = buffer;
		stream->priv->bufsize = count;
		stream->priv->written = 0;

		g_cond_signal (&stream->priv->cond);
		g_mutex_lock (&stream->priv->reply_mutex);
		g_mutex_unlock (&stream->priv->mutex);
		g_cond_wait (&stream->priv->reply_cond, &stream->priv->reply_mutex);

		if (stream->priv->written == count || stream->priv->cancelled)
		{
			g_mutex_unlock (&stream->priv->reply_mutex);
			break;
		}

		g_mutex_unlock (&stream->priv->reply_mutex);
	}

	if (stream->priv->cancelled)
	{
		g_cancellable_set_error_if_cancelled (cancellable, error);
		return -1;
	}

	return stream->priv->written;
}

static void
ggit_blob_output_stream_finalize (GObject *object)
{
	GgitBlobOutputStream *stream;

	stream = GGIT_BLOB_OUTPUT_STREAM (object);

	G_OBJECT_CLASS (ggit_blob_output_stream_parent_class)->finalize (object);

	g_mutex_clear (&stream->priv->mutex);
	g_cond_clear (&stream->priv->cond);

	g_mutex_clear (&stream->priv->reply_mutex);
	g_cond_clear (&stream->priv->reply_cond);

	if (stream->priv->oid)
	{
		ggit_oid_free (stream->priv->oid);
	}
}

static void
ggit_blob_output_stream_set_property (GObject      *object,
                                      guint         prop_id,
                                      const GValue *value,
                                      GParamSpec   *pspec)
{
	GgitBlobOutputStream *stream = GGIT_BLOB_OUTPUT_STREAM (object);

	switch (prop_id)
	{
	case PROP_REPOSITORY:
		g_clear_object (&stream->priv->repository);
		stream->priv->repository = g_value_dup_object (value);
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


	g_type_class_add_private (object_class, sizeof (GgitBlobOutputStreamPrivate));
}

static int
blob_chunk_cb (char   *content,
               size_t  maxlen,
               void   *payload)
{
	GgitBlobOutputStream *stream = payload;
	gint written = 0;

	// block until we got something to write
	while (written == 0)
	{
		g_cond_wait (&stream->priv->cond, &stream->priv->mutex);

		if (stream->priv->closing)
		{
			return 0;
		}

		if (stream->priv->cancelled)
		{
			return -1;
		}

		if (stream->priv->bufsize > maxlen)
		{
			stream->priv->written = maxlen;
		}
		else
		{
			stream->priv->written = stream->priv->bufsize;
		}

		if (stream->priv->written > 0)
		{
			memcpy (content, stream->priv->writebuf, stream->priv->written);
			written = stream->priv->written;
		}

		g_mutex_lock (&stream->priv->reply_mutex);
		g_cond_signal (&stream->priv->reply_cond);
		g_mutex_unlock (&stream->priv->mutex);
		g_mutex_unlock (&stream->priv->reply_mutex);
	}

	return written;
}

static gpointer
chunk_blob_in_thread (gpointer data)
{
	GgitBlobOutputStream *stream = data;
	git_oid oid;
	gint ret;

	ret = git_blob_create_fromchunks (&oid,
	                                  _ggit_native_get (stream->priv->repository),
	                                  NULL,
	                                  blob_chunk_cb,
	                                  data);

	stream->priv->ret = ret;
	stream->priv->closing = TRUE;

	if (ret == GIT_OK)
	{
		stream->priv->oid = _ggit_oid_wrap (&oid);
	}
	else
	{
		stream->priv->cancelled = TRUE;
	}

	g_mutex_lock (&stream->priv->reply_mutex);
	g_cond_signal (&stream->priv->reply_cond);

	g_mutex_unlock (&stream->priv->mutex);
	g_mutex_unlock (&stream->priv->reply_mutex);

	return NULL;
}

static void
ggit_blob_output_stream_init (GgitBlobOutputStream *self)
{
	self->priv = GGIT_BLOB_OUTPUT_STREAM_GET_PRIVATE (self);

	g_mutex_init (&self->priv->mutex);
	g_cond_init (&self->priv->cond);

	g_mutex_init (&self->priv->reply_mutex);
	g_cond_init (&self->priv->reply_cond);

	g_mutex_lock (&self->priv->mutex);

	self->priv->thread = g_thread_new ("ggit-blob-output-stream",
	                                   chunk_blob_in_thread,
	                                   self);
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
	g_return_val_if_fail (GGIT_IS_BLOB_OUTPUT_STREAM (stream), NULL);

	if (stream->priv->ret != GIT_OK)
	{
		_ggit_error_set (error, stream->priv->ret);
		return NULL;
	}

	return ggit_oid_copy (stream->priv->oid);
}

/* ex:set ts=8 noet: */
