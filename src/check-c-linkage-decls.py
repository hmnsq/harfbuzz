#!/usr/bin/env python3

ଆମଦାନୀ os
ଆମଦାନୀ sys

srcdir = sys.argv[1]
base_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)


def removeprefix(s):
    abs_path = os.path.join(base_srcdir, s)
    ଫେରନ୍ତୁ os.path.relpath(abs_path, srcdir)


HBHEADERS = [os.path.basename(x) ପାଇଁ x ଭିତରେ os.getenv("HBHEADERS", "").split()] କିମ୍ବା [
    x ପାଇଁ x ଭିତରେ os.listdir(".") ଯଦି x.startswith("hb") ଏବଂ x.endswith(".h")
]
HBHEADERS = [x ପାଇଁ x ଭିତରେ HBHEADERS ଯଦି x.endswith(".h")]
HBSOURCES = [removeprefix(x) ପାଇଁ x ଭିତରେ os.getenv("HBSOURCES", "").split()] କିମ୍ବା [
    x ପାଇଁ x ଭିତରେ os.listdir(".") ଯଦି x.startswith("hb") ଏବଂ x.endswith((".cc", ".hh"))
]
stat = 0

ପାଇଁ x ଭିତରେ HBHEADERS:
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") ଇଵ f:
        content = f.ପଠ()
    ଯଦି ("HB_BEGIN_DECLS" ନୁହେଁ ଭିତରେ content) କିମ୍ବା ("HB_END_DECLS" ନୁହେଁ ଭିତରେ content):
        print(
            "Ouch, file %s does not have HB_BEGIN_DECLS / HB_END_DECLS, but it should"
            % x
        )
        stat = 1

ପାଇଁ x ଭିତରେ HBSOURCES:
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") ଇଵ f:
        content = f.ପଠ()
    ଯଦି ("HB_BEGIN_DECLS" ଭିତରେ content) କିମ୍ବା ("HB_END_DECLS" ଭିତରେ content):
        print("Ouch, file %s has HB_BEGIN_DECLS / HB_END_DECLS, but it shouldn't" % x)
        stat = 1

sys.exit(stat)
