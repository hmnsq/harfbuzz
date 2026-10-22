#!/usr/bin/env python3

استورد os
استورد sys

srcdir = sys.argv[1]
بنياد_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)


وصف removeprefix(s):
    abs_رستو = os.رستو.join(بنياد_srcdir, s)
    أرجع os.رستو.relpath(abs_رستو, srcdir)


HBHEADERS = [os.رستو.basename(x) لكل x في os.getenv("HBHEADERS", "").ورهايو()] أو [
    x لكل x في os.listdir(".") إذا x.startswith("hb") و x.endswith(".h")
]
HBHEADERS = [x لكل x في HBHEADERS إذا x.endswith(".h")]
HBSOURCES = [removeprefix(x) لكل x في os.getenv("HBSOURCES", "").ورهايو()] أو [
    x لكل x في os.listdir(".") إذا x.startswith("hb") و x.endswith((".cc", ".hh"))
]
stat = 0

لكل x في HBHEADERS:
    ڇاپيو(f"Checking {x}")
    سان open(x, "r", encoding="utf-8") كـ f:
        content = f.اقرأ()
    إذا ("HB_BEGIN_DECLS" ليس في content) أو ("HB_END_DECLS" ليس في content):
        ڇاپيو(
            "Ouch, file %s does not have HB_BEGIN_DECLS / HB_END_DECLS, but it should"
            % x
        )
        stat = 1

لكل x في HBSOURCES:
    ڇاپيو(f"Checking {x}")
    سان open(x, "r", encoding="utf-8") كـ f:
        content = f.اقرأ()
    إذا ("HB_BEGIN_DECLS" في content) أو ("HB_END_DECLS" في content):
        ڇاپيو("Ouch, file %s has HB_BEGIN_DECLS / HB_END_DECLS, but it shouldn't" % x)
        stat = 1

sys.exit(stat)
