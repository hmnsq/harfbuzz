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

#నిర్వచితం_కాకపోతే HB_LIMITS_HH
#నిర్వచించు HB_LIMITS_HH

#అన్తర్భావ "hb.hh"


#నిర్వచితం_కాకపోతే HB_బఫరు_MAX_LEN_గుణకం
#నిర్వచించు HB_బఫరు_MAX_LEN_గుణకం 256
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే HB_బఫరు_MAX_LEN_MIN
#నిర్వచించు HB_బఫరు_MAX_LEN_MIN 65536
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే HB_బఫరు_MAX_LEN_DEFAULT
#నిర్వచించు HB_బఫరు_MAX_LEN_DEFAULT 0x3FFFFFFF /* Shaping more than a billion chars? Let us know! */
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HB_బఫరు_MAX_OPS_గుణకం
#నిర్వచించు HB_బఫరు_MAX_OPS_గుణకం 4096
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే HB_బఫరు_MAX_OPS_MIN
#నిర్వచించు HB_బఫరు_MAX_OPS_MIN 65536
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే HB_బఫరు_MAX_OPS_DEFAULT
#నిర్వచించు HB_బఫరు_MAX_OPS_DEFAULT 0x1FFFFFFF /* Shaping more than a billion operations? Let us know! */
#అయితే_ముగింపు


#నిర్వచితం_కాకపోతే HB_MAX_NESTING_LEVEL
#నిర్వచించు HB_MAX_NESTING_LEVEL 64
#అయితే_ముగింపు


#నిర్వచితం_కాకపోతే HB_MAX_CONTEXT_నిడివి
#నిర్వచించు HB_MAX_CONTEXT_నిడివి 64
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HB_MAX_SYLLABLE_నిడివి
#నిర్వచించు HB_MAX_SYLLABLE_నిడివి 64
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HB_సంవృతం_MAX_STAGES
/*
 * The maximum number of times a lookup can be applied during shaping.
 * Used to limit the number of iterations of the closure algorithm.
 * This must be larger than the number of times add_gsub_pause() is
 * called in a collect_features call of any shaper.
 */
#నిర్వచించు HB_సంవృతం_MAX_STAGES 12
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HB_MAX_SCRIPTS
#నిర్వచించు HB_MAX_SCRIPTS 500
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HB_MAX_LANGSYS
#నిర్వచించు HB_MAX_LANGSYS 2000
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HB_MAX_LANGSYS_FEATURE_లెక్క
#నిర్వచించు HB_MAX_LANGSYS_FEATURE_లెక్క 50000
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HB_MAX_FEATURE_INDICES
#నిర్వచించు HB_MAX_FEATURE_INDICES 8000
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HB_MAX_LOOKUP_సందర్శించు_లెక్క
#నిర్వచించు HB_MAX_LOOKUP_సందర్శించు_లెక్క 35000
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HB_MAX_లేఖాచిత్రం_అంచు_లెక్క
#నిర్వచించు HB_MAX_లేఖాచిత్రం_అంచు_లెక్క 16384
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HB_VAR_సంయుక్త_MAX_AXES
#నిర్వచించు HB_VAR_సంయుక్త_MAX_AXES 4096
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HB_GLYF_MAX_POINTS
#నిర్వచించు HB_GLYF_MAX_POINTS 200000
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HB_CFF_MAX_OPS
#నిర్వచించు HB_CFF_MAX_OPS 200000
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HB_MAX_సంయుక్త_OPERATIONS_చొప్పున_GLYPH
#నిర్వచించు HB_MAX_సంయుక్త_OPERATIONS_చొప్పున_GLYPH 64
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HB_SVG_MAX_మార్గం_SEGMENTS
#నిర్వచించు HB_SVG_MAX_మార్గం_SEGMENTS 262144
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HB_GPU_DRAW_MAX_CURVES
#నిర్వచించు HB_GPU_DRAW_MAX_CURVES 65536
#అయితే_ముగింపు

/* Total bytes of sub-blobs (gradient color stops, clip outlines) in one
 * hb_gpu_paint_t walk; exceeding it makes the paint unsupported. */
#నిర్వచితం_కాకపోతే HB_GPU_PAINT_MAX_SUB_బైట్లు
#నిర్వచించు HB_GPU_PAINT_MAX_SUB_బైట్లు ((గుర్తులేని) 64 << 20)
#అయితే_ముగింపు

/* Tiles emitted by one hb_paint_sweep_gradient_tiles() call.  Also
 * sets the angular resolution (2π over this) below which a repeating
 * color line is filled with its average color instead of tiled;
 * a repeat/reflect color line whose stops span a tiny angle could
 * otherwise emit millions of patches while covering 0..2π. */
