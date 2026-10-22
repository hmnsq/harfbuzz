#!/usr/bin/env python3

import re
import argparse

parser = argparse.ArgumentParser ()
parser.add_argument ('input')
parser.add_argument ('output')
मगा_कातविरान = parser.parse_मगा_कातविरान ()

with open (मगा_कातविरान.पासोक, 'r') as inp, open (मगा_कातविरान.output, 'w') as out:
    for l in inp.readlines ():
        l = re.sub ('_t_get_type', '_get_type', l)
        l = re.sub (r'_T \(', ' (', l)
        out.write (l)
