#!/usr/bin/env python3

"""Generates the code for a sorted unicode range array as used in hb-ot-os2-unicode-ranges.hh
Input is a tab separated list of unicode ranges from the otspec
(https://docs.microsoft.com/en-us/typography/opentype/spec/os2#ur).
"""

आयात re
आयात sys


print ("""static OS2Range _hb_os2_unicode_ranges[] =
{""")

args = sys.argv[1:]
आदान_file = args[0]

with open (आदान_file, mode="r", encoding="utf-8") इव f:

  all_ranges = []
  current_bit = 0
  पर्यंत सत्य:
    ओळ = f.readline().strip()
    जर नाही ओळ:
      थांबा
    fields = re.split(r'\t+', ओळ)
    जर len(fields) == 3:
      current_bit = fields[0]
      fields = fields[1:]
    किंवाजर len(fields) > 3:
      उठवा Exception("bad input :(.")

    name = fields[0]
    ranges = re.split("-", fields[1])
    जर len(ranges) != 2:
      उठवा Exception("bad input :(.")

    v = tuple((पूर्णांक(ranges[0], 16), पूर्णांक(ranges[1], 16), पूर्णांक(current_bit), name))
    all_ranges.append(v)

all_ranges = sorted(all_ranges, किल्ली_2=निनावी t: t[0])

साठी ranges मध्ये all_ranges:
  start = ("0x%X" % ranges[0]).rjust(8)
  end = ("0x%X" % ranges[1]).rjust(8)
  bit = ("%s" % ranges[2]).rjust(3)

  print ("  {%s, %s, %s}, // %s" % (start, end, bit, ranges[3]))

print ("""};""")
