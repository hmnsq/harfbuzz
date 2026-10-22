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

#అన్తర్భావ "hb.hh"

#అన్తర్భావ "hb-depend.hh"

#అన్తర్భావ "hb-face.hh"
#అన్తర్భావ "hb-ot-cmap-table.hh"
#అన్తర్భావ "hb-ot-glyf-table.hh"
#అన్తర్భావ "hb-ot-layout-gsub-table.hh"
#అన్తర్భావ "hb-ot-math-table.hh"
#అన్తర్భావ "hb-ot-cff1-table.hh"
#అన్తర్భావ "hb-ot-var-varc-table.hh"
#అన్తర్భావ "OT/Color/COLR/COLR.hh"
#అన్తర్భావ "OT/Color/COLR/colrv1-depend.hh"

/**
 * SECTION:hb-subset-depend
 * @title: hb-subset-depend
 * @short_description: Glyph dependency graph for font optimization
 * @include: hb-subset-depend.h
 *
 * <warning>This API is highly experimental and subject to change. It is not
 * enabled by default and should not be used in production code without
 * understanding the stability implications.</warning>
 *
 * Functions for extracting a glyph dependency graph.
 *
 * A dependency graph represents the relationships between glyphs in a font,
 * tracking which glyphs reference or produce other glyphs through OpenType
 * mechanisms such as character mapping, glyph substitution, composite
 * construction, color layering, and math variants.
 *
 * Dependency graphs enable finding all glyphs reachable from a given input set.
 * This is useful for font subsetting, analyzing glyph coverage, optimizing
 * font delivery, and determining which glyphs are needed to render specific
 * characters.
 */

hb_subset_depend_t::hb_subset_depend_t (hb_face_t *f)
{
  ఒకవేళ (unlikely (!దత్తాంశం.glyph_dependencies.resize_exact (f->get_అంకె_glyphs ()))) {
    successful = అబద్ధం;
    తిరిగి;
  }
  successful = hb_depend_దత్తాంశం_builder_t (దత్తాంశం).compile (f);
}

తార్కిక
hb_depend_దత్తాంశం_builder_t::compile (hb_face_t *face)
{
  /* Extract dependencies from all relevant OpenType tables.
   * Note: cmap (UVS) dependencies are not extracted - UVS closure is handled
   * separately via hb_font_get_variation_glyph() query API during closure
   * computation.
   *
   * For GSUB, hb_ot_layout_has_substitution() forces lazy-loader initialization
   * in libharfbuzz, after which get_relaxed() safely returns the already-
   * constructed accelerator without instantiating the GSUB_accelerator_t
   * constructor (which references a hidden is_blocklisted symbol) in this TU. */
  ఒకవేళ (hb_ot_layout_has_substitution (face))
    face->పట్టిక.GSUB.get_relaxed ()->depend (ఇది, face);
#నిర్వచితం_కాకపోతే HB_NO_MATH
  face->పట్టిక.MATH->depend (ఇది);
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే HB_NO_COLOR
  face->పట్టిక.COLR->depend (ఇది);
#అయితే_ముగింపు
  face->పట్టిక.glyf->depend (ఇది);
#నిర్వచితం_కాకపోతే HB_NO_CFF
  OT::cff1_subset_accelerator_t (face).depend (ఇది);
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే HB_NO_VAR_COMPOSITES
  face->పట్టిక.VARC->depend (ఇది);
#అయితే_ముగింపు
  తిరిగి successful;
}


#నిర్వచితం_కాకపోతే HB_NO_SUBSET_DEPEND

/**
 * hb_subset_depend_from_face_or_fail:
 * @face: font face to collect dependencies from
 *
 * Calculates the dependencies between glyphs in the supplied face.
 * Extracts dependency information from GSUB, glyf, CFF, COLR, MATH,
 * and VARC tables. UVS (Unicode Variation Sequence) dependencies
 * are not included; handle those via hb_font_get_variation_glyph().
 *
 * Example:
 * ```c
 * hb_subset_depend_t *depend = hb_subset_depend_from_face_or_fail (face);
 * if (!depend) {
 *   // Handle error (OOM or invalid face)
 *   return;
 * }
 * // ... use depend ...
 * hb_subset_depend_destroy (depend);
 * ```
 *
 * Return value: (transfer full): New depend object, or `nullptr` if creation
 * failed (out of memory or invalid face). Destroy with hb_subset_depend_destroy().
 *
 * Since: 14.3.0
 **/
hb_subset_depend_t *
hb_subset_depend_from_face_or_fail (hb_face_t *face)
{
  hb_subset_depend_t *depend;
  ఒకవేళ (unlikely (!(depend = hb_object_create<hb_subset_depend_t> (face))))
    తిరిగి శూన్యసూచిక;

  ఒకవేళ (unlikely (depend->లోపల_error ()))
  {
    hb_subset_depend_destroy (depend);
    తిరిగి శూన్యసూచిక;
  }

  తిరిగి depend;
}

