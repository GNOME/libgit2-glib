/*
 * ggit-remote-callbacks.c
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2013 - Ignacio Casal Quinteiro
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
#if LIBGIT2_VER_MAJOR > 1 || (LIBGIT2_VER_MAJOR == 1 && LIBGIT2_VER_MINOR >= 8)
#include <git2/sys/errors.h>
#endif
#include "ggit-remote-callbacks.h"
#include "ggit-cred.h"
#include "ggit-transfer-progress.h"
#include "ggit-oid.h"
#include "ggit-remote.h"
#include "ggit-enum-types.h"

/**
 * GgitRemoteCallbacks:
 *
 * Represents a git remote callbacks.
 */

typedef struct _GgitRemoteCallbacksPrivate
{
	git_remote_callbacks native;
	GCancellable *cancellable;
} GgitRemoteCallbacksPrivate;

enum
{
	PROGRESS,
	TRANSFER_PROGRESS,
	UPDATE_TIPS,
	COMPLETION,
	PACK_PROGRESS,
	PUSH_TRANSFER_PROGRESS,
	PUSH_UPDATE_REFERENCE,
	PUSH_NEGOTIATION,
	REMOTE_READY,
	NUM_SIGNALS
};

static guint signals[NUM_SIGNALS] = {0,};

/**
 * GgitRemoteCallbacksClass::credentials:
 * @callbacks: a #GgitRemoteCallbacks.
 * @url: the url.
 * @username_from_url: (allow-none): the username extracted from the url.
 * @allowed_types: the allowed credential types.
 * @error: a #GError for error reporting.
 *
 * Returns: (transfer full) (nullable): a #GgitCred or %NULL in case of an error.
 */

G_DEFINE_TYPE_WITH_PRIVATE (GgitRemoteCallbacks, ggit_remote_callbacks, G_TYPE_OBJECT)

static void
ggit_remote_callbacks_finalize (GObject *object)
{
	GgitRemoteCallbacks *callbacks = GGIT_REMOTE_CALLBACKS (object);
	GgitRemoteCallbacksPrivate *priv;

	priv = ggit_remote_callbacks_get_instance_private (callbacks);

	priv->native.payload = NULL;

	g_clear_object (&priv->cancellable);

	G_OBJECT_CLASS (ggit_remote_callbacks_parent_class)->finalize (object);
}

