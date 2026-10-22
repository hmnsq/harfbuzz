#!/usr/bin/env python3

ഇറക്കുമതി os
ഇറക്കുമതി sys

srcdir = sys.argv[1]
base_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)


def removeprefix(s):
    abs_path = os.path.join(base_srcdir, s)
    തിരികെ os.path.relpath(abs_path, srcdir)


HBHEADERS = [os.path.basename(x) വേണ്ടി x ഇൽ os.getenv("HBHEADERS", "").split()] അഥവാ [
    x വേണ്ടി x ഇൽ os.listdir(".") എങ്കിൽ x.startswith("hb") ഒപ്പം x.endswith(".h")
]
HBHEADERS = [x വേണ്ടി x ഇൽ HBHEADERS എങ്കിൽ x.endswith(".h")]
HBSOURCES = [removeprefix(x) വേണ്ടി x ഇൽ os.getenv("HBSOURCES", "").split()] അഥവാ [
    x വേണ്ടി x ഇൽ os.listdir(".") എങ്കിൽ x.startswith("hb") ഒപ്പം x.endswith((".cc", ".hh"))
]
stat = 0

വേണ്ടി x ഇൽ HBHEADERS:
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") ഇവ f:
        content = f.പഠ()
    എങ്കിൽ ("HB_BEGIN_DECLS" അല്ല ഇൽ content) അഥവാ ("HB_END_DECLS" അല്ല ഇൽ content):
        print(
            "Ouch, file %s does not have HB_BEGIN_DECLS / HB_END_DECLS, but it should"
            % x
        )
        stat = 1

വേണ്ടി x ഇൽ HBSOURCES:
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") ഇവ f:
        content = f.പഠ()
    എങ്കിൽ ("HB_BEGIN_DECLS" ഇൽ content) അഥവാ ("HB_END_DECLS" ഇൽ content):
        print("Ouch, file %s has HB_BEGIN_DECLS / HB_END_DECLS, but it shouldn't" % x)
        stat = 1

sys.exit(stat)
