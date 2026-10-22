#!/usr/bin/env python3

استورد re
استورد argparse

parser = argparse.دليل_Parser ()
parser.جوڙيو_دليل ('input')
parser.جوڙيو_دليل ('output')
دليلون = parser.parse_دليلون ()

سان open (دليلون.داخلا_2, 'r') كـ inp, open (دليلون.output, 'w') كـ ٻاهريون:
    لكل l في inp.readlines ():
        l = re.sub ('_t_get_type', '_get_type', l)
        l = re.sub (r'_T \(', ' (', l)
        ٻاهريون.اكتب (l)
