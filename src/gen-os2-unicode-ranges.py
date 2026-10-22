#!/usr/bin/env python3

"""Generates the code for a sorted unicode range array as used in hb-ot-os2-unicode-ranges.hh
Input is a tab separated list of unicode ranges from the otspec
(https://docs.microsoft.com/en-us/typography/opentype/spec/os2#ur).
"""

استورد re
استورد sys


چھاپو ("""static OS2Range _hb_os2_unicode_ranges[] =
{""")

دلیلاں = sys.argv[1:]
داخلہ_file = دلیلاں[0]

نال open (داخلہ_file, منوال="r", encoding="utf-8") كـ f:

  all_ranges = []
  current_ثنائی_ہندسہ = 0
  طالما سچ:
    سطر = f.readline().strip()
    إذا ليس سطر:
      اكسر
    fields = re.ونڈو(r'\t+', سطر)
    إذا لمبائی(fields) == 3:
      current_ثنائی_ہندسہ = fields[0]
      fields = fields[1:]
    وإلاإذا لمبائی(fields) > 3:
      ارفع Exception("bad input :(.")

    name = fields[0]
    ranges = re.ونڈو("-", fields[1])
    إذا لمبائی(ranges) != 2:
      ارفع Exception("bad input :(.")

    v = tuple((صحيح(ranges[0], 16), صحيح(ranges[1], 16), صحيح(current_ثنائی_ہندسہ), name))
    all_ranges.append(v)

all_ranges = sorted(all_ranges, کلید_2=مجهول t: t[0])

لكل ranges في all_ranges:
  شروع = ("0x%X" % ranges[0]).rjust(8)
  سرا = ("0x%X" % ranges[1]).rjust(8)
  ثنائی_ہندسہ = ("%s" % ranges[2]).rjust(3)

  چھاپو ("  {%s, %s, %s}, // %s" % (شروع, سرا, ثنائی_ہندسہ, ranges[3]))

چھاپو ("""};""")
