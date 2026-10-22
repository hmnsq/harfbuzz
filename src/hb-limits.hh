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

#اگر_غیر_تعریف_شدہ HB_LIMITS_HH
#تعریف_کرو HB_LIMITS_HH

#شامل "hb.hh"


#اگر_غیر_تعریف_شدہ HB_بفر_MAX_LEN_ضربی_عدد
#تعریف_کرو HB_بفر_MAX_LEN_ضربی_عدد 256
#اگر_ختم
#اگر_غیر_تعریف_شدہ HB_بفر_MAX_LEN_MIN
#تعریف_کرو HB_بفر_MAX_LEN_MIN 65536
#اگر_ختم
#اگر_غیر_تعریف_شدہ HB_بفر_MAX_LEN_DEFAULT
#تعریف_کرو HB_بفر_MAX_LEN_DEFAULT 0x3FFFFFFF /* Shaping more than a billion chars? Let us know! */
#اگر_ختم

#اگر_غیر_تعریف_شدہ HB_بفر_MAX_OPS_ضربی_عدد
#تعریف_کرو HB_بفر_MAX_OPS_ضربی_عدد 4096
#اگر_ختم
#اگر_غیر_تعریف_شدہ HB_بفر_MAX_OPS_MIN
#تعریف_کرو HB_بفر_MAX_OPS_MIN 65536
#اگر_ختم
#اگر_غیر_تعریف_شدہ HB_بفر_MAX_OPS_DEFAULT
#تعریف_کرو HB_بفر_MAX_OPS_DEFAULT 0x1FFFFFFF /* Shaping more than a billion operations? Let us know! */
#اگر_ختم


#اگر_غیر_تعریف_شدہ HB_MAX_NESTING_LEVEL
#تعریف_کرو HB_MAX_NESTING_LEVEL 64
#اگر_ختم


#اگر_غیر_تعریف_شدہ HB_MAX_CONTEXT_لمبائی
#تعریف_کرو HB_MAX_CONTEXT_لمبائی 64
#اگر_ختم

#اگر_غیر_تعریف_شدہ HB_MAX_SYLLABLE_لمبائی
#تعریف_کرو HB_MAX_SYLLABLE_لمبائی 64
#اگر_ختم

#اگر_غیر_تعریف_شدہ HB_احاطہ_MAX_STAGES
/*
 * The maximum number of times a lookup can be applied during shaping.
 * Used to limit the number of iterations of the closure algorithm.
 * This must be larger than the number of times add_gsub_pause() is
 * called in a collect_features call of any shaper.
 */
#تعریف_کرو HB_احاطہ_MAX_STAGES 12
#اگر_ختم

#اگر_غیر_تعریف_شدہ HB_MAX_SCRIPTS
#تعریف_کرو HB_MAX_SCRIPTS 500
#اگر_ختم

#اگر_غیر_تعریف_شدہ HB_MAX_LANGSYS
#تعریف_کرو HB_MAX_LANGSYS 2000
#اگر_ختم

#اگر_غیر_تعریف_شدہ HB_MAX_LANGSYS_FEATURE_گنتی
#تعریف_کرو HB_MAX_LANGSYS_FEATURE_گنتی 50000
#اگر_ختم

#اگر_غیر_تعریف_شدہ HB_MAX_FEATURE_INDICES
#تعریف_کرو HB_MAX_FEATURE_INDICES 8000
#اگر_ختم

#اگر_غیر_تعریف_شدہ HB_MAX_LOOKUP_دورہ_گنتی
#تعریف_کرو HB_MAX_LOOKUP_دورہ_گنتی 35000
#اگر_ختم

#اگر_غیر_تعریف_شدہ HB_MAX_گراف_کنارہ_گنتی
#تعریف_کرو HB_MAX_گراف_کنارہ_گنتی 16384
#اگر_ختم

#اگر_غیر_تعریف_شدہ HB_VAR_مرکب_MAX_AXES
#تعریف_کرو HB_VAR_مرکب_MAX_AXES 4096
#اگر_ختم

#اگر_غیر_تعریف_شدہ HB_GLYF_MAX_POINTS
#تعریف_کرو HB_GLYF_MAX_POINTS 200000
#اگر_ختم

#اگر_غیر_تعریف_شدہ HB_CFF_MAX_OPS
#تعریف_کرو HB_CFF_MAX_OPS 200000
#اگر_ختم

#اگر_غیر_تعریف_شدہ HB_MAX_مرکب_OPERATIONS_فی_GLYPH
#تعریف_کرو HB_MAX_مرکب_OPERATIONS_فی_GLYPH 64
#اگر_ختم

#اگر_غیر_تعریف_شدہ HB_SVG_MAX_راستہ_SEGMENTS
#تعریف_کرو HB_SVG_MAX_راستہ_SEGMENTS 262144
#اگر_ختم

