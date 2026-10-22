#ifdef HB_FEATURES_H
#অন্তর্ভাৱ HB_FEATURES_H
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
#অন্তর্ভাৱ "OT/Var/VARC/VARC.cc"
#অন্তর্ভাৱ "hb-aat-layout.cc"
#অন্তর্ভাৱ "hb-aat-map.cc"
#অন্তর্ভাৱ "hb-blob.cc"
#অন্তর্ভাৱ "hb-buffer-serialize.cc"
#অন্তর্ভাৱ "hb-buffer-verify.cc"
#অন্তর্ভাৱ "hb-buffer.cc"
#অন্তর্ভাৱ "hb-common.cc"
#অন্তর্ভাৱ "hb-draw.cc"
#অন্তর্ভাৱ "hb-face-builder.cc"
#অন্তর্ভাৱ "hb-face.cc"
#অন্তর্ভাৱ "hb-fallback-shape.cc"
#অন্তর্ভাৱ "hb-font.cc"
#অন্তর্ভাৱ "hb-map.cc"
#অন্তর্ভাৱ "hb-number.cc"
#অন্তর্ভাৱ "hb-ot-cff1-table.cc"
#অন্তর্ভাৱ "hb-ot-cff2-table.cc"
#অন্তর্ভাৱ "hb-ot-color.cc"
#অন্তর্ভাৱ "hb-ot-face.cc"
#অন্তর্ভাৱ "hb-ot-fetch.cc"
#অন্তর্ভাৱ "hb-ot-font.cc"
#অন্তর্ভাৱ "hb-ot-layout.cc"
#অন্তর্ভাৱ "hb-ot-map.cc"
#অন্তর্ভাৱ "hb-ot-math.cc"
#অন্তর্ভাৱ "hb-ot-meta.cc"
#অন্তর্ভাৱ "hb-ot-metrics.cc"
#অন্তর্ভাৱ "hb-ot-name.cc"
#অন্তর্ভাৱ "hb-ot-shape-fallback.cc"
#অন্তর্ভাৱ "hb-ot-shape-normalize.cc"
#অন্তর্ভাৱ "hb-ot-shape.cc"
#অন্তর্ভাৱ "hb-ot-shaper-arabic.cc"
#অন্তর্ভাৱ "hb-ot-shaper-default.cc"
#অন্তর্ভাৱ "hb-ot-shaper-hangul.cc"
#অন্তর্ভাৱ "hb-ot-shaper-hebrew.cc"
#অন্তর্ভাৱ "hb-ot-shaper-indic-table.cc"
#অন্তর্ভাৱ "hb-ot-shaper-indic.cc"
#অন্তর্ভাৱ "hb-ot-shaper-khmer.cc"
#অন্তর্ভাৱ "hb-ot-shaper-myanmar.cc"
#অন্তর্ভাৱ "hb-ot-shaper-syllabic.cc"
#অন্তর্ভাৱ "hb-ot-shaper-thai.cc"
#অন্তর্ভাৱ "hb-ot-shaper-use.cc"
#অন্তর্ভাৱ "hb-ot-shaper-vowel-constraints.cc"
#অন্তর্ভাৱ "hb-ot-tag.cc"
#অন্তর্ভাৱ "hb-ot-var.cc"
#অন্তর্ভাৱ "hb-outline.cc"
#অন্তর্ভাৱ "hb-paint-bounded.cc"
#অন্তর্ভাৱ "hb-paint-extents.cc"
#অন্তর্ভাৱ "hb-paint.cc"
#অন্তর্ভাৱ "hb-set.cc"
#অন্তর্ভাৱ "hb-shape-plan.cc"
#অন্তর্ভাৱ "hb-shape.cc"
#অন্তর্ভাৱ "hb-shaper.cc"
#অন্তর্ভাৱ "hb-static.cc"
#অন্তর্ভাৱ "hb-style.cc"
#অন্তর্ভাৱ "hb-ucd.cc"
#অন্তর্ভাৱ "hb-unicode.cc"

#ifdef HB_HAS_CAIRO
#অন্তর্ভাৱ "hb-cairo-utils.cc"
#অন্তর্ভাৱ "hb-cairo.cc"
#অন্তর্ভাৱ "hb-static.cc"
#endif

