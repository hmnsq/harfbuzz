#!/usr/bin/env python3

"""Generates the code for a sorted unicode range array as used in hb-ot-os2-unicode-ranges.hh
Input is a tab separated list of unicode ranges from the otspec
(https://docs.microsoft.com/en-us/typography/opentype/spec/os2#ur).
"""

దిగుమతి re
దిగుమతి sys


ముద్రించు ("""static OS2Range _hb_os2_unicode_ranges[] =
{""")

వాదనలు = sys.argv[1:]
ఆదానం_file = వాదనలు[0]

తోడుగా open (ఆదానం_file, బహుళకం="r", encoding="utf-8") ఇవ f:

  all_ranges = []
  current_బిట్టు = 0
  అయితే నిజం:
    పంక్తి = f.readline().strip()
    ఒకవేళ కాదు పంక్తి:
      ఆపు
    fields = re.విభజించు(r'\t+', పంక్తి)
    ఒకవేళ పొడవు(fields) == 3:
      current_బిట్టు = fields[0]
      fields = fields[1:]
    లేదావేళ పొడవు(fields) > 3:
      లేపు Exception("bad input :(.")

    name = fields[0]
    ranges = re.విభజించు("-", fields[1])
    ఒకవేళ పొడవు(ranges) != 2:
      లేపు Exception("bad input :(.")

    v = tuple((పూర్ణాంకం(ranges[0], 16), పూర్ణాంకం(ranges[1], 16), పూర్ణాంకం(current_బిట్టు), name))
    all_ranges.append(v)

all_ranges = sorted(all_ranges, కీలకం_2=అజ్ఞాత t: t[0])

కొరకు ranges లో all_ranges:
  ప్రారంభం = ("0x%X" % ranges[0]).rjust(8)
  చివర = ("0x%X" % ranges[1]).rjust(8)
  బిట్టు = ("%s" % ranges[2]).rjust(3)

  ముద్రించు ("  {%s, %s, %s}, // %s" % (ప్రారంభం, చివర, బిట్టు, ranges[3]))

ముద్రించు ("""};""")
