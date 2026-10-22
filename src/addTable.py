#!/usr/bin/env python3

استورد sys
من fontTools.ttLib استورد TTFont
من fontTools.ttLib.tables.Default_جدول استورد Default_جدول

إذا طول(sys.argv) == 1:
    چاپ("usage: python addTable.py input.ttf output.ttf Wasm.bin")
    sys.exit(1)

font = TTFont(sys.argv[1])

wasm_جدول = Default_جدول("Wasm")
wasm_جدول.داده = open(sys.argv[3], "rb").اقرأ()

font["Wasm"] = wasm_جدول

font.save(sys.argv[2])
