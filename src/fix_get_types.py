#!/usr/bin/env python3

আনযন re
আনযন argparse

parser = argparse.ArgumentParser ()
parser.add_argument ('input')
parser.add_argument ('output')
args = parser.parse_args ()

with open (args.প্ৰবেশ, 'r') ইৱ inp, open (args.output, 'w') ইৱ out:
    বাবে l অন্তঃ inp.readlines ():
        l = re.sub ('_t_get_type', '_get_type', l)
        l = re.sub (r'_T \(', ' (', l)
        out.লিখ (l)
