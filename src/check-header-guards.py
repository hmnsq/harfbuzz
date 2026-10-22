#!/usr/bin/env python3

ಆನಯನ os
ಆನಯನ re
ಆನಯನ sys

srcdir = sys.argv[1]
base_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)


def removeprefix(s):
    abs_path = os.path.join(base_srcdir, s)
    ಪ್ರತ್ಯಾಗಮ os.path.relpath(abs_path, srcdir)


HBHEADERS = [os.path.basename(x) ಪ್ರತಿ x ಅನ್ತಃ os.getenv("HBHEADERS", "").split()] ವಾ [
    x ಪ್ರತಿ x ಅನ್ತಃ os.listdir(".") ಯದಿ x.startswith("hb") ಚ x.endswith(".h")
]
HBSOURCES = [removeprefix(x) ಪ್ರತಿ x ಅನ್ತಃ os.getenv("HBSOURCES", "").split()] ವಾ [
    x ಪ್ರತಿ x ಅನ್ತಃ os.listdir(".") ಯದಿ x.startswith("hb") ಚ x.endswith((".cc", ".hh"))
]


stat = 0

ಪ್ರತಿ x ಅನ್ತಃ HBHEADERS + HBSOURCES:
    ಯದಿ ನ x.endswith("h") ವಾ x == "hb-gobject-structs.h":
        ಅನುವರ್ತ
    print(f"Checking {x}")
    tag = (
        x.upper()
        .replace(".", "_")
        .replace("-", "_")
        .replace(os.path.sep, "_")
        .replace("/", "_")
    )
    with open(x, "r", encoding="utf-8") ಇವ f:
        content = f.ಪಠ()
    ಯದಿ len(re.findall(tag + r"\b", content)) != 3:
        print(
            "Ouch, header file %s does not have correct preprocessor guards. Expected: %s"
            % (x, tag)
        )
        stat = 1

sys.exit(stat)
