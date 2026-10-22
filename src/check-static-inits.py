#!/usr/bin/env python3

ਆਯਾਤ glob
ਆਯਾਤ os
ਆਯਾਤ re
ਆਯਾਤ shutil
ਆਯਾਤ subprocess
ਆਯਾਤ sys

srcdir = sys.argv[1]
ਅਧਾਰ_srcdir = sys.argv[2]
builddir = sys.argv[3]

objdump = os.getenv("OBJDUMP", shutil.which("objdump"))
ਜੇ ਨਹੀਂ objdump:
    ਛਾਪੋ("check-static-inits.py: 'ldd' not found; skipping test")
    sys.exit(77)

ਜੇ sys.ਸੰਸਕਰਣ_info < (3, 5):
    ਛਾਪੋ("check-static-inits.py: needs python 3.5 for recursive support in glob")
    sys.exit(77)

OBJS = glob.glob(os.ਰਸਤਾ.join(builddir, "**", "*hb*.o"), recursive=ਸੱਚ)
ਜੇ ਨਹੀਂ OBJS:
    ਛਾਪੋ("check-static-inits.py: object files not found; skipping test")
    sys.exit(77)

stat = 0
tested = ਝੂਠ

ਛਾਪੋ("Checking that the following object files has no static initializers/finalizers")
ਛਾਪੋ("\n".join(OBJS))

ਨਤੀਜਾ = subprocess.ਲੜੀਵਾਰ(
    objdump.ਵੰਡੋ() + ["-t"] + OBJS, stdout=subprocess.PIPE, stderr=subprocess.PIPE
)

ਜੇ ਨਤੀਜਾ.returncode:
    ਜੇ ਨਤੀਜਾ.stderr.ਲੱਭੋ(b"not recognized") != -1:
        # https://github.com/harfbuzz/harfbuzz/issues/3019
        ਛਾਪੋ('objdump returned "not recognized", skipping')
    ਨਹੀਂਤਾਂ:
        ਛਾਪੋ("objdump returned error:\n%s" % (ਨਤੀਜਾ.stderr.decode("utf-8")))
        stat = 2
ਨਹੀਂਤਾਂ:
    tested = ਸੱਚ

ਨਤੀਜਾ = ਨਤੀਜਾ.stdout.decode("utf-8")

# Checking that no object file has static initializers
ਲਈ lib ਵਿੱਚ re.findall(r"^.*\.[cd]tors.*$", ਨਤੀਜਾ, re.MULTILINE):
    ਜੇ ਨਹੀਂ re.ਮੇਲ(r".*\b0+\b", lib):
        ਛਾਪੋ("Ouch, library has static initializers/finalizers")
        stat = 1

# Checking that no object file has lazy static C++ constructors/destructors or other such stuff
ਜੇ ("__cxa_" ਵਿੱਚ ਨਤੀਜਾ) ਅਤੇ ("__ubsan_handle" ਨਹੀਂ ਵਿੱਚ ਨਤੀਜਾ):
    ਛਾਪੋ(
        "Ouch, library has lazy static C++ constructors/destructors or other such stuff"
    )
    stat = 1


sys.exit(stat ਜੇ tested ਨਹੀਂਤਾਂ 77)
