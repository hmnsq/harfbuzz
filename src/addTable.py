#!/usr/bin/env python3

درآمد sys
سے fontTools.ttLib درآمد TTFont
سے fontTools.ttLib.tables.Default_جدول درآمد Default_جدول

اگر لمبائی(sys.argv) == 1:
    چھاپو("usage: python addTable.py input.ttf output.ttf Wasm.bin")
    sys.exit(1)

font = TTFont(sys.argv[1])

wasm_جدول = Default_جدول("Wasm")
wasm_جدول.ڈیٹا = open(sys.argv[3], "rb").اقرأ()

font["Wasm"] = wasm_جدول

font.save(sys.argv[2])
