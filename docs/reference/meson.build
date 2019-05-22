glib_prefix = glib_dep.get_pkgconfig_variable('prefix')

fixxref_args = [
  '--extra-dir=' + (glib_prefix / gnome.gtkdoc_html_dir('glib')),
  '--extra-dir=' + (glib_prefix / gnome.gtkdoc_html_dir('gobject')),
  '--extra-dir=' + (glib_prefix / gnome.gtkdoc_html_dir('gio')),
  '--html-dir=' + (libgit2_glib_prefix / gnome.gtkdoc_html_dir(libgit2_glib_api_name)),
]

content_files = configure_file(
  input: 'version.xml.in',
  output: '@BASENAME@',
  configuration: {'PACKAGE_VERSION': libgit2_glib_version},
)

gnome.gtkdoc(
  libgit2_glib_api_name,
  main_sgml: libgit2_glib_api_name + '-docs.sgml',
  src_dir: libgit2_glib_dir,
  dependencies: libgit2_glib_dep,
  ignore_headers: private_headers,
  scan_args: '--rebuild-types',
  fixxref_args: fixxref_args,
  content_files: content_files,
  install: true,
)
