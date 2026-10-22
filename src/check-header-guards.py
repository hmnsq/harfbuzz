#!/usr/bin/env python3

આયાત os
આયાત re
આયાત sys

srcdir = sys.argv[1]
base_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)


def removeprefix(s):
    abs_path = os.path.join(base_srcdir, s)
    પાછા os.path.relpath(abs_path, srcdir)


HBHEADERS = [os.path.basename(x) માટે x માં os.getenv("HBHEADERS", "").split()] અથવા [
    x માટે x માં os.listdir(".") જો x.startswith("hb") અને x.endswith(".h")
]
HBSOURCES = [removeprefix(x) માટે x માં os.getenv("HBSOURCES", "").split()] અથવા [
    x માટે x માં os.listdir(".") જો x.startswith("hb") અને x.endswith((".cc", ".hh"))
]


stat = 0

માટે x માં HBHEADERS + HBSOURCES:
    જો નહીં x.endswith("h") અથવા x == "hb-gobject-structs.h":
        ચાલુ
    print(f"Checking {x}")
    tag = (
        x.upper()
        .replace(".", "_")
        .replace("-", "_")
        .replace(os.path.sep, "_")
        .replace("/", "_")
    )
    with open(x, "r", encoding="utf-8") ઇવ f:
        content = f.પઠ()
    જો len(re.findall(tag + r"\b", content)) != 3:
        print(
            "Ouch, header file %s does not have correct preprocessor guards. Expected: %s"
            % (x, tag)
        )
        stat = 1

sys.exit(stat)
