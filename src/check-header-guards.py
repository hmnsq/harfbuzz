#!/usr/bin/env python3

ଆମଦାନୀ os
ଆମଦାନୀ re
ଆମଦାନୀ sys

srcdir = sys.argv[1]
base_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)


def removeprefix(s):
    abs_path = os.path.join(base_srcdir, s)
    ଫେରନ୍ତୁ os.path.relpath(abs_path, srcdir)


HBHEADERS = [os.path.basename(x) ପାଇଁ x ଭିତରେ os.getenv("HBHEADERS", "").split()] କିମ୍ବା [
    x ପାଇଁ x ଭିତରେ os.listdir(".") ଯଦି x.startswith("hb") ଏବଂ x.endswith(".h")
]
HBSOURCES = [removeprefix(x) ପାଇଁ x ଭିତରେ os.getenv("HBSOURCES", "").split()] କିମ୍ବା [
    x ପାଇଁ x ଭିତରେ os.listdir(".") ଯଦି x.startswith("hb") ଏବଂ x.endswith((".cc", ".hh"))
]


stat = 0

ପାଇଁ x ଭିତରେ HBHEADERS + HBSOURCES:
    ଯଦି ନୁହେଁ x.endswith("h") କିମ୍ବା x == "hb-gobject-structs.h":
        ଜାରି
    print(f"Checking {x}")
    tag = (
        x.upper()
        .replace(".", "_")
        .replace("-", "_")
        .replace(os.path.sep, "_")
        .replace("/", "_")
    )
    with open(x, "r", encoding="utf-8") ଇଵ f:
        content = f.ପଠ()
    ଯଦି len(re.findall(tag + r"\b", content)) != 3:
        print(
            "Ouch, header file %s does not have correct preprocessor guards. Expected: %s"
            % (x, tag)
        )
        stat = 1

sys.exit(stat)
