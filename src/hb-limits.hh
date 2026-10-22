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

#अगर_अघोषित HB_LIMITS_HH
#घोषणा HB_LIMITS_HH

#समावेश "hb.hh"


#अगर_अघोषित HB_बफर_MAX_LEN_FACTOR
#घोषणा HB_बफर_MAX_LEN_FACTOR 256
#पूर्ण_अगर
#अगर_अघोषित HB_बफर_MAX_LEN_MIN
#घोषणा HB_बफर_MAX_LEN_MIN 65536
#पूर्ण_अगर
#अगर_अघोषित HB_बफर_MAX_LEN_DEFAULT
#घोषणा HB_बफर_MAX_LEN_DEFAULT 0x3FFFFFFF /* Shaping more than a billion chars? Let us know! */
#पूर्ण_अगर

#अगर_अघोषित HB_बफर_MAX_OPS_FACTOR
#घोषणा HB_बफर_MAX_OPS_FACTOR 4096
#पूर्ण_अगर
#अगर_अघोषित HB_बफर_MAX_OPS_MIN
#घोषणा HB_बफर_MAX_OPS_MIN 65536
#पूर्ण_अगर
#अगर_अघोषित HB_बफर_MAX_OPS_DEFAULT
#घोषणा HB_बफर_MAX_OPS_DEFAULT 0x1FFFFFFF /* Shaping more than a billion operations? Let us know! */
#पूर्ण_अगर


#अगर_अघोषित HB_MAX_NESTING_LEVEL
#घोषणा HB_MAX_NESTING_LEVEL 64
#पूर्ण_अगर


#अगर_अघोषित HB_MAX_CONTEXT_लंबाई
#घोषणा HB_MAX_CONTEXT_लंबाई 64
#पूर्ण_अगर

#अगर_अघोषित HB_MAX_SYLLABLE_लंबाई
#घोषणा HB_MAX_SYLLABLE_लंबाई 64
#पूर्ण_अगर

#अगर_अघोषित HB_CLOSURE_MAX_STAGES
/*
 * The maximum number of times a lookup can be applied during shaping.
 * Used to limit the number of iterations of the closure algorithm.
 * This must be larger than the number of times add_gsub_pause() is
 * called in a collect_features call of any shaper.
 */
#घोषणा HB_CLOSURE_MAX_STAGES 12
#पूर्ण_अगर

#अगर_अघोषित HB_MAX_SCRIPTS
#घोषणा HB_MAX_SCRIPTS 500
#पूर्ण_अगर

#अगर_अघोषित HB_MAX_LANGSYS
#घोषणा HB_MAX_LANGSYS 2000
#पूर्ण_अगर

#अगर_अघोषित HB_MAX_LANGSYS_FEATURE_गिनती
#घोषणा HB_MAX_LANGSYS_FEATURE_गिनती 50000
#पूर्ण_अगर

#अगर_अघोषित HB_MAX_FEATURE_INDICES
#घोषणा HB_MAX_FEATURE_INDICES 8000
#पूर्ण_अगर

#अगर_अघोषित HB_MAX_LOOKUP_VISIT_गिनती
#घोषणा HB_MAX_LOOKUP_VISIT_गिनती 35000
#पूर्ण_अगर

#अगर_अघोषित HB_MAX_GRAPH_EDGE_गिनती
#घोषणा HB_MAX_GRAPH_EDGE_गिनती 16384
#पूर्ण_अगर

#अगर_अघोषित HB_VAR_COMPOSITE_MAX_AXES
#घोषणा HB_VAR_COMPOSITE_MAX_AXES 4096
#पूर्ण_अगर

#अगर_अघोषित HB_GLYF_MAX_POINTS
#घोषणा HB_GLYF_MAX_POINTS 200000
#पूर्ण_अगर

#अगर_अघोषित HB_CFF_MAX_OPS
#घोषणा HB_CFF_MAX_OPS 200000
#पूर्ण_अगर

#अगर_अघोषित HB_MAX_COMPOSITE_OPERATIONS_PER_GLYPH
#घोषणा HB_MAX_COMPOSITE_OPERATIONS_PER_GLYPH 64
#पूर्ण_अगर

#अगर_अघोषित HB_SVG_MAX_PATH_SEGMENTS
#घोषणा HB_SVG_MAX_PATH_SEGMENTS 262144
#पूर्ण_अगर

#अगर_अघोषित HB_GPU_DRAW_MAX_CURVES
#घोषणा HB_GPU_DRAW_MAX_CURVES 65536
#पूर्ण_अगर

/* Total bytes of sub-blobs (gradient color stops, clip outlines) in one
 * hb_gpu_paint_t walk; exceeding it makes the paint unsupported. */
#अगर_अघोषित HB_GPU_PAINT_MAX_SUB_BYTES
#घोषणा HB_GPU_PAINT_MAX_SUB_BYTES ((अचिन्हित) 64 << 20)
#पूर्ण_अगर

