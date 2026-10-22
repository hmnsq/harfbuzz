/*
 * Copyright © 2024  Adobe, Inc.
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
 *
 * Adobe Author(s): Skef Iterum
 */

#నిర్వచితం_కాకపోతే HB_SUBSET_DEPEND_H
#నిర్వచించు HB_SUBSET_DEPEND_H

#ఒకవేళ !defined(HB_SUBSET_H_లోపల) && !defined(HB_NO_SINGLE_శీర్షిక_ERROR)
#దోషం "Include <hb-subset.h> instead."
#అయితే_ముగింపు

#అన్తర్భావ "hb.h"

HB_BEGIN_DECLS

/**
 * hb_subset_depend_edge_flags_t:
 * @HB_SUBSET_DEPEND_EDGE_FLAG_NONE: No flags set.
 * @HB_SUBSET_DEPEND_EDGE_FLAG_FROM_CONTEXT_POSITION: Edge from a multi-position
 *   contextual rule (Context or ChainContext with inputCount > 1). Depend
 *   extraction records edges based on what glyphs could statically be at each
 *   position according to input coverage or class. However, at runtime lookups
 *   within the rule are applied sequentially: a lookup at an earlier position
 *   may transform the glyph at a later position, and two lookups at the same
 *   position may interact such that one produces a glyph that another
 *   immediately consumes as an "intermediate". A glyph that matches the static
 *   coverage may therefore not persist at that position when the rule actually
 *   fires, so this edge may not trigger during closure.
 * @HB_SUBSET_DEPEND_EDGE_FLAG_FROM_NESTED_CONTEXT: Edge from a lookup invoked
 *   within another contextual lookup. The outer context's requirements are not
 *   propagated to this edge, so the edge may fire even when those requirements
 *   are not met.
 *
 * Flags on dependency edges returned by hb_subset_depend_lookup_glyph() that
 * mark edges which may produce expected over-approximation when computing
 * closure via the depend graph, relative to
 * hb_ot_layout_lookups_substitute_closure(). These flags help distinguish
 * known limitations of static dependency analysis (expected over-approximation)
 * from bugs (unexpected over-approximation).
 *
 * Since: 14.3.0
 */
రకనిర్వచనం గణన { /*< flags >*/
  HB_SUBSET_DEPEND_అంచు_FLAG_NONE                   = 0x00u,
  HB_SUBSET_DEPEND_అంచు_FLAG_FROM_CONTEXT_POSITION  = 0x01u,
  HB_SUBSET_DEPEND_అంచు_FLAG_FROM_NESTED_CONTEXT    = 0x02u,
} hb_subset_depend_అంచు_flags_t;
#నిర్వచించు HB_SUBSET_DEPEND_అంచు_FLAGS_T_DEFINED
/* HB_MARK_AS_FLAG_T is applied in hb-depend-data.hh (internal header only) */

#నిర్వచితం_కాకపోతే HB_NO_SUBSET_DEPEND

/**
 * hb_subset_depend_t:
 *
 * Data type for holding glyph dependency graphs.
 *
 * <warning>Highly experimental API. Subject to change.</warning>
 *
 * Since: 14.3.0
 */
రకనిర్వచనం నిర్మాణం hb_subset_depend_t hb_subset_depend_t;

/**
 * hb_subset_depend_entry_t:
 * @table_tag: Source table (e.g. GSUB, glyf, CFF, COLR, MATH).
 * @dependent: Target glyph ID.
 * @layout_tag: Feature tag for GSUB edges; 0 otherwise.
 * @ligature_set_index: Index into the sets array for ligature component
 *   glyphs, or #HB_CODEPOINT_INVALID if not a ligature edge.
 * @context_set_index: Index into the sets array for context requirement
 *   glyphs, or #HB_CODEPOINT_INVALID if none. Use hb_subset_depend_lookup_set()
 *   to retrieve the set.
 * @flags: Edge flags (#hb_subset_depend_edge_flags_t).
 *
 * A single dependency edge returned by hb_subset_depend_lookup_glyph().
 *
 * Since: 14.3.0
 */
రకనిర్వచనం నిర్మాణం {
  hb_ట్యాగ్_t పట్టిక_ట్యాగ్_2;
  hb_codepoint_t dependent;
  hb_ట్యాగ్_t layout_ట్యాగ్;
  hb_codepoint_t ligature_సమితి_సూచీ;
  hb_codepoint_t context_సమితి_సూచీ;
  hb_subset_depend_అంచు_flags_t flags;
} hb_subset_depend_నమోదు_t;

HB_EXTERN hb_subset_depend_t *
hb_subset_depend_from_face_or_fail (hb_face_t *face);

HB_EXTERN గుర్తులేని పూర్ణాంకం
hb_subset_depend_lookup_glyph (hb_subset_depend_t *depend,
                                hb_codepoint_t gid,
                                గుర్తులేని పూర్ణాంకం ప్రారంభం_స్థానభ్రంశం,
                                గుర్తులేని పూర్ణాంకం *నమోదు_లెక్క, /* IN/OUT */
                                hb_subset_depend_నమోదు_t *entries /*బహిర్గతం*/);

HB_EXTERN hb_bool_t
hb_subset_depend_lookup_సమితి (hb_subset_depend_t *depend,
                              hb_codepoint_t సూచీ_2,
                              hb_సమితి_t *బహిర్గతం /*బహిర్గతం*/);

HB_EXTERN శూన్య
hb_subset_depend_destroy (hb_subset_depend_t *depend);

#అయితే_ముగింపు

HB_చివర_DECLS

#అయితే_ముగింపు /* HB_SUBSET_DEPEND_H */
