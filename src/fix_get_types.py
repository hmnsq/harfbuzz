#!/usr/bin/env python3

ਆਯਾਤ re
ਆਯਾਤ argparse

parser = argparse.ਦਲੀਲ_Parser ()
parser.ਜੋੜੋ_ਦਲੀਲ ('input')
parser.ਜੋੜੋ_ਦਲੀਲ ('output')
ਦਲੀਲਾਂ = parser.parse_ਦਲੀਲਾਂ ()

ਨਾਲ open (ਦਲੀਲਾਂ.ਨਿਵੇਸ਼, 'r') ਇਵ inp, open (ਦਲੀਲਾਂ.output, 'w') ਇਵ ਬਾਹਰਲਾ:
    ਲਈ l ਵਿੱਚ inp.readlines ():
        l = re.sub ('_t_get_type', '_get_type', l)
        l = re.sub (r'_T \(', ' (', l)
        ਬਾਹਰਲਾ.ਲਿਖ (l)
