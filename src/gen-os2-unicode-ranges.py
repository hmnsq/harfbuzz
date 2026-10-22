#!/usr/bin/env python3

"""Generates the code for a sorted unicode range array as used in hb-ot-os2-unicode-ranges.hh
Input is a tab separated list of unicode ranges from the otspec
(https://docs.microsoft.com/en-us/typography/opentype/spec/os2#ur).
"""

ಆನಯನ re
ಆನಯನ sys


print ("""static OS2Range _hb_os2_unicode_ranges[] =
{""")

args = sys.argv[1:]
ಒಳಹರಿವು_file = args[0]

with open (ಒಳಹರಿವು_file, mode="r", encoding="utf-8") ಇವ f:

  all_ranges = []
  current_bit = 0
  ಯಾವತ್ ಸತ್ಯ:
    ಸಾಲು = f.readline().strip()
    ಯದಿ ನ ಸಾಲು:
      ಭಙ್ಗ
    fields = re.split(r'\t+', ಸಾಲು)
    ಯದಿ len(fields) == 3:
      current_bit = fields[0]
      fields = fields[1:]
    ನೋಚೇದ್ಯದಿ len(fields) > 3:
      ಉತ್ಥಾಪಯ Exception("bad input :(.")

    name = fields[0]
    ranges = re.split("-", fields[1])
    ಯದಿ len(ranges) != 2:
      ಉತ್ಥಾಪಯ Exception("bad input :(.")

    v = tuple((ಪೂರ್ಣಾಙ್ಕ(ranges[0], 16), ಪೂರ್ಣಾಙ್ಕ(ranges[1], 16), ಪೂರ್ಣಾಙ್ಕ(current_bit), name))
    all_ranges.append(v)

all_ranges = sorted(all_ranges, ಕೀಲಿ_2=ಅನಾಮನ್ t: t[0])

ಪ್ರತಿ ranges ಅನ್ತಃ all_ranges:
  start = ("0x%X" % ranges[0]).rjust(8)
  end = ("0x%X" % ranges[1]).rjust(8)
  bit = ("%s" % ranges[2]).rjust(3)

  print ("  {%s, %s, %s}, // %s" % (start, end, bit, ranges[3]))

print ("""};""")
