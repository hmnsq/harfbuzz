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

#ଅନ୍ତର୍ଭାଵ "OT/Layout/GSUB/GSUB.hh"

ନାମାକାଶ OT {
ନାମାକାଶ Layout {
ନାମାକାଶ GSUB_impl {

// TODO(garretrieger): Move into the new layout directory.
/* Out-of-class implementation for methods recursing */

#ifndef HB_NO_OT_LAYOUT
/*static*/ ଧାଡ଼ିଭିତରେ ତାର୍କିକ ExtensionSubst::is_reverse () ଅଚଳ
{
  ଫେରନ୍ତୁ SubstLookup::lookup_type_is_reverse (get_type ());
}
template <typename context_t>
/*static*/ typename context_t::return_t SubstLookup::dispatch_recurse_func (context_t *c, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା lookup_index)
{
  ଅଚଳ SubstLookup &l = c->face->table.GSUB.get_relaxed ()->table->get_lookup (lookup_index);
  ଫେରନ୍ତୁ l.dispatch (c);
}

/*static*/ typename hb_closure_context_t::return_t SubstLookup::closure_glyphs_recurse_func (hb_closure_context_t *c, unsigned lookup_index, hb_set_t *covered_seq_indices, unsigned seq_index, unsigned end_index)
{
  ଅଚଳ SubstLookup &l = c->face->table.GSUB.get_relaxed ()->table->get_lookup (lookup_index);
  ଯଦି (l.may_have_non_1to1 ())
      hb_set_add_range (covered_seq_indices, seq_index, end_index);
  ଫେରନ୍ତୁ l.dispatch (c);
}

/*static*/ hb_depend_context_t::return_t SubstLookup::depend_glyphs_recurse_func (hb_depend_context_t *c, unsigned lookup_index, hb_bit_page_t *covered_seq_indices, unsigned seq_index, unsigned end_index)
{
  ଅଚଳ SubstLookup &l = c->face->table.GSUB.get_relaxed ()->table->get_lookup (lookup_index);
  /* After a non-1-to-1 lookup (expansion/contraction), subsequent lookups in the same
   * contextual rule see all glyphs, not position-specific ones. This matches closure behavior.
   * Mark sequence positions as covered so later lookups use the full glyph set. */
  ଯଦି (l.may_have_non_1to1 ())
      covered_seq_indices->add_range (seq_index, end_index);

  hb_depend_context_t::recurse_ଚାବି_t key;
  ଯଦି (!c->get_recurse_ଚାବି (lookup_index, &key))
    ଫେରନ୍ତୁ hb_empty_t ();

  hb_depend_context_t::return_t ret = l.dispatch (c);
  c->finish_recurse (key);
  ଫେରନ୍ତୁ ret;
}

template <>
ଧାଡ଼ିଭିତରେ hb_closure_lookups_context_t::return_t
SubstLookup::dispatch_recurse_func<hb_closure_lookups_context_t> (hb_closure_lookups_context_t *c, unsigned this_index)
{
  ଅଚଳ SubstLookup &l = c->face->table.GSUB.get_relaxed ()->table->get_lookup (this_index);
  ଫେରନ୍ତୁ l.closure_lookups (c, this_index);
}

template <>
ଧାଡ଼ିଭିତରେ ତାର୍କିକ SubstLookup::dispatch_recurse_func<hb_ot_apply_context_t> (hb_ot_apply_context_t *c, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା lookup_index)
{
  auto *gsub = c->face->table.GSUB.get_relaxed ();
  ଅଚଳ SubstLookup &l = gsub->table->get_lookup (lookup_index);
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା saved_lookup_props = c->lookup_props;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା saved_lookup_index = c->lookup_index;
  c->set_lookup_index (lookup_index);
  c->set_lookup_props (l.get_props ());

  uint32_t ଗଦା_match_positions[8];
  hb_vector_t<uint32_t> saved_match_positions;
  saved_match_positions.set_storage (ଗଦା_match_positions);
  hb_swap (c->match_positions, saved_match_positions);

  ତାର୍କିକ ret = ମିଥ୍ୟା;
  auto *accel = gsub->get_accel (lookup_index);
  ret = accel && accel->apply (c, ମିଥ୍ୟା);

  c->set_lookup_index (saved_lookup_index);
  c->set_lookup_props (saved_lookup_props);

  hb_swap (c->match_positions, saved_match_positions);

  ଫେରନ୍ତୁ ret;
}
#endif

} /* namespace GSUB_impl */
} /* namespace Layout */

ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
GSUB_accelerator_t::depend (hb_depend_data_builder_t *builder, hb_face_t *face) ଅଚଳ
{
  ଯଦି (!ଏହି->table->has_data ()) ଫେରନ୍ତୁ;

  unsigned ଅଙ୍କ_features = ଏହି->table->get_feature_ଗଣନା ();
  unsigned ଅଙ୍କ_lookups  = ଏହି->table->get_lookup_ଗଣନା ();

  hb_vector_t<hb_tag_t> feature_tags;
  ଯଦି (!builder->check_success (feature_tags.resize (ଅଙ୍କ_features)))
    ଫେରନ୍ତୁ;
  ଏହି->table->get_feature_tags (0, &ଅଙ୍କ_features, feature_tags.arrayZ);

  ଯଦି (!builder->init_lookup_features (ଅଙ୍କ_lookups))
    ଫେରନ୍ତୁ;

  hb_vector_t<hb_tag_t> feature_query_v;
  feature_query_v.resize (2);
  feature_query_v[1] = 0;

  hb_set_t seen_features;
  hb_set_t feature_indexes, lookup_indexes;

  ପାଇଁ (auto ft : feature_tags)
  {
    ଯଦି (seen_features.has (ft)) ଜାରି;
    seen_features.add (ft);
    feature_query_v[0] = ft;
    feature_indexes.reset ();
    hb_ot_layout_collect_features (face, HB_OT_TAG_GSUB, nullptr, nullptr,
                                   feature_query_v.arrayZ, &feature_indexes);
    lookup_indexes.reset ();
    ପାଇଁ (auto feature_index : feature_indexes)
      ଏହି->table->get_feature (feature_index).add_lookup_indexes_to (&lookup_indexes);

    ପାଇଁ (auto lookup_index : lookup_indexes)
      ଯଦି (unlikely (!builder->add_lookup_feature (lookup_index, ft)))
	ଫେରନ୍ତୁ;

    auto &fv = ଏହି->table->get_feature_variations ();
    auto fi_ଗଣନା = fv.record_ଗଣନା ();
    ପାଇଁ (unsigned i = 0; i < fi_ଗଣନା; i++)
    {
      lookup_indexes.reset ();
      ପାଇଁ (auto feature_index : feature_indexes)
      {
	auto feature_ptr = fv.find_substitute (i, feature_index);
	ଯଦି (feature_ptr != nullptr)
	  feature_ptr->add_lookup_indexes_to (&lookup_indexes);
      }
      ପାଇଁ (auto lookup_index : lookup_indexes)
	ଯଦି (unlikely (!builder->add_lookup_feature (lookup_index, ft)))
	  ଫେରନ୍ତୁ;
    }

#ifndef HB_NO_VAR
    lookup_indexes.reset ();
    fv.collect_lookup_variation_lookups (&feature_indexes, &lookup_indexes);
    ପାଇଁ (auto lookup_index : lookup_indexes)
      ଯଦି (unlikely (!builder->add_lookup_feature (lookup_index, ft)))
	ଫେରନ୍ତୁ;
#endif
  }

  ଯଦି (unlikely (!builder->finish_lookup_features ()))
    ଫେରନ୍ତୁ;

  hb_set_t all_glyphs;
  all_glyphs.add_range (0, face->get_ଅଙ୍କ_glyphs () - 1);

  hb_depend_context_t c (builder, face, &all_glyphs);

  ପାଇଁ (unsigned i = 0; i < ଅଙ୍କ_lookups; i++)
  {
    auto features = builder->get_lookup_features (i);
    ଯଦି (!features)
    {
      DEBUG_ସନ୍ଦେଶ_LEVEL (DEPEND, nullptr, 1, 0,
                       "Skipping lookup %u (no features)", i);
      ଜାରି;
    }
    DEBUG_ସନ୍ଦେଶ_LEVEL (DEPEND, nullptr, 1, 0,
                     "Processing lookup %u with features:", i);
    c.lookup_index = i;
    c.reset_recurse_cache ();
    c.lookups_seen.clear ();
    c.lookups_seen.add (i);  /* Seed for A→B→A cycle detection in recurse(). */
    ଏହି->table->get_lookup (i).depend (&c);
  }
}

} /* namespace OT */

#endif /* HB_OT_LAYOUT_GSUB_TABLE_HH */
