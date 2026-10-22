#!/usr/bin/env python3

"""Generates the code for a sorted unicode range array as used in hb-ot-os2-unicode-ranges.hh
Input is a tab separated list of unicode ranges from the otspec
(https://docs.microsoft.com/en-us/typography/opentype/spec/os2#ur).
"""

استورد re
استورد sys


ڇاپيو ("""static OS2Range _hb_os2_unicode_ranges[] =
{""")

دليلون = sys.argv[1:]
داخلا_file = دليلون[0]

سان open (داخلا_file, گهڻائي="r", encoding="utf-8") كـ f:

  all_ranges = []
  current_ٻٽو_انگ = 0
  طالما سچ:
    سٽ = f.readline().strip()
    إذا ليس سٽ:
      اكسر
    fields = re.ورهايو(r'\t+', سٽ)
    إذا ڊيگهه(fields) == 3:
      current_ٻٽو_انگ = fields[0]
      fields = fields[1:]
    وإلاإذا ڊيگهه(fields) > 3:
      ارفع Exception("bad input :(.")

    name = fields[0]
    ranges = re.ورهايو("-", fields[1])
    إذا ڊيگهه(ranges) != 2:
      ارفع Exception("bad input :(.")

    v = tuple((صحيح(ranges[0], 16), صحيح(ranges[1], 16), صحيح(current_ٻٽو_انگ), name))
    all_ranges.append(v)

all_ranges = sorted(all_ranges, ڪنجي_2=مجهول t: t[0])

لكل ranges في all_ranges:
  شروعات = ("0x%X" % ranges[0]).rjust(8)
  پڇاڙي = ("0x%X" % ranges[1]).rjust(8)
  ٻٽو_انگ = ("%s" % ranges[2]).rjust(3)

  ڇاپيو ("  {%s, %s, %s}, // %s" % (شروعات, پڇاڙي, ٻٽو_انگ, ranges[3]))

ڇاپيو ("""};""")
