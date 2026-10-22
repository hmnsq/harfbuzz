#!/usr/bin/env python3

درآمد os
درآمد re
درآمد sys

srcdir = sys.argv[1]
بنیاد_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)

HBHEADERS = [os.راستہ.basename(x) برائے x میں os.getenv("HBHEADERS", "").بانٹو()] یا [
    x برائے x میں os.listdir(".") اگر x.startswith("hb") اور x.endswith(".h")
]

stat = 0

چھاپو("Checking that all public symbols are exported with HB_EXTERN")
برائے x میں HBHEADERS:
    چھاپو(f"Checking {x}")
    ساتھ open(x, "r", encoding="utf-8") بطور f:
        content = f.اقرأ()
    برائے s میں re.findall(r"\n.+\nhb_.+\n", content):
        اگر نہیں s.startswith("\nHB_EXTERN "):
            چھاپو("failure on:", s)
            stat = 1

sys.exit(stat)
