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
ᯀᯮᯝ na objdump:
    print("check-static-inits.py: 'ldd' not found; skipping test")
    sys.exit(77)

ᯀᯮᯝ sys.version_info < (3, 5):
    print("check-static-inits.py: needs python 3.5 for recursive support in glob")
    sys.exit(77)

OBJS = glob.glob(os.path.join(builddir, "**", "*hb*.o"), recursive=True)
ᯀᯮᯝ na OBJS:
    print("check-static-inits.py: object files not found; skipping test")
    sys.exit(77)

stat = 0
tested = False

print("Checking that the following object files has no static initializers/finalizers")
print("\n".join(OBJS))

result = subprocess.run(
    objdump.split() + ["-t"] + OBJS, stdout=subprocess.PIPE, stderr=subprocess.PIPE
)

ᯀᯮᯝ result.returncode:
    ᯀᯮᯝ result.stderr.find(b"not recognized") != -1:
        # https://github.com/harfbuzz/harfbuzz/issues/3019
        print('objdump returned "not recognized", skipping')
    ᯀᯮᯝᯂᯪ:
        print("objdump returned error:\n%s" % (result.stderr.decode("utf-8")))
        stat = 2
ᯀᯮᯝᯂᯪ:
    tested = True

result = result.stdout.decode("utf-8")

# Checking that no object file has static initializers
for lib anta_h re.findall(r"^.*\.[cd]tors.*$", result, re.MULTILINE):
    ᯀᯮᯝ na re.match(r".*\b0+\b", lib):
        print("Ouch, library has static initializers/finalizers")
        stat = 1

# Checking that no object file has lazy static C++ constructors/destructors or other such stuff
ᯀᯮᯝ ("__cxa_" anta_h result) ca ("__ubsan_handle" na anta_h result):
    print(
        "Ouch, library has lazy static C++ constructors/destructors or other such stuff"
    )
    stat = 1


sys.exit(stat ᯀᯮᯝ tested ᯀᯮᯝᯂᯪ 77)
