#!/usr/bin/env python3

આયાત re
આયાત argparse

parser = argparse.ArgumentParser ()
parser.add_argument ('input')
parser.add_argument ('output')
args = parser.parse_args ()

with open (args.આગત, 'r') ઇવ inp, open (args.output, 'w') ઇવ out:
    માટે l માં inp.readlines ():
        l = re.sub ('_t_get_type', '_get_type', l)
        l = re.sub (r'_T \(', ' (', l)
        out.લિખ (l)
