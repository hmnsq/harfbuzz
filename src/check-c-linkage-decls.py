#!/usr/bin/env python3

استورد os
استورد sys

srcdir = sys.argv[1]
base_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)


def removeprefix(s):
    abs_path = os.path.join(base_srcdir, s)
    أرجع os.path.relpath(abs_path, srcdir)


HBHEADERS = [os.path.basename(x) لكل x في os.getenv("HBHEADERS", "").split()] 𐡀𐡅 [
    x لكل x في os.listdir(".") 𐡄𐡍 x.startswith("hb") 𐡅 x.endswith(".h")
]
HBHEADERS = [x لكل x في HBHEADERS 𐡄𐡍 x.endswith(".h")]
HBSOURCES = [removeprefix(x) لكل x في os.getenv("HBSOURCES", "").split()] 𐡀𐡅 [
    x لكل x في os.listdir(".") 𐡄𐡍 x.startswith("hb") 𐡅 x.endswith((".cc", ".hh"))
]
stat = 0

لكل x في HBHEADERS:
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") كـ f:
        content = f.اقرأ()
    𐡄𐡍 ("HB_BEGIN_DECLS" 𐡋𐡀 في content) 𐡀𐡅 ("HB_END_DECLS" 𐡋𐡀 في content):
        print(
            "Ouch, file %s does not have HB_BEGIN_DECLS / HB_END_DECLS, but it should"
            % x
        )
        stat = 1

لكل x في HBSOURCES:
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") كـ f:
        content = f.اقرأ()
    𐡄𐡍 ("HB_BEGIN_DECLS" في content) 𐡀𐡅 ("HB_END_DECLS" في content):
        print("Ouch, file %s has HB_BEGIN_DECLS / HB_END_DECLS, but it shouldn't" % x)
        stat = 1

sys.exit(stat)
