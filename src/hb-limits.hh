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

#جيڪڏهن_وصف_نه_ٿيل HB_LIMITS_HH
#وصف_ڪريو HB_LIMITS_HH

#ضمن "hb.hh"


#جيڪڏهن_وصف_نه_ٿيل HB_بفر_MAX_LEN_ضرب_ڪندڙ
#وصف_ڪريو HB_بفر_MAX_LEN_ضرب_ڪندڙ 256
#جيڪڏهن_پورو
#جيڪڏهن_وصف_نه_ٿيل HB_بفر_MAX_LEN_MIN
#وصف_ڪريو HB_بفر_MAX_LEN_MIN 65536
#جيڪڏهن_پورو
#جيڪڏهن_وصف_نه_ٿيل HB_بفر_MAX_LEN_DEFAULT
#وصف_ڪريو HB_بفر_MAX_LEN_DEFAULT 0x3FFFFFFF /* Shaping more than a billion chars? Let us know! */
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل HB_بفر_MAX_OPS_ضرب_ڪندڙ
#وصف_ڪريو HB_بفر_MAX_OPS_ضرب_ڪندڙ 4096
#جيڪڏهن_پورو
#جيڪڏهن_وصف_نه_ٿيل HB_بفر_MAX_OPS_MIN
#وصف_ڪريو HB_بفر_MAX_OPS_MIN 65536
#جيڪڏهن_پورو
#جيڪڏهن_وصف_نه_ٿيل HB_بفر_MAX_OPS_DEFAULT
#وصف_ڪريو HB_بفر_MAX_OPS_DEFAULT 0x1FFFFFFF /* Shaping more than a billion operations? Let us know! */
#جيڪڏهن_پورو


#جيڪڏهن_وصف_نه_ٿيل HB_MAX_NESTING_LEVEL
#وصف_ڪريو HB_MAX_NESTING_LEVEL 64
#جيڪڏهن_پورو


#جيڪڏهن_وصف_نه_ٿيل HB_MAX_CONTEXT_ڊيگهائي
#وصف_ڪريو HB_MAX_CONTEXT_ڊيگهائي 64
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل HB_MAX_SYLLABLE_ڊيگهائي
#وصف_ڪريو HB_MAX_SYLLABLE_ڊيگهائي 64
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل HB_گهيرو_MAX_STAGES
/*
 * The maximum number of times a lookup can be applied during shaping.
 * Used to limit the number of iterations of the closure algorithm.
 * This must be larger than the number of times add_gsub_pause() is
 * called in a collect_features call of any shaper.
 */
#وصف_ڪريو HB_گهيرو_MAX_STAGES 12
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل HB_MAX_SCRIPTS
#وصف_ڪريو HB_MAX_SCRIPTS 500
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل HB_MAX_LANGSYS
#وصف_ڪريو HB_MAX_LANGSYS 2000
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل HB_MAX_LANGSYS_FEATURE_ڳڻپ
#وصف_ڪريو HB_MAX_LANGSYS_FEATURE_ڳڻپ 50000
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل HB_MAX_FEATURE_INDICES
#وصف_ڪريو HB_MAX_FEATURE_INDICES 8000
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل HB_MAX_LOOKUP_گهمو_ڳڻپ
#وصف_ڪريو HB_MAX_LOOKUP_گهمو_ڳڻپ 35000
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل HB_MAX_گراف_ڪنارو_ڳڻپ
#وصف_ڪريو HB_MAX_گراف_ڪنارو_ڳڻپ 16384
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل HB_VAR_مرکب_MAX_AXES
#وصف_ڪريو HB_VAR_مرکب_MAX_AXES 4096
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل HB_GLYF_MAX_POINTS
#وصف_ڪريو HB_GLYF_MAX_POINTS 200000
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل HB_CFF_MAX_OPS
#وصف_ڪريو HB_CFF_MAX_OPS 200000
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل HB_MAX_مرکب_OPERATIONS_في_GLYPH
#وصف_ڪريو HB_MAX_مرکب_OPERATIONS_في_GLYPH 64
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل HB_SVG_MAX_رستو_SEGMENTS
#وصف_ڪريو HB_SVG_MAX_رستو_SEGMENTS 262144
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل HB_GPU_DRAW_MAX_CURVES
#وصف_ڪريو HB_GPU_DRAW_MAX_CURVES 65536
#جيڪڏهن_پورو

/* Total bytes of sub-blobs (gradient color stops, clip outlines) in one
 * hb_gpu_paint_t walk; exceeding it makes the paint unsupported. */
#جيڪڏهن_وصف_نه_ٿيل HB_GPU_PAINT_MAX_SUB_بائيٽون
#وصف_ڪريو HB_GPU_PAINT_MAX_SUB_بائيٽون ((بنانشان) 64 << 20)
#جيڪڏهن_پورو

