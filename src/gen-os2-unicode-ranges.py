#!/usr/bin/env python3

"""Generates the code for a sorted unicode range array as used in hb-ot-os2-unicode-ranges.hh
Input is a tab separated list of unicode ranges from the otspec
(https://docs.microsoft.com/en-us/typography/opentype/spec/os2#ur).
"""

استورد re
استورد sys


چاپ ("""static OS2Range _hb_os2_unicode_ranges[] =
{""")

دلیلونه = sys.argv[1:]
ننوت_file = دلیلونه[0]

سره open (ننوت_file, ډېرپېښ="r", encoding="utf-8") كـ f:

  all_ranges = []
  current_بټ = 0
  طالما ریښتیا:
    کرښه = f.readline().strip()
    إذا ليس کرښه:
      اكسر
    fields = re.وېشل(r'\t+', کرښه)
    إذا اوږدوالی(fields) == 3:
      current_بټ = fields[0]
      fields = fields[1:]
    وإلاإذا اوږدوالی(fields) > 3:
      ارفع Exception("bad input :(.")

    name = fields[0]
    ranges = re.وېشل("-", fields[1])
    إذا اوږدوالی(ranges) != 2:
      ارفع Exception("bad input :(.")

    v = tuple((صحيح(ranges[0], 16), صحيح(ranges[1], 16), صحيح(current_بټ), name))
    all_ranges.append(v)

all_ranges = sorted(all_ranges, کلی_2=مجهول t: t[0])

لكل ranges في all_ranges:
  پیل = ("0x%X" % ranges[0]).rjust(8)
  پای_ټکی = ("0x%X" % ranges[1]).rjust(8)
  بټ = ("%s" % ranges[2]).rjust(3)

  چاپ ("  {%s, %s, %s}, // %s" % (پیل, پای_ټکی, بټ, ranges[3]))

چاپ ("""};""")
