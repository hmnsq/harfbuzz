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

#اگر_غیر_تعریف_شدہ HB_OT_LAYOUT_GSUB_جدول_HH
#تعریف_کرو HB_OT_LAYOUT_GSUB_جدول_HH

#شامل "OT/Layout/GSUB/GSUB.hh"

نامخلا OT {
نامخلا Layout {
نامخلا GSUB_impl {

// TODO(garretrieger): Move into the new layout directory.
/* Out-of-class implementation for methods recursing */

#اگر_غیر_تعریف_شدہ HB_NO_OT_LAYOUT
/*static*/ اندرونی منطقی ExtensionSubst::is_الٹ () ثابت
{
  واپس SubstLookup::lookup_type_is_الٹ (get_type ());
}
سانچہ <قسم_نام context_t>
/*static*/ قسم_نام context_t::return_t SubstLookup::dispatch_recurse_func (context_t *c, غیرعلامتی صحیح lookup_اشاریہ)
{
  ثابت SubstLookup &l = c->face->جدول.GSUB.get_relaxed ()->جدول->get_lookup (lookup_اشاریہ);
  واپس l.dispatch (c);
}

/*static*/ قسم_نام hb_احاطہ_context_t::return_t SubstLookup::احاطہ_glyphs_recurse_func (hb_احاطہ_context_t *c, غیرعلامتی lookup_اشاریہ, hb_سیٹ_t *covered_seq_indices, غیرعلامتی seq_اشاریہ, غیرعلامتی انتہا_اشاریہ)
{
  ثابت SubstLookup &l = c->face->جدول.GSUB.get_relaxed ()->جدول->get_lookup (lookup_اشاریہ);
  اگر (l.may_have_non_1to1 ())
      hb_سیٹ_جوڑو_range (covered_seq_indices, seq_اشاریہ, انتہا_اشاریہ);
  واپس l.dispatch (c);
}

/*static*/ hb_depend_context_t::return_t SubstLookup::depend_glyphs_recurse_func (hb_depend_context_t *c, غیرعلامتی lookup_اشاریہ, hb_ثنائی_ہندسہ_صفحہ_t *covered_seq_indices, غیرعلامتی seq_اشاریہ, غیرعلامتی انتہا_اشاریہ)
{
  ثابت SubstLookup &l = c->face->جدول.GSUB.get_relaxed ()->جدول->get_lookup (lookup_اشاریہ);
  /* After a non-1-to-1 lookup (expansion/contraction), subsequent lookups in the same
   * contextual rule see all glyphs, not position-specific ones. This matches closure behavior.
   * Mark sequence positions as covered so later lookups use the full glyph set. */
  اگر (l.may_have_non_1to1 ())
      covered_seq_indices->جوڑو_range (seq_اشاریہ, انتہا_اشاریہ);

  hb_depend_context_t::recurse_کلید_t key;
  اگر (!c->get_recurse_کلید (lookup_اشاریہ, &key))
    واپس hb_خالی_خانے_t ();

  hb_depend_context_t::return_t ret = l.dispatch (c);
  c->اختتام_recurse (key);
  واپس ret;
}

سانچہ <>
اندرونی hb_احاطہ_lookups_context_t::return_t
SubstLookup::dispatch_recurse_func<hb_احاطہ_lookups_context_t> (hb_احاطہ_lookups_context_t *c, غیرعلامتی this_اشاریہ)
{
  ثابت SubstLookup &l = c->face->جدول.GSUB.get_relaxed ()->جدول->get_lookup (this_اشاریہ);
  واپس l.احاطہ_lookups (c, this_اشاریہ);
}

سانچہ <>
اندرونی منطقی SubstLookup::dispatch_recurse_func<hb_ot_apply_context_t> (hb_ot_apply_context_t *c, غیرعلامتی صحیح lookup_اشاریہ)
{
  خودکار *gsub = c->face->جدول.GSUB.get_relaxed ();
  ثابت SubstLookup &l = gsub->جدول->get_lookup (lookup_اشاریہ);
  غیرعلامتی صحیح saved_lookup_props = c->lookup_props;
  غیرعلامتی صحیح saved_lookup_اشاریہ = c->lookup_اشاریہ;
  c->سیٹ_lookup_اشاریہ (lookup_اشاریہ);
  c->سیٹ_lookup_props (l.get_props ());

  uint32_t ڈھیر_مطابقت_positions[8];
  hb_سمتیہ_t<uint32_t> saved_مطابقت_positions;
  saved_مطابقت_positions.سیٹ_storage (ڈھیر_مطابقت_positions);
  hb_تبادلہ (c->مطابقت_positions, saved_مطابقت_positions);

  منطقی ret = جھوٹ;
  خودکار *accel = gsub->get_accel (lookup_اشاریہ);
  ret = accel && accel->apply (c, جھوٹ);

  c->سیٹ_lookup_اشاریہ (saved_lookup_اشاریہ);
  c->سیٹ_lookup_props (saved_lookup_props);

  hb_تبادلہ (c->مطابقت_positions, saved_مطابقت_positions);

  واپس ret;
}
#اگر_ختم

} /* namespace GSUB_impl */
} /* namespace Layout */

اندرونی خالی
GSUB_accelerator_t::depend (hb_depend_ڈیٹا_builder_t *builder, hb_face_t *face) ثابت
{
  اگر (!یہ->جدول->has_ڈیٹا ()) واپس;

  غیرعلامتی عدد_features = یہ->جدول->get_feature_گنتی ();
  غیرعلامتی عدد_lookups  = یہ->جدول->get_lookup_گنتی ();

  hb_سمتیہ_t<hb_لیبل_t> feature_لیبل_فہرست;
  اگر (!builder->check_success (feature_لیبل_فہرست.resize (عدد_features)))
    واپس;
  یہ->جدول->get_feature_لیبل_فہرست (0, &عدد_features, feature_لیبل_فہرست.arrayZ);

  اگر (!builder->init_lookup_features (عدد_lookups))
    واپس;

  hb_سمتیہ_t<hb_لیبل_t> feature_query_v;
  feature_query_v.resize (2);
  feature_query_v[1] = 0;

  hb_سیٹ_t نظر_آیا_features;
  hb_سیٹ_t feature_indexes, lookup_indexes;

  برائے (خودکار ft : feature_لیبل_فہرست)
  {
    اگر (نظر_آیا_features.has (ft)) جاری;
    نظر_آیا_features.جوڑو (ft);
    feature_query_v[0] = ft;
    feature_indexes.reset ();
    hb_ot_layout_collect_features (face, HB_OT_لیبل_GSUB, خالی_اشاریہ, خالی_اشاریہ,
                                   feature_query_v.arrayZ, &feature_indexes);
    lookup_indexes.reset ();
    برائے (خودکار feature_اشاریہ : feature_indexes)
      یہ->جدول->get_feature (feature_اشاریہ).جوڑو_lookup_indexes_مقصد (&lookup_indexes);

    برائے (خودکار lookup_اشاریہ : lookup_indexes)
      اگر (unlikely (!builder->جوڑو_lookup_feature (lookup_اشاریہ, ft)))
	واپس;

    خودکار &fv = یہ->جدول->get_feature_variations ();
    خودکار fi_گنتی = fv.ریکارڈ_گنتی ();
    برائے (غیرعلامتی i = 0; i < fi_گنتی; i++)
    {
      lookup_indexes.reset ();
      برائے (خودکار feature_اشاریہ : feature_indexes)
      {
	خودکار feature_ptr = fv.ڈھونڈو_substitute (i, feature_اشاریہ);
	اگر (feature_ptr != خالی_اشاریہ)
	  feature_ptr->جوڑو_lookup_indexes_مقصد (&lookup_indexes);
      }
      برائے (خودکار lookup_اشاریہ : lookup_indexes)
	اگر (unlikely (!builder->جوڑو_lookup_feature (lookup_اشاریہ, ft)))
	  واپس;
    }

#اگر_غیر_تعریف_شدہ HB_NO_VAR
    lookup_indexes.reset ();
    fv.collect_lookup_variation_lookups (&feature_indexes, &lookup_indexes);
    برائے (خودکار lookup_اشاریہ : lookup_indexes)
      اگر (unlikely (!builder->جوڑو_lookup_feature (lookup_اشاریہ, ft)))
	واپس;
#اگر_ختم
  }

  اگر (unlikely (!builder->اختتام_lookup_features ()))
    واپس;

  hb_سیٹ_t all_glyphs;
  all_glyphs.جوڑو_range (0, face->get_عدد_glyphs () - 1);

  hb_depend_context_t c (builder, face, &all_glyphs);

  برائے (غیرعلامتی i = 0; i < عدد_lookups; i++)
  {
    خودکار features = builder->get_lookup_features (i);
    اگر (!features)
    {
      DEBUG_پیغام_LEVEL (DEPEND, خالی_اشاریہ, 1, 0,
                       "Skipping lookup %u (no features)", i);
      جاری;
    }
    DEBUG_پیغام_LEVEL (DEPEND, خالی_اشاریہ, 1, 0,
                     "Processing lookup %u with features:", i);
    c.lookup_اشاریہ = i;
    c.reset_recurse_cache ();
    c.lookups_نظر_آیا.clear ();
    c.lookups_نظر_آیا.جوڑو (i);  /* Seed for A→B→A cycle detection in recurse(). */
    یہ->جدول->get_lookup (i).depend (&c);
  }
}

} /* namespace OT */

#اگر_ختم /* HB_OT_LAYOUT_GSUB_TABLE_HH */
