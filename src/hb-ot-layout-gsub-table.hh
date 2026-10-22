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

#جے_بنا_تعریف HB_OT_LAYOUT_GSUB_جدول_HH
#تعریف_کرو HB_OT_LAYOUT_GSUB_جدول_HH

#ضمن "OT/Layout/GSUB/GSUB.hh"

نطاق OT {
نطاق Layout {
نطاق GSUB_impl {

// TODO(garretrieger): Move into the new layout directory.
/* Out-of-class implementation for methods recursing */

#جے_بنا_تعریف HB_NO_OT_LAYOUT
/*static*/ مضمن منطقي ExtensionSubst::is_الٹ () ثابت
{
  أرجع SubstLookup::lookup_type_is_الٹ (get_type ());
}
سانچا <قسم_ناں context_t>
/*static*/ قسم_ناں context_t::return_t SubstLookup::dispatch_recurse_func (context_t *c, بنانشانی صحيح lookup_اشاریہ)
{
  ثابت SubstLookup &l = c->face->جدول.GSUB.get_relaxed ()->جدول->get_lookup (lookup_اشاریہ);
  أرجع l.dispatch (c);
}

/*static*/ قسم_ناں hb_گھیرا_context_t::return_t SubstLookup::گھیرا_glyphs_recurse_func (hb_گھیرا_context_t *c, بنانشانی lookup_اشاریہ, hb_سیٹ_t *covered_seq_indices, بنانشانی seq_اشاریہ, بنانشانی سرا_اشاریہ)
{
  ثابت SubstLookup &l = c->face->جدول.GSUB.get_relaxed ()->جدول->get_lookup (lookup_اشاریہ);
  إذا (l.may_have_non_1to1 ())
      hb_سیٹ_جوڑو_range (covered_seq_indices, seq_اشاریہ, سرا_اشاریہ);
  أرجع l.dispatch (c);
}

/*static*/ hb_depend_context_t::return_t SubstLookup::depend_glyphs_recurse_func (hb_depend_context_t *c, بنانشانی lookup_اشاریہ, hb_ثنائی_ہندسہ_ورقہ_t *covered_seq_indices, بنانشانی seq_اشاریہ, بنانشانی سرا_اشاریہ)
{
  ثابت SubstLookup &l = c->face->جدول.GSUB.get_relaxed ()->جدول->get_lookup (lookup_اشاریہ);
  /* After a non-1-to-1 lookup (expansion/contraction), subsequent lookups in the same
   * contextual rule see all glyphs, not position-specific ones. This matches closure behavior.
   * Mark sequence positions as covered so later lookups use the full glyph set. */
  إذا (l.may_have_non_1to1 ())
      covered_seq_indices->جوڑو_range (seq_اشاریہ, سرا_اشاریہ);

  hb_depend_context_t::recurse_کلید_t key;
  إذا (!c->get_recurse_کلید (lookup_اشاریہ, &key))
    أرجع hb_خالی_تھاواں_t ();

  hb_depend_context_t::return_t ret = l.dispatch (c);
  c->انت_recurse (key);
  أرجع ret;
}

سانچا <>
مضمن hb_گھیرا_lookups_context_t::return_t
SubstLookup::dispatch_recurse_func<hb_گھیرا_lookups_context_t> (hb_گھیرا_lookups_context_t *c, بنانشانی this_اشاریہ)
{
  ثابت SubstLookup &l = c->face->جدول.GSUB.get_relaxed ()->جدول->get_lookup (this_اشاریہ);
  أرجع l.گھیرا_lookups (c, this_اشاریہ);
}

سانچا <>
مضمن منطقي SubstLookup::dispatch_recurse_func<hb_ot_apply_context_t> (hb_ot_apply_context_t *c, بنانشانی صحيح lookup_اشاریہ)
{
  خودکار *gsub = c->face->جدول.GSUB.get_relaxed ();
  ثابت SubstLookup &l = gsub->جدول->get_lookup (lookup_اشاریہ);
  بنانشانی صحيح saved_lookup_props = c->lookup_props;
  بنانشانی صحيح saved_lookup_اشاریہ = c->lookup_اشاریہ;
  c->سیٹ_lookup_اشاریہ (lookup_اشاریہ);
  c->سیٹ_lookup_props (l.get_props ());

  uint32_t ڈھیر_میل_positions[8];
  hb_سمتیہ_t<uint32_t> saved_میل_positions;
  saved_میل_positions.سیٹ_storage (ڈھیر_میل_positions);
  hb_وٹاندرا (c->میل_positions, saved_میل_positions);

  منطقي ret = خطأ;
  خودکار *accel = gsub->get_accel (lookup_اشاریہ);
  ret = accel && accel->apply (c, خطأ);

  c->سیٹ_lookup_اشاریہ (saved_lookup_اشاریہ);
  c->سیٹ_lookup_props (saved_lookup_props);

  hb_وٹاندرا (c->میل_positions, saved_میل_positions);

  أرجع ret;
}
#جے_انت

} /* namespace GSUB_impl */
} /* namespace Layout */

مضمن فراغ
GSUB_accelerator_t::depend (hb_depend_انکڑے_builder_t *builder, hb_face_t *face) ثابت
{
  إذا (!هذا->جدول->has_انکڑے ()) أرجع;

  بنانشانی عدد_features = هذا->جدول->get_feature_گنتی ();
  بنانشانی عدد_lookups  = هذا->جدول->get_lookup_گنتی ();

  hb_سمتیہ_t<hb_ٹیگ_t> feature_ٹیگ_سوچی;
  إذا (!builder->check_success (feature_ٹیگ_سوچی.resize (عدد_features)))
    أرجع;
  هذا->جدول->get_feature_ٹیگ_سوچی (0, &عدد_features, feature_ٹیگ_سوچی.arrayZ);

  إذا (!builder->init_lookup_features (عدد_lookups))
    أرجع;

  hb_سمتیہ_t<hb_ٹیگ_t> feature_query_v;
  feature_query_v.resize (2);
  feature_query_v[1] = 0;

  hb_سیٹ_t تکیا_features;
  hb_سیٹ_t feature_indexes, lookup_indexes;

  لكل (خودکار ft : feature_ٹیگ_سوچی)
  {
    إذا (تکیا_features.has (ft)) استمر;
    تکیا_features.جوڑو (ft);
    feature_query_v[0] = ft;
    feature_indexes.reset ();
    hb_ot_layout_collect_features (face, HB_OT_ٹیگ_GSUB, خالی_سوچک, خالی_سوچک,
                                   feature_query_v.arrayZ, &feature_indexes);
    lookup_indexes.reset ();
    لكل (خودکار feature_اشاریہ : feature_indexes)
      هذا->جدول->get_feature (feature_اشاریہ).جوڑو_lookup_indexes_منزل (&lookup_indexes);

    لكل (خودکار lookup_اشاریہ : lookup_indexes)
      إذا (unlikely (!builder->جوڑو_lookup_feature (lookup_اشاریہ, ft)))
	أرجع;

    خودکار &fv = هذا->جدول->get_feature_variations ();
    خودکار fi_گنتی = fv.ریکارڈ_گنتی ();
    لكل (بنانشانی i = 0; i < fi_گنتی; i++)
    {
      lookup_indexes.reset ();
      لكل (خودکار feature_اشاریہ : feature_indexes)
      {
	خودکار feature_ptr = fv.لبھو_substitute (i, feature_اشاریہ);
	إذا (feature_ptr != خالی_سوچک)
	  feature_ptr->جوڑو_lookup_indexes_منزل (&lookup_indexes);
      }
      لكل (خودکار lookup_اشاریہ : lookup_indexes)
	إذا (unlikely (!builder->جوڑو_lookup_feature (lookup_اشاریہ, ft)))
	  أرجع;
    }

#جے_بنا_تعریف HB_NO_VAR
    lookup_indexes.reset ();
    fv.collect_lookup_variation_lookups (&feature_indexes, &lookup_indexes);
    لكل (خودکار lookup_اشاریہ : lookup_indexes)
      إذا (unlikely (!builder->جوڑو_lookup_feature (lookup_اشاریہ, ft)))
	أرجع;
#جے_انت
  }

  إذا (unlikely (!builder->انت_lookup_features ()))
    أرجع;

  hb_سیٹ_t all_glyphs;
  all_glyphs.جوڑو_range (0, face->get_عدد_glyphs () - 1);

  hb_depend_context_t c (builder, face, &all_glyphs);

  لكل (بنانشانی i = 0; i < عدد_lookups; i++)
  {
    خودکار features = builder->get_lookup_features (i);
    إذا (!features)
    {
      DEBUG_پیغام_LEVEL (DEPEND, خالی_سوچک, 1, 0,
                       "Skipping lookup %u (no features)", i);
      استمر;
    }
    DEBUG_پیغام_LEVEL (DEPEND, خالی_سوچک, 1, 0,
                     "Processing lookup %u with features:", i);
    c.lookup_اشاریہ = i;
    c.reset_recurse_cache ();
    c.lookups_تکیا.clear ();
    c.lookups_تکیا.جوڑو (i);  /* Seed for A→B→A cycle detection in recurse(). */
    هذا->جدول->get_lookup (i).depend (&c);
  }
}

} /* namespace OT */

#جے_انت /* HB_OT_LAYOUT_GSUB_TABLE_HH */
