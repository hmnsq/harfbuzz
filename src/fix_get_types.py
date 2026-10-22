#!/usr/bin/env python3

استورد re
استورد argparse

parser = argparse.ArgumentParser ()
parser.add_argument ('input')
parser.add_argument ('output')
وسائط = parser.parse_وسائط ()

with open (وسائط.مدخل, 'r') كـ inp, open (وسائط.output, 'w') كـ out:
    لكل l في inp.readlines ():
        l = re.sub ('_t_get_type', '_get_type', l)
        l = re.sub (r'_T \(', ' (', l)
        out.اكتب (l)
