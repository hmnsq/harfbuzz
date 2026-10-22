/*
 * Copyright © 2022  Behdad Esfahbod
 *
 *  This is part of HarfBuzz, a text shaping library.
 *
 * Permission is hereby granted, without written agreement and without
 * license or royalty fees, to use, copy, modify, and distribute this
 * software and its documentation for any purpose, provided that the
 * above copyright notice and the following two paragraphs appear in
 * all copies of this software.
 *
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
 * ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN
 * IF THE COPYRIGHT HOLDER HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 * THE COPYRIGHT HOLDER SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING,
 * BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE COPYRIGHT HOLDER HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 */

#اگر_تعریف_نشده HB_LIMITS_HH
#تعریف_کن HB_LIMITS_HH

#ضمن "hb.hh"


#اگر_تعریف_نشده HB_میانگیر_MAX_LEN_ضریب
#تعریف_کن HB_میانگیر_MAX_LEN_ضریب 256
#پایان_اگر
#اگر_تعریف_نشده HB_میانگیر_MAX_LEN_MIN
#تعریف_کن HB_میانگیر_MAX_LEN_MIN 65536
#پایان_اگر
#اگر_تعریف_نشده HB_میانگیر_MAX_LEN_DEFAULT
#تعریف_کن HB_میانگیر_MAX_LEN_DEFAULT 0x3FFFFFFF /* Shaping more than a billion chars? Let us know! */
#پایان_اگر

#اگر_تعریف_نشده HB_میانگیر_MAX_OPS_ضریب
#تعریف_کن HB_میانگیر_MAX_OPS_ضریب 4096
#پایان_اگر
#اگر_تعریف_نشده HB_میانگیر_MAX_OPS_MIN
#تعریف_کن HB_میانگیر_MAX_OPS_MIN 65536
#پایان_اگر
#اگر_تعریف_نشده HB_میانگیر_MAX_OPS_DEFAULT
#تعریف_کن HB_میانگیر_MAX_OPS_DEFAULT 0x1FFFFFFF /* Shaping more than a billion operations? Let us know! */
#پایان_اگر


#اگر_تعریف_نشده HB_MAX_NESTING_LEVEL
#تعریف_کن HB_MAX_NESTING_LEVEL 64
#پایان_اگر


#اگر_تعریف_نشده HB_MAX_CONTEXT_درازی
#تعریف_کن HB_MAX_CONTEXT_درازی 64
#پایان_اگر

#اگر_تعریف_نشده HB_MAX_SYLLABLE_درازی
#تعریف_کن HB_MAX_SYLLABLE_درازی 64
#پایان_اگر

#اگر_تعریف_نشده HB_بستگی_MAX_STAGES
/*
 * The maximum number of times a lookup can be applied during shaping.
 * Used to limit the number of iterations of the closure algorithm.
 * This must be larger than the number of times add_gsub_pause() is
 * called in a collect_features call of any shaper.
 */
#تعریف_کن HB_بستگی_MAX_STAGES 12
#پایان_اگر

#اگر_تعریف_نشده HB_MAX_SCRIPTS
#تعریف_کن HB_MAX_SCRIPTS 500
#پایان_اگر

#اگر_تعریف_نشده HB_MAX_LANGSYS
#تعریف_کن HB_MAX_LANGSYS 2000
#پایان_اگر

#اگر_تعریف_نشده HB_MAX_LANGSYS_FEATURE_شمار
#تعریف_کن HB_MAX_LANGSYS_FEATURE_شمار 50000
#پایان_اگر

#اگر_تعریف_نشده HB_MAX_FEATURE_INDICES
#تعریف_کن HB_MAX_FEATURE_INDICES 8000
#پایان_اگر

#اگر_تعریف_نشده HB_MAX_LOOKUP_بازدید_شمار
#تعریف_کن HB_MAX_LOOKUP_بازدید_شمار 35000
#پایان_اگر

#اگر_تعریف_نشده HB_MAX_گراف_یال_شمار
#تعریف_کن HB_MAX_گراف_یال_شمار 16384
#پایان_اگر

#اگر_تعریف_نشده HB_VAR_مرکب_MAX_AXES
#تعریف_کن HB_VAR_مرکب_MAX_AXES 4096
#پایان_اگر

#اگر_تعریف_نشده HB_GLYF_MAX_POINTS
#تعریف_کن HB_GLYF_MAX_POINTS 200000
#پایان_اگر

#اگر_تعریف_نشده HB_CFF_MAX_OPS
#تعریف_کن HB_CFF_MAX_OPS 200000
#پایان_اگر

#اگر_تعریف_نشده HB_MAX_مرکب_OPERATIONS_در_هر_GLYPH
#تعریف_کن HB_MAX_مرکب_OPERATIONS_در_هر_GLYPH 64
#پایان_اگر

#اگر_تعریف_نشده HB_SVG_MAX_مسیر_SEGMENTS
#تعریف_کن HB_SVG_MAX_مسیر_SEGMENTS 262144
#پایان_اگر

#اگر_تعریف_نشده HB_GPU_DRAW_MAX_CURVES
#تعریف_کن HB_GPU_DRAW_MAX_CURVES 65536
#پایان_اگر

/* Total bytes of sub-blobs (gradient color stops, clip outlines) in one
 * hb_gpu_paint_t walk; exceeding it makes the paint unsupported. */
#اگر_تعریف_نشده HB_GPU_PAINT_MAX_SUB_بایتها
#تعریف_کن HB_GPU_PAINT_MAX_SUB_بایتها ((بیعلامت) 64 << 20)
#پایان_اگر

