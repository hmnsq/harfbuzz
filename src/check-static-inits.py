#!/usr/bin/env python3

आयात glob
आयात os
आयात re
आयात shutil
आयात subprocess
आयात sys

srcdir = sys.argv[1]
base_srcdir = sys.argv[2]
builddir = sys.argv[3]

objdump = os.getenv("OBJDUMP", shutil.which("objdump"))
अगर not objdump:
    print("check-static-inits.py: 'ldd' not found; skipping test")
    sys.exit(77)

अगर sys.version_info < (3, 5):
    print("check-static-inits.py: needs python 3.5 for recursive support in glob")
    sys.exit(77)

OBJS = glob.glob(os.path.मेल(builddir, "**", "*hb*.o"), recursive=True)
अगर not OBJS:
    print("check-static-inits.py: object files not found; skipping test")
    sys.exit(77)

stat = 0
tested = False

print("Checking that the following object files has no static initializers/finalizers")
print("\n".मेल(OBJS))

परिणाम = subprocess.आरम्भ(
    objdump.split() + ["-t"] + OBJS, stdout=subprocess.PIPE, stderr=subprocess.PIPE
)

अगर परिणाम.returncode:
    अगर परिणाम.stderr.find(b"not recognized") != -1:
        # https://github.com/harfbuzz/harfbuzz/issues/3019
        print('objdump returned "not recognized", skipping')
    अन्यथा:
        print("objdump returned error:\n%s" % (परिणाम.stderr.decode("utf-8")))
        stat = 2
अन्यथा:
    tested = True

परिणाम = परिणाम.stdout.decode("utf-8")

# Checking that no object file has static initializers
क्रम lib in re.findall(r"^.*\.[cd]tors.*$", परिणाम, re.MULTILINE):
    अगर not re.match(r".*\b0+\b", lib):
        print("Ouch, library has static initializers/finalizers")
        stat = 1

# Checking that no object file has lazy static C++ constructors/destructors or other such stuff
अगर ("__cxa_" in परिणाम) and ("__ubsan_handle" not in परिणाम):
    print(
        "Ouch, library has lazy static C++ constructors/destructors or other such stuff"
    )
    stat = 1


sys.exit(stat अगर tested अन्यथा 77)
