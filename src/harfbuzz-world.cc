#ifdef HB_FEATURES_H
#অন্তর্ভাব HB_FEATURES_H
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
#অন্তর্ভাব "OT/Var/VARC/VARC.cc"
#অন্তর্ভাব "hb-aat-layout.cc"
#অন্তর্ভাব "hb-aat-map.cc"
#অন্তর্ভাব "hb-blob.cc"
#অন্তর্ভাব "hb-buffer-serialize.cc"
#অন্তর্ভাব "hb-buffer-verify.cc"
#অন্তর্ভাব "hb-buffer.cc"
#অন্তর্ভাব "hb-common.cc"
#অন্তর্ভাব "hb-draw.cc"
#অন্তর্ভাব "hb-face-builder.cc"
#অন্তর্ভাব "hb-face.cc"
#অন্তর্ভাব "hb-fallback-shape.cc"
#অন্তর্ভাব "hb-font.cc"
#অন্তর্ভাব "hb-map.cc"
#অন্তর্ভাব "hb-number.cc"
#অন্তর্ভাব "hb-ot-cff1-table.cc"
#অন্তর্ভাব "hb-ot-cff2-table.cc"
#অন্তর্ভাব "hb-ot-color.cc"
#অন্তর্ভাব "hb-ot-face.cc"
#অন্তর্ভাব "hb-ot-fetch.cc"
#অন্তর্ভাব "hb-ot-font.cc"
#অন্তর্ভাব "hb-ot-layout.cc"
#অন্তর্ভাব "hb-ot-map.cc"
#অন্তর্ভাব "hb-ot-math.cc"
#অন্তর্ভাব "hb-ot-meta.cc"
#অন্তর্ভাব "hb-ot-metrics.cc"
#অন্তর্ভাব "hb-ot-name.cc"
#অন্তর্ভাব "hb-ot-shape-fallback.cc"
#অন্তর্ভাব "hb-ot-shape-normalize.cc"
#অন্তর্ভাব "hb-ot-shape.cc"
#অন্তর্ভাব "hb-ot-shaper-arabic.cc"
#অন্তর্ভাব "hb-ot-shaper-default.cc"
#অন্তর্ভাব "hb-ot-shaper-hangul.cc"
#অন্তর্ভাব "hb-ot-shaper-hebrew.cc"
#অন্তর্ভাব "hb-ot-shaper-indic-table.cc"
#অন্তর্ভাব "hb-ot-shaper-indic.cc"
#অন্তর্ভাব "hb-ot-shaper-khmer.cc"
#অন্তর্ভাব "hb-ot-shaper-myanmar.cc"
#অন্তর্ভাব "hb-ot-shaper-syllabic.cc"
#অন্তর্ভাব "hb-ot-shaper-thai.cc"
#অন্তর্ভাব "hb-ot-shaper-use.cc"
#অন্তর্ভাব "hb-ot-shaper-vowel-constraints.cc"
#অন্তর্ভাব "hb-ot-tag.cc"
#অন্তর্ভাব "hb-ot-var.cc"
#অন্তর্ভাব "hb-outline.cc"
#অন্তর্ভাব "hb-paint-bounded.cc"
#অন্তর্ভাব "hb-paint-extents.cc"
#অন্তর্ভাব "hb-paint.cc"
#অন্তর্ভাব "hb-set.cc"
#অন্তর্ভাব "hb-shape-plan.cc"
#অন্তর্ভাব "hb-shape.cc"
#অন্তর্ভাব "hb-shaper.cc"
#অন্তর্ভাব "hb-static.cc"
#অন্তর্ভাব "hb-style.cc"
#অন্তর্ভাব "hb-ucd.cc"
#অন্তর্ভাব "hb-unicode.cc"

#ifdef HB_HAS_CAIRO
#অন্তর্ভাব "hb-cairo-utils.cc"
#অন্তর্ভাব "hb-cairo.cc"
#অন্তর্ভাব "hb-static.cc"
#endif

