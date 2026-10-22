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

#جيڪڏهن_وصف_نه_ٿيل HB_OT_LAYOUT_GSUB_جدول_HH
#وصف_ڪريو HB_OT_LAYOUT_GSUB_جدول_HH

#ضمن "OT/Layout/GSUB/GSUB.hh"

نطاق OT {
نطاق Layout {
نطاق GSUB_impl {

// TODO(garretrieger): Move into the new layout directory.
/* Out-of-class implementation for methods recursing */

#جيڪڏهن_وصف_نه_ٿيل HB_NO_OT_LAYOUT
/*static*/ مضمن منطقي ExtensionSubst::is_ابتو () ثابت
{
  أرجع SubstLookup::lookup_type_is_ابتو (get_type ());
}
سانچو <قسم_نالو context_t>
/*static*/ قسم_نالو context_t::return_t SubstLookup::dispatch_recurse_func (context_t *c, بنانشان صحيح lookup_اشاريو)
{
  ثابت SubstLookup &l = c->face->جدول.GSUB.get_relaxed ()->جدول->get_lookup (lookup_اشاريو);
  أرجع l.dispatch (c);
}

/*static*/ قسم_نالو hb_گهيرو_context_t::return_t SubstLookup::گهيرو_glyphs_recurse_func (hb_گهيرو_context_t *c, بنانشان lookup_اشاريو, hb_سيٽ_t *covered_seq_indices, بنانشان seq_اشاريو, بنانشان پڇاڙي_اشاريو)
{
  ثابت SubstLookup &l = c->face->جدول.GSUB.get_relaxed ()->جدول->get_lookup (lookup_اشاريو);
  إذا (l.may_have_non_1to1 ())
      hb_سيٽ_جوڙيو_range (covered_seq_indices, seq_اشاريو, پڇاڙي_اشاريو);
  أرجع l.dispatch (c);
}

/*static*/ hb_depend_context_t::return_t SubstLookup::depend_glyphs_recurse_func (hb_depend_context_t *c, بنانشان lookup_اشاريو, hb_ٻٽو_انگ_صفحو_t *covered_seq_indices, بنانشان seq_اشاريو, بنانشان پڇاڙي_اشاريو)
{
  ثابت SubstLookup &l = c->face->جدول.GSUB.get_relaxed ()->جدول->get_lookup (lookup_اشاريو);
  /* After a non-1-to-1 lookup (expansion/contraction), subsequent lookups in the same
   * contextual rule see all glyphs, not position-specific ones. This matches closure behavior.
   * Mark sequence positions as covered so later lookups use the full glyph set. */
  إذا (l.may_have_non_1to1 ())
      covered_seq_indices->جوڙيو_range (seq_اشاريو, پڇاڙي_اشاريو);

  hb_depend_context_t::recurse_ڪنجي_t key;
  إذا (!c->get_recurse_ڪنجي (lookup_اشاريو, &key))
    أرجع hb_خالي_جايون_t ();

  hb_depend_context_t::return_t ret = l.dispatch (c);
  c->پڄاڻي_recurse (key);
  أرجع ret;
}

سانچو <>
مضمن hb_گهيرو_lookups_context_t::return_t
SubstLookup::dispatch_recurse_func<hb_گهيرو_lookups_context_t> (hb_گهيرو_lookups_context_t *c, بنانشان this_اشاريو)
{
  ثابت SubstLookup &l = c->face->جدول.GSUB.get_relaxed ()->جدول->get_lookup (this_اشاريو);
  أرجع l.گهيرو_lookups (c, this_اشاريو);
}

سانچو <>
مضمن منطقي SubstLookup::dispatch_recurse_func<hb_ot_apply_context_t> (hb_ot_apply_context_t *c, بنانشان صحيح lookup_اشاريو)
{
  خودڪار *gsub = c->face->جدول.GSUB.get_relaxed ();
  ثابت SubstLookup &l = gsub->جدول->get_lookup (lookup_اشاريو);
  بنانشان صحيح saved_lookup_props = c->lookup_props;
  بنانشان صحيح saved_lookup_اشاريو = c->lookup_اشاريو;
  c->سيٽ_lookup_اشاريو (lookup_اشاريو);
  c->سيٽ_lookup_props (l.get_props ());

  uint32_t ڍير_ميلاپ_positions[8];
  hb_ويڪٽر_t<uint32_t> saved_ميلاپ_positions;
  saved_ميلاپ_positions.سيٽ_storage (ڍير_ميلاپ_positions);
  hb_مٽا_سٽا (c->ميلاپ_positions, saved_ميلاپ_positions);

  منطقي ret = خطأ;
  خودڪار *accel = gsub->get_accel (lookup_اشاريو);
  ret = accel && accel->apply (c, خطأ);

  c->سيٽ_lookup_اشاريو (saved_lookup_اشاريو);
  c->سيٽ_lookup_props (saved_lookup_props);

  hb_مٽا_سٽا (c->ميلاپ_positions, saved_ميلاپ_positions);

  أرجع ret;
}
#جيڪڏهن_پورو

} /* namespace GSUB_impl */
} /* namespace Layout */

مضمن فراغ
GSUB_accelerator_t::depend (hb_depend_معلومات_builder_t *builder, hb_face_t *face) ثابت
{
  إذا (!هذا->جدول->has_معلومات ()) أرجع;

  بنانشان انگ_features = هذا->جدول->get_feature_ڳڻپ ();
  بنانشان انگ_lookups  = هذا->جدول->get_lookup_ڳڻپ ();

  hb_ويڪٽر_t<hb_ٽيگ_t> feature_ٽيگ_فهرست;
  إذا (!builder->check_success (feature_ٽيگ_فهرست.resize (انگ_features)))
    أرجع;
  هذا->جدول->get_feature_ٽيگ_فهرست (0, &انگ_features, feature_ٽيگ_فهرست.arrayZ);

  إذا (!builder->init_lookup_features (انگ_lookups))
    أرجع;

  hb_ويڪٽر_t<hb_ٽيگ_t> feature_query_v;
  feature_query_v.resize (2);
  feature_query_v[1] = 0;

  hb_سيٽ_t ڏٺل_features;
  hb_سيٽ_t feature_indexes, lookup_indexes;

  لكل (خودڪار ft : feature_ٽيگ_فهرست)
  {
    إذا (ڏٺل_features.has (ft)) استمر;
    ڏٺل_features.جوڙيو (ft);
    feature_query_v[0] = ft;
    feature_indexes.reset ();
    hb_ot_layout_collect_features (face, HB_OT_ٽيگ_GSUB, خالي_اشارو, خالي_اشارو,
                                   feature_query_v.arrayZ, &feature_indexes);
    lookup_indexes.reset ();
    لكل (خودڪار feature_اشاريو : feature_indexes)
      هذا->جدول->get_feature (feature_اشاريو).جوڙيو_lookup_indexes_منزل (&lookup_indexes);

    لكل (خودڪار lookup_اشاريو : lookup_indexes)
      إذا (unlikely (!builder->جوڙيو_lookup_feature (lookup_اشاريو, ft)))
	أرجع;

    خودڪار &fv = هذا->جدول->get_feature_variations ();
    خودڪار fi_ڳڻپ = fv.رڪارڊ_ڳڻپ ();
    لكل (بنانشان i = 0; i < fi_ڳڻپ; i++)
    {
      lookup_indexes.reset ();
      لكل (خودڪار feature_اشاريو : feature_indexes)
      {
	خودڪار feature_ptr = fv.ڳوليو_substitute (i, feature_اشاريو);
	إذا (feature_ptr != خالي_اشارو)
	  feature_ptr->جوڙيو_lookup_indexes_منزل (&lookup_indexes);
      }
      لكل (خودڪار lookup_اشاريو : lookup_indexes)
	إذا (unlikely (!builder->جوڙيو_lookup_feature (lookup_اشاريو, ft)))
	  أرجع;
    }

#جيڪڏهن_وصف_نه_ٿيل HB_NO_VAR
    lookup_indexes.reset ();
    fv.collect_lookup_variation_lookups (&feature_indexes, &lookup_indexes);
    لكل (خودڪار lookup_اشاريو : lookup_indexes)
      إذا (unlikely (!builder->جوڙيو_lookup_feature (lookup_اشاريو, ft)))
	أرجع;
#جيڪڏهن_پورو
  }

  إذا (unlikely (!builder->پڄاڻي_lookup_features ()))
    أرجع;

  hb_سيٽ_t all_glyphs;
  all_glyphs.جوڙيو_range (0, face->get_انگ_glyphs () - 1);

  hb_depend_context_t c (builder, face, &all_glyphs);

  لكل (بنانشان i = 0; i < انگ_lookups; i++)
  {
    خودڪار features = builder->get_lookup_features (i);
    إذا (!features)
    {
      DEBUG_نياپو_LEVEL (DEPEND, خالي_اشارو, 1, 0,
                       "Skipping lookup %u (no features)", i);
      استمر;
    }
    DEBUG_نياپو_LEVEL (DEPEND, خالي_اشارو, 1, 0,
                     "Processing lookup %u with features:", i);
    c.lookup_اشاريو = i;
    c.reset_recurse_cache ();
    c.lookups_ڏٺل.clear ();
    c.lookups_ڏٺل.جوڙيو (i);  /* Seed for A→B→A cycle detection in recurse(). */
    هذا->جدول->get_lookup (i).depend (&c);
  }
}

} /* namespace OT */

#جيڪڏهن_پورو /* HB_OT_LAYOUT_GSUB_TABLE_HH */
