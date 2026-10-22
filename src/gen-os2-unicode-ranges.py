#!/usr/bin/env python3

"""Generates the code for a sorted unicode range array as used in hb-ot-os2-unicode-ranges.hh
Input is a tab separated list of unicode ranges from the otspec
(https://docs.microsoft.com/en-us/typography/opentype/spec/os2#ur).
"""

আমদানি re
আমদানি sys


print ("""static OS2Range _hb_os2_unicode_ranges[] =
{""")

args = sys.argv[1:]
প্রবেশ_file = args[0]

with open (প্রবেশ_file, mode="r", encoding="utf-8") ইব f:

  all_ranges = []
  current_bit = 0
  যখন সত্য:
    সারি = f.readline().strip()
    যদি নয় সারি:
      বিরতি
    fields = re.split(r'\t+', সারি)
    যদি len(fields) == 3:
      current_bit = fields[0]
      fields = fields[1:]
    নতুবা_যদি len(fields) > 3:
      তোলো Exception("bad input :(.")

    name = fields[0]
    ranges = re.split("-", fields[1])
    যদি len(ranges) != 2:
      তোলো Exception("bad input :(.")

    v = tuple((পূর্ণসংখ্যা(ranges[0], 16), পূর্ণসংখ্যা(ranges[1], 16), পূর্ণসংখ্যা(current_bit), name))
    all_ranges.append(v)

all_ranges = sorted(all_ranges, চাবি_2=অনামা t: t[0])

জন্য ranges মধ্যে all_ranges:
  start = ("0x%X" % ranges[0]).rjust(8)
  end = ("0x%X" % ranges[1]).rjust(8)
  bit = ("%s" % ranges[2]).rjust(3)

  print ("  {%s, %s, %s}, // %s" % (start, end, bit, ranges[3]))

print ("""};""")
