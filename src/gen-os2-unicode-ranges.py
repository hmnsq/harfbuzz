#!/usr/bin/env python3

"""Generates the code for a sorted unicode range array as used in hb-ot-os2-unicode-ranges.hh
Input is a tab separated list of unicode ranges from the otspec
(https://docs.microsoft.com/en-us/typography/opentype/spec/os2#ur).
"""

استورد re
استورد sys


چاپ ("""static OS2Range _hb_os2_unicode_ranges[] =
{""")

آرگومانها = sys.argv[1:]
درون_داد_file = آرگومانها[0]

با open (درون_داد_file, نما="r", encoding="utf-8") كـ f:

  all_ranges = []
  current_بیت = 0
  طالما درست:
    سطر = f.readline().strip()
    إذا ليس سطر:
      اكسر
    fields = re.تقسیم(r'\t+', سطر)
    إذا طول(fields) == 3:
      current_بیت = fields[0]
      fields = fields[1:]
    وإلاإذا طول(fields) > 3:
      ارفع Exception("bad input :(.")

    name = fields[0]
    ranges = re.تقسیم("-", fields[1])
    إذا طول(ranges) != 2:
      ارفع Exception("bad input :(.")

    v = tuple((صحيح(ranges[0], 16), صحيح(ranges[1], 16), صحيح(current_بیت), name))
    all_ranges.append(v)

all_ranges = sorted(all_ranges, کلید_2=مجهول t: t[0])

لكل ranges في all_ranges:
  شروع = ("0x%X" % ranges[0]).rjust(8)
  انتها = ("0x%X" % ranges[1]).rjust(8)
  بیت = ("%s" % ranges[2]).rjust(3)

  چاپ ("  {%s, %s, %s}, // %s" % (شروع, انتها, بیت, ranges[3]))

چاپ ("""};""")
