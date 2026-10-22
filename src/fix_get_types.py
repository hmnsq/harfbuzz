#!/usr/bin/env python3

आनयन re
आनयन argparse

parser = argparse.ArgumentParser ()
parser.add_argument ('input')
parser.add_argument ('output')
args = parser.parse_args ()

with open (args.निवेशः, 'r') इव inp, open (args.output, 'w') इव out:
    प्रति l अन्तः inp.readlines ():
        l = re.sub ('_t_get_type', '_get_type', l)
        l = re.sub (r'_T \(', ' (', l)
        out.लिख (l)
