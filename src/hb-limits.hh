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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_LIMITS_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_LIMITS_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"


#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_ਬਫਰ_MAX_LEN_ਗੁਣਕ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_ਬਫਰ_MAX_LEN_ਗੁਣਕ 256
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_ਬਫਰ_MAX_LEN_MIN
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_ਬਫਰ_MAX_LEN_MIN 65536
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_ਬਫਰ_MAX_LEN_DEFAULT
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_ਬਫਰ_MAX_LEN_DEFAULT 0x3FFFFFFF /* Shaping more than a billion chars? Let us know! */
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_ਬਫਰ_MAX_OPS_ਗੁਣਕ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_ਬਫਰ_MAX_OPS_ਗੁਣਕ 4096
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_ਬਫਰ_MAX_OPS_MIN
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_ਬਫਰ_MAX_OPS_MIN 65536
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_ਬਫਰ_MAX_OPS_DEFAULT
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_ਬਫਰ_MAX_OPS_DEFAULT 0x1FFFFFFF /* Shaping more than a billion operations? Let us know! */
#ਜੇ_ਅੰਤ


#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_MAX_NESTING_LEVEL
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_MAX_NESTING_LEVEL 64
#ਜੇ_ਅੰਤ


#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_MAX_CONTEXT_ਲੰਮਾਈ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_MAX_CONTEXT_ਲੰਮਾਈ 64
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_MAX_SYLLABLE_ਲੰਮਾਈ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_MAX_SYLLABLE_ਲੰਮਾਈ 64
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_ਘੇਰਾ_MAX_STAGES
/*
 * The maximum number of times a lookup can be applied during shaping.
 * Used to limit the number of iterations of the closure algorithm.
 * This must be larger than the number of times add_gsub_pause() is
 * called in a collect_features call of any shaper.
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_ਘੇਰਾ_MAX_STAGES 12
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_MAX_SCRIPTS
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_MAX_SCRIPTS 500
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_MAX_LANGSYS
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_MAX_LANGSYS 2000
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_MAX_LANGSYS_FEATURE_ਗਿਣਤੀ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_MAX_LANGSYS_FEATURE_ਗਿਣਤੀ 50000
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_MAX_FEATURE_INDICES
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_MAX_FEATURE_INDICES 8000
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_MAX_LOOKUP_ਦੇਖੋ_ਗਿਣਤੀ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_MAX_LOOKUP_ਦੇਖੋ_ਗਿਣਤੀ 35000
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_MAX_ਗ੍ਰਾਫ_ਕਿਨਾਰਾ_ਗਿਣਤੀ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_MAX_ਗ੍ਰਾਫ_ਕਿਨਾਰਾ_ਗਿਣਤੀ 16384
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_VAR_ਸੰਯੁਕਤ_MAX_AXES
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_VAR_ਸੰਯੁਕਤ_MAX_AXES 4096
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_GLYF_MAX_POINTS
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_GLYF_MAX_POINTS 200000
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_CFF_MAX_OPS
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_CFF_MAX_OPS 200000
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_MAX_ਸੰਯੁਕਤ_OPERATIONS_ਪ੍ਰਤੀ_GLYPH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_MAX_ਸੰਯੁਕਤ_OPERATIONS_ਪ੍ਰਤੀ_GLYPH 64
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_SVG_MAX_ਰਸਤਾ_SEGMENTS
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_SVG_MAX_ਰਸਤਾ_SEGMENTS 262144
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_GPU_DRAW_MAX_CURVES
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_GPU_DRAW_MAX_CURVES 65536
#ਜੇ_ਅੰਤ

/* Total bytes of sub-blobs (gradient color stops, clip outlines) in one
 * hb_gpu_paint_t walk; exceeding it makes the paint unsupported. */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_GPU_PAINT_MAX_SUB_ਬਾਈਟਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_GPU_PAINT_MAX_SUB_ਬਾਈਟਾਂ ((ਬਿਨਾਨਿਸ਼ਾਨੀ) 64 << 20)
#ਜੇ_ਅੰਤ

