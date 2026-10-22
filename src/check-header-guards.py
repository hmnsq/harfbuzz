#!/usr/bin/env python3

استورد os
استورد re
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
HBSOURCES = [removeprefix(x) لكل x في os.getenv("HBSOURCES", "").وېشل()] أو [
    x لكل x في os.listdir(".") إذا x.startswith("hb") و x.endswith((".cc", ".hh"))
]


stat = 0

لكل x في HBHEADERS + HBSOURCES:
    إذا ليس x.endswith("h") أو x == "hb-gobject-structs.h":
        استمر
    چاپ(f"Checking {x}")
    نښان_2 = (
        x.پاسنی()
        .replace(".", "_")
        .replace("-", "_")
        .replace(os.لار.sep, "_")
        .replace("/", "_")
    )
    سره open(x, "r", encoding="utf-8") كـ f:
        content = f.اقرأ()
    إذا اوږدوالی(re.findall(نښان_2 + r"\b", content)) != 3:
        چاپ(
            "Ouch, header file %s does not have correct preprocessor guards. Expected: %s"
            % (x, نښان_2)
        )
        stat = 1

sys.exit(stat)
