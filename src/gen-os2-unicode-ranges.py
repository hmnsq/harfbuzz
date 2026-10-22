#!/usr/bin/env python3

"""Generates the code for a sorted unicode range array as used in hb-ot-os2-unicode-ranges.hh
Input is a tab separated list of unicode ranges from the otspec
(https://docs.microsoft.com/en-us/typography/opentype/spec/os2#ur).
"""

আনযন re
আনযন sys


print ("""static OS2Range _hb_os2_unicode_ranges[] =
{""")

args = sys.argv[1:]
প্ৰবেশ_file = args[0]

with open (প্ৰবেশ_file, mode="r", encoding="utf-8") ইৱ f:

  all_ranges = []
  current_bit = 0
  যেতিয়ালৈ সঁচা:
    সাৰি = f.readline().strip()
    যদি নহয় সাৰি:
      ভঙ্গ
    fields = re.split(r'\t+', সাৰি)
    যদি len(fields) == 3:
      current_bit = fields[0]
      fields = fields[1:]
    নোচেদ্যদি len(fields) > 3:
      উত্থাপয Exception("bad input :(.")

    name = fields[0]
    ranges = re.split("-", fields[1])
    যদি len(ranges) != 2:
      উত্থাপয Exception("bad input :(.")

    v = tuple((পূর্ণাঙ্ক(ranges[0], 16), পূর্ণাঙ্ক(ranges[1], 16), পূর্ণাঙ্ক(current_bit), name))
    all_ranges.append(v)

all_ranges = sorted(all_ranges, চাবি_2=অনামন্ t: t[0])

বাবে ranges অন্তঃ all_ranges:
  start = ("0x%X" % ranges[0]).rjust(8)
  end = ("0x%X" % ranges[1]).rjust(8)
  bit = ("%s" % ranges[2]).rjust(3)

  print ("  {%s, %s, %s}, // %s" % (start, end, bit, ranges[3]))

print ("""};""")
