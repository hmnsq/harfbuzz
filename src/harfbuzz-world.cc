#अगर_घोषित HB_FEATURES_H
#समावेश HB_FEATURES_H
#पूर्ण_अगर

#अगर_घोषित HB_HAS_CORETEXT
#घोषणा HAVE_CORETEXT 1
#पूर्ण_अगर
#अगर_घोषित HB_HAS_DIRECTWRITE
#घोषणा HAVE_DIRECTWRITE 1
#पूर्ण_अगर
#अगर_घोषित HB_HAS_FREETYPE
#घोषणा HAVE_FREETYPE 1
#पूर्ण_अगर
#अगर_घोषित HB_HAS_GDI
#घोषणा HAVE_GDI 1
#पूर्ण_अगर
#अगर_घोषित HB_HAS_GLIB
#घोषणा HAVE_GLIB 1
#पूर्ण_अगर
#अगर_घोषित HB_HAS_GRAPHITE
#घोषणा HAVE_GRAPHITE 1
#पूर्ण_अगर
#अगर_घोषित HB_HAS_UNISCRIBE
#घोषणा HAVE_UNISCRIBE 1
#घोषणा HAVE_GDI 1
#पूर्ण_अगर
#अगर_घोषित HB_HAS_WASM
#घोषणा HAVE_WASM 1
#पूर्ण_अगर


/* Core library. */
#समावेश "OT/Var/VARC/VARC.cc"
#समावेश "hb-aat-layout.cc"
#समावेश "hb-aat-map.cc"
#समावेश "hb-blob.cc"
#समावेश "hb-buffer-serialize.cc"
#समावेश "hb-buffer-verify.cc"
#समावेश "hb-buffer.cc"
#समावेश "hb-common.cc"
#समावेश "hb-draw.cc"
#समावेश "hb-face-builder.cc"
#समावेश "hb-face.cc"
#समावेश "hb-fallback-shape.cc"
#समावेश "hb-font.cc"
#समावेश "hb-map.cc"
#समावेश "hb-number.cc"
#समावेश "hb-ot-cff1-table.cc"
#समावेश "hb-ot-cff2-table.cc"
#समावेश "hb-ot-color.cc"
#समावेश "hb-ot-face.cc"
#समावेश "hb-ot-fetch.cc"
#समावेश "hb-ot-font.cc"
#समावेश "hb-ot-layout.cc"
#समावेश "hb-ot-map.cc"
#समावेश "hb-ot-math.cc"
#समावेश "hb-ot-meta.cc"
#समावेश "hb-ot-metrics.cc"
#समावेश "hb-ot-name.cc"
#समावेश "hb-ot-shape-fallback.cc"
#समावेश "hb-ot-shape-normalize.cc"
#समावेश "hb-ot-shape.cc"
#समावेश "hb-ot-shaper-arabic.cc"
#समावेश "hb-ot-shaper-default.cc"
#समावेश "hb-ot-shaper-hangul.cc"
#समावेश "hb-ot-shaper-hebrew.cc"
#समावेश "hb-ot-shaper-indic-table.cc"
#समावेश "hb-ot-shaper-indic.cc"
#समावेश "hb-ot-shaper-khmer.cc"
#समावेश "hb-ot-shaper-myanmar.cc"
#समावेश "hb-ot-shaper-syllabic.cc"
#समावेश "hb-ot-shaper-thai.cc"
#समावेश "hb-ot-shaper-use.cc"
#समावेश "hb-ot-shaper-vowel-constraints.cc"
#समावेश "hb-ot-tag.cc"
#समावेश "hb-ot-var.cc"
#समावेश "hb-outline.cc"
#समावेश "hb-paint-bounded.cc"
#समावेश "hb-paint-extents.cc"
#समावेश "hb-paint.cc"
#समावेश "hb-set.cc"
#समावेश "hb-shape-plan.cc"
#समावेश "hb-shape.cc"
#समावेश "hb-shaper.cc"
#समावेश "hb-static.cc"
#समावेश "hb-style.cc"
#समावेश "hb-ucd.cc"
#समावेश "hb-unicode.cc"

