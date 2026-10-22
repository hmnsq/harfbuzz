#!/usr/bin/env python3

ଆମଦାନୀ os
ଆମଦାନୀ re
ଆମଦାନୀ sys

srcdir = sys.argv[1]
base_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)

HBHEADERS = [os.path.basename(x) ପାଇଁ x ଭିତରେ os.getenv("HBHEADERS", "").split()] କିମ୍ବା [
    x ପାଇଁ x ଭିତରେ os.listdir(".") ଯଦି x.startswith("hb") ଏବଂ x.endswith(".h")
]

stat = 0

print("Checking that all public symbols are exported with HB_EXTERN")
ପାଇଁ x ଭିତରେ HBHEADERS:
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") ଇଵ f:
        content = f.ପଠ()
    ପାଇଁ s ଭିତରେ re.findall(r"\n.+\nhb_.+\n", content):
        ଯଦି ନୁହେଁ s.startswith("\nHB_EXTERN "):
            print("failure on:", s)
            stat = 1

sys.exit(stat)
