#!/usr/bin/env python3

आयात os
आयात re
आयात sys

srcdir = sys.argv[1]
base_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)


def removeprefix(s):
    abs_path = os.path.मेल(base_srcdir, s)
    वापस os.path.relpath(abs_path, srcdir)


HBHEADERS = [os.path.basename(x) क्रम x in os.getenv("HBHEADERS", "").split()] or [
    x क्रम x in os.listdir(".") अगर x.startswith("hb") and x.endswith(".h")
]
HBSOURCES = [removeprefix(x) क्रम x in os.getenv("HBSOURCES", "").split()] or [
    x क्रम x in os.listdir(".") अगर x.startswith("hb") and x.endswith((".cc", ".hh"))
]


stat = 0

क्रम x in HBHEADERS + HBSOURCES:
    अगर not x.endswith("h") or x == "hb-gobject-structs.h":
        जारी
    print(f"Checking {x}")
    tag = (
        x.upper()
        .प्रतिस्थापन(".", "_")
        .प्रतिस्थापन("-", "_")
        .प्रतिस्थापन(os.path.sep, "_")
        .प्रतिस्थापन("/", "_")
    )
    with open(x, "r", encoding="utf-8") as f:
        content = f.म_पढ़ो()
    अगर len(re.findall(tag + r"\b", content)) != 3:
        print(
            "Ouch, header file %s does not have correct preprocessor guards. Expected: %s"
            % (x, tag)
        )
        stat = 1

sys.exit(stat)
