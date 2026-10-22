#!/usr/bin/env python3

आयात os
आयात re
आयात sys

srcdir = sys.argv[1]
base_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)


def removeprefix(s):
    abs_path = os.path.join(base_srcdir, s)
    परत os.path.relpath(abs_path, srcdir)


HBHEADERS = [os.path.basename(x) साठी x मध्ये os.getenv("HBHEADERS", "").split()] किंवा [
    x साठी x मध्ये os.listdir(".") जर x.startswith("hb") आणि x.endswith(".h")
]
HBSOURCES = [removeprefix(x) साठी x मध्ये os.getenv("HBSOURCES", "").split()] किंवा [
    x साठी x मध्ये os.listdir(".") जर x.startswith("hb") आणि x.endswith((".cc", ".hh"))
]


stat = 0

साठी x मध्ये HBHEADERS + HBSOURCES:
    जर नाही x.endswith("h") किंवा x == "hb-gobject-structs.h":
        सुरूठेवा
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
    जर len(re.findall(tag + r"\b", content)) != 3:
        print(
            "Ouch, header file %s does not have correct preprocessor guards. Expected: %s"
            % (x, tag)
        )
        stat = 1

sys.exit(stat)
