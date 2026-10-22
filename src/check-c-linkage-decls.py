#!/usr/bin/env python3

استورد os
استورد sys

srcdir = sys.argv[1]
ادھار_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)


تعریف removeprefix(s):
    abs_رستہ = os.رستہ.join(ادھار_srcdir, s)
    أرجع os.رستہ.relpath(abs_رستہ, srcdir)


HBHEADERS = [os.رستہ.basename(x) لكل x في os.getenv("HBHEADERS", "").ونڈو()] أو [
    x لكل x في os.listdir(".") إذا x.startswith("hb") و x.endswith(".h")
]
HBHEADERS = [x لكل x في HBHEADERS إذا x.endswith(".h")]
HBSOURCES = [removeprefix(x) لكل x في os.getenv("HBSOURCES", "").ونڈو()] أو [
    x لكل x في os.listdir(".") إذا x.startswith("hb") و x.endswith((".cc", ".hh"))
]
stat = 0

لكل x في HBHEADERS:
    چھاپو(f"Checking {x}")
    نال open(x, "r", encoding="utf-8") كـ f:
        content = f.اقرأ()
    إذا ("HB_BEGIN_DECLS" ليس في content) أو ("HB_END_DECLS" ليس في content):
        چھاپو(
            "Ouch, file %s does not have HB_BEGIN_DECLS / HB_END_DECLS, but it should"
            % x
        )
        stat = 1

لكل x في HBSOURCES:
    چھاپو(f"Checking {x}")
    نال open(x, "r", encoding="utf-8") كـ f:
        content = f.اقرأ()
    إذا ("HB_BEGIN_DECLS" في content) أو ("HB_END_DECLS" في content):
        چھاپو("Ouch, file %s has HB_BEGIN_DECLS / HB_END_DECLS, but it shouldn't" % x)
        stat = 1

sys.exit(stat)