#నిర్వచితం_కాకపోతే HB_PAINT_MAX_SWEEP_TILES
#నిర్వచించు HB_PAINT_MAX_SWEEP_TILES 4096
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HB_SVG_MAX_DOCUMENT_కొలత
#నిర్వచించు HB_SVG_MAX_DOCUMENT_కొలత ((size_t) 16 << 20)
#అయితే_ముగింపు

/* Maximum size of one serialized vector document (SVG or PDF) produced by
 * the vector backends.  Bounds output that is not outline-derived -- most
 * of a path, but especially sweep-gradient meshes and embedded bitmaps --
 * which the outline work budget cannot see.  vector is one glyph per
 * context, so a flat cap suffices.  Unsigned (not size_t like the SVG-table
 * limit above) to match hb_vector_buf_t's unsigned length arithmetic. */
#నిర్వచితం_కాకపోతే HB_సదిశ_MAX_DOCUMENT_కొలత
#నిర్వచించు HB_సదిశ_MAX_DOCUMENT_కొలత ((గుర్తులేని) 16 << 20)
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HB_RASTER_MAX_బఫరు_కొలత
#నిర్వచించు HB_RASTER_MAX_బఫరు_కొలత ((size_t) 1 << 30)
#అయితే_ముగింపు

/* Maximum surface dimension (pixels per side) when extents are derived
 * from font data (glyph extents or accumulated outline bounds).  Bounds
 * attacker-controlled allocations; extents set explicitly through
 * hb_raster_{draw,paint}_set_extents() are not limited. */
#నిర్వచితం_కాకపోతే HB_RASTER_MAX_AUTO_DIMENSION
#నిర్వచించు HB_RASTER_MAX_AUTO_DIMENSION 4096
#అయితే_ముగింపు

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
#నిర్వచించు HB_BUDGET_1	1u
#నిర్వచించు HB_BUDGET_2	2u
#నిర్వచించు HB_BUDGET_4	4u
#నిర్వచించు HB_BUDGET_8	8u
#నిర్వచించు HB_BUDGET_16	16u
#నిర్వచించు HB_BUDGET_32	32u
#నిర్వచించు HB_BUDGET_64	64u
#నిర్వచించు HB_BUDGET_128	128u
#నిర్వచించు HB_BUDGET_256	256u
#నిర్వచించు HB_BUDGET_512	512u
#నిర్వచించు HB_BUDGET_1024	1024u

/* The common finite default for one top-level glyph rendering session.
 * Nested outline and paint work shares the same live counter. */
#నిర్వచితం_కాకపోతే HB_BUDGET_GLYPH
#నిర్వచించు HB_BUDGET_GLYPH ((int64_t) 1 << 24)
#అయితే_ముగింపు

/* Precharge COST * MULT.  Callers bound COST and MULT structurally, and live
 * budgets are concrete non-negative values when a session starts. */
స్థిర HB_ALWAYS_INLINE తార్కిక
hb_budget_spend (int64_t &budget, గుర్తులేని పూర్ణాంకం ఖర్చు, గుర్తులేని పూర్ణాంకం mult = 1)
{
  budget -= (int64_t) ఖర్చు * mult;
  తిరిగి budget >= 0;
}

/* The flat pixel-work floor for one raster paint session.  Pixel and
 * outline work use separate live counters, so keep this independent of
 * the common outline budget above. */
#నిర్వచితం_కాకపోతే HB_BUDGET_RASTER_PIXELS
#నిర్వచించు HB_BUDGET_RASTER_PIXELS ((int64_t) 1 << 26)
#అయితే_ముగింపు

/* Very large raster surfaces still get a few full-surface operations. */
#నిర్వచితం_కాకపోతే HB_BUDGET_RASTER_PAINT_PASSES
#నిర్వచించు HB_BUDGET_RASTER_PAINT_PASSES 4
#అయితే_ముగింపు

/* One raster draw session, in accumulated non-horizontal edges. */
#నిర్వచితం_కాకపోతే HB_RASTER_MAX_DRAW_అంచులు
#నిర్వచించు HB_RASTER_MAX_DRAW_అంచులు ((int64_t) 1 << 20)
#అయితే_ముగింపు


#నిర్వచితం_కాకపోతే HB_REPACKER_MAX_ITERATIONS
#నిర్వచించు HB_REPACKER_MAX_ITERATIONS 500
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HB_REPACKER_MAX_VERTICES
#నిర్వచించు HB_REPACKER_MAX_VERTICES 800000
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HB_REPACKER_MAX_SPACES
#నిర్వచించు HB_REPACKER_MAX_SPACES 8000
#అయితే_ముగింపు


#అయితే_ముగింపు /* HB_LIMITS_HH */
