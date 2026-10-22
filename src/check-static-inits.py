#!/usr/bin/env python3

import glob
import os
import re
import shutil
import subprocess
import sys

srcdir = sys.argv[1]
base_srcdir = sys.argv[2]
builddir = sys.argv[3]

objdump = os.getenv("OBJDUMP", shutil.which("objdump"))
만약 not objdump:
    print("check-static-inits.py: 'ldd' not found; skipping test")
    sys.exit(77)

만약 sys.version_info < (3, 5):
    print("check-static-inits.py: needs python 3.5 for recursive support in glob")
    sys.exit(77)

OBJS = glob.glob(os.path.join(builddir, "**", "*hb*.o"), recursive=True)
만약 not OBJS:
    print("check-static-inits.py: object files not found; skipping test")
    sys.exit(77)

stat = 0
tested = False

print("Checking that the following object files has no static initializers/finalizers")
print("\n".join(OBJS))

result = subprocess.run(
    objdump.split() + ["-t"] + OBJS, stdout=subprocess.PIPE, stderr=subprocess.PIPE
)

만약 result.returncode:
    만약 result.stderr.find(b"not recognized") != -1:
        # https://github.com/harfbuzz/harfbuzz/issues/3019
        print('objdump returned "not recognized", skipping')
    아니면:
        print("objdump returned error:\n%s" % (result.stderr.decode("utf-8")))
        stat = 2
아니면:
    tested = True

result = result.stdout.decode("utf-8")

# Checking that no object file has static initializers
위해 lib in re.findall(r"^.*\.[cd]tors.*$", result, re.MULTILINE):
    만약 not re.match(r".*\b0+\b", lib):
        print("Ouch, library has static initializers/finalizers")
        stat = 1

# Checking that no object file has lazy static C++ constructors/destructors or other such stuff
만약 ("__cxa_" in result) and ("__ubsan_handle" not in result):
    print(
        "Ouch, library has lazy static C++ constructors/destructors or other such stuff"
    )
    stat = 1


sys.exit(stat 만약 tested 아니면 77)