/* Tiles emitted by one hb_paint_sweep_gradient_tiles() call.  Also
 * sets the angular resolution (2π over this) below which a repeating
 * color line is filled with its average color instead of tiled;
 * a repeat/reflect color line whose stops span a tiny angle could
 * otherwise emit millions of patches while covering 0..2π. */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_PAINT_MAX_SWEEP_TILES
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_PAINT_MAX_SWEEP_TILES 4096
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_SVG_MAX_DOCUMENT_ਮਾਪ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_SVG_MAX_DOCUMENT_ਮਾਪ ((size_t) 16 << 20)
#ਜੇ_ਅੰਤ

/* Maximum size of one serialized vector document (SVG or PDF) produced by
 * the vector backends.  Bounds output that is not outline-derived -- most
 * of a path, but especially sweep-gradient meshes and embedded bitmaps --
 * which the outline work budget cannot see.  vector is one glyph per
 * context, so a flat cap suffices.  Unsigned (not size_t like the SVG-table
 * limit above) to match hb_vector_buf_t's unsigned length arithmetic. */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_ਸਦਿਸ਼_MAX_DOCUMENT_ਮਾਪ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_ਸਦਿਸ਼_MAX_DOCUMENT_ਮਾਪ ((ਬਿਨਾਨਿਸ਼ਾਨੀ) 16 << 20)
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_RASTER_MAX_ਬਫਰ_ਮਾਪ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_RASTER_MAX_ਬਫਰ_ਮਾਪ ((size_t) 1 << 30)
#ਜੇ_ਅੰਤ

/* Maximum surface dimension (pixels per side) when extents are derived
 * from font data (glyph extents or accumulated outline bounds).  Bounds
 * attacker-controlled allocations; extents set explicitly through
 * hb_raster_{draw,paint}_set_extents() are not limited. */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_RASTER_MAX_AUTO_DIMENSION
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_RASTER_MAX_AUTO_DIMENSION 4096
#ਜੇ_ਅੰਤ

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
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_BUDGET_1	1u
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_BUDGET_2	2u
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_BUDGET_4	4u
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_BUDGET_8	8u
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_BUDGET_16	16u
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_BUDGET_32	32u
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_BUDGET_64	64u
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_BUDGET_128	128u
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_BUDGET_256	256u
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_BUDGET_512	512u
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_BUDGET_1024	1024u

/* The common finite default for one top-level glyph rendering session.
 * Nested outline and paint work shares the same live counter. */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_BUDGET_GLYPH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_BUDGET_GLYPH ((int64_t) 1 << 24)
#ਜੇ_ਅੰਤ

/* Precharge COST * MULT.  Callers bound COST and MULT structurally, and live
 * budgets are concrete non-negative values when a session starts. */
ਸਥਿਰ HB_ALWAYS_INLINE ਤਾਰਕਿਕ
hb_budget_spend (int64_t &budget, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਲਾਗਤ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ mult = 1)
{
  budget -= (int64_t) ਲਾਗਤ * mult;
  ਵਾਪਸ budget >= 0;
}

/* The flat pixel-work floor for one raster paint session.  Pixel and
 * outline work use separate live counters, so keep this independent of
 * the common outline budget above. */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_BUDGET_RASTER_PIXELS
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_BUDGET_RASTER_PIXELS ((int64_t) 1 << 26)
#ਜੇ_ਅੰਤ

/* Very large raster surfaces still get a few full-surface operations. */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_BUDGET_RASTER_PAINT_PASSES
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_BUDGET_RASTER_PAINT_PASSES 4
#ਜੇ_ਅੰਤ

/* One raster draw session, in accumulated non-horizontal edges. */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_RASTER_MAX_DRAW_ਕਿਨਾਰੇ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_RASTER_MAX_DRAW_ਕਿਨਾਰੇ ((int64_t) 1 << 20)
#ਜੇ_ਅੰਤ


#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_REPACKER_MAX_ITERATIONS
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_REPACKER_MAX_ITERATIONS 500
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_REPACKER_MAX_VERTICES
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_REPACKER_MAX_VERTICES 800000
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_REPACKER_MAX_SPACES
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_REPACKER_MAX_SPACES 8000
#ਜੇ_ਅੰਤ


#ਜੇ_ਅੰਤ /* HB_LIMITS_HH */
