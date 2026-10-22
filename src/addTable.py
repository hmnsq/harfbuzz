#!/usr/bin/env python3

ଆମଦାନୀ sys
ତଃ fontTools.ttLib ଆମଦାନୀ TTFont
ତଃ fontTools.ttLib.tables.DefaultTable ଆମଦାନୀ DefaultTable

ଯଦି len(sys.argv) == 1:
    print("usage: python addTable.py input.ttf output.ttf Wasm.bin")
    sys.exit(1)

font = TTFont(sys.argv[1])

wasm_table = DefaultTable("Wasm")
wasm_table.data = open(sys.argv[3], "rb").ପଠ()

font["Wasm"] = wasm_table

font.save(sys.argv[2])
