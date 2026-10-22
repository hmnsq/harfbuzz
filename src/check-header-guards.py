#!/usr/bin/env python3

استورد os
استورد re
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
HBSOURCES = [removeprefix(x) لكل x في os.getenv("HBSOURCES", "").تقسیم()] أو [
    x لكل x في os.listdir(".") إذا x.startswith("hb") و x.endswith((".cc", ".hh"))
]


stat = 0

لكل x في HBHEADERS + HBSOURCES:
    إذا ليس x.endswith("h") أو x == "hb-gobject-structs.h":
        استمر
    چاپ(f"Checking {x}")
    برچسب_2 = (
        x.فوقانی()
        .replace(".", "_")
        .replace("-", "_")
        .replace(os.مسیر.sep, "_")
        .replace("/", "_")
    )
    با open(x, "r", encoding="utf-8") كـ f:
        content = f.اقرأ()
    إذا طول(re.findall(برچسب_2 + r"\b", content)) != 3:
        چاپ(
            "Ouch, header file %s does not have correct preprocessor guards. Expected: %s"
            % (x, برچسب_2)
        )
        stat = 1

sys.exit(stat)
