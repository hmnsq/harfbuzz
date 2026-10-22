#!/usr/bin/env python3

ଆମଦାନୀ re
ଆମଦାନୀ argparse

parser = argparse.ArgumentParser ()
parser.add_argument ('input')
parser.add_argument ('output')
args = parser.parse_args ()

with open (args.ନିବେଶ, 'r') ଇଵ inp, open (args.output, 'w') ଇଵ out:
    ପାଇଁ l ଭିତରେ inp.readlines ():
        l = re.sub ('_t_get_type', '_get_type', l)
        l = re.sub (r'_T \(', ' (', l)
        out.ଲିଖ (l)
