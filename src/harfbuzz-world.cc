#ifdef HB_FEATURES_H
#അന്തര്ഭാവ HB_FEATURES_H
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
#അന്തര്ഭാവ "OT/Var/VARC/VARC.cc"
#അന്തര്ഭാവ "hb-aat-layout.cc"
#അന്തര്ഭാവ "hb-aat-map.cc"
#അന്തര്ഭാവ "hb-blob.cc"
#അന്തര്ഭാവ "hb-buffer-serialize.cc"
#അന്തര്ഭാവ "hb-buffer-verify.cc"
#അന്തര്ഭാവ "hb-buffer.cc"
#അന്തര്ഭാവ "hb-common.cc"
#അന്തര്ഭാവ "hb-draw.cc"
#അന്തര്ഭാവ "hb-face-builder.cc"
#അന്തര്ഭാവ "hb-face.cc"
#അന്തര്ഭാവ "hb-fallback-shape.cc"
#അന്തര്ഭാവ "hb-font.cc"
#അന്തര്ഭാവ "hb-map.cc"
#അന്തര്ഭാവ "hb-number.cc"
#അന്തര്ഭാവ "hb-ot-cff1-table.cc"
#അന്തര്ഭാവ "hb-ot-cff2-table.cc"
#അന്തര്ഭാവ "hb-ot-color.cc"
#അന്തര്ഭാവ "hb-ot-face.cc"
#അന്തര്ഭാവ "hb-ot-fetch.cc"
#അന്തര്ഭാവ "hb-ot-font.cc"
#അന്തര്ഭാവ "hb-ot-layout.cc"
#അന്തര്ഭാവ "hb-ot-map.cc"
#അന്തര്ഭാവ "hb-ot-math.cc"
#അന്തര്ഭാവ "hb-ot-meta.cc"
#അന്തര്ഭാവ "hb-ot-metrics.cc"
#അന്തര്ഭാവ "hb-ot-name.cc"
#അന്തര്ഭാവ "hb-ot-shape-fallback.cc"
#അന്തര്ഭാവ "hb-ot-shape-normalize.cc"
#അന്തര്ഭാവ "hb-ot-shape.cc"
#അന്തര്ഭാവ "hb-ot-shaper-arabic.cc"
#അന്തര്ഭാവ "hb-ot-shaper-default.cc"
#അന്തര്ഭാവ "hb-ot-shaper-hangul.cc"
#അന്തര്ഭാവ "hb-ot-shaper-hebrew.cc"
#അന്തര്ഭാവ "hb-ot-shaper-indic-table.cc"
#അന്തര്ഭാവ "hb-ot-shaper-indic.cc"
#അന്തര്ഭാവ "hb-ot-shaper-khmer.cc"
#അന്തര്ഭാവ "hb-ot-shaper-myanmar.cc"
#അന്തര്ഭാവ "hb-ot-shaper-syllabic.cc"
#അന്തര്ഭാവ "hb-ot-shaper-thai.cc"
#അന്തര്ഭാവ "hb-ot-shaper-use.cc"
#അന്തര്ഭാവ "hb-ot-shaper-vowel-constraints.cc"
#അന്തര്ഭാവ "hb-ot-tag.cc"
#അന്തര്ഭാവ "hb-ot-var.cc"
#അന്തര്ഭാവ "hb-outline.cc"
#അന്തര്ഭാവ "hb-paint-bounded.cc"
#അന്തര്ഭാവ "hb-paint-extents.cc"
#അന്തര്ഭാവ "hb-paint.cc"
#അന്തര്ഭാവ "hb-set.cc"
#അന്തര്ഭാവ "hb-shape-plan.cc"
#അന്തര്ഭാവ "hb-shape.cc"
#അന്തര്ഭാവ "hb-shaper.cc"
#അന്തര്ഭാവ "hb-static.cc"
#അന്തര്ഭാവ "hb-style.cc"
#അന്തര്ഭാവ "hb-ucd.cc"
#അന്തര്ഭാവ "hb-unicode.cc"

#ifdef HB_HAS_CAIRO
#അന്തര്ഭാവ "hb-cairo-utils.cc"
#അന്തര്ഭാവ "hb-cairo.cc"
#അന്തര്ഭാവ "hb-static.cc"
#endif

