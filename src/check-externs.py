#!/usr/bin/env python3

आयात os
आयात re
आयात sys

srcdir = sys.argv[1]
base_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)

HBHEADERS = [os.path.basename(x) साठी x मध्ये os.getenv("HBHEADERS", "").split()] किंवा [
    x साठी x मध्ये os.listdir(".") जर x.startswith("hb") आणि x.endswith(".h")
]

stat = 0

print("Checking that all public symbols are exported with HB_EXTERN")
साठी x मध्ये HBHEADERS:
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") इव f:
        content = f.पठ()
    साठी s मध्ये re.findall(r"\n.+\nhb_.+\n", content):
        जर नाही s.startswith("\nHB_EXTERN "):
            print("failure on:", s)
            stat = 1

sys.exit(stat)
