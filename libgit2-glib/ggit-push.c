/*
 * ggit-push.c
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2012 - Ignacio Casal Quinteiro
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

#include "ggit-push.h"
#include "ggit-remote.h"
#include "ggit-enum-types.h"
#include "ggit-error.h"

#define GGIT_PUSH_GET_PRIVATE(object)(G_TYPE_INSTANCE_GET_PRIVATE((object), GGIT_TYPE_PUSH, GgitPushPrivate))

struct _GgitPushPrivate
{
	GgitRemote *remote;
};

enum
{
	PROP_0,
	PROP_REMOTE
};

enum
{
	TRANSFER_PROGRESS,
	PACKBUILDER_PROGRESS,
	NUM_SIGNALS
};

static guint signals[NUM_SIGNALS] = {0,};

static void ggit_push_initable_iface_init (GInitableIface *iface);

G_DEFINE_TYPE_EXTENDED (GgitPush, ggit_push, GGIT_TYPE_NATIVE,
                        0,
                        G_IMPLEMENT_INTERFACE (G_TYPE_INITABLE,
                                               ggit_push_initable_iface_init))


static void
ggit_push_dispose (GObject *object)
{
	GgitPushPrivate *priv;

	priv = GGIT_PUSH (object)->priv;
	g_clear_object (&priv->remote);

	G_OBJECT_CLASS (ggit_push_parent_class)->dispose (object);
}

static void
ggit_push_get_property (GObject    *object,
                        guint       prop_id,
                        GValue     *value,
                        GParamSpec *pspec)
{
	GgitPushPrivate *priv = GGIT_PUSH (object)->priv;

	switch (prop_id)
	{
		case PROP_REMOTE:
			g_value_set_object (value, priv->remote);
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void
ggit_push_set_property (GObject      *object,
                        guint         prop_id,
                        const GValue *value,
                        GParamSpec   *pspec)
{
	GgitPushPrivate *priv = GGIT_PUSH (object)->priv;

	switch (prop_id)
	{
		case PROP_REMOTE:
			priv->remote = g_value_dup_object (value);
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void
ggit_push_class_init (GgitPushClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->dispose = ggit_push_dispose;
	object_class->get_property = ggit_push_get_property;
	object_class->set_property = ggit_push_set_property;

	g_object_class_install_property (object_class,
	                                 PROP_REMOTE,
	                                 g_param_spec_object ("remote",
	                                                      "Remote",
	                                                      "The remote associated with this push",
	                                                      GGIT_TYPE_REMOTE,
	                                                      G_PARAM_READWRITE |
	                                                      G_PARAM_CONSTRUCT_ONLY));

	signals[TRANSFER_PROGRESS] =
		g_signal_new ("transfer-progress",
		              G_TYPE_FROM_CLASS (object_class),
		              G_SIGNAL_RUN_LAST,
		              G_STRUCT_OFFSET (GgitPushClass, transfer_progress),
		              NULL, NULL,
		              NULL,
		              G_TYPE_NONE,
		              3,
		              G_TYPE_UINT,
		              G_TYPE_UINT,
		              G_TYPE_UINT);

	signals[PACKBUILDER_PROGRESS] =
		g_signal_new ("packbuilder-progress",
		              G_TYPE_FROM_CLASS (object_class),
		              G_SIGNAL_RUN_LAST,
		              G_STRUCT_OFFSET (GgitPushClass, packbuilder_progress),
		              NULL, NULL,
		              NULL,
		              G_TYPE_NONE,
		              3,
		              GGIT_TYPE_PACKBUILDER_STAGE,
		              G_TYPE_UINT,
		              G_TYPE_UINT);

	g_type_class_add_private (object_class, sizeof (GgitPushPrivate));
}

static void
ggit_push_init (GgitPush *push)
{
	push->priv = GGIT_PUSH_GET_PRIVATE (push);
}

static gint
packbuilder_progress_wrapper (gint     stage,
                              guint    current,
                              guint    total,
                              gpointer payload)
{
	g_signal_emit (payload, signals[PACKBUILDER_PROGRESS], 0, stage, current, total);
	return GIT_OK;
}

static gint
transfer_progress_wrapper (guint    current,
                           guint    total,
                           gsize    bytes,
                           gpointer payload)
{
	g_signal_emit (payload, signals[TRANSFER_PROGRESS], 0, current, total, bytes);
	return GIT_OK;
}

static gboolean
ggit_push_initable_init (GInitable    *initable,
                         GCancellable *cancellable,
                         GError      **error)
{
	GgitPushPrivate *priv = GGIT_PUSH (initable)->priv;
	git_push *push;
	gint ret;

	if (cancellable != NULL)
	{
		g_set_error_literal (error, G_IO_ERROR, G_IO_ERROR_NOT_SUPPORTED,
		                     "Cancellable initialization not supported");
		return FALSE;
	}

	ret = git_push_new (&push,
	                    _ggit_native_get (priv->remote));

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
		return FALSE;
	}

	_ggit_native_set (initable,
	                  push,
	                  (GDestroyNotify)git_push_free);

	git_push_set_callbacks (push,
	                        packbuilder_progress_wrapper,
	                        initable,
	                        transfer_progress_wrapper,
	                        initable);

	return TRUE;
}

static void
ggit_push_initable_iface_init (GInitableIface *iface)
{
	iface->init = ggit_push_initable_init;
}

/**
 * ggit_push_new:
 * @remote: a #GgitRemote.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Creates a new #GgitPush for @remote.
 *
 * Returns: (transfer full): a newly created #GgitPush.
 */