/* Tiles emitted by one hb_paint_sweep_gradient_tiles() call.  Also
 * sets the angular resolution (2π over this) below which a repeating
 * color line is filled with its average color instead of tiled;
 * a repeat/reflect color line whose stops span a tiny angle could
 * otherwise emit millions of patches while covering 0..2π. */
#अगर_अघोषित HB_PAINT_MAX_SWEEP_TILES
#घोषणा HB_PAINT_MAX_SWEEP_TILES 4096
#पूर्ण_अगर

#अगर_अघोषित HB_SVG_MAX_DOCUMENT_आकार
#घोषणा HB_SVG_MAX_DOCUMENT_आकार ((माप_प्रकार) 16 << 20)
#पूर्ण_अगर

/* Maximum size of one serialized vector document (SVG or PDF) produced by
 * the vector backends.  Bounds output that is not outline-derived -- most
 * of a path, but especially sweep-gradient meshes and embedded bitmaps --
 * which the outline work budget cannot see.  vector is one glyph per
 * context, so a flat cap suffices.  Unsigned (not size_t like the SVG-table
 * limit above) to match hb_vector_buf_t's unsigned length arithmetic. */
#अगर_अघोषित HB_VECTOR_MAX_DOCUMENT_आकार
#घोषणा HB_VECTOR_MAX_DOCUMENT_आकार ((अचिन्हित) 16 << 20)
#पूर्ण_अगर

#अगर_अघोषित HB_RASTER_MAX_बफर_आकार
#घोषणा HB_RASTER_MAX_बफर_आकार ((माप_प्रकार) 1 << 30)
#पूर्ण_अगर

/* Maximum surface dimension (pixels per side) when extents are derived
 * from font data (glyph extents or accumulated outline bounds).  Bounds
 * attacker-controlled allocations; extents set explicitly through
 * hb_raster_{draw,paint}_set_extents() are not limited. */
#अगर_अघोषित HB_RASTER_MAX_AUTO_DIMENSION
#घोषणा HB_RASTER_MAX_AUTO_DIMENSION 4096
#पूर्ण_अगर

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
#घोषणा HB_BUDGET_1	1u
#घोषणा HB_BUDGET_2	2u
#घोषणा HB_BUDGET_4	4u
#घोषणा HB_BUDGET_8	8u
#घोषणा HB_BUDGET_16	16u
#घोषणा HB_BUDGET_32	32u
#घोषणा HB_BUDGET_64	64u
#घोषणा HB_BUDGET_128	128u
#घोषणा HB_BUDGET_256	256u
#घोषणा HB_BUDGET_512	512u
#घोषणा HB_BUDGET_1024	1024u

/* The common finite default for one top-level glyph rendering session.
 * Nested outline and paint work shares the same live counter. */
#अगर_अघोषित HB_BUDGET_GLYPH
#घोषणा HB_BUDGET_GLYPH ((int64_t) 1 << 24)
#पूर्ण_अगर

/* Precharge COST * MULT.  Callers bound COST and MULT structurally, and live
 * budgets are concrete non-negative values when a session starts. */
अटल HB_ALWAYS_INLINE bool
hb_budget_spend (int64_t &budget, अचिन्हित पूर्णांक cost, अचिन्हित पूर्णांक mult = 1)
{
  budget -= (int64_t) cost * mult;
  वापस budget >= 0;
}

/* The flat pixel-work floor for one raster paint session.  Pixel and
 * outline work use separate live counters, so keep this independent of
 * the common outline budget above. */
#अगर_अघोषित HB_BUDGET_RASTER_PIXELS
#घोषणा HB_BUDGET_RASTER_PIXELS ((int64_t) 1 << 26)
#पूर्ण_अगर

/* Very large raster surfaces still get a few full-surface operations. */
#अगर_अघोषित HB_BUDGET_RASTER_PAINT_PASSES
#घोषणा HB_BUDGET_RASTER_PAINT_PASSES 4
#पूर्ण_अगर

/* One raster draw session, in accumulated non-horizontal edges. */
#अगर_अघोषित HB_RASTER_MAX_DRAW_EDGES
#घोषणा HB_RASTER_MAX_DRAW_EDGES ((int64_t) 1 << 20)
#पूर्ण_अगर


#अगर_अघोषित HB_REPACKER_MAX_ITERATIONS
#घोषणा HB_REPACKER_MAX_ITERATIONS 500
#पूर्ण_अगर

#अगर_अघोषित HB_REPACKER_MAX_VERTICES
#घोषणा HB_REPACKER_MAX_VERTICES 800000
#पूर्ण_अगर

#अगर_अघोषित HB_REPACKER_MAX_SPACES
#घोषणा HB_REPACKER_MAX_SPACES 8000
#पूर्ण_अगर


#पूर्ण_अगर /* HB_LIMITS_HH */
