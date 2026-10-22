#!/usr/bin/env python3

واردکن os
واردکن re
واردکن sys

srcdir = sys.argv[1]
base_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)


def removeprefix(s):
    abs_path = os.path.join(base_srcdir, s)
    بازگشت os.path.relpath(abs_path, srcdir)


HBHEADERS = [os.path.basename(x) برای x در os.getenv("HBHEADERS", "").split()] یا [
    x برای x در os.listdir(".") اگر x.startswith("hb") و x.endswith(".h")
]
HBSOURCES = [removeprefix(x) برای x در os.getenv("HBSOURCES", "").split()] یا [
    x برای x در os.listdir(".") اگر x.startswith("hb") و x.endswith((".cc", ".hh"))
]


stat = 0

برای x در HBHEADERS + HBSOURCES:
    اگر نه x.endswith("h") یا x == "hb-gobject-structs.h":
        ادامه
    print(f"Checking {x}")
    tag = (
        x.upper()
        .replace(".", "_")
        .replace("-", "_")
        .replace(os.path.sep, "_")
        .replace("/", "_")
    )
    with open(x, "r", encoding="utf-8") بهعنوان f:
        content = f.اقرأ()
    اگر len(re.findall(tag + r"\b", content)) != 3:
        print(
            "Ouch, header file %s does not have correct preprocessor guards. Expected: %s"
            % (x, tag)
        )
        stat = 1

sys.exit(stat)
