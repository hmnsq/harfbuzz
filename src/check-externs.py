#!/usr/bin/env python3

आनयन os
आनयन re
आनयन sys

srcdir = sys.argv[1]
base_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)

HBHEADERS = [os.path.basename(x) अत्थाय x अन्तः os.getenv("HBHEADERS", "").split()] वा [
    x अत्थाय x अन्तः os.listdir(".") सचे x.startswith("hb") च x.endswith(".h")
]

stat = 0

print("Checking that all public symbols are exported with HB_EXTERN")
अत्थाय x अन्तः HBHEADERS:
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") इव f:
        content = f.पठ()
    अत्थाय s अन्तः re.findall(r"\n.+\nhb_.+\n", content):
        सचे न s.startswith("\nHB_EXTERN "):
            print("failure on:", s)
            stat = 1

sys.exit(stat)
