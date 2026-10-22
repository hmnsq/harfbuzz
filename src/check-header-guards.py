#!/usr/bin/env python3

درآمد os
درآمد re
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
HBSOURCES = [removeprefix(x) برائے x میں os.getenv("HBSOURCES", "").بانٹو()] یا [
    x برائے x میں os.listdir(".") اگر x.startswith("hb") اور x.endswith((".cc", ".hh"))
]


stat = 0

برائے x میں HBHEADERS + HBSOURCES:
    اگر نہیں x.endswith("h") یا x == "hb-gobject-structs.h":
        جاری
    چھاپو(f"Checking {x}")
    لیبل_2 = (
        x.بالائی()
        .replace(".", "_")
        .replace("-", "_")
        .replace(os.راستہ.sep, "_")
        .replace("/", "_")
    )
    ساتھ open(x, "r", encoding="utf-8") بطور f:
        content = f.اقرأ()
    اگر لمبائی(re.findall(لیبل_2 + r"\b", content)) != 3:
        چھاپو(
            "Ouch, header file %s does not have correct preprocessor guards. Expected: %s"
            % (x, لیبل_2)
        )
        stat = 1

sys.exit(stat)
