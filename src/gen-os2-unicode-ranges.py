#!/usr/bin/env python3

"""Generates the code for a sorted unicode range array as used in hb-ot-os2-unicode-ranges.hh
Input is a tab separated list of unicode ranges from the otspec
(https://docs.microsoft.com/en-us/typography/opentype/spec/os2#ur).
"""

واردکن re
واردکن sys


print ("""static OS2Range _hb_os2_unicode_ranges[] =
{""")

args = sys.argv[1:]
درون_داد_file = args[0]

with open (درون_داد_file, mode="r", encoding="utf-8") بهعنوان f:

  all_ranges = []
  current_bit = 0
  وقتی درست:
    سطر = f.readline().strip()
    اگر نه سطر:
      بشکن
    fields = re.split(r'\t+', سطر)
    اگر len(fields) == 3:
      current_bit = fields[0]
      fields = fields[1:]
    وگرنهاگر len(fields) > 3:
      بالاببر Exception("bad input :(.")

    name = fields[0]
    ranges = re.split("-", fields[1])
    اگر len(ranges) != 2:
      بالاببر Exception("bad input :(.")

    v = tuple((صحیح(ranges[0], 16), صحیح(ranges[1], 16), صحیح(current_bit), name))
    all_ranges.append(v)

all_ranges = sorted(all_ranges, کلید_2=بینام t: t[0])

برای ranges در all_ranges:
  start = ("0x%X" % ranges[0]).rjust(8)
  end = ("0x%X" % ranges[1]).rjust(8)
  bit = ("%s" % ranges[2]).rjust(3)

  print ("  {%s, %s, %s}, // %s" % (start, end, bit, ranges[3]))

print ("""};""")
