#!/usr/bin/env python3

"""Generates the code for a sorted unicode range array as used in hb-ot-os2-unicode-ranges.hh
Input is a tab separated list of unicode ranges from the otspec
(https://docs.microsoft.com/en-us/typography/opentype/spec/os2#ur).
"""

درآمد re
درآمد sys


چھاپو ("""static OS2Range _hb_os2_unicode_ranges[] =
{""")

دلائل = sys.argv[1:]
داخلہ_file = دلائل[0]

ساتھ open (داخلہ_file, منوال="r", encoding="utf-8") بطور f:

  all_ranges = []
  current_ثنائی_ہندسہ = 0
  جب سچ:
    سطر = f.readline().strip()
    اگر نہیں سطر:
      توڑو
    fields = re.بانٹو(r'\t+', سطر)
    اگر لمبائی(fields) == 3:
      current_ثنائی_ہندسہ = fields[0]
      fields = fields[1:]
    وگرنہ_اگر لمبائی(fields) > 3:
      اٹھاؤ Exception("bad input :(.")

    name = fields[0]
    ranges = re.بانٹو("-", fields[1])
    اگر لمبائی(ranges) != 2:
      اٹھاؤ Exception("bad input :(.")

    v = tuple((صحیح(ranges[0], 16), صحیح(ranges[1], 16), صحیح(current_ثنائی_ہندسہ), name))
    all_ranges.append(v)

all_ranges = sorted(all_ranges, کلید_2=بےنام t: t[0])

برائے ranges میں all_ranges:
  آغاز = ("0x%X" % ranges[0]).rjust(8)
  انتہا = ("0x%X" % ranges[1]).rjust(8)
  ثنائی_ہندسہ = ("%s" % ranges[2]).rjust(3)

  چھاپو ("  {%s, %s, %s}, // %s" % (آغاز, انتہا, ثنائی_ہندسہ, ranges[3]))

چھاپو ("""};""")
