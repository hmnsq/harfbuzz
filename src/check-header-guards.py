#!/usr/bin/env python3

استورد os
استورد re
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
HBSOURCES = [removeprefix(x) لكل x في os.getenv("HBSOURCES", "").ورهايو()] أو [
    x لكل x في os.listdir(".") إذا x.startswith("hb") و x.endswith((".cc", ".hh"))
]


stat = 0

لكل x في HBHEADERS + HBSOURCES:
    إذا ليس x.endswith("h") أو x == "hb-gobject-structs.h":
        استمر
    ڇاپيو(f"Checking {x}")
    ٽيگ_2 = (
        x.مٿانهون()
        .replace(".", "_")
        .replace("-", "_")
        .replace(os.رستو.sep, "_")
        .replace("/", "_")
    )
    سان open(x, "r", encoding="utf-8") كـ f:
        content = f.اقرأ()
    إذا ڊيگهه(re.findall(ٽيگ_2 + r"\b", content)) != 3:
        ڇاپيو(
            "Ouch, header file %s does not have correct preprocessor guards. Expected: %s"
            % (x, ٽيگ_2)
        )
        stat = 1

sys.exit(stat)
