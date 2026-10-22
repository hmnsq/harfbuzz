#ifdef HB_FEATURES_H
#அந்தர்பாவ HB_FEATURES_H
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
#அந்தர்பாவ "OT/Var/VARC/VARC.cc"
#அந்தர்பாவ "hb-aat-layout.cc"
#அந்தர்பாவ "hb-aat-map.cc"
#அந்தர்பாவ "hb-blob.cc"
#அந்தர்பாவ "hb-buffer-serialize.cc"
#அந்தர்பாவ "hb-buffer-verify.cc"
#அந்தர்பாவ "hb-buffer.cc"
#அந்தர்பாவ "hb-common.cc"
#அந்தர்பாவ "hb-draw.cc"
#அந்தர்பாவ "hb-face-builder.cc"
#அந்தர்பாவ "hb-face.cc"
#அந்தர்பாவ "hb-fallback-shape.cc"
#அந்தர்பாவ "hb-font.cc"
#அந்தர்பாவ "hb-map.cc"
#அந்தர்பாவ "hb-number.cc"
#அந்தர்பாவ "hb-ot-cff1-table.cc"
#அந்தர்பாவ "hb-ot-cff2-table.cc"
#அந்தர்பாவ "hb-ot-color.cc"
#அந்தர்பாவ "hb-ot-face.cc"
#அந்தர்பாவ "hb-ot-fetch.cc"
#அந்தர்பாவ "hb-ot-font.cc"
#அந்தர்பாவ "hb-ot-layout.cc"
#அந்தர்பாவ "hb-ot-map.cc"
#அந்தர்பாவ "hb-ot-math.cc"
#அந்தர்பாவ "hb-ot-meta.cc"
#அந்தர்பாவ "hb-ot-metrics.cc"
#அந்தர்பாவ "hb-ot-name.cc"
#அந்தர்பாவ "hb-ot-shape-fallback.cc"
#அந்தர்பாவ "hb-ot-shape-normalize.cc"
#அந்தர்பாவ "hb-ot-shape.cc"
#அந்தர்பாவ "hb-ot-shaper-arabic.cc"
#அந்தர்பாவ "hb-ot-shaper-default.cc"
#அந்தர்பாவ "hb-ot-shaper-hangul.cc"
#அந்தர்பாவ "hb-ot-shaper-hebrew.cc"
#அந்தர்பாவ "hb-ot-shaper-indic-table.cc"
#அந்தர்பாவ "hb-ot-shaper-indic.cc"
#அந்தர்பாவ "hb-ot-shaper-khmer.cc"
#அந்தர்பாவ "hb-ot-shaper-myanmar.cc"
#அந்தர்பாவ "hb-ot-shaper-syllabic.cc"
#அந்தர்பாவ "hb-ot-shaper-thai.cc"
#அந்தர்பாவ "hb-ot-shaper-use.cc"
#அந்தர்பாவ "hb-ot-shaper-vowel-constraints.cc"
#அந்தர்பாவ "hb-ot-tag.cc"
#அந்தர்பாவ "hb-ot-var.cc"
#அந்தர்பாவ "hb-outline.cc"
#அந்தர்பாவ "hb-paint-bounded.cc"
#அந்தர்பாவ "hb-paint-extents.cc"
#அந்தர்பாவ "hb-paint.cc"
#அந்தர்பாவ "hb-set.cc"
#அந்தர்பாவ "hb-shape-plan.cc"
#அந்தர்பாவ "hb-shape.cc"
#அந்தர்பாவ "hb-shaper.cc"
#அந்தர்பாவ "hb-static.cc"
#அந்தர்பாவ "hb-style.cc"
#அந்தர்பாவ "hb-ucd.cc"
#அந்தர்பாவ "hb-unicode.cc"

#ifdef HB_HAS_CAIRO
#அந்தர்பாவ "hb-cairo-utils.cc"
#அந்தர்பாவ "hb-cairo.cc"
#அந்தர்பாவ "hb-static.cc"
#endif

