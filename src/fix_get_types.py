#!/usr/bin/env python3

আমদানি re
আমদানি argparse

parser = argparse.ArgumentParser ()
parser.add_argument ('input')
parser.add_argument ('output')
args = parser.parse_args ()

with open (args.প্রবেশ, 'r') ইব inp, open (args.output, 'w') ইব out:
    জন্য l মধ্যে inp.readlines ():
        l = re.sub ('_t_get_type', '_get_type', l)
        l = re.sub (r'_T \(', ' (', l)
        out.লিখ (l)
