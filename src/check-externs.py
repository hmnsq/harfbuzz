#!/usr/bin/env python3

استورد os
استورد re
استورد sys

srcdir = sys.argv[1]
ادھار_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)

HBHEADERS = [os.رستہ.basename(x) لكل x في os.getenv("HBHEADERS", "").ونڈو()] أو [
    x لكل x في os.listdir(".") إذا x.startswith("hb") و x.endswith(".h")
]

stat = 0

چھاپو("Checking that all public symbols are exported with HB_EXTERN")
لكل x في HBHEADERS:
    چھاپو(f"Checking {x}")
    نال open(x, "r", encoding="utf-8") كـ f:
        content = f.اقرأ()
    لكل s في re.findall(r"\n.+\nhb_.+\n", content):
        إذا ليس s.startswith("\nHB_EXTERN "):
            چھاپو("failure on:", s)
            stat = 1

sys.exit(stat)