#ifdef HB_HAS_CORETEXT
#அந்தர்பாவ "hb-coretext-font.cc"
#அந்தர்பாவ "hb-coretext-shape.cc"
#அந்தர்பாவ "hb-coretext.cc"
#endif

#ifdef HB_HAS_DIRECTWRITE
#அந்தர்பாவ "hb-directwrite-font.cc"
#அந்தர்பாவ "hb-directwrite-shape.cc"
#அந்தர்பாவ "hb-directwrite.cc"
#endif

#ifdef HB_HAS_FREETYPE
#அந்தர்பாவ "hb-ft.cc"
#endif

#ifdef HB_HAS_GDI
#ifndef HAVE_GDI
#அந்தர்பாவ "hb-gdi.cc"
#endif
#endif

#ifdef HB_HAS_GLIB
#அந்தர்பாவ "hb-glib.cc"
#endif

#ifdef HB_HAS_GRAPHITE
#அந்தர்பாவ "hb-graphite2.cc"
#endif

#ifdef HB_HAS_GPU
#அந்தர்பாவ "hb-gpu-draw.cc"
#அந்தர்பாவ "hb-gpu-paint.cc"
#அந்தர்பாவ "hb-gpu.cc"
#அந்தர்பாவ "hb-static.cc"
#endif

#ifdef HB_HAS_ICU
#அந்தர்பாவ "hb-icu.cc"
#endif

#ifdef HB_HAS_RASTER
#அந்தர்பாவ "hb-raster-draw.cc"
#அந்தர்பாவ "hb-raster-image.cc"
#அந்தர்பாவ "hb-raster-paint.cc"
#அந்தர்பாவ "hb-static.cc"
#endif

#ifdef HB_HAS_SUBSET
#அந்தர்பாவ "OT/Var/VARC/VARC.cc"
#அந்தர்பாவ "graph/gsubgpos-context.cc"
#அந்தர்பாவ "hb-depend.cc"
#அந்தர்பாவ "hb-number.cc"
#அந்தர்பாவ "hb-ot-cff1-table.cc"
#அந்தர்பாவ "hb-ot-cff2-table.cc"
#அந்தர்பாவ "hb-static.cc"
#அந்தர்பாவ "hb-subset-cff-common.cc"
#அந்தர்பாவ "hb-subset-cff1.cc"
#அந்தர்பாவ "hb-subset-cff2.cc"
#அந்தர்பாவ "hb-subset-input.cc"
#அந்தர்பாவ "hb-subset-instancer-iup.cc"
#அந்தர்பாவ "hb-subset-instancer-solver.cc"
#அந்தர்பாவ "hb-subset-plan-layout.cc"
#அந்தர்பாவ "hb-subset-plan-var.cc"
#அந்தர்பாவ "hb-subset-plan.cc"
#அந்தர்பாவ "hb-subset-serialize.cc"
#அந்தர்பாவ "hb-subset-table-cff.cc"
#அந்தர்பாவ "hb-subset-table-color.cc"
#அந்தர்பாவ "hb-subset-table-layout.cc"
#அந்தர்பாவ "hb-subset-table-other.cc"
#அந்தர்பாவ "hb-subset-table-var.cc"
#அந்தர்பாவ "hb-subset.cc"
#endif

#ifdef HB_HAS_UNISCRIBE
#அந்தர்பாவ "hb-uniscribe.cc"
#endif

#ifdef HB_HAS_VECTOR
#அந்தர்பாவ "hb-static.cc"
#அந்தர்பாவ "hb-vector-draw.cc"
#அந்தர்பாவ "hb-vector-paint-pdf.cc"
#அந்தர்பாவ "hb-vector-paint-svg.cc"
#அந்தர்பாவ "hb-vector-paint.cc"
#அந்தர்பாவ "hb-vector-path.cc"
#அந்தர்பாவ "hb-vector.cc"
#அந்தர்பாவ "hb-zlib.cc"
#endif

#ifdef HB_HAS_WASM
#அந்தர்பாவ "hb-wasm-api.cc"
#அந்தர்பாவ "hb-wasm-shape.cc"
#endif
