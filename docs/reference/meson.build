docs_conf = configuration_data()
docs_conf.set('PACKAGE_VERSION', libgit2_glib_version)

name = 'version.xml'

configure_file(input: name + '.in',
               output: name,
               configuration: docs_conf)

glib_prefix = glib.get_pkgconfig_variable('prefix')
glib_docpath = join_paths(glib_prefix, 'share', 'gtk-doc', 'html')

gnome.gtkdoc(libgit2_glib_api_name,
             main_sgml: libgit2_glib_api_name + '-docs.sgml',
             src_dir: join_paths(meson.source_root(), 'libgit2-glib'),
             dependencies: libgit2_glib_dep,
             gobject_typesfile: libgit2_glib_api_name + '.types',
             scan_args: [
               '--rebuild-types',
               '--ignore-headers=' + ' '.join(private_headers),
             ],
             fixxref_args: [
               '--extra-dir=' + join_paths(glib_docpath, 'glib'),
               '--extra-dir=' + join_paths(glib_docpath, 'gobject'),
               '--extra-dir=' + join_paths(glib_docpath, 'gio'),
               '--html-dir=' + join_paths(libgit2_glib_prefix, gnome.gtkdoc_html_dir(libgit2_glib_api_name)),
             ],
             install: true)
