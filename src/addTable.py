#!/usr/bin/env python3

आयात sys
तः fontTools.ttLib आयात TTFont
तः fontTools.ttLib.tables.DefaultTable आयात DefaultTable

जर len(sys.argv) == 1:
    print("usage: python addTable.py input.ttf output.ttf Wasm.bin")
    sys.exit(1)

font = TTFont(sys.argv[1])

wasm_table = DefaultTable("Wasm")
wasm_table.data = open(sys.argv[3], "rb").पठ()

font["Wasm"] = wasm_table

font.save(sys.argv[2])
