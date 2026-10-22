#!/usr/bin/env python3

आयात re
आयात argparse

parser = argparse.ArgumentParser ()
parser.add_argument ('input')
parser.add_argument ('output')
तर्क = parser.parse_args ()

with open (तर्क.निवेश, 'r') as inp, open (तर्क.output, 'w') as बाहर:
    क्रम l in inp.readlines ():
        l = re.sub ('_t_get_type', '_get_type', l)
        l = re.sub (r'_T \(', ' (', l)
        बाहर.म_लिखो (l)
