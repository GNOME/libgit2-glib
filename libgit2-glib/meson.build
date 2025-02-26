version_data = configuration_data()
version_data.set('MAJOR_VERSION', libgit2_glib_major_version)
version_data.set('MINOR_VERSION', libgit2_glib_minor_version)
version_data.set('MICRO_VERSION', libgit2_glib_micro_version)
version_data.set('VERSION', libgit2_glib_version)
version_data.set_quoted('BUILD_TYPE', libgit2_glib_buildtype)

ggit_version_h = configure_file(
          input: 'ggit-version.h.in',
         output: 'ggit-version.h',
    install_dir: libgit2_glib_includedir / libgit2_glib_include_subdir,
        install: true,
  configuration: version_data)

ggit_types_18_h = 'ggit-types-pre18.h'
if libgit2_version_19_or_more
  ggit_types_18_h = 'ggit-types-19.h'
elif libgit2_version_18_or_more
  ggit_types_18_h = 'ggit-types-18.h'
endif

headers = [
  'ggit-annotated-commit.h',
  'ggit-blame.h',
  'ggit-blame-options.h',
  'ggit-blob.h',
  'ggit-blob-output-stream.h',
  'ggit-branch.h',
  'ggit-branch-enumerator.h',
  'ggit-checkout-options.h',
  'ggit-cherry-pick-options.h',
  'ggit-clone-options.h',
  'ggit-config.h',
  'ggit-commit.h',
  'ggit-commit-parents.h',
  'ggit-config-entry.h',
  'ggit-cred.h',
  'ggit-cred-plaintext.h',
  'ggit-diff.h',
  'ggit-diff-binary.h',
  'ggit-diff-binary-file.h',
  'ggit-diff-delta.h',
  'ggit-diff-file.h',
  'ggit-diff-find-options.h',
  'ggit-diff-format-email-options.h',
  'ggit-diff-hunk.h',
  'ggit-diff-line.h',
  'ggit-diff-options.h',
  'ggit-diff-similarity-metric.h',
  'ggit-error.h',
  'ggit-fetch-options.h',
  'ggit-index.h',
  'ggit-index-entry.h',
  'ggit-index-entry-resolve-undo.h',
  'ggit-main.h',
  'ggit-mailmap.h',
  'ggit-message.h',
  'ggit-merge-options.h',
  'ggit-native.h',
  'ggit-note.h',
  'ggit-object.h',
  'ggit-object-factory.h',
  'ggit-object-factory-base.h',
  'ggit-oid.h',
  'ggit-patch.h',
  'ggit-proxy-options.h',
  'ggit-push-options.h',
  'ggit-rebase-operation.h',
  'ggit-rebase-options.h',
  'ggit-rebase.h',
  'ggit-ref.h',
  'ggit-ref-spec.h',
  'ggit-reflog.h',
  'ggit-reflog-entry.h',
  'ggit-remote.h',
  'ggit-remote-callbacks.h',
  'ggit-repository.h',
  'ggit-revert-options.h',
  'ggit-revision-walker.h',
  'ggit-signature.h',
  'ggit-status-options.h',
  'ggit-submodule.h',
  'ggit-submodule-update-options.h',
  'ggit-tag.h',
  'ggit-transfer-progress.h',
  'ggit-tree.h',
  'ggit-tree-builder.h',
  'ggit-tree-entry.h',
  'ggit-types.h',
  ggit_types_18_h,
  ggit_version_h,
]

private_headers = [
  'ggit-convert.h',
  'ggit-utils.h',
]

ssh_headers = [
  'ggit-cred-ssh-interactive.h',
  'ggit-cred-ssh-key-from-agent.h',
]

