#!/usr/bin/env python3

import os
import subprocess
import sys

datadir = sys.argv[1]

destdir = os.environ.get('DESTDIR')
if destdir:
    datadir = os.path.splitdrive(datadir)[1].lstrip(os.path.sep)
    datadir = os.path.join(destdir, datadir)

vapidir = os.path.join(datadir, 'vala', 'vapi')
os.chdir(vapidir)

# FIXME: meson will not track the creation of these files
#        https://github.com/mesonbuild/meson/blob/master/mesonbuild/scripts/uninstall.py#L39
old = 'ggit-1.0'
new = 'libgit2-glib-1.0'

for ext in ['vapi', 'deps']:
  src = '{}.{}'.format(new, ext)
  dest = '{}.{}'.format(old, ext)
  subprocess.call(['ln', '-s', '-f', src, dest])
