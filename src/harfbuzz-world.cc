#ifdef HB_FEATURES_H
#ଅନ୍ତର୍ଭାଵ HB_FEATURES_H
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
#ଅନ୍ତର୍ଭାଵ "OT/Var/VARC/VARC.cc"
#ଅନ୍ତର୍ଭାଵ "hb-aat-layout.cc"
#ଅନ୍ତର୍ଭାଵ "hb-aat-map.cc"
#ଅନ୍ତର୍ଭାଵ "hb-blob.cc"
#ଅନ୍ତର୍ଭାଵ "hb-buffer-serialize.cc"
#ଅନ୍ତର୍ଭାଵ "hb-buffer-verify.cc"
#ଅନ୍ତର୍ଭାଵ "hb-buffer.cc"
#ଅନ୍ତର୍ଭାଵ "hb-common.cc"
#ଅନ୍ତର୍ଭାଵ "hb-draw.cc"
#ଅନ୍ତର୍ଭାଵ "hb-face-builder.cc"
#ଅନ୍ତର୍ଭାଵ "hb-face.cc"
#ଅନ୍ତର୍ଭାଵ "hb-fallback-shape.cc"
#ଅନ୍ତର୍ଭାଵ "hb-font.cc"
#ଅନ୍ତର୍ଭାଵ "hb-map.cc"
#ଅନ୍ତର୍ଭାଵ "hb-number.cc"
#ଅନ୍ତର୍ଭାଵ "hb-ot-cff1-table.cc"
#ଅନ୍ତର୍ଭାଵ "hb-ot-cff2-table.cc"
#ଅନ୍ତର୍ଭାଵ "hb-ot-color.cc"
#ଅନ୍ତର୍ଭାଵ "hb-ot-face.cc"
#ଅନ୍ତର୍ଭାଵ "hb-ot-fetch.cc"
#ଅନ୍ତର୍ଭାଵ "hb-ot-font.cc"
#ଅନ୍ତର୍ଭାଵ "hb-ot-layout.cc"
#ଅନ୍ତର୍ଭାଵ "hb-ot-map.cc"
#ଅନ୍ତର୍ଭାଵ "hb-ot-math.cc"
#ଅନ୍ତର୍ଭାଵ "hb-ot-meta.cc"
#ଅନ୍ତର୍ଭାଵ "hb-ot-metrics.cc"
#ଅନ୍ତର୍ଭାଵ "hb-ot-name.cc"
#ଅନ୍ତର୍ଭାଵ "hb-ot-shape-fallback.cc"
#ଅନ୍ତର୍ଭାଵ "hb-ot-shape-normalize.cc"
#ଅନ୍ତର୍ଭାଵ "hb-ot-shape.cc"
#ଅନ୍ତର୍ଭାଵ "hb-ot-shaper-arabic.cc"
#ଅନ୍ତର୍ଭାଵ "hb-ot-shaper-default.cc"
#ଅନ୍ତର୍ଭାଵ "hb-ot-shaper-hangul.cc"
#ଅନ୍ତର୍ଭାଵ "hb-ot-shaper-hebrew.cc"
#ଅନ୍ତର୍ଭାଵ "hb-ot-shaper-indic-table.cc"
#ଅନ୍ତର୍ଭାଵ "hb-ot-shaper-indic.cc"
#ଅନ୍ତର୍ଭାଵ "hb-ot-shaper-khmer.cc"
#ଅନ୍ତର୍ଭାଵ "hb-ot-shaper-myanmar.cc"
#ଅନ୍ତର୍ଭାଵ "hb-ot-shaper-syllabic.cc"
#ଅନ୍ତର୍ଭାଵ "hb-ot-shaper-thai.cc"
#ଅନ୍ତର୍ଭାଵ "hb-ot-shaper-use.cc"
#ଅନ୍ତର୍ଭାଵ "hb-ot-shaper-vowel-constraints.cc"
#ଅନ୍ତର୍ଭାଵ "hb-ot-tag.cc"
#ଅନ୍ତର୍ଭାଵ "hb-ot-var.cc"
#ଅନ୍ତର୍ଭାଵ "hb-outline.cc"
#ଅନ୍ତର୍ଭାଵ "hb-paint-bounded.cc"
#ଅନ୍ତର୍ଭାଵ "hb-paint-extents.cc"
#ଅନ୍ତର୍ଭାଵ "hb-paint.cc"
#ଅନ୍ତର୍ଭାଵ "hb-set.cc"
#ଅନ୍ତର୍ଭାଵ "hb-shape-plan.cc"
#ଅନ୍ତର୍ଭାଵ "hb-shape.cc"
#ଅନ୍ତର୍ଭାଵ "hb-shaper.cc"
#ଅନ୍ତର୍ଭାଵ "hb-static.cc"
#ଅନ୍ତର୍ଭାଵ "hb-style.cc"
#ଅନ୍ତର୍ଭାଵ "hb-ucd.cc"
#ଅନ୍ତର୍ଭାଵ "hb-unicode.cc"

#ifdef HB_HAS_CAIRO
#ଅନ୍ତର୍ଭାଵ "hb-cairo-utils.cc"
#ଅନ୍ତର୍ଭାଵ "hb-cairo.cc"
#ଅନ୍ତର୍ଭାଵ "hb-static.cc"
#endif