static void
ggit_remote_callbacks_class_init (GgitRemoteCallbacksClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->finalize = ggit_remote_callbacks_finalize;

	signals[UPDATE_TIPS] =
		g_signal_new ("update-tips",
		              G_TYPE_FROM_CLASS (object_class),
		              G_SIGNAL_RUN_LAST,
		              G_STRUCT_OFFSET (GgitRemoteCallbacksClass, update_tips),
		              NULL, NULL,
		              NULL,
		              G_TYPE_NONE,
		              3,
		              G_TYPE_STRING,
		              GGIT_TYPE_OID,
		              GGIT_TYPE_OID);

	signals[PROGRESS] =
		g_signal_new ("progress",
		              G_TYPE_FROM_CLASS (object_class),
		              G_SIGNAL_RUN_LAST,
		              G_STRUCT_OFFSET (GgitRemoteCallbacksClass, progress),
		              NULL, NULL,
		              NULL,
		              G_TYPE_NONE,
		              1,
		              G_TYPE_STRING);

	signals[TRANSFER_PROGRESS] =
		g_signal_new ("transfer-progress",
		              G_TYPE_FROM_CLASS (object_class),
		              G_SIGNAL_RUN_LAST,
		              G_STRUCT_OFFSET (GgitRemoteCallbacksClass, transfer_progress),
		              NULL, NULL,
		              NULL,
		              G_TYPE_NONE,
		              1,
		              GGIT_TYPE_TRANSFER_PROGRESS);

	signals[COMPLETION] =
		g_signal_new ("completion",
		              G_TYPE_FROM_CLASS (object_class),
		              G_SIGNAL_RUN_LAST,
		              G_STRUCT_OFFSET (GgitRemoteCallbacksClass, completion),
		              NULL, NULL,
		              NULL,
		              G_TYPE_NONE,
		              1,
		              GGIT_TYPE_REMOTE_COMPLETION_TYPE);

	signals[PACK_PROGRESS] =
		g_signal_new ("pack-progress",
		              G_TYPE_FROM_CLASS (object_class),
		              G_SIGNAL_RUN_LAST,
		              G_STRUCT_OFFSET (GgitRemoteCallbacksClass, pack_progress),
		              NULL, NULL,
		              NULL,
		              G_TYPE_NONE,
		              3,
		              GGIT_TYPE_PACKBUILDER_STAGE,
		              G_TYPE_UINT,
		              G_TYPE_UINT);

	signals[PUSH_TRANSFER_PROGRESS] =
		g_signal_new ("push-transfer-progress",
		              G_TYPE_FROM_CLASS (object_class),
		              G_SIGNAL_RUN_LAST,
		              G_STRUCT_OFFSET (GgitRemoteCallbacksClass, push_transfer_progress),
		              NULL, NULL,
		              NULL,
		              G_TYPE_NONE,
		              3,
		              G_TYPE_UINT,
		              G_TYPE_UINT,
		              G_TYPE_UINT);

	signals[PUSH_UPDATE_REFERENCE] =
		g_signal_new ("push-update-reference",
		              G_TYPE_FROM_CLASS (object_class),
		              G_SIGNAL_RUN_LAST,
		              G_STRUCT_OFFSET (GgitRemoteCallbacksClass, push_update_reference),
		              NULL, NULL,
		              NULL,
		              G_TYPE_NONE,
		              2,
		              G_TYPE_STRING,
		              G_TYPE_STRING);

	signals[PUSH_NEGOTIATION] =
		g_signal_new ("push-negotiation",
		              G_TYPE_FROM_CLASS (object_class),
		              G_SIGNAL_RUN_LAST,
		              G_STRUCT_OFFSET (GgitRemoteCallbacksClass, push_negotiation),
		              NULL, NULL,
		              NULL,
		              G_TYPE_NONE,
		              0);

	signals[REMOTE_READY] =
		g_signal_new ("remote-ready",
		              G_TYPE_FROM_CLASS (object_class),
		              G_SIGNAL_RUN_LAST,
		              G_STRUCT_OFFSET (GgitRemoteCallbacksClass, remote_ready),
		              NULL, NULL,
		              NULL,
		              G_TYPE_NONE,
		              1,
		              GGIT_TYPE_DIRECTION);
}

static int
credentials_wrap (git_cred     **cred,
                  const char    *url,
                  const char    *username_from_url,
                  unsigned int   allowed_types,
                  void          *data)
{
	GgitRemoteCallbacks *callbacks = GGIT_REMOTE_CALLBACKS (data);
	GgitRemoteCallbacksClass *cls = GGIT_REMOTE_CALLBACKS_GET_CLASS (callbacks);

	*cred = NULL;

	if (cls->credentials != NULL)
	{
		GgitCred *mcred = NULL;
		GError *error = NULL;

		mcred = cls->credentials (callbacks,
		                          url,
		                          username_from_url,
		                          allowed_types,
		                          &error);

		if (mcred != NULL)
		{
			*cred = _ggit_native_release (mcred);
			g_object_unref (mcred);

			return GIT_OK;
		}
		else
		{
			if (error)
			{
#if (LIBGIT2_VER_MAJOR > 0 && LIBGIT2_VER_MINOR < 8) || (LIBGIT2_VER_MAJOR == 0 && LIBGIT2_VER_MINOR >= 28)
				git_error_set_str (GIT_ERROR, error->message);
#else
				giterr_set_str (GIT_ERROR, error->message);
#endif
				g_error_free (error);

				return GIT_ERROR;
			}
			else
			{
				return GIT_PASSTHROUGH;
			}
		}
	}
	else
	{
		return GIT_OK;
	}
}


