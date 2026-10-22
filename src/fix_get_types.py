#!/usr/bin/env python3

ಆನಯನ re
ಆನಯನ argparse

parser = argparse.ArgumentParser ()
parser.add_argument ('input')
parser.add_argument ('output')
args = parser.parse_args ()

with open (args.ಒಳಹರಿವು, 'r') ಇವ inp, open (args.output, 'w') ಇವ out:
    ಪ್ರತಿ l ಅನ್ತಃ inp.readlines ():
        l = re.sub ('_t_get_type', '_get_type', l)
        l = re.sub (r'_T \(', ' (', l)
        out.ಲಿಖ (l)