#ifdef HB_HAS_CORETEXT
#অন্তর্ভাব "hb-coretext-font.cc"
#অন্তর্ভাব "hb-coretext-shape.cc"
#অন্তর্ভাব "hb-coretext.cc"
#endif

#ifdef HB_HAS_DIRECTWRITE
#অন্তর্ভাব "hb-directwrite-font.cc"
#অন্তর্ভাব "hb-directwrite-shape.cc"
#অন্তর্ভাব "hb-directwrite.cc"
#endif

#ifdef HB_HAS_FREETYPE
#অন্তর্ভাব "hb-ft.cc"
#endif

#ifdef HB_HAS_GDI
#ifndef HAVE_GDI
#অন্তর্ভাব "hb-gdi.cc"
#endif
#endif

#ifdef HB_HAS_GLIB
#অন্তর্ভাব "hb-glib.cc"
#endif

#ifdef HB_HAS_GRAPHITE
#অন্তর্ভাব "hb-graphite2.cc"
#endif

#ifdef HB_HAS_GPU
#অন্তর্ভাব "hb-gpu-draw.cc"
#অন্তর্ভাব "hb-gpu-paint.cc"
#অন্তর্ভাব "hb-gpu.cc"
#অন্তর্ভাব "hb-static.cc"
#endif

#ifdef HB_HAS_ICU
#অন্তর্ভাব "hb-icu.cc"
#endif

#ifdef HB_HAS_RASTER
#অন্তর্ভাব "hb-raster-draw.cc"
#অন্তর্ভাব "hb-raster-image.cc"
#অন্তর্ভাব "hb-raster-paint.cc"
#অন্তর্ভাব "hb-static.cc"
#endif

#ifdef HB_HAS_SUBSET
#অন্তর্ভাব "OT/Var/VARC/VARC.cc"
#অন্তর্ভাব "graph/gsubgpos-context.cc"
#অন্তর্ভাব "hb-depend.cc"
#অন্তর্ভাব "hb-number.cc"
#অন্তর্ভাব "hb-ot-cff1-table.cc"
#অন্তর্ভাব "hb-ot-cff2-table.cc"
#অন্তর্ভাব "hb-static.cc"
#অন্তর্ভাব "hb-subset-cff-common.cc"
#অন্তর্ভাব "hb-subset-cff1.cc"
#অন্তর্ভাব "hb-subset-cff2.cc"
#অন্তর্ভাব "hb-subset-input.cc"
#অন্তর্ভাব "hb-subset-instancer-iup.cc"
#অন্তর্ভাব "hb-subset-instancer-solver.cc"
#অন্তর্ভাব "hb-subset-plan-layout.cc"
#অন্তর্ভাব "hb-subset-plan-var.cc"
#অন্তর্ভাব "hb-subset-plan.cc"
#অন্তর্ভাব "hb-subset-serialize.cc"
#অন্তর্ভাব "hb-subset-table-cff.cc"
#অন্তর্ভাব "hb-subset-table-color.cc"
#অন্তর্ভাব "hb-subset-table-layout.cc"
#অন্তর্ভাব "hb-subset-table-other.cc"
#অন্তর্ভাব "hb-subset-table-var.cc"
#অন্তর্ভাব "hb-subset.cc"
#endif

#ifdef HB_HAS_UNISCRIBE
#অন্তর্ভাব "hb-uniscribe.cc"
#endif

#ifdef HB_HAS_VECTOR
#অন্তর্ভাব "hb-static.cc"
#অন্তর্ভাব "hb-vector-draw.cc"
#অন্তর্ভাব "hb-vector-paint-pdf.cc"
#অন্তর্ভাব "hb-vector-paint-svg.cc"
#অন্তর্ভাব "hb-vector-paint.cc"
#অন্তর্ভাব "hb-vector-path.cc"
#অন্তর্ভাব "hb-vector.cc"
#অন্তর্ভাব "hb-zlib.cc"
#endif

#ifdef HB_HAS_WASM
#অন্তর্ভাব "hb-wasm-api.cc"
#অন্তর্ভাব "hb-wasm-shape.cc"
#endif
