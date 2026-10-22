#ifdef HB_FEATURES_H
#અન્તર્ભાવ HB_FEATURES_H
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
#અન્તર્ભાવ "OT/Var/VARC/VARC.cc"
#અન્તર્ભાવ "hb-aat-layout.cc"
#અન્તર્ભાવ "hb-aat-map.cc"
#અન્તર્ભાવ "hb-blob.cc"
#અન્તર્ભાવ "hb-buffer-serialize.cc"
#અન્તર્ભાવ "hb-buffer-verify.cc"
#અન્તર્ભાવ "hb-buffer.cc"
#અન્તર્ભાવ "hb-common.cc"
#અન્તર્ભાવ "hb-draw.cc"
#અન્તર્ભાવ "hb-face-builder.cc"
#અન્તર્ભાવ "hb-face.cc"
#અન્તર્ભાવ "hb-fallback-shape.cc"
#અન્તર્ભાવ "hb-font.cc"
#અન્તર્ભાવ "hb-map.cc"
#અન્તર્ભાવ "hb-number.cc"
#અન્તર્ભાવ "hb-ot-cff1-table.cc"
#અન્તર્ભાવ "hb-ot-cff2-table.cc"
#અન્તર્ભાવ "hb-ot-color.cc"
#અન્તર્ભાવ "hb-ot-face.cc"
#અન્તર્ભાવ "hb-ot-fetch.cc"
#અન્તર્ભાવ "hb-ot-font.cc"
#અન્તર્ભાવ "hb-ot-layout.cc"
#અન્તર્ભાવ "hb-ot-map.cc"
#અન્તર્ભાવ "hb-ot-math.cc"
#અન્તર્ભાવ "hb-ot-meta.cc"
#અન્તર્ભાવ "hb-ot-metrics.cc"
#અન્તર્ભાવ "hb-ot-name.cc"
#અન્તર્ભાવ "hb-ot-shape-fallback.cc"
#અન્તર્ભાવ "hb-ot-shape-normalize.cc"
#અન્તર્ભાવ "hb-ot-shape.cc"
#અન્તર્ભાવ "hb-ot-shaper-arabic.cc"
#અન્તર્ભાવ "hb-ot-shaper-default.cc"
#અન્તર્ભાવ "hb-ot-shaper-hangul.cc"
#અન્તર્ભાવ "hb-ot-shaper-hebrew.cc"
#અન્તર્ભાવ "hb-ot-shaper-indic-table.cc"
#અન્તર્ભાવ "hb-ot-shaper-indic.cc"
#અન્તર્ભાવ "hb-ot-shaper-khmer.cc"
#અન્તર્ભાવ "hb-ot-shaper-myanmar.cc"
#અન્તર્ભાવ "hb-ot-shaper-syllabic.cc"
#અન્તર્ભાવ "hb-ot-shaper-thai.cc"
#અન્તર્ભાવ "hb-ot-shaper-use.cc"
#અન્તર્ભાવ "hb-ot-shaper-vowel-constraints.cc"
#અન્તર્ભાવ "hb-ot-tag.cc"
#અન્તર્ભાવ "hb-ot-var.cc"
#અન્તર્ભાવ "hb-outline.cc"
#અન્તર્ભાવ "hb-paint-bounded.cc"
#અન્તર્ભાવ "hb-paint-extents.cc"
#અન્તર્ભાવ "hb-paint.cc"
#અન્તર્ભાવ "hb-set.cc"
#અન્તર્ભાવ "hb-shape-plan.cc"
#અન્તર્ભાવ "hb-shape.cc"
#અન્તર્ભાવ "hb-shaper.cc"
#અન્તર્ભાવ "hb-static.cc"
#અન્તર્ભાવ "hb-style.cc"
#અન્તર્ભાવ "hb-ucd.cc"
#અન્તર્ભાવ "hb-unicode.cc"

#ifdef HB_HAS_CAIRO
#અન્તર્ભાવ "hb-cairo-utils.cc"
#અન્તર્ભાવ "hb-cairo.cc"
#અન્તર્ભાવ "hb-static.cc"
#endif

