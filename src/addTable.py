#!/usr/bin/env python3

ਆਯਾਤ sys
ਤਃ fontTools.ttLib ਆਯਾਤ TTFont
ਤਃ fontTools.ttLib.tables.Default_ਸਾਰਣੀ ਆਯਾਤ Default_ਸਾਰਣੀ

ਜੇ ਲੰਬਾਈ(sys.argv) == 1:
    ਛਾਪੋ("usage: python addTable.py input.ttf output.ttf Wasm.bin")
    sys.exit(1)

font = TTFont(sys.argv[1])

wasm_ਸਾਰਣੀ = Default_ਸਾਰਣੀ("Wasm")
wasm_ਸਾਰਣੀ.ਅੰਕੜੇ = open(sys.argv[3], "rb").ਪਠ()

font["Wasm"] = wasm_ਸਾਰਣੀ

font.save(sys.argv[2])