#अगर_घोषित HB_HAS_CAIRO
#समावेश "hb-cairo-utils.cc"
#समावेश "hb-cairo.cc"
#समावेश "hb-static.cc"
#पूर्ण_अगर

#अगर_घोषित HB_HAS_CORETEXT
#समावेश "hb-coretext-font.cc"
#समावेश "hb-coretext-shape.cc"
#समावेश "hb-coretext.cc"
#पूर्ण_अगर

#अगर_घोषित HB_HAS_DIRECTWRITE
#समावेश "hb-directwrite-font.cc"
#समावेश "hb-directwrite-shape.cc"
#समावेश "hb-directwrite.cc"
#पूर्ण_अगर

#अगर_घोषित HB_HAS_FREETYPE
#समावेश "hb-ft.cc"
#पूर्ण_अगर

#अगर_घोषित HB_HAS_GDI
#अगर_अघोषित HAVE_GDI
#समावेश "hb-gdi.cc"
#पूर्ण_अगर
#पूर्ण_अगर

#अगर_घोषित HB_HAS_GLIB
#समावेश "hb-glib.cc"
#पूर्ण_अगर

#अगर_घोषित HB_HAS_GRAPHITE
#समावेश "hb-graphite2.cc"
#पूर्ण_अगर

#अगर_घोषित HB_HAS_GPU
#समावेश "hb-gpu-draw.cc"
#समावेश "hb-gpu-paint.cc"
#समावेश "hb-gpu.cc"
#समावेश "hb-static.cc"
#पूर्ण_अगर

#अगर_घोषित HB_HAS_ICU
#समावेश "hb-icu.cc"
#पूर्ण_अगर

#अगर_घोषित HB_HAS_RASTER
#समावेश "hb-raster-draw.cc"
#समावेश "hb-raster-image.cc"
#समावेश "hb-raster-paint.cc"
#समावेश "hb-static.cc"
#पूर्ण_अगर

#अगर_घोषित HB_HAS_SUBSET
#समावेश "OT/Var/VARC/VARC.cc"
#समावेश "graph/gsubgpos-context.cc"
#समावेश "hb-depend.cc"
#समावेश "hb-number.cc"
#समावेश "hb-ot-cff1-table.cc"
#समावेश "hb-ot-cff2-table.cc"
#समावेश "hb-static.cc"
#समावेश "hb-subset-cff-common.cc"
#समावेश "hb-subset-cff1.cc"
#समावेश "hb-subset-cff2.cc"
#समावेश "hb-subset-input.cc"
#समावेश "hb-subset-instancer-iup.cc"
#समावेश "hb-subset-instancer-solver.cc"
#समावेश "hb-subset-plan-layout.cc"
#समावेश "hb-subset-plan-var.cc"
#समावेश "hb-subset-plan.cc"
#समावेश "hb-subset-serialize.cc"
#समावेश "hb-subset-table-cff.cc"
#समावेश "hb-subset-table-color.cc"
#समावेश "hb-subset-table-layout.cc"
#समावेश "hb-subset-table-other.cc"
#समावेश "hb-subset-table-var.cc"
#समावेश "hb-subset.cc"
#पूर्ण_अगर

#अगर_घोषित HB_HAS_UNISCRIBE
#समावेश "hb-uniscribe.cc"
#पूर्ण_अगर

#अगर_घोषित HB_HAS_VECTOR
#समावेश "hb-static.cc"
#समावेश "hb-vector-draw.cc"
#समावेश "hb-vector-paint-pdf.cc"
#समावेश "hb-vector-paint-svg.cc"
#समावेश "hb-vector-paint.cc"
#समावेश "hb-vector-path.cc"
#समावेश "hb-vector.cc"
#समावेश "hb-zlib.cc"
#पूर्ण_अगर

#अगर_घोषित HB_HAS_WASM
#समावेश "hb-wasm-api.cc"
#समावेश "hb-wasm-shape.cc"
#पूर्ण_अगर
