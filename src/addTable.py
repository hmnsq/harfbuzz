#!/usr/bin/env python3

আমদানি sys
তঃ fontTools.ttLib আমদানি TTFont
তঃ fontTools.ttLib.tables.DefaultTable আমদানি DefaultTable

যদি len(sys.argv) == 1:
    print("usage: python addTable.py input.ttf output.ttf Wasm.bin")
    sys.exit(1)

font = TTFont(sys.argv[1])

wasm_table = DefaultTable("Wasm")
wasm_table.তথ্য = open(sys.argv[3], "rb").পঠ()

font["Wasm"] = wasm_table

font.save(sys.argv[2])
