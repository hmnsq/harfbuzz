#!/usr/bin/env python3

"""Generates the code for a sorted unicode range array as used in hb-ot-os2-unicode-ranges.hh
Input is a tab separated list of unicode ranges from the otspec
(https://docs.microsoft.com/en-us/typography/opentype/spec/os2#ur).
"""

आनयन re
आनयन sys


print ("""static OS2Range _hb_os2_unicode_ranges[] =
{""")

args = sys.argv[1:]
निवेशः_file = args[0]

with open (निवेशः_file, mode="r", encoding="utf-8") इव f:

  all_ranges = []
  current_bit = 0
  जाव सच्च:
    पङ्क्तिः = f.readline().strip()
    जइ ण पङ्क्तिः:
      भङ्ग
    fields = re.split(r'\t+', पङ्क्तिः)
    जइ len(fields) == 3:
      current_bit = fields[0]
      fields = fields[1:]
    नोचेद्यदि len(fields) > 3:
      उत्थापय Exception("bad input :(.")

    name = fields[0]
    ranges = re.split("-", fields[1])
    जइ len(ranges) != 2:
      उत्थापय Exception("bad input :(.")

    v = tuple((पूर्णाङ्क(ranges[0], 16), पूर्णाङ्क(ranges[1], 16), पूर्णाङ्क(current_bit), name))
    all_ranges.append(v)

all_ranges = sorted(all_ranges, कुञ्चिका_2=अनामन् t: t[0])

प्रति ranges अन्तः all_ranges:
  start = ("0x%X" % ranges[0]).rjust(8)
  end = ("0x%X" % ranges[1]).rjust(8)
  bit = ("%s" % ranges[2]).rjust(3)

  print ("  {%s, %s, %s}, // %s" % (start, end, bit, ranges[3]))

print ("""};""")
