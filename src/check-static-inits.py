#!/usr/bin/env python3

درآمد glob
درآمد os
درآمد re
درآمد shutil
درآمد subprocess
درآمد sys

srcdir = sys.argv[1]
بنیاد_srcdir = sys.argv[2]
builddir = sys.argv[3]

objdump = os.getenv("OBJDUMP", shutil.which("objdump"))
اگر نہیں objdump:
    چھاپو("check-static-inits.py: 'ldd' not found; skipping test")
    sys.exit(77)

اگر sys.نسخہ_info < (3, 5):
    چھاپو("check-static-inits.py: needs python 3.5 for recursive support in glob")
    sys.exit(77)

OBJS = glob.glob(os.راستہ.join(builddir, "**", "*hb*.o"), recursive=سچ)
اگر نہیں OBJS:
    چھاپو("check-static-inits.py: object files not found; skipping test")
    sys.exit(77)

stat = 0
tested = جھوٹ

چھاپو("Checking that the following object files has no static initializers/finalizers")
چھاپو("\n".join(OBJS))

نتیجہ = subprocess.تسلسل(
    objdump.بانٹو() + ["-t"] + OBJS, stdout=subprocess.PIPE, stderr=subprocess.PIPE
)

اگر نتیجہ.returncode:
    اگر نتیجہ.stderr.ڈھونڈو(b"not recognized") != -1:
        # https://github.com/harfbuzz/harfbuzz/issues/3019
        چھاپو('objdump returned "not recognized", skipping')
    ورنہ:
        چھاپو("objdump returned error:\n%s" % (نتیجہ.stderr.decode("utf-8")))
        stat = 2
ورنہ:
    tested = سچ

نتیجہ = نتیجہ.stdout.decode("utf-8")

# Checking that no object file has static initializers
برائے lib میں re.findall(r"^.*\.[cd]tors.*$", نتیجہ, re.MULTILINE):
    اگر نہیں re.مطابقت(r".*\b0+\b", lib):
        چھاپو("Ouch, library has static initializers/finalizers")
        stat = 1

# Checking that no object file has lazy static C++ constructors/destructors or other such stuff
اگر ("__cxa_" میں نتیجہ) اور ("__ubsan_handle" نہیں میں نتیجہ):
    چھاپو(
        "Ouch, library has lazy static C++ constructors/destructors or other such stuff"
    )
    stat = 1


sys.exit(stat اگر tested ورنہ 77)
