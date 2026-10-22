#!/usr/bin/env python3

"""Generates the code for a sorted unicode range array as used in hb-ot-os2-unicode-ranges.hh
Input is a tab separated list of unicode ranges from the otspec
(https://docs.microsoft.com/en-us/typography/opentype/spec/os2#ur).
"""

આયાત re
આયાત sys


print ("""static OS2Range _hb_os2_unicode_ranges[] =
{""")

args = sys.argv[1:]
આગત_file = args[0]

with open (આગત_file, mode="r", encoding="utf-8") ઇવ f:

  all_ranges = []
  current_bit = 0
  જ્યારે સાચું:
    લીટી = f.readline().strip()
    જો નહીં લીટી:
      તોડો
    fields = re.split(r'\t+', લીટી)
    જો len(fields) == 3:
      current_bit = fields[0]
      fields = fields[1:]
    અથવાજો len(fields) > 3:
      ઉઠાવો Exception("bad input :(.")

    name = fields[0]
    ranges = re.split("-", fields[1])
    જો len(ranges) != 2:
      ઉઠાવો Exception("bad input :(.")

    v = tuple((પૂર્ણાંક(ranges[0], 16), પૂર્ણાંક(ranges[1], 16), પૂર્ણાંક(current_bit), name))
    all_ranges.append(v)

all_ranges = sorted(all_ranges, ચાવી_2=અનામી t: t[0])

માટે ranges માં all_ranges:
  start = ("0x%X" % ranges[0]).rjust(8)
  end = ("0x%X" % ranges[1]).rjust(8)
  bit = ("%s" % ranges[2]).rjust(3)

  print ("  {%s, %s, %s}, // %s" % (start, end, bit, ranges[3]))

print ("""};""")
