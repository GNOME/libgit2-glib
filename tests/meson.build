unit_test = 'repository'

exe = executable(
  unit_test,
  unit_test + '.c',
  include_directories: top_inc,
  dependencies: libgit2_glib_dep,
)

test(
  unit_test,
  exe,
  args: ['--tap', '-k'],
)
