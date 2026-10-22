#!/usr/bin/env python3

దిగుమతి re
దిగుమతి argparse

parser = argparse.వాదం_Parser ()
parser.కూడు_వాదం ('input')
parser.కూడు_వాదం ('output')
వాదనలు = parser.parse_వాదనలు ()

తోడుగా open (వాదనలు.ఆదానం, 'r') ఇవ inp, open (వాదనలు.output, 'w') ఇవ బహిర్గతం:
    కొరకు l లో inp.readlines ():
        l = re.sub ('_t_get_type', '_get_type', l)
        l = re.sub (r'_T \(', ' (', l)
        బహిర్గతం.లిఖ (l)