sources = [
  'ggit-annotated-commit.c',
  'ggit-blame.c',
  'ggit-blame-options.c',
  'ggit-blob.c',
  'ggit-blob-output-stream.c',
  'ggit-branch.c',
  'ggit-branch-enumerator.c',
  'ggit-checkout-options.c',
  'ggit-cherry-pick-options.c',
  'ggit-clone-options.c',
  'ggit-commit.c',
  'ggit-commit-parents.c',
  'ggit-config.c',
  'ggit-config-entry.c',
  'ggit-convert.c',
  'ggit-cred.c',
  'ggit-cred-plaintext.c',
  'ggit-diff.c',
  'ggit-diff-binary.c',
  'ggit-diff-binary-file.c',
  'ggit-diff-delta.c',
  'ggit-diff-file.c',
  'ggit-diff-find-options.c',
  'ggit-diff-format-email-options.c',
  'ggit-diff-hunk.c',
  'ggit-diff-line.c',
  'ggit-diff-options.c',
  'ggit-diff-similarity-metric.c',
  'ggit-error.c',
  'ggit-fetch-options.c',
  'ggit-index.c',
  'ggit-index-entry.c',
  'ggit-index-entry-resolve-undo.c',
  'ggit-main.c',
  'ggit-mailmap.c',
  'ggit-message.c',
  'ggit-merge-options.c',
  'ggit-native.c',
  'ggit-note.c',
  'ggit-object.c',
  'ggit-object-factory.c',
  'ggit-object-factory-base.c',
  'ggit-oid.c',
  'ggit-patch.c',
  'ggit-proxy-options.c',
  'ggit-push-options.c',
  'ggit-rebase-operation.c',
  'ggit-rebase-options.c',
  'ggit-rebase.c',
  'ggit-ref.c',
  'ggit-ref-spec.c',
  'ggit-reflog.c',
  'ggit-reflog-entry.c',
  'ggit-remote.c',
  'ggit-remote-callbacks.c',
  'ggit-repository.c',
  'ggit-revert-options.c',
  'ggit-revision-walker.c',
  'ggit-signature.c',
  'ggit-status-options.c',
  'ggit-submodule.c',
  'ggit-submodule-update-options.c',
  'ggit-tag.c',
  'ggit-transfer-progress.c',
  'ggit-tree.c',
  'ggit-tree-builder.c',
  'ggit-tree-entry.c',
  'ggit-types.c',
  'ggit-utils.c',
]

cflags = []

if enable_ssh
  headers += ssh_headers

  sources += [
    'ggit-cred-ssh-interactive.c',
    'ggit-cred-ssh-key-from-agent.c',
  ]

  ssh_includes = '''
#include <libgit2-glib/ggit-cred-ssh-key-from-agent.h>
#include <libgit2-glib/ggit-cred-ssh-interactive.h>
'''

  cflags += ['-DGIT_SSH=1']
else
  private_headers += ssh_headers

  ssh_includes = ''
endif

ggit_h = 'ggit.h'

sources += configure_file(
  input: ggit_h + '.in',
  output: '@BASENAME@',
  configuration: {'GGIT_SSH_INCLUDES': ssh_includes},
  install_dir: libgit2_glib_includedir / libgit2_glib_include_subdir,
)

enum_sources = gnome.mkenums_simple(
  'ggit-enum-types',
  sources: headers,
  install_header: true,
  install_dir: libgit2_glib_includedir / libgit2_glib_include_subdir,
)

install_headers(
  headers,
  subdir: libgit2_glib_include_subdir,
)

platform_deps = [
  gio_dep,
  glib_dep,
  gobject_dep,
  libgit2_dep,
]

libgit2_glib = shared_library(
  'git2-glib-' + libgit2_glib_api_version,
  version: libversion,
  soversion: soversion,
  darwin_versions: darwin_versions,
  sources: sources + enum_sources,
  include_directories: top_inc,
  dependencies: platform_deps,
  c_args: cflags + ['-DG_LOG_DOMAIN="@0@"'.format(libgit2_glib_ns)],
  install: true,
)

pkg.generate(
  libraries: libgit2_glib,
  version: meson.project_version(),
  name: meson.project_name(),
  description: 'libgit2-glib, a a glib wrapper library around the libgit2 git access library.',
  filebase: libgit2_glib_api_name,
  subdirs: libgit2_glib_api_name,
  requires: platform_deps,
  variables: 'exec_prefix=${prefix}',
  extra_cflags: cflags,
)

# Internal dependency, for tests and benchmarks
libgit2_glib_dep = declare_dependency(
  # Everything that uses libgit2-glib needs this built to compile
  sources: enum_sources[1],
  include_directories: '.',
  dependencies: platform_deps,
  link_with: libgit2_glib,
)

if enable_python
  python.install_sources(
    'Ggit.py',
    pure: false,
    subdir: 'gi/overrides',
  )
endif

if enable_gir
  libgit2_glib_gir = gnome.generate_gir(
    libgit2_glib,
    sources: headers + sources + enum_sources,
    namespace: libgit2_glib_ns,
    nsversion: libgit2_glib_api_version,
    identifier_prefix: libgit2_glib_ns,
    symbol_prefix: libgit2_glib_ns.to_lower(),
    export_packages: libgit2_glib_api_name,
    includes: ['GObject-2.0', 'GLib-2.0', 'Gio-2.0'],
    header: meson.project_name() / ggit_h,
    install: true,
  )

  if enable_vapi
    gnome.generate_vapi(
      libgit2_glib_api_name,
      sources: libgit2_glib_gir[0],
      packages: 'gio-2.0',
      install: true,
    )
  endif
endif
