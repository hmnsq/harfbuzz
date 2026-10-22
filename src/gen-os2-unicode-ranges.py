#!/usr/bin/env python3

"""Generates the code for a sorted unicode range array as used in hb-ot-os2-unicode-ranges.hh
Input is a tab separated list of unicode ranges from the otspec
(https://docs.microsoft.com/en-us/typography/opentype/spec/os2#ur).
"""

ਆਯਾਤ re
ਆਯਾਤ sys


ਛਾਪੋ ("""static OS2Range _hb_os2_unicode_ranges[] =
{""")

ਦਲੀਲਾਂ = sys.argv[1:]
ਨਿਵੇਸ਼_file = ਦਲੀਲਾਂ[0]

ਨਾਲ open (ਨਿਵੇਸ਼_file, ਬਹੁਲਕ="r", encoding="utf-8") ਇਵ f:

  all_ranges = []
  current_ਦੋਆਧਾਰੀ_ਅੰਕ = 0
  ਜਦੋਂ ਸੱਚ:
    ਸਤਰ = f.readline().strip()
    ਜੇ ਨਹੀਂ ਸਤਰ:
      ਰੋਕੋ
    fields = re.ਵੰਡੋ(r'\t+', ਸਤਰ)
    ਜੇ ਲੰਬਾਈ(fields) == 3:
      current_ਦੋਆਧਾਰੀ_ਅੰਕ = fields[0]
      fields = fields[1:]
    ਜਾਂਜੇ ਲੰਬਾਈ(fields) > 3:
      ਉਠਾਓ Exception("bad input :(.")

    name = fields[0]
    ranges = re.ਵੰਡੋ("-", fields[1])
    ਜੇ ਲੰਬਾਈ(ranges) != 2:
      ਉਠਾਓ Exception("bad input :(.")

    v = tuple((ਪੂਰਨ(ranges[0], 16), ਪੂਰਨ(ranges[1], 16), ਪੂਰਨ(current_ਦੋਆਧਾਰੀ_ਅੰਕ), name))
    all_ranges.append(v)

all_ranges = sorted(all_ranges, ਕੁੰਜੀ_2=ਅਨਾਮ t: t[0])

ਲਈ ranges ਵਿੱਚ all_ranges:
  ਸ਼ੁਰੂ = ("0x%X" % ranges[0]).rjust(8)
  ਸਿਰਾ = ("0x%X" % ranges[1]).rjust(8)
  ਦੋਆਧਾਰੀ_ਅੰਕ = ("%s" % ranges[2]).rjust(3)

  ਛਾਪੋ ("  {%s, %s, %s}, // %s" % (ਸ਼ੁਰੂ, ਸਿਰਾ, ਦੋਆਧਾਰੀ_ਅੰਕ, ranges[3]))

ਛਾਪੋ ("""};""")
