#!/usr/bin/env python3

আমদানি os
আমদানি sys

srcdir = sys.argv[1]
base_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)


def removeprefix(s):
    abs_path = os.path.join(base_srcdir, s)
    ফেরত os.path.relpath(abs_path, srcdir)


HBHEADERS = [os.path.basename(x) জন্য x মধ্যে os.getenv("HBHEADERS", "").split()] অথবা [
    x জন্য x মধ্যে os.listdir(".") যদি x.startswith("hb") এবং x.endswith(".h")
]
HBHEADERS = [x জন্য x মধ্যে HBHEADERS যদি x.endswith(".h")]
HBSOURCES = [removeprefix(x) জন্য x মধ্যে os.getenv("HBSOURCES", "").split()] অথবা [
    x জন্য x মধ্যে os.listdir(".") যদি x.startswith("hb") এবং x.endswith((".cc", ".hh"))
]
stat = 0

জন্য x মধ্যে HBHEADERS:
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") ইব f:
        content = f.পঠ()
    যদি ("HB_BEGIN_DECLS" নয় মধ্যে content) অথবা ("HB_END_DECLS" নয় মধ্যে content):
        print(
            "Ouch, file %s does not have HB_BEGIN_DECLS / HB_END_DECLS, but it should"
            % x
        )
        stat = 1

জন্য x মধ্যে HBSOURCES:
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") ইব f:
        content = f.পঠ()
    যদি ("HB_BEGIN_DECLS" মধ্যে content) অথবা ("HB_END_DECLS" মধ্যে content):
        print("Ouch, file %s has HB_BEGIN_DECLS / HB_END_DECLS, but it shouldn't" % x)
        stat = 1

sys.exit(stat)
