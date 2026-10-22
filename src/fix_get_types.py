#!/usr/bin/env python3

இறக்குமதி re
இறக்குமதி argparse

parser = argparse.ArgumentParser ()
parser.add_argument ('input')
parser.add_argument ('output')
args = parser.parse_args ()

with open (args.உள்ளீடு, 'r') இவ inp, open (args.output, 'w') இவ out:
    க்கு l உள் inp.readlines ():
        l = re.sub ('_t_get_type', '_get_type', l)
        l = re.sub (r'_T \(', ' (', l)
        out.லிக (l)