#ifdef HB_HAS_CORETEXT
#অন্তর্ভাৱ "hb-coretext-font.cc"
#অন্তর্ভাৱ "hb-coretext-shape.cc"
#অন্তর্ভাৱ "hb-coretext.cc"
#endif

#ifdef HB_HAS_DIRECTWRITE
#অন্তর্ভাৱ "hb-directwrite-font.cc"
#অন্তর্ভাৱ "hb-directwrite-shape.cc"
#অন্তর্ভাৱ "hb-directwrite.cc"
#endif

#ifdef HB_HAS_FREETYPE
#অন্তর্ভাৱ "hb-ft.cc"
#endif

#ifdef HB_HAS_GDI
#ifndef HAVE_GDI
#অন্তর্ভাৱ "hb-gdi.cc"
#endif
#endif

#ifdef HB_HAS_GLIB
#অন্তর্ভাৱ "hb-glib.cc"
#endif

#ifdef HB_HAS_GRAPHITE
#অন্তর্ভাৱ "hb-graphite2.cc"
#endif

#ifdef HB_HAS_GPU
#অন্তর্ভাৱ "hb-gpu-draw.cc"
#অন্তর্ভাৱ "hb-gpu-paint.cc"
#অন্তর্ভাৱ "hb-gpu.cc"
#অন্তর্ভাৱ "hb-static.cc"
#endif

#ifdef HB_HAS_ICU
#অন্তর্ভাৱ "hb-icu.cc"
#endif

#ifdef HB_HAS_RASTER
#অন্তর্ভাৱ "hb-raster-draw.cc"
#অন্তর্ভাৱ "hb-raster-image.cc"
#অন্তর্ভাৱ "hb-raster-paint.cc"
#অন্তর্ভাৱ "hb-static.cc"
#endif

#ifdef HB_HAS_SUBSET
#অন্তর্ভাৱ "OT/Var/VARC/VARC.cc"
#অন্তর্ভাৱ "graph/gsubgpos-context.cc"
#অন্তর্ভাৱ "hb-depend.cc"
#অন্তর্ভাৱ "hb-number.cc"
#অন্তর্ভাৱ "hb-ot-cff1-table.cc"
#অন্তর্ভাৱ "hb-ot-cff2-table.cc"
#অন্তর্ভাৱ "hb-static.cc"
#অন্তর্ভাৱ "hb-subset-cff-common.cc"
#অন্তর্ভাৱ "hb-subset-cff1.cc"
#অন্তর্ভাৱ "hb-subset-cff2.cc"
#অন্তর্ভাৱ "hb-subset-input.cc"
#অন্তর্ভাৱ "hb-subset-instancer-iup.cc"
#অন্তর্ভাৱ "hb-subset-instancer-solver.cc"
#অন্তর্ভাৱ "hb-subset-plan-layout.cc"
#অন্তর্ভাৱ "hb-subset-plan-var.cc"
#অন্তর্ভাৱ "hb-subset-plan.cc"
#অন্তর্ভাৱ "hb-subset-serialize.cc"
#অন্তর্ভাৱ "hb-subset-table-cff.cc"
#অন্তর্ভাৱ "hb-subset-table-color.cc"
#অন্তর্ভাৱ "hb-subset-table-layout.cc"
#অন্তর্ভাৱ "hb-subset-table-other.cc"
#অন্তর্ভাৱ "hb-subset-table-var.cc"
#অন্তর্ভাৱ "hb-subset.cc"
#endif

#ifdef HB_HAS_UNISCRIBE
#অন্তর্ভাৱ "hb-uniscribe.cc"
#endif

#ifdef HB_HAS_VECTOR
#অন্তর্ভাৱ "hb-static.cc"
#অন্তর্ভাৱ "hb-vector-draw.cc"
#অন্তর্ভাৱ "hb-vector-paint-pdf.cc"
#অন্তর্ভাৱ "hb-vector-paint-svg.cc"
#অন্তর্ভাৱ "hb-vector-paint.cc"
#অন্তর্ভাৱ "hb-vector-path.cc"
#অন্তর্ভাৱ "hb-vector.cc"
#অন্তর্ভাৱ "hb-zlib.cc"
#endif

#ifdef HB_HAS_WASM
#অন্তর্ভাৱ "hb-wasm-api.cc"
#অন্তর্ভাৱ "hb-wasm-shape.cc"
#endif
