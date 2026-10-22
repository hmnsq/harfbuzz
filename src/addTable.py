#!/usr/bin/env python3

దిగుమతి sys
తః fontTools.ttLib దిగుమతి TTFont
తః fontTools.ttLib.tables.Default_పట్టిక దిగుమతి Default_పట్టిక

ఒకవేళ పొడవు(sys.argv) == 1:
    ముద్రించు("usage: python addTable.py input.ttf output.ttf Wasm.bin")
    sys.exit(1)

font = TTFont(sys.argv[1])

wasm_పట్టిక = Default_పట్టిక("Wasm")
wasm_పట్టిక.దత్తాంశం = open(sys.argv[3], "rb").పఠ()

font["Wasm"] = wasm_పట్టిక

font.save(sys.argv[2])
