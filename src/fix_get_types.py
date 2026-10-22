#!/usr/bin/env python3

استورد re
استورد argparse

parser = argparse.دلیل_Parser ()
parser.جمع_کړه_دلیل ('input')
parser.جمع_کړه_دلیل ('output')
دلیلونه = parser.parse_دلیلونه ()

سره open (دلیلونه.ننوت, 'r') كـ inp, open (دلیلونه.output, 'w') كـ بهر:
    لكل l في inp.readlines ():
        l = re.sub ('_t_get_type', '_get_type', l)
        l = re.sub (r'_T \(', ' (', l)
        بهر.اكتب (l)