#ifdef HB_HAS_CORETEXT
#അന്തര്ഭാവ "hb-coretext-font.cc"
#അന്തര്ഭാവ "hb-coretext-shape.cc"
#അന്തര്ഭാവ "hb-coretext.cc"
#endif

#ifdef HB_HAS_DIRECTWRITE
#അന്തര്ഭാവ "hb-directwrite-font.cc"
#അന്തര്ഭാവ "hb-directwrite-shape.cc"
#അന്തര്ഭാവ "hb-directwrite.cc"
#endif

#ifdef HB_HAS_FREETYPE
#അന്തര്ഭാവ "hb-ft.cc"
#endif

#ifdef HB_HAS_GDI
#ifndef HAVE_GDI
#അന്തര്ഭാവ "hb-gdi.cc"
#endif
#endif

#ifdef HB_HAS_GLIB
#അന്തര്ഭാവ "hb-glib.cc"
#endif

#ifdef HB_HAS_GRAPHITE
#അന്തര്ഭാവ "hb-graphite2.cc"
#endif

#ifdef HB_HAS_GPU
#അന്തര്ഭാവ "hb-gpu-draw.cc"
#അന്തര്ഭാവ "hb-gpu-paint.cc"
#അന്തര്ഭാവ "hb-gpu.cc"
#അന്തര്ഭാവ "hb-static.cc"
#endif

#ifdef HB_HAS_ICU
#അന്തര്ഭാവ "hb-icu.cc"
#endif

#ifdef HB_HAS_RASTER
#അന്തര്ഭാവ "hb-raster-draw.cc"
#അന്തര്ഭാവ "hb-raster-image.cc"
#അന്തര്ഭാവ "hb-raster-paint.cc"
#അന്തര്ഭാവ "hb-static.cc"
#endif

#ifdef HB_HAS_SUBSET
#അന്തര്ഭാവ "OT/Var/VARC/VARC.cc"
#അന്തര്ഭാവ "graph/gsubgpos-context.cc"
#അന്തര്ഭാവ "hb-depend.cc"
#അന്തര്ഭാവ "hb-number.cc"
#അന്തര്ഭാവ "hb-ot-cff1-table.cc"
#അന്തര്ഭാവ "hb-ot-cff2-table.cc"
#അന്തര്ഭാവ "hb-static.cc"
#അന്തര്ഭാവ "hb-subset-cff-common.cc"
#അന്തര്ഭാവ "hb-subset-cff1.cc"
#അന്തര്ഭാവ "hb-subset-cff2.cc"
#അന്തര്ഭാവ "hb-subset-input.cc"
#അന്തര്ഭാവ "hb-subset-instancer-iup.cc"
#അന്തര്ഭാവ "hb-subset-instancer-solver.cc"
#അന്തര്ഭാവ "hb-subset-plan-layout.cc"
#അന്തര്ഭാവ "hb-subset-plan-var.cc"
#അന്തര്ഭാവ "hb-subset-plan.cc"
#അന്തര്ഭാവ "hb-subset-serialize.cc"
#അന്തര്ഭാവ "hb-subset-table-cff.cc"
#അന്തര്ഭാവ "hb-subset-table-color.cc"
#അന്തര്ഭാവ "hb-subset-table-layout.cc"
#അന്തര്ഭാവ "hb-subset-table-other.cc"
#അന്തര്ഭാവ "hb-subset-table-var.cc"
#അന്തര്ഭാവ "hb-subset.cc"
#endif

#ifdef HB_HAS_UNISCRIBE
#അന്തര്ഭാവ "hb-uniscribe.cc"
#endif

#ifdef HB_HAS_VECTOR
#അന്തര്ഭാവ "hb-static.cc"
#അന്തര്ഭാവ "hb-vector-draw.cc"
#അന്തര്ഭാവ "hb-vector-paint-pdf.cc"
#അന്തര്ഭാവ "hb-vector-paint-svg.cc"
#അന്തര്ഭാവ "hb-vector-paint.cc"
#അന്തര്ഭാവ "hb-vector-path.cc"
#അന്തര്ഭാവ "hb-vector.cc"
#അന്തര്ഭാവ "hb-zlib.cc"
#endif

#ifdef HB_HAS_WASM
#അന്തര്ഭാവ "hb-wasm-api.cc"
#അന്തര്ഭാവ "hb-wasm-shape.cc"
#endif