static int
progress_wrap (const char *str,
               int         len,
               void       *data)
{
	GgitRemoteCallbacks *callbacks = GGIT_REMOTE_CALLBACKS (data);
	GgitRemoteCallbacksPrivate *priv;
	gchar *message;

	priv = ggit_remote_callbacks_get_instance_private (callbacks);

	if (priv->cancellable != NULL &&
	    g_cancellable_is_cancelled (priv->cancellable))
	{
		return GIT_EUSER;
	}

	message = g_strndup (str, len);

	g_signal_emit (callbacks, signals[PROGRESS], 0, message);

	g_free (message);
	return GIT_OK;
}

static int
transfer_progress_wrap (const git_transfer_progress *stats,
                        void                        *data)
{
	GgitRemoteCallbacks *callbacks = GGIT_REMOTE_CALLBACKS (data);
	GgitRemoteCallbacksPrivate *priv;
	GgitTransferProgress *p;

	priv = ggit_remote_callbacks_get_instance_private (callbacks);

	if (priv->cancellable != NULL &&
	    g_cancellable_is_cancelled (priv->cancellable))
	{
		return GIT_EUSER;
	}

	p = _ggit_transfer_progress_wrap (stats);

	g_signal_emit (callbacks, signals[TRANSFER_PROGRESS], 0, p);
	ggit_transfer_progress_free (p);

	return GIT_OK;
}

static int
update_tips_wrap (const char    *refname,
                  const git_oid *a,
                  const git_oid *b,
                  void          *data)
{
	GgitRemoteCallbacks *callbacks = GGIT_REMOTE_CALLBACKS (data);
	GgitOId *na;
	GgitOId *nb;

	na = _ggit_oid_wrap (a);
	nb = _ggit_oid_wrap (b);

	g_signal_emit (callbacks, signals[UPDATE_TIPS], 0, refname, na, nb);

	ggit_oid_free (na);
	ggit_oid_free (nb);

	return GIT_OK;
}

static int
completion_wrap (git_remote_completion_type  type,
                 void                       *data)
{
	GgitRemoteCallbacks *callbacks = GGIT_REMOTE_CALLBACKS (data);
	GgitRemoteCompletionType rt = (GgitRemoteCompletionType)type;

	g_signal_emit (callbacks, signals[COMPLETION], 0, rt);

	return GIT_OK;
}

static int
pack_progress_wrap (int       stage,
                    uint32_t  current,
                    uint32_t  total,
                    void     *data)
{
	GgitRemoteCallbacks *callbacks = GGIT_REMOTE_CALLBACKS (data);
	GgitRemoteCallbacksPrivate *priv;

	priv = ggit_remote_callbacks_get_instance_private (callbacks);

	if (priv->cancellable != NULL &&
	    g_cancellable_is_cancelled (priv->cancellable))
	{
		return GIT_EUSER;
	}

	g_signal_emit (callbacks, signals[PACK_PROGRESS], 0,
	               (GgitPackbuilderStage)stage, (guint)current, (guint)total);

	return GIT_OK;
}

static int
push_transfer_progress_wrap (unsigned int  current,
                             unsigned int  total,
                             size_t        bytes,
                             void         *data)
{
	GgitRemoteCallbacks *callbacks = GGIT_REMOTE_CALLBACKS (data);
	GgitRemoteCallbacksPrivate *priv;

	priv = ggit_remote_callbacks_get_instance_private (callbacks);

	if (priv->cancellable != NULL &&
	    g_cancellable_is_cancelled (priv->cancellable))
	{
		return GIT_EUSER;
	}

	g_signal_emit (callbacks, signals[PUSH_TRANSFER_PROGRESS], 0,
	               (guint)current, (guint)total, (guint)bytes);

	return GIT_OK;
}

static int
push_update_reference_wrap (const char *refname,
                            const char *status,
                            void       *data)
{
	GgitRemoteCallbacks *callbacks = GGIT_REMOTE_CALLBACKS (data);

	g_signal_emit (callbacks, signals[PUSH_UPDATE_REFERENCE], 0,
	               refname, status);

	return GIT_OK;
}

