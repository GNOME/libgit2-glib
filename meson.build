project(
  'libgit2-glib', 'c',
  version: '1.2.0',
  default_options: 'buildtype=debugoptimized',
  license: 'LGPL2+',
  meson_version: '>= 0.61.0',
)

libgit2_glib_version = meson.project_version()
version_array = libgit2_glib_version.split('.')
libgit2_glib_major_version = version_array[0].to_int()
libgit2_glib_minor_version = version_array[1].to_int()
libgit2_glib_micro_version = version_array[2].to_int()

libgit2_glib_api_version = '1.0'
libgit2_glib_api_name = '@0@-@1@'.format(meson.project_name(), libgit2_glib_api_version)

libgit2_glib_ns = 'Ggit'

libgit2_glib_buildtype = get_option('buildtype')

# The interface age is reset every time we add new API; this
# should only happen during development cycles, otherwise the
# interface age is the same as the micro version
if libgit2_glib_minor_version.is_odd()
  libgit2_glib_interface_age = 0
else
  libgit2_glib_interface_age = libgit2_glib_micro_version
endif

soversion = 0
# maintaining compatibility with the previous libtool versioning
# current = minor * 100 + micro - interface
# revision = interface
current = libgit2_glib_minor_version * 100 + libgit2_glib_micro_version - libgit2_glib_interface_age
revision = libgit2_glib_interface_age
libversion = '@0@.@1@.@2@'.format(soversion, current, revision)
darwin_versions = [current + 1, '@0@.@1@'.format(current + 1, revision)]

libgit2_glib_prefix = get_option('prefix')
libgit2_glib_libdir = get_option('libdir')
libgit2_glib_datadir = get_option('datadir')
libgit2_glib_includedir = get_option('includedir')

libgit2_glib_include_subdir = libgit2_glib_api_name / meson.project_name()

gnome = import('gnome')
pkg = import('pkgconfig')

source_root = meson.current_source_dir()

top_inc = include_directories('.')

cc = meson.get_compiler('c')

# Compiler and Debugging flags
if cc.get_id() == 'msvc'
  # Compiler options taken from msvc_recommended_pragmas.h
  # in GLib, based on _Win32_Programming_ by Rector and Newcomer
  common_flags = ['-FImsvc_recommended_pragmas.h']
else
  common_flags = cc.get_supported_arguments([
    '-ffast-math',
    '-fstrict-aliasing',
    '-Wpointer-arith',
    '-Wmissing-declarations',
    '-Wformat=2',
    '-Wstrict-prototypes',
    '-Wmissing-prototypes',
    '-Wnested-externs',
    '-Wold-style-definition',
    '-Wdeclaration-after-statement',
    '-Wunused',
    '-Wuninitialized',
    '-Wshadow',
    '-Wmissing-noreturn',
    '-Wmissing-format-attribute',
    '-Wredundant-decls',
    '-Wlogical-op',
    '-Wcast-align',
    '-Wno-unused-local-typedefs',
    '-Werror=implicit',
    '-Werror=init-self',
    '-Werror=main',
    '-Werror=missing-braces',
    '-Werror=return-type',
    '-Werror=array-bounds',
    '-Werror=write-strings',
  ])
endif

if libgit2_glib_buildtype.contains('debug')
  common_flags += ['-DLIBGIT2_GLIB_ENABLE_DEBUG']

  if libgit2_glib_buildtype.contains('optimized')
    common_flags += ['-DG_DISABLE_CAST_CHECKS']
  endif
else
  common_flags += [
    '-DG_DISABLE_CAST_CHECKS',
    '-DG_DISABLE_CHECKS',
  ]
endif

if ['true', 'if-release'].contains(get_option('b_ndebug'))
  common_flags += ['-DG_DISABLE_ASSERT']
endif

if get_option('translate_windows_paths')
  common_flags += ['-DTRANSLATE_WINDOWS_PATHS']
endif

add_project_arguments(common_flags, language: 'c')

if cc.has_link_argument('-Wl,-Bsymbolic-functions')
  add_project_link_arguments('-Wl,-Bsymbolic-functions', language: 'c')
endif

# Termios
have_termios = cc.has_header('termios.h')

# Required dependencies
glib_req = '2.44.0'

glib_dep = dependency('glib-2.0', version: '>=' + glib_req)
gobject_dep = dependency('gobject-2.0', version: '>=' + glib_req)
gio_dep = dependency('gio-2.0', version: '>=' + glib_req)

libgit2_dep = dependency('libgit2', version: '>= 0.25.0')

# XXX: Not nice, but probably our best option
enable_gir = get_option('introspection') and find_program('g-ir-scanner', required: false).found()

enable_vapi = get_option('vapi')
if enable_vapi
  assert(enable_gir, 'vapi support was requested, but introspection support is mandatory.')
  assert(add_languages('vala', required: false), 'vapi support was requested, but vala not found.')

  foreach ext : ['vapi', 'deps']
    install_symlink(
      'ggit-1.0.' + ext,
      install_dir: libgit2_glib_prefix / libgit2_glib_datadir / 'vala' / 'vapi',
      pointing_to: 'libgit2-glib-1.0.' + ext,
    )
  endforeach
endif

# Check for libgit2 ssh support
enable_ssh = get_option('ssh')
if enable_ssh
  libgit2_ssh_src = '''
    #include <git2.h>
    int
    main(int argc, const char *argv[])
    {
            git_libgit2_init ();
            return ((git_libgit2_features() & GIT_FEATURE_SSH) != 0) ? 0 : 1;
    }
  '''

  assert(cc.compiles(libgit2_ssh_src, name: 'libgit2 supports SSH'), 'libgit2 ssh support was requested, but not found. Use -Dssh=false to build without it.')
endif

# Check for python
enable_python = get_option('python')
if enable_python
  python = import('python').find_installation('python3')

  python_req = '3.2.3'

  dependency('pygobject-3.0', version: '>= 3.0.0')
  assert(python.language_version().version_compare('>= ' + python_req), 'python support was requested, but version >= ' + python_req + ' not found.')

  meson.add_install_script('meson_python_compile.py', libgit2_glib_libdir, python.path())
endif

subdir('libgit2-glib')
subdir('examples')
subdir('tests')

if get_option('gtk_doc')
  assert(enable_gir, 'documentation generation support requested but introspection is disabled. Please use -Dgtk_doc=false.')
  subdir('docs/reference')
endif
