private_headers = [
  'config.h',
  'ggit-convert.h',
  'ggit-utils.h'
]

docs_conf = configuration_data()
docs_conf.set('PACKAGE_VERSION', libgit2_glib_version)

configure_file(input : 'version.xml.in',
               output : 'version.xml',
               configuration : docs_conf)

glib_prefix = dependency('glib-2.0').get_pkgconfig_variable('prefix')
glib_docpath = join_paths(glib_prefix, 'share', 'gtk-doc', 'html')

gnome.gtkdoc('libgit2-glib-@0@'.format(libgit2_glib_api_version),
             main_sgml: 'libgit2-glib-@0@-docs.sgml'.format(libgit2_glib_api_version),
             src_dir: join_paths(meson.source_root(), 'libgit2-glib'),
             dependencies: libgit2_glib_dep,
             gobject_typesfile: 'libgit2-glib-@0@.types'.format(libgit2_glib_api_version),
             scan_args: [
               '--rebuild-types',
               '--ignore-headers=' + ' '.join(private_headers),
             ],
             fixxref_args: [
               '--extra-dir=' + join_paths(glib_docpath, 'glib'),
               '--extra-dir=' + join_paths(glib_docpath, 'gobject'),
               '--extra-dir=' + join_paths(glib_docpath, 'gio'),
             ],
             install: true)
