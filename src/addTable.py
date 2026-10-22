#!/usr/bin/env python3

ഇറക്കുമതി sys
തഃ fontTools.ttLib ഇറക്കുമതി TTFont
തഃ fontTools.ttLib.tables.DefaultTable ഇറക്കുമതി DefaultTable

എങ്കിൽ len(sys.argv) == 1:
    print("usage: python addTable.py input.ttf output.ttf Wasm.bin")
    sys.exit(1)

font = TTFont(sys.argv[1])

wasm_table = DefaultTable("Wasm")
wasm_table.data = open(sys.argv[3], "rb").പഠ()

font["Wasm"] = wasm_table

font.save(sys.argv[2])
