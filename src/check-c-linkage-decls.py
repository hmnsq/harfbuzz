#!/usr/bin/env python3

ಆನಯನ os
ಆನಯನ sys

srcdir = sys.argv[1]
base_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)


def removeprefix(s):
    abs_path = os.path.join(base_srcdir, s)
    ಪ್ರತ್ಯಾಗಮ os.path.relpath(abs_path, srcdir)


HBHEADERS = [os.path.basename(x) ಪ್ರತಿ x ಅನ್ತಃ os.getenv("HBHEADERS", "").split()] ವಾ [
    x ಪ್ರತಿ x ಅನ್ತಃ os.listdir(".") ಯದಿ x.startswith("hb") ಚ x.endswith(".h")
]
HBHEADERS = [x ಪ್ರತಿ x ಅನ್ತಃ HBHEADERS ಯದಿ x.endswith(".h")]
HBSOURCES = [removeprefix(x) ಪ್ರತಿ x ಅನ್ತಃ os.getenv("HBSOURCES", "").split()] ವಾ [
    x ಪ್ರತಿ x ಅನ್ತಃ os.listdir(".") ಯದಿ x.startswith("hb") ಚ x.endswith((".cc", ".hh"))
]
stat = 0

ಪ್ರತಿ x ಅನ್ತಃ HBHEADERS:
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") ಇವ f:
        content = f.ಪಠ()
    ಯದಿ ("HB_BEGIN_DECLS" ನ ಅನ್ತಃ content) ವಾ ("HB_END_DECLS" ನ ಅನ್ತಃ content):
        print(
            "Ouch, file %s does not have HB_BEGIN_DECLS / HB_END_DECLS, but it should"
            % x
        )
        stat = 1

ಪ್ರತಿ x ಅನ್ತಃ HBSOURCES:
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") ಇವ f:
        content = f.ಪಠ()
    ಯದಿ ("HB_BEGIN_DECLS" ಅನ್ತಃ content) ವಾ ("HB_END_DECLS" ಅನ್ತಃ content):
        print("Ouch, file %s has HB_BEGIN_DECLS / HB_END_DECLS, but it shouldn't" % x)
        stat = 1

sys.exit(stat)