/* Tiles emitted by one hb_paint_sweep_gradient_tiles() call.  Also
 * sets the angular resolution (2π over this) below which a repeating
 * color line is filled with its average color instead of tiled;
 * a repeat/reflect color line whose stops span a tiny angle could
 * otherwise emit millions of patches while covering 0..2π. */
#اگر_تعریف_نشده HB_PAINT_MAX_SWEEP_TILES
#تعریف_کن HB_PAINT_MAX_SWEEP_TILES 4096
#پایان_اگر

#اگر_تعریف_نشده HB_SVG_MAX_DOCUMENT_اندازه
#تعریف_کن HB_SVG_MAX_DOCUMENT_اندازه ((size_t) 16 << 20)
#پایان_اگر

/* Maximum size of one serialized vector document (SVG or PDF) produced by
 * the vector backends.  Bounds output that is not outline-derived -- most
 * of a path, but especially sweep-gradient meshes and embedded bitmaps --
 * which the outline work budget cannot see.  vector is one glyph per
 * context, so a flat cap suffices.  Unsigned (not size_t like the SVG-table
 * limit above) to match hb_vector_buf_t's unsigned length arithmetic. */
#اگر_تعریف_نشده HB_وکتور_MAX_DOCUMENT_اندازه
#تعریف_کن HB_وکتور_MAX_DOCUMENT_اندازه ((بیعلامت) 16 << 20)
#پایان_اگر

#اگر_تعریف_نشده HB_RASTER_MAX_میانگیر_اندازه
#تعریف_کن HB_RASTER_MAX_میانگیر_اندازه ((size_t) 1 << 30)
#پایان_اگر

/* Maximum surface dimension (pixels per side) when extents are derived
 * from font data (glyph extents or accumulated outline bounds).  Bounds
 * attacker-controlled allocations; extents set explicitly through
 * hb_raster_{draw,paint}_set_extents() are not limited. */
#اگر_تعریف_نشده HB_RASTER_MAX_AUTO_DIMENSION
#تعریف_کن HB_RASTER_MAX_AUTO_DIMENSION 4096
#پایان_اگر

/*
 * Cumulative work budgets.
 *
 * The limits above bound work within one subsystem: points per glyf
 * glyph, ops per CFF charstring, nodes in a COLR or VARC graph,
 * pixels per raster surface.  When one subsystem drives another --
 * COLR driving a paint backend, a paint backend or VARC loading
 * glyf/CFF outlines -- those limits multiply.  Each driving session
 * therefore carries one cumulative budget, initialized once per
 * top-level entry and only ever decremented, shared by everything
 * the session consumes.  Once a budget is exhausted, further work
 * is skipped best-effort.
 */

/* Fixed relative weights.  Keep the value in the name so charge sites are
 * easy to audit; tune the common session cap, not these values. */
#تعریف_کن HB_BUDGET_1	1u
#تعریف_کن HB_BUDGET_2	2u
#تعریف_کن HB_BUDGET_4	4u
#تعریف_کن HB_BUDGET_8	8u
#تعریف_کن HB_BUDGET_16	16u
#تعریف_کن HB_BUDGET_32	32u
#تعریف_کن HB_BUDGET_64	64u
#تعریف_کن HB_BUDGET_128	128u
#تعریف_کن HB_BUDGET_256	256u
#تعریف_کن HB_BUDGET_512	512u
#تعریف_کن HB_BUDGET_1024	1024u

/* The common finite default for one top-level glyph rendering session.
 * Nested outline and paint work shares the same live counter. */
#اگر_تعریف_نشده HB_BUDGET_GLYPH
#تعریف_کن HB_BUDGET_GLYPH ((int64_t) 1 << 24)
#پایان_اگر

/* Precharge COST * MULT.  Callers bound COST and MULT structurally, and live
 * budgets are concrete non-negative values when a session starts. */
ساكن HB_ALWAYS_INLINE منطقي
hb_budget_spend (int64_t &budget, بیعلامت صحيح هزینه, بیعلامت صحيح mult = 1)
{
  budget -= (int64_t) هزینه * mult;
  أرجع budget >= 0;
}

/* The flat pixel-work floor for one raster paint session.  Pixel and
 * outline work use separate live counters, so keep this independent of
 * the common outline budget above. */
#اگر_تعریف_نشده HB_BUDGET_RASTER_PIXELS
#تعریف_کن HB_BUDGET_RASTER_PIXELS ((int64_t) 1 << 26)
#پایان_اگر

/* Very large raster surfaces still get a few full-surface operations. */
#اگر_تعریف_نشده HB_BUDGET_RASTER_PAINT_PASSES
#تعریف_کن HB_BUDGET_RASTER_PAINT_PASSES 4
#پایان_اگر

/* One raster draw session, in accumulated non-horizontal edges. */
#اگر_تعریف_نشده HB_RASTER_MAX_DRAW_یالها
#تعریف_کن HB_RASTER_MAX_DRAW_یالها ((int64_t) 1 << 20)
#پایان_اگر


#اگر_تعریف_نشده HB_REPACKER_MAX_ITERATIONS
#تعریف_کن HB_REPACKER_MAX_ITERATIONS 500
#پایان_اگر

#اگر_تعریف_نشده HB_REPACKER_MAX_VERTICES
#تعریف_کن HB_REPACKER_MAX_VERTICES 800000
#پایان_اگر

#اگر_تعریف_نشده HB_REPACKER_MAX_SPACES
#تعریف_کن HB_REPACKER_MAX_SPACES 8000
#پایان_اگر


#پایان_اگر /* HB_LIMITS_HH */
