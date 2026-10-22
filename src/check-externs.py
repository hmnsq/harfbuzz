#!/usr/bin/env python3

আনযন os
আনযন re
আনযন sys

srcdir = sys.argv[1]
base_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)

HBHEADERS = [os.path.basename(x) প্রতি x অন্তঃ os.getenv("HBHEADERS", "").split()] বা [
    x প্রতি x অন্তঃ os.listdir(".") যদি x.startswith("hb") চ x.endswith(".h")
]

stat = 0

print("Checking that all public symbols are exported with HB_EXTERN")
প্রতি x অন্তঃ HBHEADERS:
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") ইব f:
        content = f.পঠ()
    প্রতি s অন্তঃ re.findall(r"\n.+\nhb_.+\n", content):
        যদি ন s.startswith("\nHB_EXTERN "):
            print("failure on:", s)
            stat = 1

sys.exit(stat)
