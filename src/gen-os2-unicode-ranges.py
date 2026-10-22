#!/usr/bin/env python3

"""Generates the code for a sorted unicode range array as used in hb-ot-os2-unicode-ranges.hh
Input is a tab separated list of unicode ranges from the otspec
(https://docs.microsoft.com/en-us/typography/opentype/spec/os2#ur).
"""

இறக்குமதி re
இறக்குமதி sys


print ("""static OS2Range _hb_os2_unicode_ranges[] =
{""")

args = sys.argv[1:]
உள்ளீடு_file = args[0]

with open (உள்ளீடு_file, mode="r", encoding="utf-8") இவ f:

  all_ranges = []
  current_bit = 0
  போது உண்மை:
    வரி = f.readline().strip()
    எனில் இல்லை வரி:
      நிறுத்து
    fields = re.split(r'\t+', வரி)
    எனில் len(fields) == 3:
      current_bit = fields[0]
      fields = fields[1:]
    அல்லதெனில் len(fields) > 3:
      எழுப்பு Exception("bad input :(.")

    name = fields[0]
    ranges = re.split("-", fields[1])
    எனில் len(ranges) != 2:
      எழுப்பு Exception("bad input :(.")

    v = tuple((முழு(ranges[0], 16), முழு(ranges[1], 16), முழு(current_bit), name))
    all_ranges.append(v)

all_ranges = sorted(all_ranges, சாவி_2=பெயரிலி t: t[0])

க்கு ranges உள் all_ranges:
  start = ("0x%X" % ranges[0]).rjust(8)
  end = ("0x%X" % ranges[1]).rjust(8)
  bit = ("%s" % ranges[2]).rjust(3)

  print ("  {%s, %s, %s}, // %s" % (start, end, bit, ranges[3]))

print ("""};""")
