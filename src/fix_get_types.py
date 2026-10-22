#!/usr/bin/env python3

استورد re
استورد argparse

parser = argparse.ArgumentParser ()
parser.add_argument ('input')
parser.add_argument ('output')
حجوجي = parser.parse_حجوجي ()

with open (حجوجي.شغرو, 'r') كـ inp, open (حجوجي.output, 'w') كـ out:
    لكل l في inp.readlines ():
        l = re.sub ('_t_get_type', '_get_type', l)
        l = re.sub (r'_T \(', ' (', l)
        out.اكتب (l)
