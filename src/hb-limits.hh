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

#جے_بنا_تعریف HB_LIMITS_HH
#تعریف_کرو HB_LIMITS_HH

#ضمن "hb.hh"


#جے_بنا_تعریف HB_بفر_MAX_LEN_ضارب
#تعریف_کرو HB_بفر_MAX_LEN_ضارب 256
#جے_انت
#جے_بنا_تعریف HB_بفر_MAX_LEN_MIN
#تعریف_کرو HB_بفر_MAX_LEN_MIN 65536
#جے_انت
#جے_بنا_تعریف HB_بفر_MAX_LEN_DEFAULT
#تعریف_کرو HB_بفر_MAX_LEN_DEFAULT 0x3FFFFFFF /* Shaping more than a billion chars? Let us know! */
#جے_انت

#جے_بنا_تعریف HB_بفر_MAX_OPS_ضارب
#تعریف_کرو HB_بفر_MAX_OPS_ضارب 4096
#جے_انت
#جے_بنا_تعریف HB_بفر_MAX_OPS_MIN
#تعریف_کرو HB_بفر_MAX_OPS_MIN 65536
#جے_انت
#جے_بنا_تعریف HB_بفر_MAX_OPS_DEFAULT
#تعریف_کرو HB_بفر_MAX_OPS_DEFAULT 0x1FFFFFFF /* Shaping more than a billion operations? Let us know! */
#جے_انت


#جے_بنا_تعریف HB_MAX_NESTING_LEVEL
#تعریف_کرو HB_MAX_NESTING_LEVEL 64
#جے_انت


#جے_بنا_تعریف HB_MAX_CONTEXT_لمائی
#تعریف_کرو HB_MAX_CONTEXT_لمائی 64
#جے_انت

#جے_بنا_تعریف HB_MAX_SYLLABLE_لمائی
#تعریف_کرو HB_MAX_SYLLABLE_لمائی 64
#جے_انت

#جے_بنا_تعریف HB_گھیرا_MAX_STAGES
/*
 * The maximum number of times a lookup can be applied during shaping.
 * Used to limit the number of iterations of the closure algorithm.
 * This must be larger than the number of times add_gsub_pause() is
 * called in a collect_features call of any shaper.
 */
#تعریف_کرو HB_گھیرا_MAX_STAGES 12
#جے_انت

#جے_بنا_تعریف HB_MAX_SCRIPTS
#تعریف_کرو HB_MAX_SCRIPTS 500
#جے_انت

#جے_بنا_تعریف HB_MAX_LANGSYS
#تعریف_کرو HB_MAX_LANGSYS 2000
#جے_انت

#جے_بنا_تعریف HB_MAX_LANGSYS_FEATURE_گنتی
#تعریف_کرو HB_MAX_LANGSYS_FEATURE_گنتی 50000
#جے_انت

#جے_بنا_تعریف HB_MAX_FEATURE_INDICES
#تعریف_کرو HB_MAX_FEATURE_INDICES 8000
#جے_انت

#جے_بنا_تعریف HB_MAX_LOOKUP_ویکھو_گنتی
#تعریف_کرو HB_MAX_LOOKUP_ویکھو_گنتی 35000
#جے_انت

#جے_بنا_تعریف HB_MAX_گراف_کنارا_گنتی
#تعریف_کرو HB_MAX_گراف_کنارا_گنتی 16384
#جے_انت

#جے_بنا_تعریف HB_VAR_مرکب_MAX_AXES
#تعریف_کرو HB_VAR_مرکب_MAX_AXES 4096
#جے_انت

#جے_بنا_تعریف HB_GLYF_MAX_POINTS
#تعریف_کرو HB_GLYF_MAX_POINTS 200000
#جے_انت

#جے_بنا_تعریف HB_CFF_MAX_OPS
#تعریف_کرو HB_CFF_MAX_OPS 200000
#جے_انت

#جے_بنا_تعریف HB_MAX_مرکب_OPERATIONS_فی_GLYPH
#تعریف_کرو HB_MAX_مرکب_OPERATIONS_فی_GLYPH 64
#جے_انت

#جے_بنا_تعریف HB_SVG_MAX_رستہ_SEGMENTS
#تعریف_کرو HB_SVG_MAX_رستہ_SEGMENTS 262144
#جے_انت

#جے_بنا_تعریف HB_GPU_DRAW_MAX_CURVES
#تعریف_کرو HB_GPU_DRAW_MAX_CURVES 65536
#جے_انت

/* Total bytes of sub-blobs (gradient color stops, clip outlines) in one
 * hb_gpu_paint_t walk; exceeding it makes the paint unsupported. */
#جے_بنا_تعریف HB_GPU_PAINT_MAX_SUB_بائٹاں
#تعریف_کرو HB_GPU_PAINT_MAX_SUB_بائٹاں ((بنانشانی) 64 << 20)
#جے_انت

