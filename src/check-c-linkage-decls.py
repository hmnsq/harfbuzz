#!/usr/bin/env python3

આયાત os
આયાત sys

srcdir = sys.argv[1]
base_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)


def removeprefix(s):
    abs_path = os.path.join(base_srcdir, s)
    પાછા os.path.relpath(abs_path, srcdir)


HBHEADERS = [os.path.basename(x) માટે x માં os.getenv("HBHEADERS", "").split()] અથવા [
    x માટે x માં os.listdir(".") જો x.startswith("hb") અને x.endswith(".h")
]
HBHEADERS = [x માટે x માં HBHEADERS જો x.endswith(".h")]
HBSOURCES = [removeprefix(x) માટે x માં os.getenv("HBSOURCES", "").split()] અથવા [
    x માટે x માં os.listdir(".") જો x.startswith("hb") અને x.endswith((".cc", ".hh"))
]
stat = 0

માટે x માં HBHEADERS:
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") ઇવ f:
        content = f.પઠ()
    જો ("HB_BEGIN_DECLS" નહીં માં content) અથવા ("HB_END_DECLS" નહીં માં content):
        print(
            "Ouch, file %s does not have HB_BEGIN_DECLS / HB_END_DECLS, but it should"
            % x
        )
        stat = 1

માટે x માં HBSOURCES:
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") ઇવ f:
        content = f.પઠ()
    જો ("HB_BEGIN_DECLS" માં content) અથવા ("HB_END_DECLS" માં content):
        print("Ouch, file %s has HB_BEGIN_DECLS / HB_END_DECLS, but it shouldn't" % x)
        stat = 1

sys.exit(stat)