/* Tiles emitted by one hb_paint_sweep_gradient_tiles() call.  Also
 * sets the angular resolution (2π over this) below which a repeating
 * color line is filled with its average color instead of tiled;
 * a repeat/reflect color line whose stops span a tiny angle could
 * otherwise emit millions of patches while covering 0..2π. */
#جيڪڏهن_وصف_نه_ٿيل HB_PAINT_MAX_SWEEP_TILES
#وصف_ڪريو HB_PAINT_MAX_SWEEP_TILES 4096
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل HB_SVG_MAX_DOCUMENT_سائيز
#وصف_ڪريو HB_SVG_MAX_DOCUMENT_سائيز ((size_t) 16 << 20)
#جيڪڏهن_پورو

/* Maximum size of one serialized vector document (SVG or PDF) produced by
 * the vector backends.  Bounds output that is not outline-derived -- most
 * of a path, but especially sweep-gradient meshes and embedded bitmaps --
 * which the outline work budget cannot see.  vector is one glyph per
 * context, so a flat cap suffices.  Unsigned (not size_t like the SVG-table
 * limit above) to match hb_vector_buf_t's unsigned length arithmetic. */
#جيڪڏهن_وصف_نه_ٿيل HB_ويڪٽر_MAX_DOCUMENT_سائيز
#وصف_ڪريو HB_ويڪٽر_MAX_DOCUMENT_سائيز ((بنانشان) 16 << 20)
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل HB_RASTER_MAX_بفر_سائيز
#وصف_ڪريو HB_RASTER_MAX_بفر_سائيز ((size_t) 1 << 30)
#جيڪڏهن_پورو

/* Maximum surface dimension (pixels per side) when extents are derived
 * from font data (glyph extents or accumulated outline bounds).  Bounds
 * attacker-controlled allocations; extents set explicitly through
 * hb_raster_{draw,paint}_set_extents() are not limited. */
#جيڪڏهن_وصف_نه_ٿيل HB_RASTER_MAX_AUTO_DIMENSION
#وصف_ڪريو HB_RASTER_MAX_AUTO_DIMENSION 4096
#جيڪڏهن_پورو

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
#وصف_ڪريو HB_BUDGET_1	1u
#وصف_ڪريو HB_BUDGET_2	2u
#وصف_ڪريو HB_BUDGET_4	4u
#وصف_ڪريو HB_BUDGET_8	8u
#وصف_ڪريو HB_BUDGET_16	16u
#وصف_ڪريو HB_BUDGET_32	32u
#وصف_ڪريو HB_BUDGET_64	64u
#وصف_ڪريو HB_BUDGET_128	128u
#وصف_ڪريو HB_BUDGET_256	256u
#وصف_ڪريو HB_BUDGET_512	512u
#وصف_ڪريو HB_BUDGET_1024	1024u

/* The common finite default for one top-level glyph rendering session.
 * Nested outline and paint work shares the same live counter. */
#جيڪڏهن_وصف_نه_ٿيل HB_BUDGET_GLYPH
#وصف_ڪريو HB_BUDGET_GLYPH ((int64_t) 1 << 24)
#جيڪڏهن_پورو

/* Precharge COST * MULT.  Callers bound COST and MULT structurally, and live
 * budgets are concrete non-negative values when a session starts. */
ساكن HB_ALWAYS_INLINE منطقي
hb_budget_spend (int64_t &budget, بنانشان صحيح لاڳت, بنانشان صحيح mult = 1)
{
  budget -= (int64_t) لاڳت * mult;
  أرجع budget >= 0;
}

/* The flat pixel-work floor for one raster paint session.  Pixel and
 * outline work use separate live counters, so keep this independent of
 * the common outline budget above. */
#جيڪڏهن_وصف_نه_ٿيل HB_BUDGET_RASTER_PIXELS
#وصف_ڪريو HB_BUDGET_RASTER_PIXELS ((int64_t) 1 << 26)
#جيڪڏهن_پورو

/* Very large raster surfaces still get a few full-surface operations. */
#جيڪڏهن_وصف_نه_ٿيل HB_BUDGET_RASTER_PAINT_PASSES
#وصف_ڪريو HB_BUDGET_RASTER_PAINT_PASSES 4
#جيڪڏهن_پورو

/* One raster draw session, in accumulated non-horizontal edges. */
#جيڪڏهن_وصف_نه_ٿيل HB_RASTER_MAX_DRAW_ڪنارا
#وصف_ڪريو HB_RASTER_MAX_DRAW_ڪنارا ((int64_t) 1 << 20)
#جيڪڏهن_پورو


#جيڪڏهن_وصف_نه_ٿيل HB_REPACKER_MAX_ITERATIONS
#وصف_ڪريو HB_REPACKER_MAX_ITERATIONS 500
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل HB_REPACKER_MAX_VERTICES
#وصف_ڪريو HB_REPACKER_MAX_VERTICES 800000
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل HB_REPACKER_MAX_SPACES
#وصف_ڪريو HB_REPACKER_MAX_SPACES 8000
#جيڪڏهن_پورو


#جيڪڏهن_پورو /* HB_LIMITS_HH */
