#!/usr/bin/env python3

ਆਯਾਤ os
ਆਯਾਤ re
ਆਯਾਤ sys

srcdir = sys.argv[1]
ਅਧਾਰ_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)


ਪਰਿਭਾਸ਼ਾ removeprefix(s):
    abs_ਰਸਤਾ = os.ਰਸਤਾ.join(ਅਧਾਰ_srcdir, s)
    ਵਾਪਸ os.ਰਸਤਾ.relpath(abs_ਰਸਤਾ, srcdir)


HBHEADERS = [os.ਰਸਤਾ.basename(x) ਲਈ x ਵਿੱਚ os.getenv("HBHEADERS", "").ਵੰਡੋ()] ਜਾਂ [
    x ਲਈ x ਵਿੱਚ os.listdir(".") ਜੇ x.startswith("hb") ਅਤੇ x.endswith(".h")
]

HBSOURCES = [removeprefix(x) ਲਈ x ਵਿੱਚ os.getenv("HBSOURCES", "").ਵੰਡੋ()] ਜਾਂ [
    x ਲਈ x ਵਿੱਚ os.listdir(".") ਜੇ x.startswith("hb") ਅਤੇ x.endswith((".cc", ".hh"))
]


stat = 0

ਛਾਪੋ(
    'Checking that public header files #include "hb-common.h" or "hb.h" first (or none)'
)
ਲਈ x ਵਿੱਚ HBHEADERS:
    ਜੇ x == "hb.h" ਜਾਂ x == "hb-common.h":
        ਜਾਰੀ
    ਛਾਪੋ(f"Checking {x}")
    ਨਾਲ open(x, "r", encoding="utf-8") ਇਵ f:
        content = f.ਪਠ()
    ਪਹਿਲਾ = re.findall(r"#.*include.*", content)[0]
    ਜੇ ਪਹਿਲਾ ਨਹੀਂ ਵਿੱਚ ['#include "hb.h"', '#include "hb-common.h"']:
        ਛਾਪੋ("failure on %s" % x)
        stat = 1

ਛਾਪੋ("Checking that source files #include a private header first (or none)")
ਲਈ x ਵਿੱਚ HBSOURCES:
    ਛਾਪੋ(f"Checking {x}")
    ਨਾਲ open(x, "r", encoding="utf-8") ਇਵ f:
        content = f.ਪਠ()
    includes = re.findall(r"#.*include.*", content)
    ਜੇ includes:
        ਜੇ ਨਹੀਂ ਲੰਬਾਈ(re.findall(r'".*\.hh"', includes[0])):
            ਛਾਪੋ("failure on %s" % x)
            stat = 1

ਛਾਪੋ("Checking that there is no #include <hb-*.h>")
ਲਈ x ਵਿੱਚ HBHEADERS + HBSOURCES:
    ਛਾਪੋ(f"Checking {x}")
    ਨਾਲ open(x, "r", encoding="utf-8") ਇਵ f:
        content = f.ਪਠ()
    ਜੇ re.findall("#.*include.*<.*hb", content):
        ਛਾਪੋ("failure on %s" % x)
        stat = 1

sys.exit(stat)
