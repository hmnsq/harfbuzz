#!/usr/bin/env python3

"""Generates the code for a sorted unicode range array as used in hb-ot-os2-unicode-ranges.hh
Input is a tab separated list of unicode ranges from the otspec
(https://docs.microsoft.com/en-us/typography/opentype/spec/os2#ur).
"""

استورد re
استورد sys


print ("""static OS2Range _hb_os2_unicode_ranges[] =
{""")

args = sys.argv[1:]
إدخال_file = args[0]

with open (إدخال_file, mode="r", encoding="utf-8") كـ f:

  all_ranges = []
  current_bit = 0
  بينما صحيح:
    سطر = f.readline().strip()
    إذا ليس سطر:
      اكسر
    fields = re.split(r'\t+', سطر)
    إذا len(fields) == 3:
      current_bit = fields[0]
      fields = fields[1:]
    وإلاإذا len(fields) > 3:
      ارفع Exception("bad input :(.")

    name = fields[0]
    ranges = re.split("-", fields[1])
    إذا len(ranges) != 2:
      ارفع Exception("bad input :(.")

    v = tuple((int(ranges[0], 16), int(ranges[1], 16), int(current_bit), name))
    all_ranges.append(v)

all_ranges = sorted(all_ranges, مفتاح_2=مجهول t: t[0])

لكل ranges في all_ranges:
  start = ("0x%X" % ranges[0]).rjust(8)
  end = ("0x%X" % ranges[1]).rjust(8)
  bit = ("%s" % ranges[2]).rjust(3)

  print ("  {%s, %s, %s}, // %s" % (start, end, bit, ranges[3]))

print ("""};""")
