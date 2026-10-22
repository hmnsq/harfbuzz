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

ਲਈ x ਵਿੱਚ HBHEADERS + HBSOURCES:
    ਜੇ ਨਹੀਂ x.endswith("h") ਜਾਂ x == "hb-gobject-structs.h":
        ਜਾਰੀ
    ਛਾਪੋ(f"Checking {x}")
    ਟੈਗ_2 = (
        x.ਉੱਤਲਾ()
        .replace(".", "_")
        .replace("-", "_")
        .replace(os.ਰਸਤਾ.sep, "_")
        .replace("/", "_")
    )
    ਨਾਲ open(x, "r", encoding="utf-8") ਇਵ f:
        content = f.ਪਠ()
    ਜੇ ਲੰਬਾਈ(re.findall(ਟੈਗ_2 + r"\b", content)) != 3:
        ਛਾਪੋ(
            "Ouch, header file %s does not have correct preprocessor guards. Expected: %s"
            % (x, ਟੈਗ_2)
        )
        stat = 1

sys.exit(stat)
