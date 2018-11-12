#!/usr/bin/env python3

import os
import subprocess
import sys

vapidir = os.path.join(os.environ['MESON_INSTALL_DESTDIR_PREFIX'], sys.argv[1], 'vala', 'vapi')

# FIXME: meson will not track the creation of these files
#        https://github.com/mesonbuild/meson/blob/master/mesonbuild/scripts/uninstall.py#L39
old = 'ggit-1.0'
new = 'libgit2-glib-1.0'

wd = os.getcwd()
os.chdir(vapidir)

for ext in ['vapi', 'deps']:
  src = os.path.join('{}.{}'.format(new, ext))
  dest = os.path.join('{}.{}'.format(old, ext))
  subprocess.call(['ln', '-s', '-f', src, dest])

os.chdir(wd)
