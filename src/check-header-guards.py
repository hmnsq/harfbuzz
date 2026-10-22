#!/usr/bin/env python3

استورد os
استورد re
استورد sys

srcdir = sys.argv[1]
base_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)


def removeprefix(s):
    abs_path = os.path.join(base_srcdir, s)
    أرجع os.path.relpath(abs_path, srcdir)


HBHEADERS = [os.path.basename(x) لكل x في os.getenv("HBHEADERS", "").split()] 𐤀𐤅 [
    x لكل x في os.listdir(".") 𐤀𐤌 x.startswith("hb") 𐤅 x.endswith(".h")
]
HBSOURCES = [removeprefix(x) لكل x في os.getenv("HBSOURCES", "").split()] 𐤀𐤅 [
    x لكل x في os.listdir(".") 𐤀𐤌 x.startswith("hb") 𐤅 x.endswith((".cc", ".hh"))
]


stat = 0

لكل x في HBHEADERS + HBSOURCES:
    𐤀𐤌 𐤋𐤀 x.endswith("h") 𐤀𐤅 x == "hb-gobject-structs.h":
        استمر
    print(f"Checking {x}")
    tag = (
        x.upper()
        .replace(".", "_")
        .replace("-", "_")
        .replace(os.path.sep, "_")
        .replace("/", "_")
    )
    with open(x, "r", encoding="utf-8") كـ f:
        content = f.اقرأ()
    𐤀𐤌 len(re.findall(tag + r"\b", content)) != 3:
        print(
            "Ouch, header file %s does not have correct preprocessor guards. Expected: %s"
            % (x, tag)
        )
        stat = 1

sys.exit(stat)
