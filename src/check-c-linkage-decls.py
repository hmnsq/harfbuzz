#!/usr/bin/env python3

आनयन os
आनयन sys

srcdir = sys.argv[1]
base_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)


def removeprefix(s):
    abs_path = os.path.join(base_srcdir, s)
    प्रत्यागम os.path.relpath(abs_path, srcdir)


HBHEADERS = [os.path.basename(x) प्रति x अन्तः os.getenv("HBHEADERS", "").split()] वा [
    x प्रति x अन्तः os.listdir(".") यदि x.startswith("hb") च x.endswith(".h")
]
HBHEADERS = [x प्रति x अन्तः HBHEADERS यदि x.endswith(".h")]
HBSOURCES = [removeprefix(x) प्रति x अन्तः os.getenv("HBSOURCES", "").split()] वा [
    x प्रति x अन्तः os.listdir(".") यदि x.startswith("hb") च x.endswith((".cc", ".hh"))
]
stat = 0

प्रति x अन्तः HBHEADERS:
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") इव f:
        content = f.पठ()
    यदि ("HB_BEGIN_DECLS" न अन्तः content) वा ("HB_END_DECLS" न अन्तः content):
        print(
            "Ouch, file %s does not have HB_BEGIN_DECLS / HB_END_DECLS, but it should"
            % x
        )
        stat = 1

प्रति x अन्तः HBSOURCES:
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") इव f:
        content = f.पठ()
    यदि ("HB_BEGIN_DECLS" अन्तः content) वा ("HB_END_DECLS" अन्तः content):
        print("Ouch, file %s has HB_BEGIN_DECLS / HB_END_DECLS, but it shouldn't" % x)
        stat = 1

sys.exit(stat)
