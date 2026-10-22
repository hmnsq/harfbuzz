#!/usr/bin/env python3

استورد os
استورد sys

srcdir = sys.argv[1]
پایه_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)


تعریف removeprefix(s):
    abs_مسیر = os.مسیر.join(پایه_srcdir, s)
    أرجع os.مسیر.relpath(abs_مسیر, srcdir)


HBHEADERS = [os.مسیر.basename(x) لكل x في os.getenv("HBHEADERS", "").تقسیم()] أو [
    x لكل x في os.listdir(".") إذا x.startswith("hb") و x.endswith(".h")
]
HBHEADERS = [x لكل x في HBHEADERS إذا x.endswith(".h")]
HBSOURCES = [removeprefix(x) لكل x في os.getenv("HBSOURCES", "").تقسیم()] أو [
    x لكل x في os.listdir(".") إذا x.startswith("hb") و x.endswith((".cc", ".hh"))
]
stat = 0

لكل x في HBHEADERS:
    چاپ(f"Checking {x}")
    با open(x, "r", encoding="utf-8") كـ f:
        content = f.اقرأ()
    إذا ("HB_BEGIN_DECLS" ليس في content) أو ("HB_END_DECLS" ليس في content):
        چاپ(
            "Ouch, file %s does not have HB_BEGIN_DECLS / HB_END_DECLS, but it should"
            % x
        )
        stat = 1

لكل x في HBSOURCES:
    چاپ(f"Checking {x}")
    با open(x, "r", encoding="utf-8") كـ f:
        content = f.اقرأ()
    إذا ("HB_BEGIN_DECLS" في content) أو ("HB_END_DECLS" في content):
        چاپ("Ouch, file %s has HB_BEGIN_DECLS / HB_END_DECLS, but it shouldn't" % x)
        stat = 1

sys.exit(stat)
