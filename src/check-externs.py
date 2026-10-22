#!/usr/bin/env python3

आयात os
आयात re
आयात sys

srcdir = sys.argv[1]
base_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)

HBHEADERS = [os.path.basename(x) क्रम x in os.getenv("HBHEADERS", "").split()] or [
    x क्रम x in os.listdir(".") अगर x.startswith("hb") and x.endswith(".h")
]

stat = 0

print("Checking that all public symbols are exported with HB_EXTERN")
क्रम x in HBHEADERS:
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") as f:
        content = f.म_पढ़ो()
    क्रम s in re.findall(r"\n.+\nhb_.+\n", content):
        अगर not s.startswith("\nHB_EXTERN "):
            print("failure on:", s)
            stat = 1

sys.exit(stat)
