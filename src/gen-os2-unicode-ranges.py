#!/usr/bin/env python3

"""Generates the code for a sorted unicode range array as used in hb-ot-os2-unicode-ranges.hh
Input is a tab separated list of unicode ranges from the otspec
(https://docs.microsoft.com/en-us/typography/opentype/spec/os2#ur).
"""

आयात re
आयात sys


print ("""static OS2Range _hb_os2_unicode_ranges[] =
{""")

तर्क = sys.argv[1:]
निवेश_file = तर्क[0]

with open (निवेश_file, mode="r", encoding="utf-8") as f:

  all_ranges = []
  current_bit = 0
  जबतक True:
    पंक्ति = f.readline().strip()
    अगर not पंक्ति:
      अवरोध
    fields = re.split(r'\t+', पंक्ति)
    अगर len(fields) == 3:
      current_bit = fields[0]
      fields = fields[1:]
    elif len(fields) > 3:
      raise Exception("bad input :(.")

    name = fields[0]
    ranges = re.split("-", fields[1])
    अगर len(ranges) != 2:
      raise Exception("bad input :(.")

    v = tuple((पूर्णांक(ranges[0], 16), पूर्णांक(ranges[1], 16), पूर्णांक(current_bit), name))
    all_ranges.append(v)

all_ranges = sorted(all_ranges, कुंजी_2=lambda t: t[0])

क्रम ranges in all_ranges:
  शुरू = ("0x%X" % ranges[0]).rjust(8)
  end = ("0x%X" % ranges[1]).rjust(8)
  bit = ("%s" % ranges[2]).rjust(3)

  print ("  {%s, %s, %s}, // %s" % (शुरू, end, bit, ranges[3]))

print ("""};""")
