#!/usr/bin/env python3

ਆਯਾਤ os
ਆਯਾਤ re
ਆਯਾਤ sys

srcdir = sys.argv[1]
ਅਧਾਰ_srcdir = sys.argv[2]
builddir = sys.argv[3]

os.chdir(srcdir)

HBHEADERS = [os.ਰਸਤਾ.basename(x) ਲਈ x ਵਿੱਚ os.getenv("HBHEADERS", "").ਵੰਡੋ()] ਜਾਂ [
    x ਲਈ x ਵਿੱਚ os.listdir(".") ਜੇ x.startswith("hb") ਅਤੇ x.endswith(".h")
]

stat = 0

ਛਾਪੋ("Checking that all public symbols are exported with HB_EXTERN")
ਲਈ x ਵਿੱਚ HBHEADERS:
    ਛਾਪੋ(f"Checking {x}")
    ਨਾਲ open(x, "r", encoding="utf-8") ਇਵ f:
        content = f.ਪਠ()
    ਲਈ s ਵਿੱਚ re.findall(r"\n.+\nhb_.+\n", content):
        ਜੇ ਨਹੀਂ s.startswith("\nHB_EXTERN "):
            ਛਾਪੋ("failure on:", s)
            stat = 1

sys.exit(stat)
