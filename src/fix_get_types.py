#!/usr/bin/env python3

درآمد re
درآمد argparse

parser = argparse.دلیل_Parser ()
parser.جوڑو_دلیل ('input')
parser.جوڑو_دلیل ('output')
دلائل = parser.parse_دلائل ()

ساتھ open (دلائل.داخلہ, 'r') بطور inp, open (دلائل.output, 'w') بطور برآمد:
    برائے l میں inp.readlines ():
        l = re.sub ('_t_get_type', '_get_type', l)
        l = re.sub (r'_T \(', ' (', l)
        برآمد.اكتب (l)
