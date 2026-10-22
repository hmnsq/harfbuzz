/*
 * Copyright © 2007,2008,2009,2010  Red Hat, Inc.
 * Copyright © 2010,2012,2013  Google, Inc.
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
 * Red Hat Author(s): Behdad Esfahbod
 * Google Author(s): Behdad Esfahbod
 */

#که_نه_تعریف_شوی HB_OT_LAYOUT_GSUB_جدول_HH
#تعریف_کړه HB_OT_LAYOUT_GSUB_جدول_HH

#ضمن "OT/Layout/GSUB/GSUB.hh"

نطاق OT {
نطاق Layout {
نطاق GSUB_impl {

// TODO(garretrieger): Move into the new layout directory.
/* Out-of-class implementation for methods recursing */

#که_نه_تعریف_شوی HB_NO_OT_LAYOUT
/*static*/ مضمن منطقي ExtensionSubst::is_سرچپه () ثابت
{
  أرجع SubstLookup::lookup_type_is_سرچپه (get_type ());
}
کينډۍ <ډول_نوم context_t>
/*static*/ ډول_نوم context_t::return_t SubstLookup::dispatch_recurse_func (context_t *c, بینښه صحيح lookup_شاخص)
{
  ثابت SubstLookup &l = c->face->جدول.GSUB.get_relaxed ()->جدول->get_lookup (lookup_شاخص);
  أرجع l.dispatch (c);
}

/*static*/ ډول_نوم hb_تړون_context_t::return_t SubstLookup::تړون_glyphs_recurse_func (hb_تړون_context_t *c, بینښه lookup_شاخص, hb_سېټ_t *covered_seq_indices, بینښه seq_شاخص, بینښه پای_ټکی_شاخص)
{
  ثابت SubstLookup &l = c->face->جدول.GSUB.get_relaxed ()->جدول->get_lookup (lookup_شاخص);
  إذا (l.may_have_non_1to1 ())
      hb_سېټ_جمع_کړه_range (covered_seq_indices, seq_شاخص, پای_ټکی_شاخص);
  أرجع l.dispatch (c);
}

/*static*/ hb_depend_context_t::return_t SubstLookup::depend_glyphs_recurse_func (hb_depend_context_t *c, بینښه lookup_شاخص, hb_بټ_پاڼه_t *covered_seq_indices, بینښه seq_شاخص, بینښه پای_ټکی_شاخص)
{
  ثابت SubstLookup &l = c->face->جدول.GSUB.get_relaxed ()->جدول->get_lookup (lookup_شاخص);
  /* After a non-1-to-1 lookup (expansion/contraction), subsequent lookups in the same
   * contextual rule see all glyphs, not position-specific ones. This matches closure behavior.
   * Mark sequence positions as covered so later lookups use the full glyph set. */
  إذا (l.may_have_non_1to1 ())
      covered_seq_indices->جمع_کړه_range (seq_شاخص, پای_ټکی_شاخص);

  hb_depend_context_t::recurse_کلی_t key;
  إذا (!c->get_recurse_کلی (lookup_شاخص, &key))
    أرجع hb_تشه_t ();

  hb_depend_context_t::return_t ret = l.dispatch (c);
  c->پای_recurse (key);
  أرجع ret;
}

کينډۍ <>
مضمن hb_تړون_lookups_context_t::return_t
SubstLookup::dispatch_recurse_func<hb_تړون_lookups_context_t> (hb_تړون_lookups_context_t *c, بینښه this_شاخص)
{
  ثابت SubstLookup &l = c->face->جدول.GSUB.get_relaxed ()->جدول->get_lookup (this_شاخص);
  أرجع l.تړون_lookups (c, this_شاخص);
}

کينډۍ <>
مضمن منطقي SubstLookup::dispatch_recurse_func<hb_ot_apply_context_t> (hb_ot_apply_context_t *c, بینښه صحيح lookup_شاخص)
{
  خپلکاری *gsub = c->face->جدول.GSUB.get_relaxed ();
  ثابت SubstLookup &l = gsub->جدول->get_lookup (lookup_شاخص);
  بینښه صحيح saved_lookup_props = c->lookup_props;
  بینښه صحيح saved_lookup_شاخص = c->lookup_شاخص;
  c->سېټ_lookup_شاخص (lookup_شاخص);
  c->سېټ_lookup_props (l.get_props ());

  uint32_t ډېری_سمون_positions[8];
  hb_ویکټور_t<uint32_t> saved_سمون_positions;
  saved_سمون_positions.سېټ_storage (ډېری_سمون_positions);
  hb_بدلون (c->سمون_positions, saved_سمون_positions);

  منطقي ret = خطأ;
  خپلکاری *accel = gsub->get_accel (lookup_شاخص);
  ret = accel && accel->apply (c, خطأ);

  c->سېټ_lookup_شاخص (saved_lookup_شاخص);
  c->سېټ_lookup_props (saved_lookup_props);

  hb_بدلون (c->سمون_positions, saved_سمون_positions);

  أرجع ret;
}
#د_که_پای

} /* namespace GSUB_impl */
} /* namespace Layout */

مضمن فراغ
GSUB_accelerator_t::depend (hb_depend_معلومات_builder_t *builder, hb_face_t *face) ثابت
{
  إذا (!هذا->جدول->has_معلومات ()) أرجع;

  بینښه عدد_features = هذا->جدول->get_feature_شمېر ();
  بینښه عدد_lookups  = هذا->جدول->get_lookup_شمېر ();

  hb_ویکټور_t<hb_نښان_t> feature_نښانونه;
  إذا (!builder->check_success (feature_نښانونه.resize (عدد_features)))
    أرجع;
  هذا->جدول->get_feature_نښانونه (0, &عدد_features, feature_نښانونه.arrayZ);

  إذا (!builder->init_lookup_features (عدد_lookups))
    أرجع;

  hb_ویکټور_t<hb_نښان_t> feature_query_v;
  feature_query_v.resize (2);
  feature_query_v[1] = 0;

  hb_سېټ_t کتل_شوی_features;
  hb_سېټ_t feature_indexes, lookup_indexes;

  لكل (خپلکاری ft : feature_نښانونه)
  {
    إذا (کتل_شوی_features.has (ft)) استمر;
    کتل_شوی_features.جمع_کړه (ft);
    feature_query_v[0] = ft;
    feature_indexes.reset ();
    hb_ot_layout_collect_features (face, HB_OT_نښان_GSUB, تش_اشاره, تش_اشاره,
                                   feature_query_v.arrayZ, &feature_indexes);
    lookup_indexes.reset ();
    لكل (خپلکاری feature_شاخص : feature_indexes)
      هذا->جدول->get_feature (feature_شاخص).جمع_کړه_lookup_indexes_منزل (&lookup_indexes);

    لكل (خپلکاری lookup_شاخص : lookup_indexes)
      إذا (unlikely (!builder->جمع_کړه_lookup_feature (lookup_شاخص, ft)))
	أرجع;

    خپلکاری &fv = هذا->جدول->get_feature_variations ();
    خپلکاری fi_شمېر = fv.ریکارډ_شمېر ();
    لكل (بینښه i = 0; i < fi_شمېر; i++)
    {
      lookup_indexes.reset ();
      لكل (خپلکاری feature_شاخص : feature_indexes)
      {
	خپلکاری feature_ptr = fv.پیدا_کړه_substitute (i, feature_شاخص);
	إذا (feature_ptr != تش_اشاره)
	  feature_ptr->جمع_کړه_lookup_indexes_منزل (&lookup_indexes);
      }
      لكل (خپلکاری lookup_شاخص : lookup_indexes)
	إذا (unlikely (!builder->جمع_کړه_lookup_feature (lookup_شاخص, ft)))
	  أرجع;
    }

#که_نه_تعریف_شوی HB_NO_VAR
    lookup_indexes.reset ();
    fv.collect_lookup_variation_lookups (&feature_indexes, &lookup_indexes);
    لكل (خپلکاری lookup_شاخص : lookup_indexes)
      إذا (unlikely (!builder->جمع_کړه_lookup_feature (lookup_شاخص, ft)))
	أرجع;
#د_که_پای
  }

  إذا (unlikely (!builder->پای_lookup_features ()))
    أرجع;

  hb_سېټ_t all_glyphs;
  all_glyphs.جمع_کړه_range (0, face->get_عدد_glyphs () - 1);

  hb_depend_context_t c (builder, face, &all_glyphs);

  لكل (بینښه i = 0; i < عدد_lookups; i++)
  {
    خپلکاری features = builder->get_lookup_features (i);
    إذا (!features)
    {
      DEBUG_پیغام_LEVEL (DEPEND, تش_اشاره, 1, 0,
                       "Skipping lookup %u (no features)", i);
      استمر;
    }
    DEBUG_پیغام_LEVEL (DEPEND, تش_اشاره, 1, 0,
                     "Processing lookup %u with features:", i);
    c.lookup_شاخص = i;
    c.reset_recurse_cache ();
    c.lookups_کتل_شوی.clear ();
    c.lookups_کتل_شوی.جمع_کړه (i);  /* Seed for A→B→A cycle detection in recurse(). */
    هذا->جدول->get_lookup (i).depend (&c);
  }
}

} /* namespace OT */

#د_که_پای /* HB_OT_LAYOUT_GSUB_TABLE_HH */
