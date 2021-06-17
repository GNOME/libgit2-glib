#!/usr/bin/env python3

import os
import shutil
import sys

prefix = os.environ['MESON_INSTALL_DESTDIR_PREFIX'] if os.environ.get('DESTDIR') else os.environ['MESON_INSTALL_PREFIX']

datadir = os.path.join(prefix, sys.argv[1])

vapidir = os.path.join(datadir, 'vala', 'vapi')
os.chdir(vapidir)

# FIXME: meson will not track the creation of these files
#        https://github.com/mesonbuild/meson/blob/master/mesonbuild/scripts/uninstall.py#L39
old = 'ggit-1.0'
new = 'libgit2-glib-1.0'

for ext in ['vapi', 'deps']:
  src = '{}.{}'.format(new, ext)
  dest = '{}.{}'.format(old, ext)
  try:
      os.symlink(src, dest)
  except OSError:
      shutil.copy(src, dest)
