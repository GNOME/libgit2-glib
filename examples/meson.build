examples = [
  'general',
  'walk',
]

if have_termios
  examples += [ 'clone' ]
endif

foreach example: examples
  executable(example, example + '.c',
             dependencies: libgit2_glib_dep,
             include_directories: libgit2_glib_inc)
endforeach
