#!/usr/bin/env python3

استورد os
استورد sys

srcdir = sys.argv[1]
base_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)


def removeprefix(s):
    abs_path = os.path.join(base_srcdir, s)
    ܦܢܐ os.path.relpath(abs_path, srcdir)


HBHEADERS = [os.path.basename(x) لكل x في os.getenv("HBHEADERS", "").split()] ܐܘ [
    x لكل x في os.listdir(".") ܐܢ x.startswith("hb") ܘ x.endswith(".h")
]
HBHEADERS = [x لكل x في HBHEADERS ܐܢ x.endswith(".h")]
HBSOURCES = [removeprefix(x) لكل x في os.getenv("HBSOURCES", "").split()] ܐܘ [
    x لكل x في os.listdir(".") ܐܢ x.startswith("hb") ܘ x.endswith((".cc", ".hh"))
]
stat = 0

لكل x في HBHEADERS:
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") كـ f:
        content = f.اقرأ()
    ܐܢ ("HB_BEGIN_DECLS" ܠܐ في content) ܐܘ ("HB_END_DECLS" ܠܐ في content):
        print(
            "Ouch, file %s does not have HB_BEGIN_DECLS / HB_END_DECLS, but it should"
            % x
        )
        stat = 1

لكل x في HBSOURCES:
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") كـ f:
        content = f.اقرأ()
    ܐܢ ("HB_BEGIN_DECLS" في content) ܐܘ ("HB_END_DECLS" في content):
        print("Ouch, file %s has HB_BEGIN_DECLS / HB_END_DECLS, but it shouldn't" % x)
        stat = 1

sys.exit(stat)
