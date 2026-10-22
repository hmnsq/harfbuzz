#!/usr/bin/env python3

இறக்குமதி glob
இறக்குமதி os
இறக்குமதி re
இறக்குமதி shutil
இறக்குமதி subprocess
இறக்குமதி sys

srcdir = sys.argv[1]
base_srcdir = sys.argv[2]
builddir = sys.argv[3]

objdump = os.getenv("OBJDUMP", shutil.which("objdump"))
எனில் இல்லை objdump:
    print("check-static-inits.py: 'ldd' not found; skipping test")
    sys.exit(77)

எனில் sys.version_info < (3, 5):
    print("check-static-inits.py: needs python 3.5 for recursive support in glob")
    sys.exit(77)

OBJS = glob.glob(os.path.join(builddir, "**", "*hb*.o"), recursive=உண்மை)
எனில் இல்லை OBJS:
    print("check-static-inits.py: object files not found; skipping test")
    sys.exit(77)

stat = 0
tested = பொய்

print("Checking that the following object files has no static initializers/finalizers")
print("\n".join(OBJS))

result = subprocess.run(
    objdump.split() + ["-t"] + OBJS, stdout=subprocess.PIPE, stderr=subprocess.PIPE
)

எனில் result.returncode:
    எனில் result.stderr.find(b"not recognized") != -1:
        # https://github.com/harfbuzz/harfbuzz/issues/3019
        print('objdump returned "not recognized", skipping')
    இல்லையெனில்:
        print("objdump returned error:\n%s" % (result.stderr.decode("utf-8")))
        stat = 2
இல்லையெனில்:
    tested = உண்மை

result = result.stdout.decode("utf-8")

# Checking that no object file has static initializers
க்கு lib உள் re.findall(r"^.*\.[cd]tors.*$", result, re.MULTILINE):
    எனில் இல்லை re.match(r".*\b0+\b", lib):
        print("Ouch, library has static initializers/finalizers")
        stat = 1

# Checking that no object file has lazy static C++ constructors/destructors or other such stuff
எனில் ("__cxa_" உள் result) மற்றும் ("__ubsan_handle" இல்லை உள் result):
    print(
        "Ouch, library has lazy static C++ constructors/destructors or other such stuff"
    )
    stat = 1


sys.exit(stat எனில் tested இல்லையெனில் 77)
