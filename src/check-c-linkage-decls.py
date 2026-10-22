#!/usr/bin/env python3

واردکن os
واردکن sys

srcdir = sys.argv[1]
base_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)


def removeprefix(s):
    abs_path = os.path.join(base_srcdir, s)
    بازگشت os.path.relpath(abs_path, srcdir)


HBHEADERS = [os.path.basename(x) برای x در os.getenv("HBHEADERS", "").split()] یا [
    x برای x در os.listdir(".") اگر x.startswith("hb") و x.endswith(".h")
]
HBHEADERS = [x برای x در HBHEADERS اگر x.endswith(".h")]
HBSOURCES = [removeprefix(x) برای x در os.getenv("HBSOURCES", "").split()] یا [
    x برای x در os.listdir(".") اگر x.startswith("hb") و x.endswith((".cc", ".hh"))
]
stat = 0

برای x در HBHEADERS:
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") بهعنوان f:
        content = f.اقرأ()
    اگر ("HB_BEGIN_DECLS" نه در content) یا ("HB_END_DECLS" نه در content):
        print(
            "Ouch, file %s does not have HB_BEGIN_DECLS / HB_END_DECLS, but it should"
            % x
        )
        stat = 1

برای x در HBSOURCES:
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") بهعنوان f:
        content = f.اقرأ()
    اگر ("HB_BEGIN_DECLS" در content) یا ("HB_END_DECLS" در content):
        print("Ouch, file %s has HB_BEGIN_DECLS / HB_END_DECLS, but it shouldn't" % x)
        stat = 1

sys.exit(stat)