/**
 * hb_subset_depend_lookup_glyph:
 * @depend: depend object
 * @gid: GID to retrieve dependencies from
 * @start_offset: offset of first entry to retrieve
 * @entry_count: (inout) (optional): Input = number of entries to fill; output = number
 *   actually filled. Pass NULL to query total count without filling.
 * @entries: (out) (optional) (array length=entry_count): Array to fill with dependency
 *   edge data. May be NULL if @entry_count is also NULL.
 *
 * Retrieve dependency edges for a glyph. Follows the standard HarfBuzz array-getter
 * pattern: always returns the total number of edges for @gid regardless of
 * @start_offset or @entry_count.
 *
 * Example (iterate all entries):
 * ```c
 * unsigned int total = hb_subset_depend_lookup_glyph (depend, gid, 0, NULL, NULL);
 * for (unsigned int i = 0; i < total; i++) {
 *   hb_subset_depend_entry_t entry;
 *   unsigned int count = 1;
 *   hb_subset_depend_lookup_glyph (depend, gid, i, &count, &entry);
 *   // Process entry.table_tag, entry.dependent, etc.
 * }
 * ```
 *
 * Return value: Total number of dependency edges for @gid.
 *
 * Since: 14.3.0
 **/
గుర్తులేని పూర్ణాంకం
hb_subset_depend_lookup_glyph (hb_subset_depend_t *depend,
                                hb_codepoint_t gid,
                                గుర్తులేని పూర్ణాంకం ప్రారంభం_స్థానభ్రంశం,
                                గుర్తులేని పూర్ణాంకం *నమోదు_లెక్క,
                                hb_subset_depend_నమోదు_t *entries)
{
  గుర్తులేని పూర్ణాంకం మొత్తం = depend->దత్తాంశం.get_glyph_నమోదు_లెక్క (gid);
  ఒకవేళ (నమోదు_లెక్క)
  {
    గుర్తులేని పూర్ణాంకం లెక్క = hb_min (*నమోదు_లెక్క,
                                  ప్రారంభం_స్థానభ్రంశం < మొత్తం ? మొత్తం - ప్రారంభం_స్థానభ్రంశం : 0u);
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
    {
      uint8_t flags_byte = 0;
      depend->దత్తాంశం.get_glyph_నమోదు (gid, ప్రారంభం_స్థానభ్రంశం + i,
                                    &entries[i].పట్టిక_ట్యాగ్_2, &entries[i].dependent,
                                    &entries[i].layout_ట్యాగ్, &entries[i].ligature_సమితి_సూచీ,
                                    &entries[i].context_సమితి_సూచీ,
                                    &flags_byte);
      entries[i].flags = (hb_subset_depend_అంచు_flags_t) flags_byte;
    }
    *నమోదు_లెక్క = లెక్క;
  }
  తిరిగి మొత్తం;
}

/**
 * hb_subset_depend_lookup_set:
 * @depend: depend object
 * @index: the index of the set
 * @out: (out): A pointer to a set to copy into
 *
 * Get all glyphs in a set identified by @index.
 * The set index comes from the ligature_set_index or context_set_index field
 * returned by hb_subset_depend_lookup_glyph().
 *
 * Example:
 * ```c
 * hb_set_t *ligature_glyphs = hb_set_create ();
 * if (hb_subset_depend_lookup_set (depend, entry.ligature_set_index, ligature_glyphs)) {
 *   // Process glyphs in the set...
 * }
 * hb_set_destroy (ligature_glyphs);
 * ```
 *
 * Return value: true if there is such a set, false otherwise
 *
 * Since: 14.3.0
 **/
hb_bool_t
hb_subset_depend_lookup_సమితి (hb_subset_depend_t *depend, hb_codepoint_t సూచీ_2,
                              hb_సమితి_t *బహిర్గతం)
{
  స్థిరరాశి hb_సమితి_t *s = depend->get_సమితి_from_సూచీ (సూచీ_2);
  ఒకవేళ (!s) తిరిగి అబద్ధం;
  బహిర్గతం->సమితి (*s);
  తిరిగి నిజం;
}

/**
 * hb_subset_depend_destroy:
 * @depend: a #hb_subset_depend_t
 *
 * Decreases the reference count on @depend, and if it reaches zero, destroys
 * @depend, freeing all memory.
 *
 * Since: 14.3.0
 **/
శూన్య
hb_subset_depend_destroy (hb_subset_depend_t *depend)
{
  ఒకవేళ (!hb_object_destroy (depend)) తిరిగి;

  hb_free (depend);
}

#అయితే_ముగింపు /* !HB_NO_SUBSET_DEPEND */
