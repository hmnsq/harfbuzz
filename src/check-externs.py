#!/usr/bin/env python3

ಆನಯನ os
ಆನಯನ re
ಆನಯನ sys

srcdir = sys.argv[1]
base_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)

HBHEADERS = [os.path.basename(x) ಪ್ರತಿ x ಅನ್ತಃ os.getenv("HBHEADERS", "").split()] ವಾ [
    x ಪ್ರತಿ x ಅನ್ತಃ os.listdir(".") ಯದಿ x.startswith("hb") ಚ x.endswith(".h")
]

stat = 0

print("Checking that all public symbols are exported with HB_EXTERN")
ಪ್ರತಿ x ಅನ್ತಃ HBHEADERS:
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") ಇವ f:
        content = f.ಪಠ()
    ಪ್ರತಿ s ಅನ್ತಃ re.findall(r"\n.+\nhb_.+\n", content):
        ಯದಿ ನ s.startswith("\nHB_EXTERN "):
            print("failure on:", s)
            stat = 1

sys.exit(stat)
