doc_module = meson.project_name()

toml_conf = {
  'version': libgit2_glib_version,
  'license': ','.join(meson.project_license()),
}

libgit2_glib_toml = configure_file(
  input: doc_module + '.toml.in',
  output: '@BASENAME@',
  configuration: toml_conf,
)

custom_target(
  doc_module + '-doc',
  input: [
    libgit2_glib_toml,
    libgit2_glib_gir,
  ],
  output: libgit2_glib_api_name,
  command: [
    find_program('gi-docgen'),
    'generate',
    '--quiet',
    '--no-namespace-dir',
    '--add-include-path=@0@'.format(join_paths(source_root, 'libgit2-glib')),
    '--config=@INPUT0@',
    '--output-dir=@OUTPUT@',
    '--content-dir=@0@'.format(meson.current_source_dir()),
    '@INPUT1@',
  ],
  build_by_default: true,
  install: true,
  install_dir: gnome.gtkdoc_html_dir(''),
)
