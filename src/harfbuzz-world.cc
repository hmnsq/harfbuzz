#ifdef HB_FEATURES_H
#अन्तर्भाव HB_FEATURES_H
#endif

#ifdef HB_HAS_CORETEXT
#define HAVE_CORETEXT 1
#endif
#ifdef HB_HAS_DIRECTWRITE
#define HAVE_DIRECTWRITE 1
#endif
#ifdef HB_HAS_FREETYPE
#define HAVE_FREETYPE 1
#endif
#ifdef HB_HAS_GDI
#define HAVE_GDI 1
#endif
#ifdef HB_HAS_GLIB
#define HAVE_GLIB 1
#endif
#ifdef HB_HAS_GRAPHITE
#define HAVE_GRAPHITE 1
#endif
#ifdef HB_HAS_UNISCRIBE
#define HAVE_UNISCRIBE 1
#define HAVE_GDI 1
#endif
#ifdef HB_HAS_WASM
#define HAVE_WASM 1
#endif


/* Core library. */
#अन्तर्भाव "OT/Var/VARC/VARC.cc"
#अन्तर्भाव "hb-aat-layout.cc"
#अन्तर्भाव "hb-aat-map.cc"
#अन्तर्भाव "hb-blob.cc"
#अन्तर्भाव "hb-buffer-serialize.cc"
#अन्तर्भाव "hb-buffer-verify.cc"
#अन्तर्भाव "hb-buffer.cc"
#अन्तर्भाव "hb-common.cc"
#अन्तर्भाव "hb-draw.cc"
#अन्तर्भाव "hb-face-builder.cc"
#अन्तर्भाव "hb-face.cc"
#अन्तर्भाव "hb-fallback-shape.cc"
#अन्तर्भाव "hb-font.cc"
#अन्तर्भाव "hb-map.cc"
#अन्तर्भाव "hb-number.cc"
#अन्तर्भाव "hb-ot-cff1-table.cc"
#अन्तर्भाव "hb-ot-cff2-table.cc"
#अन्तर्भाव "hb-ot-color.cc"
#अन्तर्भाव "hb-ot-face.cc"
#अन्तर्भाव "hb-ot-fetch.cc"
#अन्तर्भाव "hb-ot-font.cc"
#अन्तर्भाव "hb-ot-layout.cc"
#अन्तर्भाव "hb-ot-map.cc"
#अन्तर्भाव "hb-ot-math.cc"
#अन्तर्भाव "hb-ot-meta.cc"
#अन्तर्भाव "hb-ot-metrics.cc"
#अन्तर्भाव "hb-ot-name.cc"
#अन्तर्भाव "hb-ot-shape-fallback.cc"
#अन्तर्भाव "hb-ot-shape-normalize.cc"
#अन्तर्भाव "hb-ot-shape.cc"
#अन्तर्भाव "hb-ot-shaper-arabic.cc"
#अन्तर्भाव "hb-ot-shaper-default.cc"
#अन्तर्भाव "hb-ot-shaper-hangul.cc"
#अन्तर्भाव "hb-ot-shaper-hebrew.cc"
#अन्तर्भाव "hb-ot-shaper-indic-table.cc"
#अन्तर्भाव "hb-ot-shaper-indic.cc"
#अन्तर्भाव "hb-ot-shaper-khmer.cc"
#अन्तर्भाव "hb-ot-shaper-myanmar.cc"
#अन्तर्भाव "hb-ot-shaper-syllabic.cc"
#अन्तर्भाव "hb-ot-shaper-thai.cc"
#अन्तर्भाव "hb-ot-shaper-use.cc"
#अन्तर्भाव "hb-ot-shaper-vowel-constraints.cc"
#अन्तर्भाव "hb-ot-tag.cc"
#अन्तर्भाव "hb-ot-var.cc"
#अन्तर्भाव "hb-outline.cc"
#अन्तर्भाव "hb-paint-bounded.cc"
#अन्तर्भाव "hb-paint-extents.cc"
#अन्तर्भाव "hb-paint.cc"
#अन्तर्भाव "hb-set.cc"
#अन्तर्भाव "hb-shape-plan.cc"
#अन्तर्भाव "hb-shape.cc"
#अन्तर्भाव "hb-shaper.cc"
#अन्तर्भाव "hb-static.cc"
#अन्तर्भाव "hb-style.cc"
#अन्तर्भाव "hb-ucd.cc"
#अन्तर्भाव "hb-unicode.cc"

