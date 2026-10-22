#!/usr/bin/env python3

దిగుమతి glob
దిగుమతి os
దిగుమతి re
దిగుమతి shutil
దిగుమతి subprocess
దిగుమతి sys

srcdir = sys.argv[1]
పునాది_srcdir = sys.argv[2]
builddir = sys.argv[3]

objdump = os.getenv("OBJDUMP", shutil.which("objdump"))
ఒకవేళ కాదు objdump:
    ముద్రించు("check-static-inits.py: 'ldd' not found; skipping test")
    sys.exit(77)

ఒకవేళ sys.సంస్కరణ_info < (3, 5):
    ముద్రించు("check-static-inits.py: needs python 3.5 for recursive support in glob")
    sys.exit(77)

OBJS = glob.glob(os.మార్గం.join(builddir, "**", "*hb*.o"), recursive=నిజం)
ఒకవేళ కాదు OBJS:
    ముద్రించు("check-static-inits.py: object files not found; skipping test")
    sys.exit(77)

stat = 0
tested = అబద్ధం

ముద్రించు("Checking that the following object files has no static initializers/finalizers")
ముద్రించు("\n".join(OBJS))

ఫలితం = subprocess.నిరంతరం(
    objdump.విభజించు() + ["-t"] + OBJS, stdout=subprocess.PIPE, stderr=subprocess.PIPE
)

ఒకవేళ ఫలితం.returncode:
    ఒకవేళ ఫలితం.stderr.కనుగొను(b"not recognized") != -1:
        # https://github.com/harfbuzz/harfbuzz/issues/3019
        ముద్రించు('objdump returned "not recognized", skipping')
    లేకపోతే:
        ముద్రించు("objdump returned error:\n%s" % (ఫలితం.stderr.decode("utf-8")))
        stat = 2
లేకపోతే:
    tested = నిజం

ఫలితం = ఫలితం.stdout.decode("utf-8")

# Checking that no object file has static initializers
కొరకు lib లో re.findall(r"^.*\.[cd]tors.*$", ఫలితం, re.MULTILINE):
    ఒకవేళ కాదు re.సరిపోలిక(r".*\b0+\b", lib):
        ముద్రించు("Ouch, library has static initializers/finalizers")
        stat = 1

# Checking that no object file has lazy static C++ constructors/destructors or other such stuff
ఒకవేళ ("__cxa_" లో ఫలితం) మరియు ("__ubsan_handle" కాదు లో ఫలితం):
    ముద్రించు(
        "Ouch, library has lazy static C++ constructors/destructors or other such stuff"
    )
    stat = 1


sys.exit(stat ఒకవేళ tested లేకపోతే 77)
