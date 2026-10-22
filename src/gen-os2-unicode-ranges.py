#!/usr/bin/env python3

"""Generates the code for a sorted unicode range array as used in hb-ot-os2-unicode-ranges.hh
Input is a tab separated list of unicode ranges from the otspec
(https://docs.microsoft.com/en-us/typography/opentype/spec/os2#ur).
"""

ଆମଦାନୀ re
ଆମଦାନୀ sys


print ("""static OS2Range _hb_os2_unicode_ranges[] =
{""")

args = sys.argv[1:]
ନିବେଶ_file = args[0]

with open (ନିବେଶ_file, mode="r", encoding="utf-8") ଇଵ f:

  all_ranges = []
  current_bit = 0
  ଯେତେବେଳେ ସତ୍ୟ:
    ଧାଡ଼ି = f.readline().strip()
    ଯଦି ନୁହେଁ ଧାଡ଼ି:
      ଭାଙ୍ଗ
    fields = re.split(r'\t+', ଧାଡ଼ି)
    ଯଦି len(fields) == 3:
      current_bit = fields[0]
      fields = fields[1:]
    ନଚେତଯଦି len(fields) > 3:
      ଉଠାଅ Exception("bad input :(.")

    name = fields[0]
    ranges = re.split("-", fields[1])
    ଯଦି len(ranges) != 2:
      ଉଠାଅ Exception("bad input :(.")

    v = tuple((ପୂର୍ଣ୍ଣସଂଖ୍ୟା(ranges[0], 16), ପୂର୍ଣ୍ଣସଂଖ୍ୟା(ranges[1], 16), ପୂର୍ଣ୍ଣସଂଖ୍ୟା(current_bit), name))
    all_ranges.append(v)

all_ranges = sorted(all_ranges, ଚାବି_2=ଅନାମ t: t[0])

ପାଇଁ ranges ଭିତରେ all_ranges:
  start = ("0x%X" % ranges[0]).rjust(8)
  end = ("0x%X" % ranges[1]).rjust(8)
  bit = ("%s" % ranges[2]).rjust(3)

  print ("  {%s, %s, %s}, // %s" % (start, end, bit, ranges[3]))

print ("""};""")
