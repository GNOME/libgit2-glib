examples = [
  'general',
  'walk',
]

if have_termios
  examples += ['clone']
endif

foreach example: examples
  executable(
    example,
    example + '.c',
    include_directories: top_inc,
    dependencies: libgit2_glib_dep,
  )
endforeach
