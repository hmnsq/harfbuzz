#!/usr/bin/env python3

استورد os
استورد sys

srcdir = sys.argv[1]
base_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)


def removeprefix(s):
    abs_path = os.path.join(base_srcdir, s)
    החזר os.path.relpath(abs_path, srcdir)


HBHEADERS = [os.path.basename(x) עבור x في os.getenv("HBHEADERS", "").split()] או [
    x עבור x في os.listdir(".") אם x.startswith("hb") וגם x.endswith(".h")
]
HBHEADERS = [x עבור x في HBHEADERS אם x.endswith(".h")]
HBSOURCES = [removeprefix(x) עבור x في os.getenv("HBSOURCES", "").split()] או [
    x עבור x في os.listdir(".") אם x.startswith("hb") וגם x.endswith((".cc", ".hh"))
]
stat = 0

עבור x في HBHEADERS:
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") كـ f:
        content = f.اقرأ()
    אם ("HB_BEGIN_DECLS" לא في content) או ("HB_END_DECLS" לא في content):
        print(
            "Ouch, file %s does not have HB_BEGIN_DECLS / HB_END_DECLS, but it should"
            % x
        )
        stat = 1

עבור x في HBSOURCES:
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") كـ f:
        content = f.اقرأ()
    אם ("HB_BEGIN_DECLS" في content) או ("HB_END_DECLS" في content):
        print("Ouch, file %s has HB_BEGIN_DECLS / HB_END_DECLS, but it shouldn't" % x)
        stat = 1

sys.exit(stat)