#ifdef HB_HAS_CAIRO
#अन्तर्भाव "hb-cairo-utils.cc"
#अन्तर्भाव "hb-cairo.cc"
#अन्तर्भाव "hb-static.cc"
#endif

#ifdef HB_HAS_CORETEXT
#अन्तर्भाव "hb-coretext-font.cc"
#अन्तर्भाव "hb-coretext-shape.cc"
#अन्तर्भाव "hb-coretext.cc"
#endif

#ifdef HB_HAS_DIRECTWRITE
#अन्तर्भाव "hb-directwrite-font.cc"
#अन्तर्भाव "hb-directwrite-shape.cc"
#अन्तर्भाव "hb-directwrite.cc"
#endif

#ifdef HB_HAS_FREETYPE
#अन्तर्भाव "hb-ft.cc"
#endif

#ifdef HB_HAS_GDI
#ifndef HAVE_GDI
#अन्तर्भाव "hb-gdi.cc"
#endif
#endif

#ifdef HB_HAS_GLIB
#अन्तर्भाव "hb-glib.cc"
#endif

#ifdef HB_HAS_GRAPHITE
#अन्तर्भाव "hb-graphite2.cc"
#endif

#ifdef HB_HAS_GPU
#अन्तर्भाव "hb-gpu-draw.cc"
#अन्तर्भाव "hb-gpu-paint.cc"
#अन्तर्भाव "hb-gpu.cc"
#अन्तर्भाव "hb-static.cc"
#endif

#ifdef HB_HAS_ICU
#अन्तर्भाव "hb-icu.cc"
#endif

#ifdef HB_HAS_RASTER
#अन्तर्भाव "hb-raster-draw.cc"
#अन्तर्भाव "hb-raster-image.cc"
#अन्तर्भाव "hb-raster-paint.cc"
#अन्तर्भाव "hb-static.cc"
#endif

#ifdef HB_HAS_SUBSET
#अन्तर्भाव "OT/Var/VARC/VARC.cc"
#अन्तर्भाव "graph/gsubgpos-context.cc"
#अन्तर्भाव "hb-depend.cc"
#अन्तर्भाव "hb-number.cc"
#अन्तर्भाव "hb-ot-cff1-table.cc"
#अन्तर्भाव "hb-ot-cff2-table.cc"
#अन्तर्भाव "hb-static.cc"
#अन्तर्भाव "hb-subset-cff-common.cc"
#अन्तर्भाव "hb-subset-cff1.cc"
#अन्तर्भाव "hb-subset-cff2.cc"
#अन्तर्भाव "hb-subset-input.cc"
#अन्तर्भाव "hb-subset-instancer-iup.cc"
#अन्तर्भाव "hb-subset-instancer-solver.cc"
#अन्तर्भाव "hb-subset-plan-layout.cc"
#अन्तर्भाव "hb-subset-plan-var.cc"
#अन्तर्भाव "hb-subset-plan.cc"
#अन्तर्भाव "hb-subset-serialize.cc"
#अन्तर्भाव "hb-subset-table-cff.cc"
#अन्तर्भाव "hb-subset-table-color.cc"
#अन्तर्भाव "hb-subset-table-layout.cc"
#अन्तर्भाव "hb-subset-table-other.cc"
#अन्तर्भाव "hb-subset-table-var.cc"
#अन्तर्भाव "hb-subset.cc"
#endif

#ifdef HB_HAS_UNISCRIBE
#अन्तर्भाव "hb-uniscribe.cc"
#endif

#ifdef HB_HAS_VECTOR
#अन्तर्भाव "hb-static.cc"
#अन्तर्भाव "hb-vector-draw.cc"
#अन्तर्भाव "hb-vector-paint-pdf.cc"
#अन्तर्भाव "hb-vector-paint-svg.cc"
#अन्तर्भाव "hb-vector-paint.cc"
#अन्तर्भाव "hb-vector-path.cc"
#अन्तर्भाव "hb-vector.cc"
#अन्तर्भाव "hb-zlib.cc"
#endif

#ifdef HB_HAS_WASM
#अन्तर्भाव "hb-wasm-api.cc"
#अन्तर्भाव "hb-wasm-shape.cc"
#endif
