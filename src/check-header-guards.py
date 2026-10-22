#!/usr/bin/env python3

आनयन os
आनयन re
आनयन sys

srcdir = sys.argv[1]
base_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)


def removeprefix(s):
    abs_path = os.path.join(base_srcdir, s)
    पटिनिवत्त os.path.relpath(abs_path, srcdir)


HBHEADERS = [os.path.basename(x) अत्थाय x अन्तः os.getenv("HBHEADERS", "").split()] वा [
    x अत्थाय x अन्तः os.listdir(".") सचे x.startswith("hb") च x.endswith(".h")
]
HBSOURCES = [removeprefix(x) अत्थाय x अन्तः os.getenv("HBSOURCES", "").split()] वा [
    x अत्थाय x अन्तः os.listdir(".") सचे x.startswith("hb") च x.endswith((".cc", ".hh"))
]


stat = 0

अत्थाय x अन्तः HBHEADERS + HBSOURCES:
    सचे न x.endswith("h") वा x == "hb-gobject-structs.h":
        अनुवर्त
    print(f"Checking {x}")
    tag = (
        x.upper()
        .replace(".", "_")
        .replace("-", "_")
        .replace(os.path.sep, "_")
        .replace("/", "_")
    )
    with open(x, "r", encoding="utf-8") इव f:
        content = f.पठ()
    सचे len(re.findall(tag + r"\b", content)) != 3:
        print(
            "Ouch, header file %s does not have correct preprocessor guards. Expected: %s"
            % (x, tag)
        )
        stat = 1

sys.exit(stat)
