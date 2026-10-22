#ifdef HB_FEATURES_H
#شامل HB_FEATURES_H
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
#شامل "OT/Var/VARC/VARC.cc"
#شامل "hb-aat-layout.cc"
#شامل "hb-aat-map.cc"
#شامل "hb-blob.cc"
#شامل "hb-buffer-serialize.cc"
#شامل "hb-buffer-verify.cc"
#شامل "hb-buffer.cc"
#شامل "hb-common.cc"
#شامل "hb-draw.cc"
#شامل "hb-face-builder.cc"
#شامل "hb-face.cc"
#شامل "hb-fallback-shape.cc"
#شامل "hb-font.cc"
#شامل "hb-map.cc"
#شامل "hb-number.cc"
#شامل "hb-ot-cff1-table.cc"
#شامل "hb-ot-cff2-table.cc"
#شامل "hb-ot-color.cc"
#شامل "hb-ot-face.cc"
#شامل "hb-ot-fetch.cc"
#شامل "hb-ot-font.cc"
#شامل "hb-ot-layout.cc"
#شامل "hb-ot-map.cc"
#شامل "hb-ot-math.cc"
#شامل "hb-ot-meta.cc"
#شامل "hb-ot-metrics.cc"
#شامل "hb-ot-name.cc"
#شامل "hb-ot-shape-fallback.cc"
#شامل "hb-ot-shape-normalize.cc"
#شامل "hb-ot-shape.cc"
#شامل "hb-ot-shaper-arabic.cc"
#شامل "hb-ot-shaper-default.cc"
#شامل "hb-ot-shaper-hangul.cc"
#شامل "hb-ot-shaper-hebrew.cc"
#شامل "hb-ot-shaper-indic-table.cc"
#شامل "hb-ot-shaper-indic.cc"
#شامل "hb-ot-shaper-khmer.cc"
#شامل "hb-ot-shaper-myanmar.cc"
#شامل "hb-ot-shaper-syllabic.cc"
#شامل "hb-ot-shaper-thai.cc"
#شامل "hb-ot-shaper-use.cc"
#شامل "hb-ot-shaper-vowel-constraints.cc"
#شامل "hb-ot-tag.cc"
#شامل "hb-ot-var.cc"
#شامل "hb-outline.cc"
#شامل "hb-paint-bounded.cc"
#شامل "hb-paint-extents.cc"
#شامل "hb-paint.cc"
#شامل "hb-set.cc"
#شامل "hb-shape-plan.cc"
#شامل "hb-shape.cc"
#شامل "hb-shaper.cc"
#شامل "hb-static.cc"
#شامل "hb-style.cc"
#شامل "hb-ucd.cc"
#شامل "hb-unicode.cc"

#ifdef HB_HAS_CAIRO
#شامل "hb-cairo-utils.cc"
#شامل "hb-cairo.cc"
#شامل "hb-static.cc"
#endif

#ifdef HB_HAS_CORETEXT
#شامل "hb-coretext-font.cc"
#شامل "hb-coretext-shape.cc"
#شامل "hb-coretext.cc"
#endif

#ifdef HB_HAS_DIRECTWRITE
#شامل "hb-directwrite-font.cc"
#شامل "hb-directwrite-shape.cc"
#شامل "hb-directwrite.cc"
#endif

#ifdef HB_HAS_FREETYPE
#شامل "hb-ft.cc"
#endif

#ifdef HB_HAS_GDI
#ifndef HAVE_GDI
#شامل "hb-gdi.cc"
#endif
#endif

#ifdef HB_HAS_GLIB
#شامل "hb-glib.cc"
#endif

#ifdef HB_HAS_GRAPHITE
#شامل "hb-graphite2.cc"
#endif

#ifdef HB_HAS_GPU
#شامل "hb-gpu-draw.cc"
#شامل "hb-gpu-paint.cc"
#شامل "hb-gpu.cc"
#شامل "hb-static.cc"
#endif

#ifdef HB_HAS_ICU
#شامل "hb-icu.cc"
#endif

#ifdef HB_HAS_RASTER
#شامل "hb-raster-draw.cc"
#شامل "hb-raster-image.cc"
#شامل "hb-raster-paint.cc"
#شامل "hb-static.cc"
#endif

#ifdef HB_HAS_SUBSET
#شامل "OT/Var/VARC/VARC.cc"
#شامل "graph/gsubgpos-context.cc"
#شامل "hb-depend.cc"
#شامل "hb-number.cc"
#شامل "hb-ot-cff1-table.cc"
#شامل "hb-ot-cff2-table.cc"
#شامل "hb-static.cc"
#شامل "hb-subset-cff-common.cc"
#شامل "hb-subset-cff1.cc"
#شامل "hb-subset-cff2.cc"
#شامل "hb-subset-input.cc"
#شامل "hb-subset-instancer-iup.cc"
#شامل "hb-subset-instancer-solver.cc"
#شامل "hb-subset-plan-layout.cc"
#شامل "hb-subset-plan-var.cc"
#شامل "hb-subset-plan.cc"
#شامل "hb-subset-serialize.cc"
#شامل "hb-subset-table-cff.cc"
#شامل "hb-subset-table-color.cc"
#شامل "hb-subset-table-layout.cc"
#شامل "hb-subset-table-other.cc"
#شامل "hb-subset-table-var.cc"
#شامل "hb-subset.cc"
#endif

#ifdef HB_HAS_UNISCRIBE
#شامل "hb-uniscribe.cc"
#endif

#ifdef HB_HAS_VECTOR
#شامل "hb-static.cc"
#شامل "hb-vector-draw.cc"
#شامل "hb-vector-paint-pdf.cc"
#شامل "hb-vector-paint-svg.cc"
#شامل "hb-vector-paint.cc"
#شامل "hb-vector-path.cc"
#شامل "hb-vector.cc"
#شامل "hb-zlib.cc"
#endif

#ifdef HB_HAS_WASM
#شامل "hb-wasm-api.cc"
#شامل "hb-wasm-shape.cc"
#endif
