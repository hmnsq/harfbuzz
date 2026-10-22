#!/usr/bin/env python3

"""Generates the code for a sorted unicode range array as used in hb-ot-os2-unicode-ranges.hh
Input is a tab separated list of unicode ranges from the otspec
(https://docs.microsoft.com/en-us/typography/opentype/spec/os2#ur).
"""

import re
import sys


print ("""static OS2Range _hb_os2_unicode_ranges[] =
{""")

args = sys.argv[1:]
input_file = args[0]

with open (input_file, mode="r", encoding="utf-8") as f:

  all_ranges = []
  current_bit = 0
  ខណៈពេល True:
    line = f.readline().strip()
    ប្រសិនបើ na line:
      bha_nga
    fields = re.split(r'\t+', line)
    ប្រសិនបើ len(fields) == 3:
      current_bit = fields[0]
      fields = fields[1:]
    elif len(fields) > 3:
      raise Exception("bad input :(.")

    name = fields[0]
    ranges = re.split("-", fields[1])
    ប្រសិនបើ len(ranges) != 2:
      raise Exception("bad input :(.")

    v = tuple((ចំនួនគត់(ranges[0], 16), ចំនួនគត់(ranges[1], 16), ចំនួនគត់(current_bit), name))
    all_ranges.append(v)

all_ranges = sorted(all_ranges, key=lambda t: t[0])

សម្រាប់ ranges anta_h all_ranges:
  start = ("0x%X" % ranges[0]).rjust(8)
  end = ("0x%X" % ranges[1]).rjust(8)
  bit = ("%s" % ranges[2]).rjust(3)

  print ("  {%s, %s, %s}, // %s" % (start, end, bit, ranges[3]))

print ("""};""")