#ifdef HB_HAS_CORETEXT
#અન્તર્ભાવ "hb-coretext-font.cc"
#અન્તર્ભાવ "hb-coretext-shape.cc"
#અન્તર્ભાવ "hb-coretext.cc"
#endif

#ifdef HB_HAS_DIRECTWRITE
#અન્તર્ભાવ "hb-directwrite-font.cc"
#અન્તર્ભાવ "hb-directwrite-shape.cc"
#અન્તર્ભાવ "hb-directwrite.cc"
#endif

#ifdef HB_HAS_FREETYPE
#અન્તર્ભાવ "hb-ft.cc"
#endif

#ifdef HB_HAS_GDI
#ifndef HAVE_GDI
#અન્તર્ભાવ "hb-gdi.cc"
#endif
#endif

#ifdef HB_HAS_GLIB
#અન્તર્ભાવ "hb-glib.cc"
#endif

#ifdef HB_HAS_GRAPHITE
#અન્તર્ભાવ "hb-graphite2.cc"
#endif

#ifdef HB_HAS_GPU
#અન્તર્ભાવ "hb-gpu-draw.cc"
#અન્તર્ભાવ "hb-gpu-paint.cc"
#અન્તર્ભાવ "hb-gpu.cc"
#અન્તર્ભાવ "hb-static.cc"
#endif

#ifdef HB_HAS_ICU
#અન્તર્ભાવ "hb-icu.cc"
#endif

#ifdef HB_HAS_RASTER
#અન્તર્ભાવ "hb-raster-draw.cc"
#અન્તર્ભાવ "hb-raster-image.cc"
#અન્તર્ભાવ "hb-raster-paint.cc"
#અન્તર્ભાવ "hb-static.cc"
#endif

#ifdef HB_HAS_SUBSET
#અન્તર્ભાવ "OT/Var/VARC/VARC.cc"
#અન્તર્ભાવ "graph/gsubgpos-context.cc"
#અન્તર્ભાવ "hb-depend.cc"
#અન્તર્ભાવ "hb-number.cc"
#અન્તર્ભાવ "hb-ot-cff1-table.cc"
#અન્તર્ભાવ "hb-ot-cff2-table.cc"
#અન્તર્ભાવ "hb-static.cc"
#અન્તર્ભાવ "hb-subset-cff-common.cc"
#અન્તર્ભાવ "hb-subset-cff1.cc"
#અન્તર્ભાવ "hb-subset-cff2.cc"
#અન્તર્ભાવ "hb-subset-input.cc"
#અન્તર્ભાવ "hb-subset-instancer-iup.cc"
#અન્તર્ભાવ "hb-subset-instancer-solver.cc"
#અન્તર્ભાવ "hb-subset-plan-layout.cc"
#અન્તર્ભાવ "hb-subset-plan-var.cc"
#અન્તર્ભાવ "hb-subset-plan.cc"
#અન્તર્ભાવ "hb-subset-serialize.cc"
#અન્તર્ભાવ "hb-subset-table-cff.cc"
#અન્તર્ભાવ "hb-subset-table-color.cc"
#અન્તર્ભાવ "hb-subset-table-layout.cc"
#અન્તર્ભાવ "hb-subset-table-other.cc"
#અન્તર્ભાવ "hb-subset-table-var.cc"
#અન્તર્ભાવ "hb-subset.cc"
#endif

#ifdef HB_HAS_UNISCRIBE
#અન્તર્ભાવ "hb-uniscribe.cc"
#endif

#ifdef HB_HAS_VECTOR
#અન્તર્ભાવ "hb-static.cc"
#અન્તર્ભાવ "hb-vector-draw.cc"
#અન્તર્ભાવ "hb-vector-paint-pdf.cc"
#અન્તર્ભાવ "hb-vector-paint-svg.cc"
#અન્તર્ભાવ "hb-vector-paint.cc"
#અન્તર્ભાવ "hb-vector-path.cc"
#અન્તર્ભાવ "hb-vector.cc"
#અન્તર્ભાવ "hb-zlib.cc"
#endif

#ifdef HB_HAS_WASM
#અન્તર્ભાવ "hb-wasm-api.cc"
#અન્તર્ભાવ "hb-wasm-shape.cc"
#endif
