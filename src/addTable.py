#!/usr/bin/env python3

இறக்குமதி sys
தஃ fontTools.ttLib இறக்குமதி TTFont
தஃ fontTools.ttLib.tables.DefaultTable இறக்குமதி DefaultTable

எனில் len(sys.argv) == 1:
    print("usage: python addTable.py input.ttf output.ttf Wasm.bin")
    sys.exit(1)

font = TTFont(sys.argv[1])

wasm_table = DefaultTable("Wasm")
wasm_table.data = open(sys.argv[3], "rb").பட()

font["Wasm"] = wasm_table

font.save(sys.argv[2])
