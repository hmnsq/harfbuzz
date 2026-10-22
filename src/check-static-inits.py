#!/usr/bin/env python3

استورد glob
استورد os
استورد re
استورد shutil
استورد subprocess
استورد sys

srcdir = sys.argv[1]
بنياد_srcdir = sys.argv[2]
builddir = sys.argv[3]

objdump = os.getenv("OBJDUMP", shutil.which("objdump"))
إذا ليس objdump:
    ڇاپيو("check-static-inits.py: 'ldd' not found; skipping test")
    sys.exit(77)

إذا sys.نسخو_info < (3, 5):
    ڇاپيو("check-static-inits.py: needs python 3.5 for recursive support in glob")
    sys.exit(77)

OBJS = glob.glob(os.رستو.join(builddir, "**", "*hb*.o"), recursive=سچ)
إذا ليس OBJS:
    ڇاپيو("check-static-inits.py: object files not found; skipping test")
    sys.exit(77)

stat = 0
tested = خطأ

ڇاپيو("Checking that the following object files has no static initializers/finalizers")
ڇاپيو("\n".join(OBJS))

نتيجو = subprocess.لاڳيتو(
    objdump.ورهايو() + ["-t"] + OBJS, stdout=subprocess.PIPE, stderr=subprocess.PIPE
)

إذا نتيجو.returncode:
    إذا نتيجو.stderr.ڳوليو(b"not recognized") != -1:
        # https://github.com/harfbuzz/harfbuzz/issues/3019
        ڇاپيو('objdump returned "not recognized", skipping')
    وإلا:
        ڇاپيو("objdump returned error:\n%s" % (نتيجو.stderr.decode("utf-8")))
        stat = 2
وإلا:
    tested = سچ

نتيجو = نتيجو.stdout.decode("utf-8")

# Checking that no object file has static initializers
لكل lib في re.findall(r"^.*\.[cd]tors.*$", نتيجو, re.MULTILINE):
    إذا ليس re.ميلاپ(r".*\b0+\b", lib):
        ڇاپيو("Ouch, library has static initializers/finalizers")
        stat = 1

# Checking that no object file has lazy static C++ constructors/destructors or other such stuff
إذا ("__cxa_" في نتيجو) و ("__ubsan_handle" ليس في نتيجو):
    ڇاپيو(
        "Ouch, library has lazy static C++ constructors/destructors or other such stuff"
    )
    stat = 1


sys.exit(stat إذا tested وإلا 77)
