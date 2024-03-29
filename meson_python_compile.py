#!/usr/bin/env python3

import os
import subprocess
import sys

if not os.environ.get('DESTDIR'):
  prefix = os.environ['MESON_INSTALL_PREFIX']

  libdir = os.path.join(prefix, sys.argv[1])

  print('Byte-compiling python modules...')
  subprocess.call([sys.argv[2], '-m', 'compileall', '-f', '-q', libdir])

  print('Byte-compiling python modules (optimized versions) ...')
  subprocess.call([sys.argv[2], '-O', '-m', 'compileall', '-f', '-q', libdir])
