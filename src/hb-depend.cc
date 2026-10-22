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

#ضمن "hb.hh"

#ضمن "hb-depend.hh"

#ضمن "hb-face.hh"
#ضمن "hb-ot-cmap-table.hh"
#ضمن "hb-ot-glyf-table.hh"
#ضمن "hb-ot-layout-gsub-table.hh"
#ضمن "hb-ot-math-table.hh"
#ضمن "hb-ot-cff1-table.hh"
#ضمن "hb-ot-var-varc-table.hh"
#ضمن "OT/Color/COLR/COLR.hh"
#ضمن "OT/Color/COLR/colrv1-depend.hh"

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
  إذا (unlikely (!معلومات.glyph_dependencies.resize_exact (f->get_عدد_glyphs ()))) {
    successful = خطأ;
    أرجع;
  }
  successful = hb_depend_معلومات_builder_t (معلومات).compile (f);
}

منطقي
hb_depend_معلومات_builder_t::compile (hb_face_t *face)
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
  إذا (hb_ot_layout_has_substitution (face))
    face->جدول.GSUB.get_relaxed ()->depend (هذا, face);
#که_نه_تعریف_شوی HB_NO_MATH
  face->جدول.MATH->depend (هذا);
#د_که_پای
#که_نه_تعریف_شوی HB_NO_COLOR
  face->جدول.COLR->depend (هذا);
#د_که_پای
  face->جدول.glyf->depend (هذا);
#که_نه_تعریف_شوی HB_NO_CFF
  OT::cff1_subset_accelerator_t (face).depend (هذا);
#د_که_پای
#که_نه_تعریف_شوی HB_NO_VAR_COMPOSITES
  face->جدول.VARC->depend (هذا);
#د_که_پای
  أرجع successful;
}


#که_نه_تعریف_شوی HB_NO_SUBSET_DEPEND

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
  إذا (unlikely (!(depend = hb_object_create<hb_subset_depend_t> (face))))
    أرجع تش_اشاره;

  إذا (unlikely (depend->دننه_error ()))
  {
    hb_subset_depend_destroy (depend);
    أرجع تش_اشاره;
  }

  أرجع depend;
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
بینښه صحيح
hb_subset_depend_lookup_glyph (hb_subset_depend_t *depend,
                                hb_codepoint_t gid,
                                بینښه صحيح پیل_دننه_واټن,
                                بینښه صحيح *ثبت_شمېر,
                                hb_subset_depend_ثبت_t *entries)
{
  بینښه صحيح ټول = depend->معلومات.get_glyph_ثبت_شمېر (gid);
  إذا (ثبت_شمېر)
  {
    بینښه صحيح شمېر = hb_min (*ثبت_شمېر,
                                  پیل_دننه_واټن < ټول ? ټول - پیل_دننه_واټن : 0u);
    لكل (بینښه صحيح i = 0; i < شمېر; i++)
    {
      uint8_t flags_byte = 0;
      depend->معلومات.get_glyph_ثبت (gid, پیل_دننه_واټن + i,
                                    &entries[i].جدول_نښان_2, &entries[i].dependent,
                                    &entries[i].layout_نښان, &entries[i].ligature_سېټ_شاخص,
                                    &entries[i].context_سېټ_شاخص,
                                    &flags_byte);
      entries[i].flags = (hb_subset_depend_څنډه_flags_t) flags_byte;
    }
    *ثبت_شمېر = شمېر;
  }
  أرجع ټول;
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
hb_subset_depend_lookup_سېټ (hb_subset_depend_t *depend, hb_codepoint_t شاخص_2,
                              hb_سېټ_t *بهر)
{
  ثابت hb_سېټ_t *s = depend->get_سېټ_from_شاخص (شاخص_2);
  إذا (!s) أرجع خطأ;
  بهر->سېټ (*s);
  أرجع ریښتیا;
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
فراغ
hb_subset_depend_destroy (hb_subset_depend_t *depend)
{
  إذا (!hb_object_destroy (depend)) أرجع;

  hb_free (depend);
}

#د_که_پای /* !HB_NO_SUBSET_DEPEND */
