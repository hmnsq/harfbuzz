#!/usr/bin/env python3

"""Generates the code for a sorted unicode range array as used in hb-ot-os2-unicode-ranges.hh
Input is a tab separated list of unicode ranges from the otspec
(https://docs.microsoft.com/en-us/typography/opentype/spec/os2#ur).
"""

ഇറക്കുമതി re
ഇറക്കുമതി sys


print ("""static OS2Range _hb_os2_unicode_ranges[] =
{""")

args = sys.argv[1:]
നിവേശം_file = args[0]

with open (നിവേശം_file, mode="r", encoding="utf-8") ഇവ f:

  all_ranges = []
  current_bit = 0
  ആകുമ്പോൾ സത്യം:
    വരി = f.readline().strip()
    എങ്കിൽ അല്ല വരി:
      നിർത്തുക
    fields = re.split(r'\t+', വരി)
    എങ്കിൽ len(fields) == 3:
      current_bit = fields[0]
      fields = fields[1:]
    അല്ലെങ്കിലെങ്കിൽ len(fields) > 3:
      ഉയർത്തുക Exception("bad input :(.")

    name = fields[0]
    ranges = re.split("-", fields[1])
    എങ്കിൽ len(ranges) != 2:
      ഉയർത്തുക Exception("bad input :(.")

    v = tuple((പൂർണ്ണസംഖ്യ(ranges[0], 16), പൂർണ്ണസംഖ്യ(ranges[1], 16), പൂർണ്ണസംഖ്യ(current_bit), name))
    all_ranges.append(v)

all_ranges = sorted(all_ranges, താക്കോൽ_2=അജ്ഞാത t: t[0])

വേണ്ടി ranges ഇൽ all_ranges:
  start = ("0x%X" % ranges[0]).rjust(8)
  end = ("0x%X" % ranges[1]).rjust(8)
  bit = ("%s" % ranges[2]).rjust(3)

  print ("  {%s, %s, %s}, // %s" % (start, end, bit, ranges[3]))

print ("""};""")
