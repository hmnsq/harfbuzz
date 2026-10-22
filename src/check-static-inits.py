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
जइ ण objdump:
    print("check-static-inits.py: 'ldd' not found; skipping test")
    sys.exit(77)

जइ sys.version_info < (3, 5):
    print("check-static-inits.py: needs python 3.5 for recursive support in glob")
    sys.exit(77)

OBJS = glob.glob(os.path.join(builddir, "**", "*hb*.o"), recursive=सच्च)
जइ ण OBJS:
    print("check-static-inits.py: object files not found; skipping test")
    sys.exit(77)

stat = 0
tested = मिच्छ

print("Checking that the following object files has no static initializers/finalizers")
print("\n".join(OBJS))

result = subprocess.run(
    objdump.split() + ["-t"] + OBJS, stdout=subprocess.PIPE, stderr=subprocess.PIPE
)

जइ result.returncode:
    जइ result.stderr.find(b"not recognized") != -1:
        # https://github.com/harfbuzz/harfbuzz/issues/3019
        print('objdump returned "not recognized", skipping')
    अण्णहा:
        print("objdump returned error:\n%s" % (result.stderr.decode("utf-8")))
        stat = 2
अण्णहा:
    tested = सच्च

result = result.stdout.decode("utf-8")

# Checking that no object file has static initializers
प्रति lib अन्तः re.findall(r"^.*\.[cd]tors.*$", result, re.MULTILINE):
    जइ ण re.match(r".*\b0+\b", lib):
        print("Ouch, library has static initializers/finalizers")
        stat = 1

# Checking that no object file has lazy static C++ constructors/destructors or other such stuff
जइ ("__cxa_" अन्तः result) य ("__ubsan_handle" ण अन्तः result):
    print(
        "Ouch, library has lazy static C++ constructors/destructors or other such stuff"
    )
    stat = 1


sys.exit(stat जइ tested अण्णहा 77)
