#!/usr/bin/env python3

import os, sys, glob, pickle, subprocess

sys.path.insert(0, os.path.dirname(__file__))
from clang import cindex
sys.path = sys.path[1:]

def configure_libclang():
    llvm_libdirs = ['/usr/lib/llvm-3.2/lib', '/usr/lib64/llvm']

    try:
        libdir = subprocess.check_output(['llvm-config', '--libdir']).decode('utf-8').strip()
        llvm_libdirs.insert(0, libdir)
    except OSError:
        pass

    for d in llvm_libdirs:
        if not os.path.exists(d):
            continue

        files = glob.glob(os.path.join(d, 'libclang.so*'))

        if len(files) != 0:
            cindex.Config.set_library_file(files[0])
            return

class Call:
    def __init__(self, cursor, decl):
        self.ident = cursor.displayname.decode('utf-8')
        self.filename = cursor.location.file.name.decode('utf-8')

        ex = cursor.extent

        self.start_line = ex.start.line
        self.start_column = ex.start.column

        self.end_line = ex.end.line
        self.end_column = ex.end.column

        self.decl_filename = decl.location.file.name.decode('utf-8')

class Definition:
    def __init__(self, cursor):
        self.ident = cursor.spelling.decode('utf-8')
        self.display = cursor.displayname.decode('utf-8')

        self.filename = cursor.location.file.name.decode('utf-8')

        ex = cursor.extent

        self.start_line = ex.start.line
        self.start_column = ex.start.column

        self.end_line = ex.end.line
        self.end_column = ex.end.column

def process_diagnostics(tu):
    diagnostics = tu.diagnostics

    haserr = False

    for d in diagnostics:
        sys.stderr.write('{0}\n'.format(d.format.decode('utf-8')))

        if d.severity > cindex.Diagnostic.Warning:
            haserr = True

    if haserr:
        sys.exit(1)

def walk_cursors(tu, files):
    proc = list(tu.cursor.get_children())

    while len(proc) > 0:
        cursor = proc[0]
        proc = proc[1:]

        if cursor.location.file is None:
            continue

        fname = cursor.location.file.name.decode('utf-8')

        if fname in files:
            yield cursor

            proc += list(cursor.get_children())

def newer(a, b):
    try:
        return os.stat(a).st_mtime > os.stat(b).st_mtime
    except:
        return True

def scan_libgit2_glib(cflags, files, git2dir):
    files = [os.path.abspath(f) for f in files]

    dname = os.path.dirname(__file__)
    allcalls = {}

    for f in files:
        if not os.getenv('SILENT'):
            sys.stderr.write('Processing {0}\n'.format(f))

        astf = os.path.join(dname, '.' + os.path.basename(f) + '.cache')

        if not newer(f, astf):
            with open(astf, 'rb') as fo:
                calls = pickle.load(fo)
        else:
            tu = cindex.TranslationUnit.from_source(f, cflags)

            process_diagnostics(tu)
            calls = {}

            for cursor in walk_cursors(tu, files):
                if cursor.kind == cindex.CursorKind.CALL_EXPR or \
                   cursor.kind == cindex.CursorKind.DECL_REF_EXPR:

                    cdecl = cursor.get_referenced()

                    if cdecl.kind != cindex.CursorKind.FUNCTION_DECL:
                        continue

                    if (not cdecl is None) and (not cdecl.location.file is None):
                        fdefname = cdecl.location.file.name.decode('utf-8')

                        if fdefname.startswith(git2dir):
                            call = Call(cursor, cdecl)

                            if call.ident in calls:
                                calls[call.ident].append(call)
                            else:
                                calls[call.ident] = [call]

        with open(astf, 'wb') as fo:
            pickle.dump(calls, fo)

        for k in calls:
            if k in allcalls:
                allcalls[k] += calls[k]
            else:
                allcalls[k] = list(calls[k])

    return allcalls

def scan_libgit2(cflags, git2dir):
    tu = cindex.TranslationUnit.from_source(git2dir + '.h', cflags)
    process_diagnostics(tu)
    headers = glob.glob(os.path.join(git2dir, '*.h'))

    defs = {}

    for cursor in walk_cursors(tu, headers):
        if cursor.kind == cindex.CursorKind.FUNCTION_DECL:
            deff = Definition(cursor)
            defs[deff.ident] = deff

    return defs

configure_libclang()

pos = sys.argv.index('--')

cflags = sys.argv[1:pos]
files = sys.argv[pos+1:]

incdir = os.getenv('LIBGIT2_INCLUDE_DIR')

defs = scan_libgit2(cflags, incdir)
calls = scan_libgit2_glib(cflags, files, incdir)

notused = {}
perfile = {}
nperfile = {}

for d in defs:
    o = defs[d]

    if not d in calls:
        notused[d] = defs[d]

        if not o.filename in nperfile:
            nperfile[o.filename] = [o]
        else:
            nperfile[o.filename].append(o)

    if not o.filename in perfile:
        perfile[o.filename] = [o]
    else:
        perfile[o.filename].append(o)

ss = [notused[f] for f in notused]
ss.sort(key=lambda x: '{0} {1}'.format(os.path.basename(x.filename), x.ident))

lastf = None

keys = list(perfile.keys())
keys.sort()

for filename in keys:
    b = os.path.basename(filename)
    f = perfile[filename]

    n_perfile = len(f)

    if filename in nperfile:
        n_nperfile = len(nperfile[filename])
    else:
        n_nperfile = 0

    perc = int(((n_perfile - n_nperfile) / n_perfile) * 100)

    print('\n  File {0}, coverage {1}% ({2} out of {3}):'.format(b, perc, n_perfile - n_nperfile, n_perfile))

    cp = list(f)
    cp.sort(key=lambda x: "{0} {1}".format(not x.ident in calls, x.ident))

    for d in cp:
        if d.ident in calls:
            print('      \033[32m✓ {0}\033[0m'.format(d.display))
        else:
            print('      \033[31m✗ {0}\033[0m'.format(d.display))

perc = int(((len(defs) - len(notused)) / len(defs)) * 100)

print('\nTotal coverage: {0}% ({1} functions out of {2} are being called)\n'.format(perc, len(defs) - len(notused), len(defs)))

# vi:ts=4:et
