#!/usr/bin/env python3

आनयन glob
आनयन os
आनयन re
आनयन shutil
आनयन subprocess
आनयन sys

srcdir = sys.argv[1]
base_srcdir = sys.argv[2]
builddir = sys.argv[3]

objdump = os.getenv("OBJDUMP", shutil.which("objdump"))
यदि होइन objdump:
    print("check-static-inits.py: 'ldd' not found; skipping test")
    sys.exit(77)

यदि sys.version_info < (3, 5):
    print("check-static-inits.py: needs python 3.5 for recursive support in glob")
    sys.exit(77)

OBJS = glob.glob(os.path.join(builddir, "**", "*hb*.o"), recursive=सत्य)
यदि होइन OBJS:
    print("check-static-inits.py: object files not found; skipping test")
    sys.exit(77)

stat = 0
tested = असत्य

print("Checking that the following object files has no static initializers/finalizers")
print("\n".join(OBJS))

result = subprocess.run(
    objdump.split() + ["-t"] + OBJS, stdout=subprocess.PIPE, stderr=subprocess.PIPE
)

यदि result.returncode:
    यदि result.stderr.find(b"not recognized") != -1:
        # https://github.com/harfbuzz/harfbuzz/issues/3019
        print('objdump returned "not recognized", skipping')
    अन्यथा:
        print("objdump returned error:\n%s" % (result.stderr.decode("utf-8")))
        stat = 2
अन्यथा:
    tested = सत्य

result = result.stdout.decode("utf-8")

# Checking that no object file has static initializers
लागि lib अन्तः re.findall(r"^.*\.[cd]tors.*$", result, re.MULTILINE):
    यदि होइन re.match(r".*\b0+\b", lib):
        print("Ouch, library has static initializers/finalizers")
        stat = 1

# Checking that no object file has lazy static C++ constructors/destructors or other such stuff
यदि ("__cxa_" अन्तः result) र ("__ubsan_handle" होइन अन्तः result):
    print(
        "Ouch, library has lazy static C++ constructors/destructors or other such stuff"
    )
    stat = 1


sys.exit(stat यदि tested अन्यथा 77)
