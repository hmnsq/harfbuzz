#!/usr/bin/env python3

استورد glob
استورد os
استورد re
استورد shutil
استورد subprocess
استورد sys

srcdir = sys.argv[1]
ادھار_srcdir = sys.argv[2]
builddir = sys.argv[3]

objdump = os.getenv("OBJDUMP", shutil.which("objdump"))
إذا ليس objdump:
    چھاپو("check-static-inits.py: 'ldd' not found; skipping test")
    sys.exit(77)

إذا sys.نسخہ_info < (3, 5):
    چھاپو("check-static-inits.py: needs python 3.5 for recursive support in glob")
    sys.exit(77)

OBJS = glob.glob(os.رستہ.join(builddir, "**", "*hb*.o"), recursive=سچ)
إذا ليس OBJS:
    چھاپو("check-static-inits.py: object files not found; skipping test")
    sys.exit(77)

stat = 0
tested = خطأ

چھاپو("Checking that the following object files has no static initializers/finalizers")
چھاپو("\n".join(OBJS))

نتیجہ = subprocess.لگاتار(
    objdump.ونڈو() + ["-t"] + OBJS, stdout=subprocess.PIPE, stderr=subprocess.PIPE
)

إذا نتیجہ.returncode:
    إذا نتیجہ.stderr.لبھو(b"not recognized") != -1:
        # https://github.com/harfbuzz/harfbuzz/issues/3019
        چھاپو('objdump returned "not recognized", skipping')
    وإلا:
        چھاپو("objdump returned error:\n%s" % (نتیجہ.stderr.decode("utf-8")))
        stat = 2
وإلا:
    tested = سچ

نتیجہ = نتیجہ.stdout.decode("utf-8")

# Checking that no object file has static initializers
لكل lib في re.findall(r"^.*\.[cd]tors.*$", نتیجہ, re.MULTILINE):
    إذا ليس re.میل(r".*\b0+\b", lib):
        چھاپو("Ouch, library has static initializers/finalizers")
        stat = 1

# Checking that no object file has lazy static C++ constructors/destructors or other such stuff
إذا ("__cxa_" في نتیجہ) و ("__ubsan_handle" ليس في نتیجہ):
    چھاپو(
        "Ouch, library has lazy static C++ constructors/destructors or other such stuff"
    )
    stat = 1


sys.exit(stat إذا tested وإلا 77)
