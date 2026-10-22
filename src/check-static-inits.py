#!/usr/bin/env python3

استورد glob
استورد os
استورد re
استورد shutil
استورد subprocess
استورد sys

srcdir = sys.argv[1]
بنسټ_srcdir = sys.argv[2]
builddir = sys.argv[3]

objdump = os.getenv("OBJDUMP", shutil.which("objdump"))
إذا ليس objdump:
    چاپ("check-static-inits.py: 'ldd' not found; skipping test")
    sys.exit(77)

إذا sys.نسخه_info < (3, 5):
    چاپ("check-static-inits.py: needs python 3.5 for recursive support in glob")
    sys.exit(77)

OBJS = glob.glob(os.لار.join(builddir, "**", "*hb*.o"), recursive=ریښتیا)
إذا ليس OBJS:
    چاپ("check-static-inits.py: object files not found; skipping test")
    sys.exit(77)

stat = 0
tested = خطأ

چاپ("Checking that the following object files has no static initializers/finalizers")
چاپ("\n".join(OBJS))

پایله = subprocess.پرلپسې(
    objdump.وېشل() + ["-t"] + OBJS, stdout=subprocess.PIPE, stderr=subprocess.PIPE
)

إذا پایله.returncode:
    إذا پایله.stderr.پیدا_کړه(b"not recognized") != -1:
        # https://github.com/harfbuzz/harfbuzz/issues/3019
        چاپ('objdump returned "not recognized", skipping')
    وإلا:
        چاپ("objdump returned error:\n%s" % (پایله.stderr.decode("utf-8")))
        stat = 2
وإلا:
    tested = ریښتیا

پایله = پایله.stdout.decode("utf-8")

# Checking that no object file has static initializers
لكل lib في re.findall(r"^.*\.[cd]tors.*$", پایله, re.MULTILINE):
    إذا ليس re.سمون(r".*\b0+\b", lib):
        چاپ("Ouch, library has static initializers/finalizers")
        stat = 1

# Checking that no object file has lazy static C++ constructors/destructors or other such stuff
إذا ("__cxa_" في پایله) و ("__ubsan_handle" ليس في پایله):
    چاپ(
        "Ouch, library has lazy static C++ constructors/destructors or other such stuff"
    )
    stat = 1


sys.exit(stat إذا tested وإلا 77)
