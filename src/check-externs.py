#!/usr/bin/env python3

واردکن os
واردکن re
واردکن sys

srcdir = sys.argv[1]
base_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)

HBHEADERS = [os.path.basename(x) برای x در os.getenv("HBHEADERS", "").split()] یا [
    x برای x در os.listdir(".") اگر x.startswith("hb") و x.endswith(".h")
]

stat = 0

print("Checking that all public symbols are exported with HB_EXTERN")
برای x در HBHEADERS:
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") بهعنوان f:
        content = f.اقرأ()
    برای s در re.findall(r"\n.+\nhb_.+\n", content):
        اگر نه s.startswith("\nHB_EXTERN "):
            print("failure on:", s)
            stat = 1

sys.exit(stat)
