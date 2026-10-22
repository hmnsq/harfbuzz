#!/usr/bin/env python3

आयात os
आयात sys

srcdir = sys.argv[1]
base_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)


def removeprefix(s):
    abs_path = os.path.join(base_srcdir, s)
    परत os.path.relpath(abs_path, srcdir)


HBHEADERS = [os.path.basename(x) साठी x मध्ये os.getenv("HBHEADERS", "").split()] किंवा [
    x साठी x मध्ये os.listdir(".") जर x.startswith("hb") आणि x.endswith(".h")
]
HBHEADERS = [x साठी x मध्ये HBHEADERS जर x.endswith(".h")]
HBSOURCES = [removeprefix(x) साठी x मध्ये os.getenv("HBSOURCES", "").split()] किंवा [
    x साठी x मध्ये os.listdir(".") जर x.startswith("hb") आणि x.endswith((".cc", ".hh"))
]
stat = 0

साठी x मध्ये HBHEADERS:
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") इव f:
        content = f.पठ()
    जर ("HB_BEGIN_DECLS" नाही मध्ये content) किंवा ("HB_END_DECLS" नाही मध्ये content):
        print(
            "Ouch, file %s does not have HB_BEGIN_DECLS / HB_END_DECLS, but it should"
            % x
        )
        stat = 1

साठी x मध्ये HBSOURCES:
    print(f"Checking {x}")
    with open(x, "r", encoding="utf-8") इव f:
        content = f.पठ()
    जर ("HB_BEGIN_DECLS" मध्ये content) किंवा ("HB_END_DECLS" मध्ये content):
        print("Ouch, file %s has HB_BEGIN_DECLS / HB_END_DECLS, but it shouldn't" % x)
        stat = 1

sys.exit(stat)
