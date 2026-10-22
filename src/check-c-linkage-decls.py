#!/usr/bin/env python3

استورد os
استورد sys

srcdir = sys.argv[1]
بنسټ_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)


تعریف removeprefix(s):
    abs_لار = os.لار.join(بنسټ_srcdir, s)
    أرجع os.لار.relpath(abs_لار, srcdir)


HBHEADERS = [os.لار.basename(x) لكل x في os.getenv("HBHEADERS", "").وېشل()] أو [
    x لكل x في os.listdir(".") إذا x.startswith("hb") و x.endswith(".h")
]
HBHEADERS = [x لكل x في HBHEADERS إذا x.endswith(".h")]
HBSOURCES = [removeprefix(x) لكل x في os.getenv("HBSOURCES", "").وېشل()] أو [
    x لكل x في os.listdir(".") إذا x.startswith("hb") و x.endswith((".cc", ".hh"))
]
stat = 0

لكل x في HBHEADERS:
    چاپ(f"Checking {x}")
    سره open(x, "r", encoding="utf-8") كـ f:
        content = f.اقرأ()
    إذا ("HB_BEGIN_DECLS" ليس في content) أو ("HB_END_DECLS" ليس في content):
        چاپ(
            "Ouch, file %s does not have HB_BEGIN_DECLS / HB_END_DECLS, but it should"
            % x
        )
        stat = 1

لكل x في HBSOURCES:
    چاپ(f"Checking {x}")
    سره open(x, "r", encoding="utf-8") كـ f:
        content = f.اقرأ()
    إذا ("HB_BEGIN_DECLS" في content) أو ("HB_END_DECLS" في content):
        چاپ("Ouch, file %s has HB_BEGIN_DECLS / HB_END_DECLS, but it shouldn't" % x)
        stat = 1

sys.exit(stat)
