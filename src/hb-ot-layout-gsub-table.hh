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

#اگر_تعریف_نشده HB_OT_LAYOUT_GSUB_جدول_HH
#تعریف_کن HB_OT_LAYOUT_GSUB_جدول_HH

#ضمن "OT/Layout/GSUB/GSUB.hh"

نطاق OT {
نطاق Layout {
نطاق GSUB_impl {

// TODO(garretrieger): Move into the new layout directory.
/* Out-of-class implementation for methods recursing */

#اگر_تعریف_نشده HB_NO_OT_LAYOUT
/*static*/ مضمن منطقي ExtensionSubst::is_معکوس () ثابت
{
  أرجع SubstLookup::lookup_type_is_معکوس (get_type ());
}
قالب <نام_نوع context_t>
/*static*/ نام_نوع context_t::return_t SubstLookup::dispatch_recurse_func (context_t *c, بیعلامت صحيح lookup_شاخص)
{
  ثابت SubstLookup &l = c->face->جدول.GSUB.get_relaxed ()->جدول->get_lookup (lookup_شاخص);
  أرجع l.dispatch (c);
}

/*static*/ نام_نوع hb_بستگی_context_t::return_t SubstLookup::بستگی_glyphs_recurse_func (hb_بستگی_context_t *c, بیعلامت lookup_شاخص, hb_مجموعه_t *covered_seq_indices, بیعلامت seq_شاخص, بیعلامت انتها_شاخص)
{
  ثابت SubstLookup &l = c->face->جدول.GSUB.get_relaxed ()->جدول->get_lookup (lookup_شاخص);
  إذا (l.may_have_non_1to1 ())
      hb_مجموعه_جمع_کن_range (covered_seq_indices, seq_شاخص, انتها_شاخص);
  أرجع l.dispatch (c);
}

/*static*/ hb_depend_context_t::return_t SubstLookup::depend_glyphs_recurse_func (hb_depend_context_t *c, بیعلامت lookup_شاخص, hb_بیت_صفحه_t *covered_seq_indices, بیعلامت seq_شاخص, بیعلامت انتها_شاخص)
{
  ثابت SubstLookup &l = c->face->جدول.GSUB.get_relaxed ()->جدول->get_lookup (lookup_شاخص);
  /* After a non-1-to-1 lookup (expansion/contraction), subsequent lookups in the same
   * contextual rule see all glyphs, not position-specific ones. This matches closure behavior.
   * Mark sequence positions as covered so later lookups use the full glyph set. */
  إذا (l.may_have_non_1to1 ())
      covered_seq_indices->جمع_کن_range (seq_شاخص, انتها_شاخص);

  hb_depend_context_t::recurse_کلید_t key;
  إذا (!c->get_recurse_کلید (lookup_شاخص, &key))
    أرجع hb_خالی_t ();

  hb_depend_context_t::return_t ret = l.dispatch (c);
  c->پایان_recurse (key);
  أرجع ret;
}

قالب <>
مضمن hb_بستگی_lookups_context_t::return_t
SubstLookup::dispatch_recurse_func<hb_بستگی_lookups_context_t> (hb_بستگی_lookups_context_t *c, بیعلامت this_شاخص)
{
  ثابت SubstLookup &l = c->face->جدول.GSUB.get_relaxed ()->جدول->get_lookup (this_شاخص);
  أرجع l.بستگی_lookups (c, this_شاخص);
}

قالب <>
مضمن منطقي SubstLookup::dispatch_recurse_func<hb_ot_apply_context_t> (hb_ot_apply_context_t *c, بیعلامت صحيح lookup_شاخص)
{
  خودکار *gsub = c->face->جدول.GSUB.get_relaxed ();
  ثابت SubstLookup &l = gsub->جدول->get_lookup (lookup_شاخص);
  بیعلامت صحيح saved_lookup_props = c->lookup_props;
  بیعلامت صحيح saved_lookup_شاخص = c->lookup_شاخص;
  c->مجموعه_lookup_شاخص (lookup_شاخص);
  c->مجموعه_lookup_props (l.get_props ());

  uint32_t پشته_تطابق_positions[8];
  hb_وکتور_t<uint32_t> saved_تطابق_positions;
  saved_تطابق_positions.مجموعه_storage (پشته_تطابق_positions);
  hb_جابجا (c->تطابق_positions, saved_تطابق_positions);

  منطقي ret = خطأ;
  خودکار *accel = gsub->get_accel (lookup_شاخص);
  ret = accel && accel->apply (c, خطأ);

  c->مجموعه_lookup_شاخص (saved_lookup_شاخص);
  c->مجموعه_lookup_props (saved_lookup_props);

  hb_جابجا (c->تطابق_positions, saved_تطابق_positions);

  أرجع ret;
}
#پایان_اگر

} /* namespace GSUB_impl */
} /* namespace Layout */

مضمن فراغ
GSUB_accelerator_t::depend (hb_depend_داده_builder_t *builder, hb_face_t *face) ثابت
{
  إذا (!هذا->جدول->has_داده ()) أرجع;

  بیعلامت رقم_features = هذا->جدول->get_feature_شمار ();
  بیعلامت رقم_lookups  = هذا->جدول->get_lookup_شمار ();

  hb_وکتور_t<hb_برچسب_t> feature_برچسبها;
  إذا (!builder->check_success (feature_برچسبها.resize (رقم_features)))
    أرجع;
  هذا->جدول->get_feature_برچسبها (0, &رقم_features, feature_برچسبها.arrayZ);

  إذا (!builder->init_lookup_features (رقم_lookups))
    أرجع;

  hb_وکتور_t<hb_برچسب_t> feature_query_v;
  feature_query_v.resize (2);
  feature_query_v[1] = 0;

  hb_مجموعه_t دیده_features;
  hb_مجموعه_t feature_indexes, lookup_indexes;

  لكل (خودکار ft : feature_برچسبها)
  {
    إذا (دیده_features.has (ft)) استمر;
    دیده_features.جمع_کن (ft);
    feature_query_v[0] = ft;
    feature_indexes.reset ();
    hb_ot_layout_collect_features (face, HB_OT_برچسب_GSUB, اشاره_تهی, اشاره_تهی,
                                   feature_query_v.arrayZ, &feature_indexes);
    lookup_indexes.reset ();
    لكل (خودکار feature_شاخص : feature_indexes)
      هذا->جدول->get_feature (feature_شاخص).جمع_کن_lookup_indexes_مقصد (&lookup_indexes);

    لكل (خودکار lookup_شاخص : lookup_indexes)
      إذا (unlikely (!builder->جمع_کن_lookup_feature (lookup_شاخص, ft)))
	أرجع;

    خودکار &fv = هذا->جدول->get_feature_variations ();
    خودکار fi_شمار = fv.رکورد_شمار ();
    لكل (بیعلامت i = 0; i < fi_شمار; i++)
    {
      lookup_indexes.reset ();
      لكل (خودکار feature_شاخص : feature_indexes)
      {
	خودکار feature_ptr = fv.پیدا_کن_substitute (i, feature_شاخص);
	إذا (feature_ptr != اشاره_تهی)
	  feature_ptr->جمع_کن_lookup_indexes_مقصد (&lookup_indexes);
      }
      لكل (خودکار lookup_شاخص : lookup_indexes)
	إذا (unlikely (!builder->جمع_کن_lookup_feature (lookup_شاخص, ft)))
	  أرجع;
    }

#اگر_تعریف_نشده HB_NO_VAR
    lookup_indexes.reset ();
    fv.collect_lookup_variation_lookups (&feature_indexes, &lookup_indexes);
    لكل (خودکار lookup_شاخص : lookup_indexes)
      إذا (unlikely (!builder->جمع_کن_lookup_feature (lookup_شاخص, ft)))
	أرجع;
#پایان_اگر
  }

  إذا (unlikely (!builder->پایان_lookup_features ()))
    أرجع;

  hb_مجموعه_t all_glyphs;
  all_glyphs.جمع_کن_range (0, face->get_رقم_glyphs () - 1);

  hb_depend_context_t c (builder, face, &all_glyphs);

  لكل (بیعلامت i = 0; i < رقم_lookups; i++)
  {
    خودکار features = builder->get_lookup_features (i);
    إذا (!features)
    {
      DEBUG_پیام_LEVEL (DEPEND, اشاره_تهی, 1, 0,
                       "Skipping lookup %u (no features)", i);
      استمر;
    }
    DEBUG_پیام_LEVEL (DEPEND, اشاره_تهی, 1, 0,
                     "Processing lookup %u with features:", i);
    c.lookup_شاخص = i;
    c.reset_recurse_cache ();
    c.lookups_دیده.clear ();
    c.lookups_دیده.جمع_کن (i);  /* Seed for A→B→A cycle detection in recurse(). */
    هذا->جدول->get_lookup (i).depend (&c);
  }
}

} /* namespace OT */

#پایان_اگر /* HB_OT_LAYOUT_GSUB_TABLE_HH */
