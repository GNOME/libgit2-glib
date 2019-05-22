glib_prefix = glib_dep.get_pkgconfig_variable('prefix')

fixxref_args = [
  '--extra-dir=' + (glib_prefix / gnome.gtkdoc_html_dir('glib')),
  '--extra-dir=' + (glib_prefix / gnome.gtkdoc_html_dir('gobject')),
  '--extra-dir=' + (glib_prefix / gnome.gtkdoc_html_dir('gio')),
  '--html-dir=' + (libgit2_glib_prefix / gnome.gtkdoc_html_dir(libgit2_glib_api_name)),
]

docs_conf = configuration_data()
docs_conf.set('PACKAGE_VERSION', libgit2_glib_version)

name = 'version.xml'

content_files = configure_file(
  input: name + '.in',
  output: name,
  configuration: docs_conf,
)

gnome.gtkdoc(
  libgit2_glib_api_name,
  main_sgml: libgit2_glib_api_name + '-docs.sgml',
  src_dir: meson.source_root() / 'libgit2-glib',
  dependencies: libgit2_glib_dep,
  ignore_headers: private_headers,
  gobject_typesfile: libgit2_glib_api_name + '.types',
  scan_args: '--rebuild-types',
  fixxref_args: fixxref_args,
  content_files: content_files,
  install: true,
)