#اگر_غیر_تعریف_شدہ HB_GPU_DRAW_MAX_CURVES
#تعریف_کرو HB_GPU_DRAW_MAX_CURVES 65536
#اگر_ختم

/* Total bytes of sub-blobs (gradient color stops, clip outlines) in one
 * hb_gpu_paint_t walk; exceeding it makes the paint unsupported. */
#اگر_غیر_تعریف_شدہ HB_GPU_PAINT_MAX_SUB_بائٹیں
#تعریف_کرو HB_GPU_PAINT_MAX_SUB_بائٹیں ((غیرعلامتی) 64 << 20)
#اگر_ختم

/* Tiles emitted by one hb_paint_sweep_gradient_tiles() call.  Also
 * sets the angular resolution (2π over this) below which a repeating
 * color line is filled with its average color instead of tiled;
 * a repeat/reflect color line whose stops span a tiny angle could
 * otherwise emit millions of patches while covering 0..2π. */
#اگر_غیر_تعریف_شدہ HB_PAINT_MAX_SWEEP_TILES
#تعریف_کرو HB_PAINT_MAX_SWEEP_TILES 4096
#اگر_ختم

#اگر_غیر_تعریف_شدہ HB_SVG_MAX_DOCUMENT_سائز
#تعریف_کرو HB_SVG_MAX_DOCUMENT_سائز ((size_t) 16 << 20)
#اگر_ختم

/* Maximum size of one serialized vector document (SVG or PDF) produced by
 * the vector backends.  Bounds output that is not outline-derived -- most
 * of a path, but especially sweep-gradient meshes and embedded bitmaps --
 * which the outline work budget cannot see.  vector is one glyph per
 * context, so a flat cap suffices.  Unsigned (not size_t like the SVG-table
 * limit above) to match hb_vector_buf_t's unsigned length arithmetic. */
#اگر_غیر_تعریف_شدہ HB_سمتیہ_MAX_DOCUMENT_سائز
#تعریف_کرو HB_سمتیہ_MAX_DOCUMENT_سائز ((غیرعلامتی) 16 << 20)
#اگر_ختم

#اگر_غیر_تعریف_شدہ HB_RASTER_MAX_بفر_سائز
#تعریف_کرو HB_RASTER_MAX_بفر_سائز ((size_t) 1 << 30)
#اگر_ختم

/* Maximum surface dimension (pixels per side) when extents are derived
 * from font data (glyph extents or accumulated outline bounds).  Bounds
 * attacker-controlled allocations; extents set explicitly through
 * hb_raster_{draw,paint}_set_extents() are not limited. */
#اگر_غیر_تعریف_شدہ HB_RASTER_MAX_AUTO_DIMENSION
#تعریف_کرو HB_RASTER_MAX_AUTO_DIMENSION 4096
#اگر_ختم

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
#اگر_غیر_تعریف_شدہ HB_BUDGET_GLYPH
#تعریف_کرو HB_BUDGET_GLYPH ((int64_t) 1 << 24)
#اگر_ختم

/* Precharge COST * MULT.  Callers bound COST and MULT structurally, and live
 * budgets are concrete non-negative values when a session starts. */
ساکن HB_ALWAYS_INLINE منطقی
hb_budget_spend (int64_t &budget, غیرعلامتی صحیح لاگت, غیرعلامتی صحیح mult = 1)
{
  budget -= (int64_t) لاگت * mult;
  واپس budget >= 0;
}

/* The flat pixel-work floor for one raster paint session.  Pixel and
 * outline work use separate live counters, so keep this independent of
 * the common outline budget above. */
#اگر_غیر_تعریف_شدہ HB_BUDGET_RASTER_PIXELS
#تعریف_کرو HB_BUDGET_RASTER_PIXELS ((int64_t) 1 << 26)
#اگر_ختم

/* Very large raster surfaces still get a few full-surface operations. */
#اگر_غیر_تعریف_شدہ HB_BUDGET_RASTER_PAINT_PASSES
#تعریف_کرو HB_BUDGET_RASTER_PAINT_PASSES 4
#اگر_ختم

/* One raster draw session, in accumulated non-horizontal edges. */
#اگر_غیر_تعریف_شدہ HB_RASTER_MAX_DRAW_کنارے
#تعریف_کرو HB_RASTER_MAX_DRAW_کنارے ((int64_t) 1 << 20)
#اگر_ختم


#اگر_غیر_تعریف_شدہ HB_REPACKER_MAX_ITERATIONS
#تعریف_کرو HB_REPACKER_MAX_ITERATIONS 500
#اگر_ختم

#اگر_غیر_تعریف_شدہ HB_REPACKER_MAX_VERTICES
#تعریف_کرو HB_REPACKER_MAX_VERTICES 800000
#اگر_ختم

#اگر_غیر_تعریف_شدہ HB_REPACKER_MAX_SPACES
#تعریف_کرو HB_REPACKER_MAX_SPACES 8000
#اگر_ختم


#اگر_ختم /* HB_LIMITS_HH */