#ifdef HB_HAS_CORETEXT
#ଅନ୍ତର୍ଭାଵ "hb-coretext-font.cc"
#ଅନ୍ତର୍ଭାଵ "hb-coretext-shape.cc"
#ଅନ୍ତର୍ଭାଵ "hb-coretext.cc"
#endif

#ifdef HB_HAS_DIRECTWRITE
#ଅନ୍ତର୍ଭାଵ "hb-directwrite-font.cc"
#ଅନ୍ତର୍ଭାଵ "hb-directwrite-shape.cc"
#ଅନ୍ତର୍ଭାଵ "hb-directwrite.cc"
#endif

#ifdef HB_HAS_FREETYPE
#ଅନ୍ତର୍ଭାଵ "hb-ft.cc"
#endif

#ifdef HB_HAS_GDI
#ifndef HAVE_GDI
#ଅନ୍ତର୍ଭାଵ "hb-gdi.cc"
#endif
#endif

#ifdef HB_HAS_GLIB
#ଅନ୍ତର୍ଭାଵ "hb-glib.cc"
#endif

#ifdef HB_HAS_GRAPHITE
#ଅନ୍ତର୍ଭାଵ "hb-graphite2.cc"
#endif

#ifdef HB_HAS_GPU
#ଅନ୍ତର୍ଭାଵ "hb-gpu-draw.cc"
#ଅନ୍ତର୍ଭାଵ "hb-gpu-paint.cc"
#ଅନ୍ତର୍ଭାଵ "hb-gpu.cc"
#ଅନ୍ତର୍ଭାଵ "hb-static.cc"
#endif

#ifdef HB_HAS_ICU
#ଅନ୍ତର୍ଭାଵ "hb-icu.cc"
#endif

#ifdef HB_HAS_RASTER
#ଅନ୍ତର୍ଭାଵ "hb-raster-draw.cc"
#ଅନ୍ତର୍ଭାଵ "hb-raster-image.cc"
#ଅନ୍ତର୍ଭାଵ "hb-raster-paint.cc"
#ଅନ୍ତର୍ଭାଵ "hb-static.cc"
#endif

#ifdef HB_HAS_SUBSET
#ଅନ୍ତର୍ଭାଵ "OT/Var/VARC/VARC.cc"
#ଅନ୍ତର୍ଭାଵ "graph/gsubgpos-context.cc"
#ଅନ୍ତର୍ଭାଵ "hb-depend.cc"
#ଅନ୍ତର୍ଭାଵ "hb-number.cc"
#ଅନ୍ତର୍ଭାଵ "hb-ot-cff1-table.cc"
#ଅନ୍ତର୍ଭାଵ "hb-ot-cff2-table.cc"
#ଅନ୍ତର୍ଭାଵ "hb-static.cc"
#ଅନ୍ତର୍ଭାଵ "hb-subset-cff-common.cc"
#ଅନ୍ତର୍ଭାଵ "hb-subset-cff1.cc"
#ଅନ୍ତର୍ଭାଵ "hb-subset-cff2.cc"
#ଅନ୍ତର୍ଭାଵ "hb-subset-input.cc"
#ଅନ୍ତର୍ଭାଵ "hb-subset-instancer-iup.cc"
#ଅନ୍ତର୍ଭାଵ "hb-subset-instancer-solver.cc"
#ଅନ୍ତର୍ଭାଵ "hb-subset-plan-layout.cc"
#ଅନ୍ତର୍ଭାଵ "hb-subset-plan-var.cc"
#ଅନ୍ତର୍ଭାଵ "hb-subset-plan.cc"
#ଅନ୍ତର୍ଭାଵ "hb-subset-serialize.cc"
#ଅନ୍ତର୍ଭାଵ "hb-subset-table-cff.cc"
#ଅନ୍ତର୍ଭାଵ "hb-subset-table-color.cc"
#ଅନ୍ତର୍ଭାଵ "hb-subset-table-layout.cc"
#ଅନ୍ତର୍ଭାଵ "hb-subset-table-other.cc"
#ଅନ୍ତର୍ଭାଵ "hb-subset-table-var.cc"
#ଅନ୍ତର୍ଭାଵ "hb-subset.cc"
#endif

#ifdef HB_HAS_UNISCRIBE
#ଅନ୍ତର୍ଭାଵ "hb-uniscribe.cc"
#endif

#ifdef HB_HAS_VECTOR
#ଅନ୍ତର୍ଭାଵ "hb-static.cc"
#ଅନ୍ତର୍ଭାଵ "hb-vector-draw.cc"
#ଅନ୍ତର୍ଭାଵ "hb-vector-paint-pdf.cc"
#ଅନ୍ତର୍ଭାଵ "hb-vector-paint-svg.cc"
#ଅନ୍ତର୍ଭାଵ "hb-vector-paint.cc"
#ଅନ୍ତର୍ଭାଵ "hb-vector-path.cc"
#ଅନ୍ତର୍ଭାଵ "hb-vector.cc"
#ଅନ୍ତର୍ଭାଵ "hb-zlib.cc"
#endif

#ifdef HB_HAS_WASM
#ଅନ୍ତର୍ଭାଵ "hb-wasm-api.cc"
#ଅନ୍ତର୍ଭାଵ "hb-wasm-shape.cc"
#endif
