#!/usr/bin/env python3

درآمد os
درآمد sys

srcdir = sys.argv[1]
بنیاد_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)


تعریف removeprefix(s):
    abs_راستہ = os.راستہ.join(بنیاد_srcdir, s)
    واپس os.راستہ.relpath(abs_راستہ, srcdir)


HBHEADERS = [os.راستہ.basename(x) برائے x میں os.getenv("HBHEADERS", "").بانٹو()] یا [
    x برائے x میں os.listdir(".") اگر x.startswith("hb") اور x.endswith(".h")
]
HBHEADERS = [x برائے x میں HBHEADERS اگر x.endswith(".h")]
HBSOURCES = [removeprefix(x) برائے x میں os.getenv("HBSOURCES", "").بانٹو()] یا [
    x برائے x میں os.listdir(".") اگر x.startswith("hb") اور x.endswith((".cc", ".hh"))
]
stat = 0

برائے x میں HBHEADERS:
    چھاپو(f"Checking {x}")
    ساتھ open(x, "r", encoding="utf-8") بطور f:
        content = f.اقرأ()
    اگر ("HB_BEGIN_DECLS" نہیں میں content) یا ("HB_END_DECLS" نہیں میں content):
        چھاپو(
            "Ouch, file %s does not have HB_BEGIN_DECLS / HB_END_DECLS, but it should"
            % x
        )
        stat = 1

برائے x میں HBSOURCES:
    چھاپو(f"Checking {x}")
    ساتھ open(x, "r", encoding="utf-8") بطور f:
        content = f.اقرأ()
    اگر ("HB_BEGIN_DECLS" میں content) یا ("HB_END_DECLS" میں content):
        چھاپو("Ouch, file %s has HB_BEGIN_DECLS / HB_END_DECLS, but it shouldn't" % x)
        stat = 1

sys.exit(stat)
