#اگر_تعریف_شده HB_FEATURES_H
#ضمن HB_FEATURES_H
#پایان_اگر

#اگر_تعریف_شده HB_HAS_CORETEXT
#تعریف_کن HAVE_CORETEXT 1
#پایان_اگر
#اگر_تعریف_شده HB_HAS_DIRECTWRITE
#تعریف_کن HAVE_DIRECTWRITE 1
#پایان_اگر
#اگر_تعریف_شده HB_HAS_FREETYPE
#تعریف_کن HAVE_FREETYPE 1
#پایان_اگر
#اگر_تعریف_شده HB_HAS_GDI
#تعریف_کن HAVE_GDI 1
#پایان_اگر
#اگر_تعریف_شده HB_HAS_GLIB
#تعریف_کن HAVE_GLIB 1
#پایان_اگر
#اگر_تعریف_شده HB_HAS_GRAPHITE
#تعریف_کن HAVE_GRAPHITE 1
#پایان_اگر
#اگر_تعریف_شده HB_HAS_UNISCRIBE
#تعریف_کن HAVE_UNISCRIBE 1
#تعریف_کن HAVE_GDI 1
#پایان_اگر
#اگر_تعریف_شده HB_HAS_WASM
#تعریف_کن HAVE_WASM 1
#پایان_اگر


/* Core library. */
#ضمن "OT/Var/VARC/VARC.cc"
#ضمن "hb-aat-layout.cc"
#ضمن "hb-aat-map.cc"
#ضمن "hb-blob.cc"
#ضمن "hb-buffer-serialize.cc"
#ضمن "hb-buffer-verify.cc"
#ضمن "hb-buffer.cc"
#ضمن "hb-common.cc"
#ضمن "hb-draw.cc"
#ضمن "hb-face-builder.cc"
#ضمن "hb-face.cc"
#ضمن "hb-fallback-shape.cc"
#ضمن "hb-font.cc"
#ضمن "hb-map.cc"
#ضمن "hb-number.cc"
#ضمن "hb-ot-cff1-table.cc"
#ضمن "hb-ot-cff2-table.cc"
#ضمن "hb-ot-color.cc"
#ضمن "hb-ot-face.cc"
#ضمن "hb-ot-fetch.cc"
#ضمن "hb-ot-font.cc"
#ضمن "hb-ot-layout.cc"
#ضمن "hb-ot-map.cc"
#ضمن "hb-ot-math.cc"
#ضمن "hb-ot-meta.cc"
#ضمن "hb-ot-metrics.cc"
#ضمن "hb-ot-name.cc"
#ضمن "hb-ot-shape-fallback.cc"
#ضمن "hb-ot-shape-normalize.cc"
#ضمن "hb-ot-shape.cc"
#ضمن "hb-ot-shaper-arabic.cc"
#ضمن "hb-ot-shaper-default.cc"
#ضمن "hb-ot-shaper-hangul.cc"
#ضمن "hb-ot-shaper-hebrew.cc"
#ضمن "hb-ot-shaper-indic-table.cc"
#ضمن "hb-ot-shaper-indic.cc"
#ضمن "hb-ot-shaper-khmer.cc"
#ضمن "hb-ot-shaper-myanmar.cc"
#ضمن "hb-ot-shaper-syllabic.cc"
#ضمن "hb-ot-shaper-thai.cc"
#ضمن "hb-ot-shaper-use.cc"
#ضمن "hb-ot-shaper-vowel-constraints.cc"
#ضمن "hb-ot-tag.cc"
#ضمن "hb-ot-var.cc"
#ضمن "hb-outline.cc"
#ضمن "hb-paint-bounded.cc"
#ضمن "hb-paint-extents.cc"
#ضمن "hb-paint.cc"
#ضمن "hb-set.cc"
#ضمن "hb-shape-plan.cc"
#ضمن "hb-shape.cc"
#ضمن "hb-shaper.cc"
#ضمن "hb-static.cc"
#ضمن "hb-style.cc"
#ضمن "hb-ucd.cc"
#ضمن "hb-unicode.cc"

