#!/usr/bin/env python3

ଆମଦାନୀ glob
ଆମଦାନୀ os
ଆମଦାନୀ re
ଆମଦାନୀ shutil
ଆମଦାନୀ subprocess
ଆମଦାନୀ sys

srcdir = sys.argv[1]
base_srcdir = sys.argv[2]
builddir = sys.argv[3]

objdump = os.getenv("OBJDUMP", shutil.which("objdump"))
ଯଦି ନୁହେଁ objdump:
    print("check-static-inits.py: 'ldd' not found; skipping test")
    sys.exit(77)

ଯଦି sys.version_info < (3, 5):
    print("check-static-inits.py: needs python 3.5 for recursive support in glob")
    sys.exit(77)

OBJS = glob.glob(os.path.join(builddir, "**", "*hb*.o"), recursive=ସତ୍ୟ)
ଯଦି ନୁହେଁ OBJS:
    print("check-static-inits.py: object files not found; skipping test")
    sys.exit(77)

stat = 0
tested = ମିଥ୍ୟା

print("Checking that the following object files has no static initializers/finalizers")
print("\n".join(OBJS))

result = subprocess.run(
    objdump.split() + ["-t"] + OBJS, stdout=subprocess.PIPE, stderr=subprocess.PIPE
)

ଯଦି result.returncode:
    ଯଦି result.stderr.find(b"not recognized") != -1:
        # https://github.com/harfbuzz/harfbuzz/issues/3019
        print('objdump returned "not recognized", skipping')
    ଅନ୍ୟଥା:
        print("objdump returned error:\n%s" % (result.stderr.decode("utf-8")))
        stat = 2
ଅନ୍ୟଥା:
    tested = ସତ୍ୟ

result = result.stdout.decode("utf-8")

# Checking that no object file has static initializers
ପାଇଁ lib ଭିତରେ re.findall(r"^.*\.[cd]tors.*$", result, re.MULTILINE):
    ଯଦି ନୁହେଁ re.match(r".*\b0+\b", lib):
        print("Ouch, library has static initializers/finalizers")
        stat = 1

# Checking that no object file has lazy static C++ constructors/destructors or other such stuff
ଯଦି ("__cxa_" ଭିତରେ result) ଏବଂ ("__ubsan_handle" ନୁହେଁ ଭିତରେ result):
    print(
        "Ouch, library has lazy static C++ constructors/destructors or other such stuff"
    )
    stat = 1


sys.exit(stat ଯଦି tested ଅନ୍ୟଥା 77)
