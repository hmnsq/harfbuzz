#!/usr/bin/env python3

ഇറക്കുമതി os
ഇറക്കുമതി re
ഇറക്കുമതി sys

srcdir = sys.argv[1]
base_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)

HBHEADERS = [os.path.basename(x) വേണ്ടി x ഇൽ os.getenv("HBHEADERS", "").split()] അഥവാ [
    x വേണ്ടി x ഇൽ os.listdir(".") എങ്കിൽ x.startswith("hb") ഒപ്പം x.endswith(".h")
]

stat = 0

print("Checking that all public symbols are exported with HB_EXTERN")
വേണ്ടി x ഇൽ HBHEADERS:
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") ഇവ f:
        content = f.പഠ()
    വേണ്ടി s ഇൽ re.findall(r"\n.+\nhb_.+\n", content):
        എങ്കിൽ അല്ല s.startswith("\nHB_EXTERN "):
            print("failure on:", s)
            stat = 1

sys.exit(stat)
