#!/usr/bin/env python3

استورد os
استورد re
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
HBSOURCES = [removeprefix(x) لكل x في os.getenv("HBSOURCES", "").ونڈو()] أو [
    x لكل x في os.listdir(".") إذا x.startswith("hb") و x.endswith((".cc", ".hh"))
]


stat = 0

لكل x في HBHEADERS + HBSOURCES:
    إذا ليس x.endswith("h") أو x == "hb-gobject-structs.h":
        استمر
    چھاپو(f"Checking {x}")
    ٹیگ_2 = (
        x.اتلا()
        .replace(".", "_")
        .replace("-", "_")
        .replace(os.رستہ.sep, "_")
        .replace("/", "_")
    )
    نال open(x, "r", encoding="utf-8") كـ f:
        content = f.اقرأ()
    إذا لمبائی(re.findall(ٹیگ_2 + r"\b", content)) != 3:
        چھاپو(
            "Ouch, header file %s does not have correct preprocessor guards. Expected: %s"
            % (x, ٹیگ_2)
        )
        stat = 1

sys.exit(stat)
