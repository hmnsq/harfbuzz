#!/usr/bin/env python3

ഇറക്കുമതി os
ഇറക്കുമതി re
ഇറക്കുമതി sys

srcdir = sys.argv[1]
base_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)


def removeprefix(s):
    abs_path = os.path.join(base_srcdir, s)
    തിരികെ os.path.relpath(abs_path, srcdir)


HBHEADERS = [os.path.basename(x) വേണ്ടി x ഇൽ os.getenv("HBHEADERS", "").split()] അഥവാ [
    x വേണ്ടി x ഇൽ os.listdir(".") എങ്കിൽ x.startswith("hb") ഒപ്പം x.endswith(".h")
]
HBSOURCES = [removeprefix(x) വേണ്ടി x ഇൽ os.getenv("HBSOURCES", "").split()] അഥവാ [
    x വേണ്ടി x ഇൽ os.listdir(".") എങ്കിൽ x.startswith("hb") ഒപ്പം x.endswith((".cc", ".hh"))
]


stat = 0

വേണ്ടി x ഇൽ HBHEADERS + HBSOURCES:
    എങ്കിൽ അല്ല x.endswith("h") അഥവാ x == "hb-gobject-structs.h":
        തുടരുക
    print(f"Checking {x}")
    tag = (
        x.upper()
        .replace(".", "_")
        .replace("-", "_")
        .replace(os.path.sep, "_")
        .replace("/", "_")
    )
    with open(x, "r", encoding="utf-8") ഇവ f:
        content = f.പഠ()
    എങ്കിൽ len(re.findall(tag + r"\b", content)) != 3:
        print(
            "Ouch, header file %s does not have correct preprocessor guards. Expected: %s"
            % (x, tag)
        )
        stat = 1

sys.exit(stat)
