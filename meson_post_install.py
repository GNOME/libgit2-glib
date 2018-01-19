#!/usr/bin/env python3

import os
import subprocess
import sys

if not os.environ.get('DESTDIR'):
  libdir = sys.argv[1]

  print('Byte-compiling python modules...')
  subprocess.call(['python', '-m', 'compileall', '-f', '-q', libdir])

  print('Byte-compiling python modules (optimized versions) ...')
  subprocess.call(['python', '-O', '-m', 'compileall', '-f', '-q', libdir])
