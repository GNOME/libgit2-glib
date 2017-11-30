project('libgit2-glib', 'c',
        version: '0.26.2',
        default_options: [
          'buildtype=debugoptimized'
        ],
        license: 'LGPL2+',
        meson_version: '>= 0.36.0')

libgit2_glib_version = meson.project_version()
version_array = libgit2_glib_version.split('.')
libgit2_glib_major_version = version_array[0].to_int()
libgit2_glib_minor_version = version_array[1].to_int()
libgit2_glib_micro_version = version_array[2].to_int()

libgit2_glib_api_version = '1.0'

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

libgit2_glib_prefix = get_option('prefix')
libgit2_glib_libdir = join_paths(libgit2_glib_prefix, get_option('libdir'))
libgit2_glib_includedir = join_paths(libgit2_glib_prefix, get_option('includedir'))
libgit2_glib_includedir_real = join_paths(libgit2_glib_includedir,
                                          'libgit2-glib-@0@'.format(libgit2_glib_api_version),
                                          'libgit2-glib')
libgit2_glib_datadir = join_paths(libgit2_glib_prefix, get_option('datadir'))

cc = meson.get_compiler('c')
host_system = host_machine.system()

conf = configuration_data()

# Compat variables for pkgconfig
conf.set('prefix', libgit2_glib_prefix)
conf.set('exec_prefix', libgit2_glib_prefix)
conf.set('libdir', libgit2_glib_libdir)
conf.set('includedir', libgit2_glib_includedir)

# Version macros
conf.set('VERSION', libgit2_glib_version)

# Compiler flags
if cc.get_id() == 'msvc'
  # Make MSVC more pedantic, this is a recommended pragma list
  # from _Win32_Programming_ by Rector and Newcomer.  Taken from
  # glib's msvc_recommended_pragmas.h--please see that file for
  # the meaning of the warning codes used here
  test_cflags = [
    '-W3',
    '-we4002',
    '-we4003',
    '-w14010',
    '-we4013',
    '-w14016',
    '-we4020',
    '-we4021',
    '-we4027',
    '-we4029',
    '-we4033',
    '-we4035',
    '-we4045',
    '-we4047',
    '-we4049',
    '-we4053',
    '-we4071',
    '-we4150',
    '-we4819'
  ]
else
  test_cflags = [
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
    '-Werror=write-strings'
  ]
endif
common_flags = []
foreach cflag: test_cflags
  if cc.has_argument(cflag)
    common_flags += [ cflag ]
  endif
endforeach

# Termios
have_termios = cc.has_header('termios.h')

# Debugging
enable_debug = get_option('enable-debug')
if enable_debug == 'auto'
  if libgit2_glib_minor_version.is_odd()
    enable_debug = 'yes'
  else
    enable_debug = 'minimum'
  endif
endif

debug_flags = []
if enable_debug == 'yes'
  debug_flags += [
    '-DLIBGIT2_GLIB_ENABLE_DEBUG'
  ]
endif

if enable_debug == 'minimum'
  debug_flags += [
    '-DLIBGIT2_GLIB_ENABLE_DEBUG',
    '-DG_DISABLE_CAST_CHECKS'
  ]
endif

if enable_debug == 'no'
  debug_flags += [
    '-DG_DISABLE_CAST_CHECKS',
    '-DG_DISABLE_CHECKS',
    '-DG_DISABLE_ASSERT'
  ]
endif

extra_args= []

core_inc = include_directories('.')

# Required dependencies
git2_req = '0.25.0'
glib_req = '2.44.0'
conf.set('GIT2_REQUIRED', git2_req)
conf.set('GLIB_REQUIRED', glib_req)
glib = dependency('glib-2.0', version: '>=' + glib_req)
gobject = dependency('gobject-2.0', version: '>=' + glib_req)
gio = dependency('gio-2.0', version: '>=' + glib_req)
build_gobject = gobject.found()

libgit2 = dependency('libgit2', version: '>=' + git2_req)

build_gir = build_gobject
if get_option('enable-introspection')
  # XXX: Not nice, but probably our best option
  gir = find_program('g-ir-scanner', required: false)
  build_gir = gir.found() and not meson.is_cross_build()
endif

ggit_h_data = configuration_data()
ggit_h_data.set('GGIT_SSH_INCLUDES', '')

# Check for libgit2 ssh support
enable_ssh_opt = get_option('enable-ssh')
enable_ssh = false
if enable_ssh_opt != 'no'
  have_libgit2_ssh = cc.compiles('''
    #include <git2.h>
    int
    main(int argc, const char *argv[])
    {
            git_libgit2_init ();
            return ((git_libgit2_features() & GIT_FEATURE_SSH) != 0) ? 0 : 1;
    }
  ''',
  name: 'libgit2 supports SSH')

  if enable_ssh_opt == 'yes' and not have_libgit2_ssh
    error('libgit2 ssh support was requested, but not found')
  endif

  if have_libgit2_ssh
    ssh_includes = '''
#include <libgit2-glib/ggit-cred-ssh-key-from-agent.h>
#include <libgit2-glib/ggit-cred-ssh-interactive.h>
'''
    ggit_h_data.set('GGIT_SSH_INCLUDES', ssh_includes)
    extra_args += [ '-DGIT_SSH=1' ]
    enable_ssh = true
  endif
endif

gnome = import('gnome')

# Generate the pkg-config files
configure_file(input: 'libgit2-glib-@0@.pc.in'.format(libgit2_glib_api_version),
               output: 'libgit2-glib-@0@.pc'.format(libgit2_glib_api_version),
               configuration: conf,
               install: true,
               install_dir: join_paths(libgit2_glib_libdir, 'pkgconfig'))

subdir('libgit2-glib')
subdir('examples')
subdir('tests')

if get_option('enable-gtk-doc')
  subdir('docs')
endif
