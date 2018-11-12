#!/usr/bin/env python3

import os
import subprocess
import sys

if not os.environ.get('DESTDIR'):
  prefix = os.environ['MESON_INSTALL_PREFIX']

  libdir = os.path.join(prefix, sys.argv[1])

  print('Byte-compiling python modules...')
  subprocess.call(['python', '-m', 'compileall', '-f', '-q', libdir])

  print('Byte-compiling python modules (optimized versions) ...')
  subprocess.call(['python', '-O', '-m', 'compileall', '-f', '-q', libdir])
