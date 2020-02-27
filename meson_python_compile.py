#!/usr/bin/env python3

import os
import subprocess
import sys

if not os.environ.get('DESTDIR'):
  libdir = sys.argv[1]

  print('Byte-compiling python modules...')
  subprocess.check_call(['python3', '-m', 'compileall', '-f', '-q', libdir])

  print('Byte-compiling python modules (optimized versions) ...')
  subprocess.check_call(['python3', '-O', '-m', 'compileall', '-f', '-q', libdir])
