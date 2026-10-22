#!/usr/bin/env python3

ਆਯਾਤ os
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
HBHEADERS = [x ਲਈ x ਵਿੱਚ HBHEADERS ਜੇ x.endswith(".h")]
HBSOURCES = [removeprefix(x) ਲਈ x ਵਿੱਚ os.getenv("HBSOURCES", "").ਵੰਡੋ()] ਜਾਂ [
    x ਲਈ x ਵਿੱਚ os.listdir(".") ਜੇ x.startswith("hb") ਅਤੇ x.endswith((".cc", ".hh"))
]
stat = 0

ਲਈ x ਵਿੱਚ HBHEADERS:
    ਛਾਪੋ(f"Checking {x}")
    ਨਾਲ open(x, "r", encoding="utf-8") ਇਵ f:
        content = f.ਪਠ()
    ਜੇ ("HB_BEGIN_DECLS" ਨਹੀਂ ਵਿੱਚ content) ਜਾਂ ("HB_END_DECLS" ਨਹੀਂ ਵਿੱਚ content):
        ਛਾਪੋ(
            "Ouch, file %s does not have HB_BEGIN_DECLS / HB_END_DECLS, but it should"
            % x
        )
        stat = 1

ਲਈ x ਵਿੱਚ HBSOURCES:
    ਛਾਪੋ(f"Checking {x}")
    ਨਾਲ open(x, "r", encoding="utf-8") ਇਵ f:
        content = f.ਪਠ()
    ਜੇ ("HB_BEGIN_DECLS" ਵਿੱਚ content) ਜਾਂ ("HB_END_DECLS" ਵਿੱਚ content):
        ਛਾਪੋ("Ouch, file %s has HB_BEGIN_DECLS / HB_END_DECLS, but it shouldn't" % x)
        stat = 1

sys.exit(stat)
