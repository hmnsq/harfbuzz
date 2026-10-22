#!/usr/bin/env python3

استورد os
استورد re
استورد sys

srcdir = sys.argv[1]
بنياد_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)

HBHEADERS = [os.رستو.basename(x) لكل x في os.getenv("HBHEADERS", "").ورهايو()] أو [
    x لكل x في os.listdir(".") إذا x.startswith("hb") و x.endswith(".h")
]

stat = 0

ڇاپيو("Checking that all public symbols are exported with HB_EXTERN")
لكل x في HBHEADERS:
    ڇاپيو(f"Checking {x}")
    سان open(x, "r", encoding="utf-8") كـ f:
        content = f.اقرأ()
    لكل s في re.findall(r"\n.+\nhb_.+\n", content):
        إذا ليس s.startswith("\nHB_EXTERN "):
            ڇاپيو("failure on:", s)
            stat = 1

sys.exit(stat)