static int
certificate_check_wrap (git_cert   *cert,
                        int         valid,
                        const char *host,
                        void       *data)
{
	GgitRemoteCallbacks *callbacks = GGIT_REMOTE_CALLBACKS (data);
	GgitRemoteCallbacksClass *cls = GGIT_REMOTE_CALLBACKS_GET_CLASS (callbacks);

	if (cls->certificate_check != NULL)
	{
		return cls->certificate_check (callbacks, cert, valid ? TRUE : FALSE, host);
	}

	return GIT_PASSTHROUGH;
}

static int
push_negotiation_wrap (const git_push_update **updates,
                       size_t                  len,
                       void                   *data)
{
	GgitRemoteCallbacks *callbacks = GGIT_REMOTE_CALLBACKS (data);

	g_signal_emit (callbacks, signals[PUSH_NEGOTIATION], 0);

	return GIT_OK;
}

static int
remote_ready_wrap (git_remote *remote,
                   int         direction,
                   void       *data)
{
	GgitRemoteCallbacks *callbacks = GGIT_REMOTE_CALLBACKS (data);

	g_signal_emit (callbacks, signals[REMOTE_READY], 0,
	               (GgitDirection)direction);

	return GIT_OK;
}

static void
ggit_remote_callbacks_init (GgitRemoteCallbacks *callbacks)
{
	GgitRemoteCallbacksPrivate *priv;

	git_remote_callbacks gcallbacks = GIT_REMOTE_CALLBACKS_INIT;

	priv = ggit_remote_callbacks_get_instance_private (callbacks);

	priv->native = gcallbacks;

	priv->native.sideband_progress = progress_wrap;
	priv->native.transfer_progress = transfer_progress_wrap;
	priv->native.update_tips = update_tips_wrap;
	priv->native.completion = completion_wrap;
	priv->native.pack_progress = pack_progress_wrap;
	priv->native.push_transfer_progress = push_transfer_progress_wrap;
	priv->native.push_update_reference = push_update_reference_wrap;
	priv->native.push_negotiation = push_negotiation_wrap;
	priv->native.certificate_check = certificate_check_wrap;
	priv->native.remote_ready = remote_ready_wrap;

	priv->native.credentials = credentials_wrap;

	priv->native.payload = callbacks;
}

git_remote_callbacks *
_ggit_remote_callbacks_get_native (GgitRemoteCallbacks *callbacks)
{
	GgitRemoteCallbacksPrivate *priv;

	priv = ggit_remote_callbacks_get_instance_private (callbacks);

	return &priv->native;
}

/**
 * ggit_remote_callbacks_set_cancellable:
 * @callbacks: a #GgitRemoteCallbacks.
 * @cancellable: (allow-none): a #GCancellable or %NULL.
 *
 * Sets a #GCancellable to abort ongoing remote operations (e.g. clone, fetch).
 * When the cancellable is triggered, the transfer and progress callbacks
 * will return an error, causing libgit2 to abort the operation.
 */
void
ggit_remote_callbacks_set_cancellable (GgitRemoteCallbacks *callbacks,
                                       GCancellable        *cancellable)
{
	GgitRemoteCallbacksPrivate *priv;

	g_return_if_fail (GGIT_IS_REMOTE_CALLBACKS (callbacks));

	priv = ggit_remote_callbacks_get_instance_private (callbacks);

	g_clear_object (&priv->cancellable);

	if (cancellable != NULL)
	{
		priv->cancellable = g_object_ref (cancellable);
	}
}

/**
 * ggit_remote_callbacks_get_cancellable:
 * @callbacks: a #GgitRemoteCallbacks.
 *
 * Gets the #GCancellable set on this callbacks object.
 *
 * Returns: (transfer none) (nullable): the cancellable or %NULL.
 */
GCancellable *
ggit_remote_callbacks_get_cancellable (GgitRemoteCallbacks *callbacks)
{
	GgitRemoteCallbacksPrivate *priv;

	g_return_val_if_fail (GGIT_IS_REMOTE_CALLBACKS (callbacks), NULL);

	priv = ggit_remote_callbacks_get_instance_private (callbacks);

	return priv->cancellable;
}

/* ex:set ts=8 noet: */
