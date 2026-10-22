#!/usr/bin/env python3

আমদানি os
আমদানি re
আমদানি sys

srcdir = sys.argv[1]
base_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)

HBHEADERS = [os.path.basename(x) জন্য x মধ্যে os.getenv("HBHEADERS", "").split()] অথবা [
    x জন্য x মধ্যে os.listdir(".") যদি x.startswith("hb") এবং x.endswith(".h")
]

stat = 0

print("Checking that all public symbols are exported with HB_EXTERN")
জন্য x মধ্যে HBHEADERS:
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") ইব f:
        content = f.পঠ()
    জন্য s মধ্যে re.findall(r"\n.+\nhb_.+\n", content):
        যদি নয় s.startswith("\nHB_EXTERN "):
            print("failure on:", s)
            stat = 1

sys.exit(stat)
