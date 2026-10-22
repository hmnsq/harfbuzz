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

#ifndef HB_OT_LAYOUT_GSUB_TABLE_HH
#define HB_OT_LAYOUT_GSUB_TABLE_HH

#অন্তর্ভাব "OT/Layout/GSUB/GSUB.hh"

নামাকাশ OT {
নামাকাশ Layout {
নামাকাশ GSUB_impl {

// TODO(garretrieger): Move into the new layout directory.
/* Out-of-class implementation for methods recursing */

#ifndef HB_NO_OT_LAYOUT
/*static*/ অন্তঃরেখ যৌক্তিক ExtensionSubst::is_reverse () ধ্রুবক
{
  ফেরত SubstLookup::lookup_type_is_reverse (get_type ());
}
template <typename context_t>
/*static*/ typename context_t::return_t SubstLookup::dispatch_recurse_func (context_t *c, unsigned পূর্ণসংখ্যা lookup_সূচক)
{
  ধ্রুবক SubstLookup &l = c->face->table.GSUB.get_relaxed ()->table->get_lookup (lookup_সূচক);
  ফেরত l.dispatch (c);
}

/*static*/ typename hb_closure_context_t::return_t SubstLookup::closure_glyphs_recurse_func (hb_closure_context_t *c, unsigned lookup_সূচক, hb_set_t *covered_seq_indices, unsigned seq_সূচক, unsigned end_সূচক)
{
  ধ্রুবক SubstLookup &l = c->face->table.GSUB.get_relaxed ()->table->get_lookup (lookup_সূচক);
  যদি (l.may_have_non_1to1 ())
      hb_set_add_range (covered_seq_indices, seq_সূচক, end_সূচক);
  ফেরত l.dispatch (c);
}

/*static*/ hb_depend_context_t::return_t SubstLookup::depend_glyphs_recurse_func (hb_depend_context_t *c, unsigned lookup_সূচক, hb_bit_page_t *covered_seq_indices, unsigned seq_সূচক, unsigned end_সূচক)
{
  ধ্রুবক SubstLookup &l = c->face->table.GSUB.get_relaxed ()->table->get_lookup (lookup_সূচক);
  /* After a non-1-to-1 lookup (expansion/contraction), subsequent lookups in the same
   * contextual rule see all glyphs, not position-specific ones. This matches closure behavior.
   * Mark sequence positions as covered so later lookups use the full glyph set. */
  যদি (l.may_have_non_1to1 ())
      covered_seq_indices->add_range (seq_সূচক, end_সূচক);

  hb_depend_context_t::recurse_চাবি_t key;
  যদি (!c->get_recurse_চাবি (lookup_সূচক, &key))
    ফেরত hb_empty_t ();

  hb_depend_context_t::return_t ret = l.dispatch (c);
  c->finish_recurse (key);
  ফেরত ret;
}

template <>
অন্তঃরেখ hb_closure_lookups_context_t::return_t
SubstLookup::dispatch_recurse_func<hb_closure_lookups_context_t> (hb_closure_lookups_context_t *c, unsigned this_সূচক)
{
  ধ্রুবক SubstLookup &l = c->face->table.GSUB.get_relaxed ()->table->get_lookup (this_সূচক);
  ফেরত l.closure_lookups (c, this_সূচক);
}

template <>
অন্তঃরেখ যৌক্তিক SubstLookup::dispatch_recurse_func<hb_ot_apply_context_t> (hb_ot_apply_context_t *c, unsigned পূর্ণসংখ্যা lookup_সূচক)
{
  auto *gsub = c->face->table.GSUB.get_relaxed ();
  ধ্রুবক SubstLookup &l = gsub->table->get_lookup (lookup_সূচক);
  unsigned পূর্ণসংখ্যা saved_lookup_props = c->lookup_props;
  unsigned পূর্ণসংখ্যা saved_lookup_সূচক = c->lookup_সূচক;
  c->set_lookup_সূচক (lookup_সূচক);
  c->set_lookup_props (l.get_props ());

  uint32_t স্তূপ_match_positions[8];
  hb_vector_t<uint32_t> saved_match_positions;
  saved_match_positions.set_storage (স্তূপ_match_positions);
  hb_swap (c->match_positions, saved_match_positions);

  যৌক্তিক ret = মিথ্যা;
  auto *accel = gsub->get_accel (lookup_সূচক);
  ret = accel && accel->apply (c, মিথ্যা);

  c->set_lookup_সূচক (saved_lookup_সূচক);
  c->set_lookup_props (saved_lookup_props);

  hb_swap (c->match_positions, saved_match_positions);

  ফেরত ret;
}
#endif

} /* namespace GSUB_impl */
} /* namespace Layout */

অন্তঃরেখ শূন্য
GSUB_accelerator_t::depend (hb_depend_তথ্য_builder_t *builder, hb_face_t *face) ধ্রুবক
{
  যদি (!এই->table->has_তথ্য ()) ফেরত;

  unsigned অঙ্ক_features = এই->table->get_feature_গুনতি ();
  unsigned অঙ্ক_lookups  = এই->table->get_lookup_গুনতি ();

  hb_vector_t<hb_tag_t> feature_tags;
  যদি (!builder->check_success (feature_tags.resize (অঙ্ক_features)))
    ফেরত;
  এই->table->get_feature_tags (0, &অঙ্ক_features, feature_tags.arrayZ);

  যদি (!builder->init_lookup_features (অঙ্ক_lookups))
    ফেরত;

  hb_vector_t<hb_tag_t> feature_query_v;
  feature_query_v.resize (2);
  feature_query_v[1] = 0;

  hb_set_t seen_features;
  hb_set_t feature_indexes, lookup_indexes;

  জন্য (auto ft : feature_tags)
  {
    যদি (seen_features.has (ft)) চালু;
    seen_features.add (ft);
    feature_query_v[0] = ft;
    feature_indexes.reset ();
    hb_ot_layout_collect_features (face, HB_OT_TAG_GSUB, nullptr, nullptr,
                                   feature_query_v.arrayZ, &feature_indexes);
    lookup_indexes.reset ();
    জন্য (auto feature_সূচক : feature_indexes)
      এই->table->get_feature (feature_সূচক).add_lookup_indexes_to (&lookup_indexes);

    জন্য (auto lookup_সূচক : lookup_indexes)
      যদি (unlikely (!builder->add_lookup_feature (lookup_সূচক, ft)))
	ফেরত;

    auto &fv = এই->table->get_feature_variations ();
    auto fi_গুনতি = fv.record_গুনতি ();
    জন্য (unsigned i = 0; i < fi_গুনতি; i++)
    {
      lookup_indexes.reset ();
      জন্য (auto feature_সূচক : feature_indexes)
      {
	auto feature_ptr = fv.find_substitute (i, feature_সূচক);
	যদি (feature_ptr != nullptr)
	  feature_ptr->add_lookup_indexes_to (&lookup_indexes);
      }
      জন্য (auto lookup_সূচক : lookup_indexes)
	যদি (unlikely (!builder->add_lookup_feature (lookup_সূচক, ft)))
	  ফেরত;
    }

#ifndef HB_NO_VAR
    lookup_indexes.reset ();
    fv.collect_lookup_variation_lookups (&feature_indexes, &lookup_indexes);
    জন্য (auto lookup_সূচক : lookup_indexes)
      যদি (unlikely (!builder->add_lookup_feature (lookup_সূচক, ft)))
	ফেরত;
#endif
  }

  যদি (unlikely (!builder->finish_lookup_features ()))
    ফেরত;

  hb_set_t all_glyphs;
  all_glyphs.add_range (0, face->get_অঙ্ক_glyphs () - 1);

  hb_depend_context_t c (builder, face, &all_glyphs);

  জন্য (unsigned i = 0; i < অঙ্ক_lookups; i++)
  {
    auto features = builder->get_lookup_features (i);
    যদি (!features)
    {
      DEBUG_বার্তা_LEVEL (DEPEND, nullptr, 1, 0,
                       "Skipping lookup %u (no features)", i);
      চালু;
    }
    DEBUG_বার্তা_LEVEL (DEPEND, nullptr, 1, 0,
                     "Processing lookup %u with features:", i);
    c.lookup_সূচক = i;
    c.reset_recurse_cache ();
    c.lookups_seen.clear ();
    c.lookups_seen.add (i);  /* Seed for A→B→A cycle detection in recurse(). */
    এই->table->get_lookup (i).depend (&c);
  }
}

} /* namespace OT */

#endif /* HB_OT_LAYOUT_GSUB_TABLE_HH */
