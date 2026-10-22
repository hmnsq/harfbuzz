#!/usr/bin/env python3

আনযন sys
তঃ fontTools.ttLib আনযন TTFont
তঃ fontTools.ttLib.tables.DefaultTable আনযন DefaultTable

যদি len(sys.argv) == 1:
    print("usage: python addTable.py input.ttf output.ttf Wasm.bin")
    sys.exit(1)

font = TTFont(sys.argv[1])

wasm_table = DefaultTable("Wasm")
wasm_table.data = open(sys.argv[3], "rb").পঠ()

font["Wasm"] = wasm_table

font.save(sys.argv[2])
