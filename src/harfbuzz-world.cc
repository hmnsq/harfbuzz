#ifdef HB_FEATURES_H
#ಅನ್ತರ್ಭಾವ HB_FEATURES_H
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
#ಅನ್ತರ್ಭಾವ "OT/Var/VARC/VARC.cc"
#ಅನ್ತರ್ಭಾವ "hb-aat-layout.cc"
#ಅನ್ತರ್ಭಾವ "hb-aat-map.cc"
#ಅನ್ತರ್ಭಾವ "hb-blob.cc"
#ಅನ್ತರ್ಭಾವ "hb-buffer-serialize.cc"
#ಅನ್ತರ್ಭಾವ "hb-buffer-verify.cc"
#ಅನ್ತರ್ಭಾವ "hb-buffer.cc"
#ಅನ್ತರ್ಭಾವ "hb-common.cc"
#ಅನ್ತರ್ಭಾವ "hb-draw.cc"
#ಅನ್ತರ್ಭಾವ "hb-face-builder.cc"
#ಅನ್ತರ್ಭಾವ "hb-face.cc"
#ಅನ್ತರ್ಭಾವ "hb-fallback-shape.cc"
#ಅನ್ತರ್ಭಾವ "hb-font.cc"
#ಅನ್ತರ್ಭಾವ "hb-map.cc"
#ಅನ್ತರ್ಭಾವ "hb-number.cc"
#ಅನ್ತರ್ಭಾವ "hb-ot-cff1-table.cc"
#ಅನ್ತರ್ಭಾವ "hb-ot-cff2-table.cc"
#ಅನ್ತರ್ಭಾವ "hb-ot-color.cc"
#ಅನ್ತರ್ಭಾವ "hb-ot-face.cc"
#ಅನ್ತರ್ಭಾವ "hb-ot-fetch.cc"
#ಅನ್ತರ್ಭಾವ "hb-ot-font.cc"
#ಅನ್ತರ್ಭಾವ "hb-ot-layout.cc"
#ಅನ್ತರ್ಭಾವ "hb-ot-map.cc"
#ಅನ್ತರ್ಭಾವ "hb-ot-math.cc"
#ಅನ್ತರ್ಭಾವ "hb-ot-meta.cc"
#ಅನ್ತರ್ಭಾವ "hb-ot-metrics.cc"
#ಅನ್ತರ್ಭಾವ "hb-ot-name.cc"
#ಅನ್ತರ್ಭಾವ "hb-ot-shape-fallback.cc"
#ಅನ್ತರ್ಭಾವ "hb-ot-shape-normalize.cc"
#ಅನ್ತರ್ಭಾವ "hb-ot-shape.cc"
#ಅನ್ತರ್ಭಾವ "hb-ot-shaper-arabic.cc"
#ಅನ್ತರ್ಭಾವ "hb-ot-shaper-default.cc"
#ಅನ್ತರ್ಭಾವ "hb-ot-shaper-hangul.cc"
#ಅನ್ತರ್ಭಾವ "hb-ot-shaper-hebrew.cc"
#ಅನ್ತರ್ಭಾವ "hb-ot-shaper-indic-table.cc"
#ಅನ್ತರ್ಭಾವ "hb-ot-shaper-indic.cc"
#ಅನ್ತರ್ಭಾವ "hb-ot-shaper-khmer.cc"
#ಅನ್ತರ್ಭಾವ "hb-ot-shaper-myanmar.cc"
#ಅನ್ತರ್ಭಾವ "hb-ot-shaper-syllabic.cc"
#ಅನ್ತರ್ಭಾವ "hb-ot-shaper-thai.cc"
#ಅನ್ತರ್ಭಾವ "hb-ot-shaper-use.cc"
#ಅನ್ತರ್ಭಾವ "hb-ot-shaper-vowel-constraints.cc"
#ಅನ್ತರ್ಭಾವ "hb-ot-tag.cc"
#ಅನ್ತರ್ಭಾವ "hb-ot-var.cc"
#ಅನ್ತರ್ಭಾವ "hb-outline.cc"
#ಅನ್ತರ್ಭಾವ "hb-paint-bounded.cc"
#ಅನ್ತರ್ಭಾವ "hb-paint-extents.cc"
#ಅನ್ತರ್ಭಾವ "hb-paint.cc"
#ಅನ್ತರ್ಭಾವ "hb-set.cc"
#ಅನ್ತರ್ಭಾವ "hb-shape-plan.cc"
#ಅನ್ತರ್ಭಾವ "hb-shape.cc"
#ಅನ್ತರ್ಭಾವ "hb-shaper.cc"
#ಅನ್ತರ್ಭಾವ "hb-static.cc"
#ಅನ್ತರ್ಭಾವ "hb-style.cc"
#ಅನ್ತರ್ಭಾವ "hb-ucd.cc"
#ಅನ್ತರ್ಭಾವ "hb-unicode.cc"

#ifdef HB_HAS_CAIRO
#ಅನ್ತರ್ಭಾವ "hb-cairo-utils.cc"
#ಅನ್ತರ್ಭಾವ "hb-cairo.cc"
#ಅನ್ತರ್ಭಾವ "hb-static.cc"
#endif

