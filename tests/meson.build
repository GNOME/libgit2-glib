unit_tests = ['repository']

foreach unit: unit_tests
  exe = executable(
    unit,
    unit + '.c',
    dependencies: libgit2_glib_dep,
    include_directories: libgit2_glib_inc,
  )

  test(
    unit,
    exe,
    args: ['--tap', '-k'],
  )
endforeach
