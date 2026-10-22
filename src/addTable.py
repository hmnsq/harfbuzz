#!/usr/bin/env python3

आयात sys
from fontTools.ttLib आयात TTFont
from fontTools.ttLib.tables.DefaultTable आयात DefaultTable

अगर len(sys.argv) == 1:
    print("usage: python addTable.py input.ttf output.ttf Wasm.bin")
    sys.exit(1)

font = TTFont(sys.argv[1])

wasm_table = DefaultTable("Wasm")
wasm_table.तथ्य = open(sys.argv[3], "rb").म_पढ़ो()

font["Wasm"] = wasm_table

font.save(sys.argv[2])