#ifdef HB_HAS_CORETEXT
#ಅನ್ತರ್ಭಾವ "hb-coretext-font.cc"
#ಅನ್ತರ್ಭಾವ "hb-coretext-shape.cc"
#ಅನ್ತರ್ಭಾವ "hb-coretext.cc"
#endif

#ifdef HB_HAS_DIRECTWRITE
#ಅನ್ತರ್ಭಾವ "hb-directwrite-font.cc"
#ಅನ್ತರ್ಭಾವ "hb-directwrite-shape.cc"
#ಅನ್ತರ್ಭಾವ "hb-directwrite.cc"
#endif

#ifdef HB_HAS_FREETYPE
#ಅನ್ತರ್ಭಾವ "hb-ft.cc"
#endif

#ifdef HB_HAS_GDI
#ifndef HAVE_GDI
#ಅನ್ತರ್ಭಾವ "hb-gdi.cc"
#endif
#endif

#ifdef HB_HAS_GLIB
#ಅನ್ತರ್ಭಾವ "hb-glib.cc"
#endif

#ifdef HB_HAS_GRAPHITE
#ಅನ್ತರ್ಭಾವ "hb-graphite2.cc"
#endif

#ifdef HB_HAS_GPU
#ಅನ್ತರ್ಭಾವ "hb-gpu-draw.cc"
#ಅನ್ತರ್ಭಾವ "hb-gpu-paint.cc"
#ಅನ್ತರ್ಭಾವ "hb-gpu.cc"
#ಅನ್ತರ್ಭಾವ "hb-static.cc"
#endif

#ifdef HB_HAS_ICU
#ಅನ್ತರ್ಭಾವ "hb-icu.cc"
#endif

#ifdef HB_HAS_RASTER
#ಅನ್ತರ್ಭಾವ "hb-raster-draw.cc"
#ಅನ್ತರ್ಭಾವ "hb-raster-image.cc"
#ಅನ್ತರ್ಭಾವ "hb-raster-paint.cc"
#ಅನ್ತರ್ಭಾವ "hb-static.cc"
#endif

#ifdef HB_HAS_SUBSET
#ಅನ್ತರ್ಭಾವ "OT/Var/VARC/VARC.cc"
#ಅನ್ತರ್ಭಾವ "graph/gsubgpos-context.cc"
#ಅನ್ತರ್ಭಾವ "hb-depend.cc"
#ಅನ್ತರ್ಭಾವ "hb-number.cc"
#ಅನ್ತರ್ಭಾವ "hb-ot-cff1-table.cc"
#ಅನ್ತರ್ಭಾವ "hb-ot-cff2-table.cc"
#ಅನ್ತರ್ಭಾವ "hb-static.cc"
#ಅನ್ತರ್ಭಾವ "hb-subset-cff-common.cc"
#ಅನ್ತರ್ಭಾವ "hb-subset-cff1.cc"
#ಅನ್ತರ್ಭಾವ "hb-subset-cff2.cc"
#ಅನ್ತರ್ಭಾವ "hb-subset-input.cc"
#ಅನ್ತರ್ಭಾವ "hb-subset-instancer-iup.cc"
#ಅನ್ತರ್ಭಾವ "hb-subset-instancer-solver.cc"
#ಅನ್ತರ್ಭಾವ "hb-subset-plan-layout.cc"
#ಅನ್ತರ್ಭಾವ "hb-subset-plan-var.cc"
#ಅನ್ತರ್ಭಾವ "hb-subset-plan.cc"
#ಅನ್ತರ್ಭಾವ "hb-subset-serialize.cc"
#ಅನ್ತರ್ಭಾವ "hb-subset-table-cff.cc"
#ಅನ್ತರ್ಭಾವ "hb-subset-table-color.cc"
#ಅನ್ತರ್ಭಾವ "hb-subset-table-layout.cc"
#ಅನ್ತರ್ಭಾವ "hb-subset-table-other.cc"
#ಅನ್ತರ್ಭಾವ "hb-subset-table-var.cc"
#ಅನ್ತರ್ಭಾವ "hb-subset.cc"
#endif

#ifdef HB_HAS_UNISCRIBE
#ಅನ್ತರ್ಭಾವ "hb-uniscribe.cc"
#endif

#ifdef HB_HAS_VECTOR
#ಅನ್ತರ್ಭಾವ "hb-static.cc"
#ಅನ್ತರ್ಭಾವ "hb-vector-draw.cc"
#ಅನ್ತರ್ಭಾವ "hb-vector-paint-pdf.cc"
#ಅನ್ತರ್ಭಾವ "hb-vector-paint-svg.cc"
#ಅನ್ತರ್ಭಾವ "hb-vector-paint.cc"
#ಅನ್ತರ್ಭಾವ "hb-vector-path.cc"
#ಅನ್ತರ್ಭಾವ "hb-vector.cc"
#ಅನ್ತರ್ಭಾವ "hb-zlib.cc"
#endif

#ifdef HB_HAS_WASM
#ಅನ್ತರ್ಭಾವ "hb-wasm-api.cc"
#ಅನ್ತರ್ಭಾವ "hb-wasm-shape.cc"
#endif
