#!/usr/bin/env python3

استورد os
استورد re
استورد sys

srcdir = sys.argv[1]
base_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)

HBHEADERS = [os.path.basename(x) עבור x في os.getenv("HBHEADERS", "").split()] או [
    x עבור x في os.listdir(".") אם x.startswith("hb") וגם x.endswith(".h")
]

stat = 0

print("Checking that all public symbols are exported with HB_EXTERN")
עבור x في HBHEADERS:
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") كـ f:
        content = f.اقرأ()
    עבור s في re.findall(r"\n.+\nhb_.+\n", content):
        אם לא s.startswith("\nHB_EXTERN "):
            print("failure on:", s)
            stat = 1

sys.exit(stat)
