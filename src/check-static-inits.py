#!/usr/bin/env python3

আমদানি glob
আমদানি os
আমদানি re
আমদানি shutil
আমদানি subprocess
আমদানি sys

srcdir = sys.argv[1]
base_srcdir = sys.argv[2]
builddir = sys.argv[3]

objdump = os.getenv("OBJDUMP", shutil.which("objdump"))
যদি নয় objdump:
    print("check-static-inits.py: 'ldd' not found; skipping test")
    sys.exit(77)

যদি sys.version_info < (3, 5):
    print("check-static-inits.py: needs python 3.5 for recursive support in glob")
    sys.exit(77)

OBJS = glob.glob(os.path.join(builddir, "**", "*hb*.o"), recursive=সত্য)
যদি নয় OBJS:
    print("check-static-inits.py: object files not found; skipping test")
    sys.exit(77)

stat = 0
tested = মিথ্যা

print("Checking that the following object files has no static initializers/finalizers")
print("\n".join(OBJS))

ফলাফল = subprocess.run(
    objdump.split() + ["-t"] + OBJS, stdout=subprocess.PIPE, stderr=subprocess.PIPE
)

যদি ফলাফল.returncode:
    যদি ফলাফল.stderr.find(b"not recognized") != -1:
        # https://github.com/harfbuzz/harfbuzz/issues/3019
        print('objdump returned "not recognized", skipping')
    অন্যথা:
        print("objdump returned error:\n%s" % (ফলাফল.stderr.decode("utf-8")))
        stat = 2
অন্যথা:
    tested = সত্য

ফলাফল = ফলাফল.stdout.decode("utf-8")

# Checking that no object file has static initializers
জন্য lib মধ্যে re.findall(r"^.*\.[cd]tors.*$", ফলাফল, re.MULTILINE):
    যদি নয় re.match(r".*\b0+\b", lib):
        print("Ouch, library has static initializers/finalizers")
        stat = 1

# Checking that no object file has lazy static C++ constructors/destructors or other such stuff
যদি ("__cxa_" মধ্যে ফলাফল) এবং ("__ubsan_handle" নয় মধ্যে ফলাফল):
    print(
        "Ouch, library has lazy static C++ constructors/destructors or other such stuff"
    )
    stat = 1


sys.exit(stat যদি tested অন্যথা 77)
