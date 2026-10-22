#!/usr/bin/env python3

இறக்குமதி os
இறக்குமதி re
இறக்குமதி sys

srcdir = sys.argv[1]
base_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)

HBHEADERS = [os.path.basename(x) க்கு x உள் os.getenv("HBHEADERS", "").split()] அல்லது [
    x க்கு x உள் os.listdir(".") எனில் x.startswith("hb") மற்றும் x.endswith(".h")
]

stat = 0

print("Checking that all public symbols are exported with HB_EXTERN")
க்கு x உள் HBHEADERS:
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") இவ f:
        content = f.பட()
    க்கு s உள் re.findall(r"\n.+\nhb_.+\n", content):
        எனில் இல்லை s.startswith("\nHB_EXTERN "):
            print("failure on:", s)
            stat = 1

sys.exit(stat)