/* Tiles emitted by one hb_paint_sweep_gradient_tiles() call.  Also
 * sets the angular resolution (2π over this) below which a repeating
 * color line is filled with its average color instead of tiled;
 * a repeat/reflect color line whose stops span a tiny angle could
 * otherwise emit millions of patches while covering 0..2π. */
#جے_بنا_تعریف HB_PAINT_MAX_SWEEP_TILES
#تعریف_کرو HB_PAINT_MAX_SWEEP_TILES 4096
#جے_انت

#جے_بنا_تعریف HB_SVG_MAX_DOCUMENT_ناپ
#تعریف_کرو HB_SVG_MAX_DOCUMENT_ناپ ((size_t) 16 << 20)
#جے_انت

/* Maximum size of one serialized vector document (SVG or PDF) produced by
 * the vector backends.  Bounds output that is not outline-derived -- most
 * of a path, but especially sweep-gradient meshes and embedded bitmaps --
 * which the outline work budget cannot see.  vector is one glyph per
 * context, so a flat cap suffices.  Unsigned (not size_t like the SVG-table
 * limit above) to match hb_vector_buf_t's unsigned length arithmetic. */
#جے_بنا_تعریف HB_سمتیہ_MAX_DOCUMENT_ناپ
#تعریف_کرو HB_سمتیہ_MAX_DOCUMENT_ناپ ((بنانشانی) 16 << 20)
#جے_انت

#جے_بنا_تعریف HB_RASTER_MAX_بفر_ناپ
#تعریف_کرو HB_RASTER_MAX_بفر_ناپ ((size_t) 1 << 30)
#جے_انت

/* Maximum surface dimension (pixels per side) when extents are derived
 * from font data (glyph extents or accumulated outline bounds).  Bounds
 * attacker-controlled allocations; extents set explicitly through
 * hb_raster_{draw,paint}_set_extents() are not limited. */
#جے_بنا_تعریف HB_RASTER_MAX_AUTO_DIMENSION
#تعریف_کرو HB_RASTER_MAX_AUTO_DIMENSION 4096
#جے_انت

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
#تعریف_کرو HB_BUDGET_1	1u
#تعریف_کرو HB_BUDGET_2	2u
#تعریف_کرو HB_BUDGET_4	4u
#تعریف_کرو HB_BUDGET_8	8u
#تعریف_کرو HB_BUDGET_16	16u
#تعریف_کرو HB_BUDGET_32	32u
#تعریف_کرو HB_BUDGET_64	64u
#تعریف_کرو HB_BUDGET_128	128u
#تعریف_کرو HB_BUDGET_256	256u
#تعریف_کرو HB_BUDGET_512	512u
#تعریف_کرو HB_BUDGET_1024	1024u

/* The common finite default for one top-level glyph rendering session.
 * Nested outline and paint work shares the same live counter. */
#جے_بنا_تعریف HB_BUDGET_GLYPH
#تعریف_کرو HB_BUDGET_GLYPH ((int64_t) 1 << 24)
#جے_انت

/* Precharge COST * MULT.  Callers bound COST and MULT structurally, and live
 * budgets are concrete non-negative values when a session starts. */
ساكن HB_ALWAYS_INLINE منطقي
hb_budget_spend (int64_t &budget, بنانشانی صحيح لاگت, بنانشانی صحيح mult = 1)
{
  budget -= (int64_t) لاگت * mult;
  أرجع budget >= 0;
}

/* The flat pixel-work floor for one raster paint session.  Pixel and
 * outline work use separate live counters, so keep this independent of
 * the common outline budget above. */
#جے_بنا_تعریف HB_BUDGET_RASTER_PIXELS
#تعریف_کرو HB_BUDGET_RASTER_PIXELS ((int64_t) 1 << 26)
#جے_انت

/* Very large raster surfaces still get a few full-surface operations. */
#جے_بنا_تعریف HB_BUDGET_RASTER_PAINT_PASSES
#تعریف_کرو HB_BUDGET_RASTER_PAINT_PASSES 4
#جے_انت

/* One raster draw session, in accumulated non-horizontal edges. */
#جے_بنا_تعریف HB_RASTER_MAX_DRAW_کنارے
#تعریف_کرو HB_RASTER_MAX_DRAW_کنارے ((int64_t) 1 << 20)
#جے_انت


#جے_بنا_تعریف HB_REPACKER_MAX_ITERATIONS
#تعریف_کرو HB_REPACKER_MAX_ITERATIONS 500
#جے_انت

#جے_بنا_تعریف HB_REPACKER_MAX_VERTICES
#تعریف_کرو HB_REPACKER_MAX_VERTICES 800000
#جے_انت

#جے_بنا_تعریف HB_REPACKER_MAX_SPACES
#تعریف_کرو HB_REPACKER_MAX_SPACES 8000
#جے_انت


#جے_انت /* HB_LIMITS_HH */
