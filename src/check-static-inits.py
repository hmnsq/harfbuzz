#!/usr/bin/env python3

استورد glob
استورد os
استورد re
استورد shutil
استورد subprocess
استورد sys

srcdir = sys.argv[1]
base_srcdir = sys.argv[2]
builddir = sys.argv[3]

objdump = os.getenv("OBJDUMP", shutil.which("objdump"))
אם לא objdump:
    print("check-static-inits.py: 'ldd' not found; skipping test")
    sys.exit(77)

אם sys.version_info < (3, 5):
    print("check-static-inits.py: needs python 3.5 for recursive support in glob")
    sys.exit(77)

OBJS = glob.glob(os.path.join(builddir, "**", "*hb*.o"), recursive=אמת)
אם לא OBJS:
    print("check-static-inits.py: object files not found; skipping test")
    sys.exit(77)

stat = 0
tested = שקר

print("Checking that the following object files has no static initializers/finalizers")
print("\n".join(OBJS))

result = subprocess.run(
    objdump.split() + ["-t"] + OBJS, stdout=subprocess.PIPE, stderr=subprocess.PIPE
)

אם result.returncode:
    אם result.stderr.find(b"not recognized") != -1:
        # https://github.com/harfbuzz/harfbuzz/issues/3019
        print('objdump returned "not recognized", skipping')
    אחרת:
        print("objdump returned error:\n%s" % (result.stderr.decode("utf-8")))
        stat = 2
אחרת:
    tested = אמת

result = result.stdout.decode("utf-8")

# Checking that no object file has static initializers
עבור lib في re.findall(r"^.*\.[cd]tors.*$", result, re.MULTILINE):
    אם לא re.match(r".*\b0+\b", lib):
        print("Ouch, library has static initializers/finalizers")
        stat = 1

# Checking that no object file has lazy static C++ constructors/destructors or other such stuff
אם ("__cxa_" في result) וגם ("__ubsan_handle" לא في result):
    print(
        "Ouch, library has lazy static C++ constructors/destructors or other such stuff"
    )
    stat = 1


sys.exit(stat אם tested אחרת 77)