GgitPush *
ggit_push_new (GgitRemote  *remote,
               GError     **error)
{
	g_return_val_if_fail (remote != NULL, NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	return g_initable_new (GGIT_TYPE_PUSH, NULL, error,
	                       "remote", remote,
	                       NULL);
}

/**
 * ggit_push_add_refspec:
 * @push: a #GgitPush.
 * @refspec: Refspec string.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Adds a refspec to be pushed.
 */
void
ggit_push_add_refspec (GgitPush     *push,
                       const gchar  *refspec,
                       GError      **error)
{
	gint ret;

	g_return_if_fail (GGIT_IS_PUSH (push));
	g_return_if_fail (refspec != NULL);
	g_return_if_fail (error == NULL || *error == NULL);

	ret = git_push_add_refspec (_ggit_native_get (push),
	                            refspec);

	if (ret != GIT_OK)
	{
		_ggit_error_set (error, ret);
	}
}

/**
 * ggit_push_finish:
 * @push: a #GgitPush.
 * @error: a #GError for error reporting, or %NULL.
 *
 * Actually push all the refspecs added to the push object.
 */
gboolean
ggit_push_finish (GgitPush  *push,
                  GError   **error)
{
	gint ret;

	g_return_val_if_fail (GGIT_IS_PUSH (push), FALSE);
	g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

	/* Options might have changed externally, need to make sure to set
	 * them again.
	 */
	git_push_set_options (_ggit_native_get (push),
	                      _ggit_push_options_get_push_options (push->priv->options));

	ret = git_push_finish (_ggit_native_get (push));

	if (ret != GIT_OK)
	{
		if (error && *error == NULL)
		{
			_ggit_error_set (error, ret);
		}

		return FALSE;
	}

	return TRUE;
}

/**
 * ggit_push_is_unpack_ok:
 * @push: a #GgitPush.
 *
 * Checks if the remote side is successfully unpacked.
 *
 * Returns: if the remote side is successfully unpacked.
 */
gboolean
ggit_push_is_unpack_ok (GgitPush *push)
{
	g_return_val_if_fail (GGIT_IS_PUSH (push), FALSE);

	return git_push_unpack_ok (_ggit_native_get (push));
}

/* ex:set ts=8 noet: */
