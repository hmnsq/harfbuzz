#!/usr/bin/env python3

আনযন os
আনযন sys

srcdir = sys.argv[1]
base_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)


def removeprefix(s):
    abs_path = os.path.join(base_srcdir, s)
    প্রত্যাগম os.path.relpath(abs_path, srcdir)


HBHEADERS = [os.path.basename(x) প্রতি x অন্তঃ os.getenv("HBHEADERS", "").split()] বা [
    x প্রতি x অন্তঃ os.listdir(".") যদি x.startswith("hb") চ x.endswith(".h")
]
HBHEADERS = [x প্রতি x অন্তঃ HBHEADERS যদি x.endswith(".h")]
HBSOURCES = [removeprefix(x) প্রতি x অন্তঃ os.getenv("HBSOURCES", "").split()] বা [
    x প্রতি x অন্তঃ os.listdir(".") যদি x.startswith("hb") চ x.endswith((".cc", ".hh"))
]
stat = 0

প্রতি x অন্তঃ HBHEADERS:
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") ইব f:
        content = f.পঠ()
    যদি ("HB_BEGIN_DECLS" ন অন্তঃ content) বা ("HB_END_DECLS" ন অন্তঃ content):
        print(
            "Ouch, file %s does not have HB_BEGIN_DECLS / HB_END_DECLS, but it should"
            % x
        )
        stat = 1

প্রতি x অন্তঃ HBSOURCES:
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") ইব f:
        content = f.পঠ()
    যদি ("HB_BEGIN_DECLS" অন্তঃ content) বা ("HB_END_DECLS" অন্তঃ content):
        print("Ouch, file %s has HB_BEGIN_DECLS / HB_END_DECLS, but it shouldn't" % x)
        stat = 1

sys.exit(stat)
