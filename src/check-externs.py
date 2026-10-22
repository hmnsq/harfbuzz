#!/usr/bin/env python3

આયાત os
આયાત re
આયાત sys

srcdir = sys.argv[1]
base_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)

HBHEADERS = [os.path.basename(x) માટે x માં os.getenv("HBHEADERS", "").split()] અથવા [
    x માટે x માં os.listdir(".") જો x.startswith("hb") અને x.endswith(".h")
]

stat = 0

print("Checking that all public symbols are exported with HB_EXTERN")
માટે x માં HBHEADERS:
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") ઇવ f:
        content = f.પઠ()
    માટે s માં re.findall(r"\n.+\nhb_.+\n", content):
        જો નહીં s.startswith("\nHB_EXTERN "):
            print("failure on:", s)
            stat = 1

sys.exit(stat)
