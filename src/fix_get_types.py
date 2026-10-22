#!/usr/bin/env python3

ഇറക്കുമതി re
ഇറക്കുമതി argparse

parser = argparse.ArgumentParser ()
parser.add_argument ('input')
parser.add_argument ('output')
args = parser.parse_args ()

with open (args.നിവേശം, 'r') ഇവ inp, open (args.output, 'w') ഇവ out:
    വേണ്ടി l ഇൽ inp.readlines ():
        l = re.sub ('_t_get_type', '_get_type', l)
        l = re.sub (r'_T \(', ' (', l)
        out.ലിഖ (l)
