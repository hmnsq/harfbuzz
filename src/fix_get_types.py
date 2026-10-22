#!/usr/bin/env python3

استورد re
استورد argparse

parser = argparse.آرگومان_Parser ()
parser.جمع_کن_آرگومان ('input')
parser.جمع_کن_آرگومان ('output')
آرگومانها = parser.parse_آرگومانها ()

با open (آرگومانها.درون_داد, 'r') كـ inp, open (آرگومانها.output, 'w') كـ بیرون:
    لكل l في inp.readlines ():
        l = re.sub ('_t_get_type', '_get_type', l)
        l = re.sub (r'_T \(', ' (', l)
        بیرون.اكتب (l)
