#!/usr/bin/env python3

استورد re
استورد argparse

parser = argparse.دلیل_Parser ()
parser.جوڑو_دلیل ('input')
parser.جوڑو_دلیل ('output')
دلیلاں = parser.parse_دلیلاں ()

نال open (دلیلاں.داخلہ, 'r') كـ inp, open (دلیلاں.output, 'w') كـ باہرلا:
    لكل l في inp.readlines ():
        l = re.sub ('_t_get_type', '_get_type', l)
        l = re.sub (r'_T \(', ' (', l)
        باہرلا.اكتب (l)
