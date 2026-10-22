#!/usr/bin/env python3

واردکن re
واردکن argparse

parser = argparse.ArgumentParser ()
parser.add_argument ('input')
parser.add_argument ('output')
args = parser.parse_args ()

with open (args.درون_داد, 'r') بهعنوان inp, open (args.output, 'w') بهعنوان out:
    برای l در inp.readlines ():
        l = re.sub ('_t_get_type', '_get_type', l)
        l = re.sub (r'_T \(', ' (', l)
        out.اكتب (l)
