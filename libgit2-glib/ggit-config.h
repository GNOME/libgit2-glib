#ifndef __GGIT_CONFIG_H__
#define __GGIT_CONFIG_H__

#include <glib-object.h>
#include <gio/gio.h>

G_BEGIN_DECLS

#define GGIT_TYPE_CONFIG		(ggit_config_get_type ())
#define GGIT_CONFIG(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), GGIT_TYPE_CONFIG, GgitConfig))
#define GGIT_CONFIG_CONST(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), GGIT_TYPE_CONFIG, GgitConfig const))
#define GGIT_CONFIG_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST ((klass), GGIT_TYPE_CONFIG, GgitConfigClass))
#define GGIT_IS_CONFIG(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), GGIT_TYPE_CONFIG))
#define GGIT_IS_CONFIG_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), GGIT_TYPE_CONFIG))
#define GGIT_CONFIG_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS ((obj), GGIT_TYPE_CONFIG, GgitConfigClass))

typedef struct _GgitConfig		GgitConfig;
typedef struct _GgitConfigClass		GgitConfigClass;
typedef struct _GgitConfigPrivate	GgitConfigPrivate;

struct _GgitConfig
{
	GObject parent;

	GgitConfigPrivate *priv;
};

struct _GgitConfigClass
{
	GObjectClass parent_class;
};

GType        ggit_config_get_type   (void) G_GNUC_CONST;

GgitConfig  *ggit_config_new        (GFile        *file);
GgitConfig  *ggit_config_get_global (void);

gboolean     ggit_config_get_bool   (GgitConfig   *config,
                                     const gchar  *name,
                                     GError      **error);

gboolean     ggit_config_set_bool   (GgitConfig   *config,
                                     const gchar  *name,
                                     gboolean      value,
                                     GError      **error);

const gchar *ggit_config_get_string (GgitConfig   *config,
                                     const gchar  *name,
                                     GError      **error);

gboolean     ggit_config_set_string (GgitConfig   *config,
                                     const gchar  *name,
                                     const gchar  *value,
                                     GError      **error);

gboolean     ggit_config_delete     (GgitConfig   *config,
                                     const gchar  *name,
                                     GError      **error);

G_END_DECLS

#endif /* __GGIT_CONFIG_H__ */