#اگر_تعریف_شده HB_HAS_CAIRO
#ضمن "hb-cairo-utils.cc"
#ضمن "hb-cairo.cc"
#ضمن "hb-static.cc"
#پایان_اگر

#اگر_تعریف_شده HB_HAS_CORETEXT
#ضمن "hb-coretext-font.cc"
#ضمن "hb-coretext-shape.cc"
#ضمن "hb-coretext.cc"
#پایان_اگر

#اگر_تعریف_شده HB_HAS_DIRECTWRITE
#ضمن "hb-directwrite-font.cc"
#ضمن "hb-directwrite-shape.cc"
#ضمن "hb-directwrite.cc"
#پایان_اگر

#اگر_تعریف_شده HB_HAS_FREETYPE
#ضمن "hb-ft.cc"
#پایان_اگر

#اگر_تعریف_شده HB_HAS_GDI
#اگر_تعریف_نشده HAVE_GDI
#ضمن "hb-gdi.cc"
#پایان_اگر
#پایان_اگر

#اگر_تعریف_شده HB_HAS_GLIB
#ضمن "hb-glib.cc"
#پایان_اگر

#اگر_تعریف_شده HB_HAS_GRAPHITE
#ضمن "hb-graphite2.cc"
#پایان_اگر

#اگر_تعریف_شده HB_HAS_GPU
#ضمن "hb-gpu-draw.cc"
#ضمن "hb-gpu-paint.cc"
#ضمن "hb-gpu.cc"
#ضمن "hb-static.cc"
#پایان_اگر

#اگر_تعریف_شده HB_HAS_ICU
#ضمن "hb-icu.cc"
#پایان_اگر

#اگر_تعریف_شده HB_HAS_RASTER
#ضمن "hb-raster-draw.cc"
#ضمن "hb-raster-image.cc"
#ضمن "hb-raster-paint.cc"
#ضمن "hb-static.cc"
#پایان_اگر

#اگر_تعریف_شده HB_HAS_SUBSET
#ضمن "OT/Var/VARC/VARC.cc"
#ضمن "graph/gsubgpos-context.cc"
#ضمن "hb-depend.cc"
#ضمن "hb-number.cc"
#ضمن "hb-ot-cff1-table.cc"
#ضمن "hb-ot-cff2-table.cc"
#ضمن "hb-static.cc"
#ضمن "hb-subset-cff-common.cc"
#ضمن "hb-subset-cff1.cc"
#ضمن "hb-subset-cff2.cc"
#ضمن "hb-subset-input.cc"
#ضمن "hb-subset-instancer-iup.cc"
#ضمن "hb-subset-instancer-solver.cc"
#ضمن "hb-subset-plan-layout.cc"
#ضمن "hb-subset-plan-var.cc"
#ضمن "hb-subset-plan.cc"
#ضمن "hb-subset-serialize.cc"
#ضمن "hb-subset-table-cff.cc"
#ضمن "hb-subset-table-color.cc"
#ضمن "hb-subset-table-layout.cc"
#ضمن "hb-subset-table-other.cc"
#ضمن "hb-subset-table-var.cc"
#ضمن "hb-subset.cc"
#پایان_اگر

#اگر_تعریف_شده HB_HAS_UNISCRIBE
#ضمن "hb-uniscribe.cc"
#پایان_اگر

#اگر_تعریف_شده HB_HAS_وکتور
#ضمن "hb-static.cc"
#ضمن "hb-vector-draw.cc"
#ضمن "hb-vector-paint-pdf.cc"
#ضمن "hb-vector-paint-svg.cc"
#ضمن "hb-vector-paint.cc"
#ضمن "hb-vector-path.cc"
#ضمن "hb-vector.cc"
#ضمن "hb-zlib.cc"
#پایان_اگر

#اگر_تعریف_شده HB_HAS_WASM
#ضمن "hb-wasm-api.cc"
#ضمن "hb-wasm-shape.cc"
#پایان_اگر
