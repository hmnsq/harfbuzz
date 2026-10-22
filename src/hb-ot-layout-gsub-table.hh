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

#அந்தர்பாவ "OT/Layout/GSUB/GSUB.hh"

நாமாகாஶ OT {
நாமாகாஶ Layout {
நாமாகாஶ GSUB_impl {

// TODO(garretrieger): Move into the new layout directory.
/* Out-of-class implementation for methods recursing */

#ifndef HB_NO_OT_LAYOUT
/*static*/ உள்வரி தருக்க ExtensionSubst::is_reverse () மாறிலி
{
  திரும்பு SubstLookup::lookup_type_is_reverse (get_type ());
}
template <typename context_t>
/*static*/ typename context_t::return_t SubstLookup::dispatch_recurse_func (context_t *c, unsigned முழு lookup_index)
{
  மாறிலி SubstLookup &l = c->face->table.GSUB.get_relaxed ()->table->get_lookup (lookup_index);
  திரும்பு l.dispatch (c);
}

/*static*/ typename hb_closure_context_t::return_t SubstLookup::closure_glyphs_recurse_func (hb_closure_context_t *c, unsigned lookup_index, hb_set_t *covered_seq_indices, unsigned seq_index, unsigned end_index)
{
  மாறிலி SubstLookup &l = c->face->table.GSUB.get_relaxed ()->table->get_lookup (lookup_index);
  எனில் (l.may_have_non_1to1 ())
      hb_set_add_range (covered_seq_indices, seq_index, end_index);
  திரும்பு l.dispatch (c);
}

/*static*/ hb_depend_context_t::return_t SubstLookup::depend_glyphs_recurse_func (hb_depend_context_t *c, unsigned lookup_index, hb_bit_page_t *covered_seq_indices, unsigned seq_index, unsigned end_index)
{
  மாறிலி SubstLookup &l = c->face->table.GSUB.get_relaxed ()->table->get_lookup (lookup_index);
  /* After a non-1-to-1 lookup (expansion/contraction), subsequent lookups in the same
   * contextual rule see all glyphs, not position-specific ones. This matches closure behavior.
   * Mark sequence positions as covered so later lookups use the full glyph set. */
  எனில் (l.may_have_non_1to1 ())
      covered_seq_indices->add_range (seq_index, end_index);

  hb_depend_context_t::recurse_சாவி_t key;
  எனில் (!c->get_recurse_சாவி (lookup_index, &key))
    திரும்பு hb_empty_t ();

  hb_depend_context_t::return_t ret = l.dispatch (c);
  c->finish_recurse (key);
  திரும்பு ret;
}

template <>
உள்வரி hb_closure_lookups_context_t::return_t
SubstLookup::dispatch_recurse_func<hb_closure_lookups_context_t> (hb_closure_lookups_context_t *c, unsigned this_index)
{
  மாறிலி SubstLookup &l = c->face->table.GSUB.get_relaxed ()->table->get_lookup (this_index);
  திரும்பு l.closure_lookups (c, this_index);
}

template <>
உள்வரி தருக்க SubstLookup::dispatch_recurse_func<hb_ot_apply_context_t> (hb_ot_apply_context_t *c, unsigned முழு lookup_index)
{
  auto *gsub = c->face->table.GSUB.get_relaxed ();
  மாறிலி SubstLookup &l = gsub->table->get_lookup (lookup_index);
  unsigned முழு saved_lookup_props = c->lookup_props;
  unsigned முழு saved_lookup_index = c->lookup_index;
  c->set_lookup_index (lookup_index);
  c->set_lookup_props (l.get_props ());

  uint32_t அடுக்கு_match_positions[8];
  hb_vector_t<uint32_t> saved_match_positions;
  saved_match_positions.set_storage (அடுக்கு_match_positions);
  hb_swap (c->match_positions, saved_match_positions);

  தருக்க ret = பொய்;
  auto *accel = gsub->get_accel (lookup_index);
  ret = accel && accel->apply (c, பொய்);

  c->set_lookup_index (saved_lookup_index);
  c->set_lookup_props (saved_lookup_props);

  hb_swap (c->match_positions, saved_match_positions);

  திரும்பு ret;
}
#endif

} /* namespace GSUB_impl */
} /* namespace Layout */

உள்வரி வெறுமை
GSUB_accelerator_t::depend (hb_depend_data_builder_t *builder, hb_face_t *face) மாறிலி
{
  எனில் (!இது->table->has_data ()) திரும்பு;

  unsigned இலக்கம்_features = இது->table->get_feature_மொத்தம் ();
  unsigned இலக்கம்_lookups  = இது->table->get_lookup_மொத்தம் ();

  hb_vector_t<hb_tag_t> feature_tags;
  எனில் (!builder->check_success (feature_tags.resize (இலக்கம்_features)))
    திரும்பு;
  இது->table->get_feature_tags (0, &இலக்கம்_features, feature_tags.arrayZ);

  எனில் (!builder->init_lookup_features (இலக்கம்_lookups))
    திரும்பு;

  hb_vector_t<hb_tag_t> feature_query_v;
  feature_query_v.resize (2);
  feature_query_v[1] = 0;

  hb_set_t seen_features;
  hb_set_t feature_indexes, lookup_indexes;

  க்கு (auto ft : feature_tags)
  {
    எனில் (seen_features.has (ft)) தொடர்;
    seen_features.add (ft);
    feature_query_v[0] = ft;
    feature_indexes.reset ();
    hb_ot_layout_collect_features (face, HB_OT_TAG_GSUB, nullptr, nullptr,
                                   feature_query_v.arrayZ, &feature_indexes);
    lookup_indexes.reset ();
    க்கு (auto feature_index : feature_indexes)
      இது->table->get_feature (feature_index).add_lookup_indexes_to (&lookup_indexes);

    க்கு (auto lookup_index : lookup_indexes)
      எனில் (unlikely (!builder->add_lookup_feature (lookup_index, ft)))
	திரும்பு;

    auto &fv = இது->table->get_feature_variations ();
    auto fi_மொத்தம் = fv.record_மொத்தம் ();
    க்கு (unsigned i = 0; i < fi_மொத்தம்; i++)
    {
      lookup_indexes.reset ();
      க்கு (auto feature_index : feature_indexes)
      {
	auto feature_ptr = fv.find_substitute (i, feature_index);
	எனில் (feature_ptr != nullptr)
	  feature_ptr->add_lookup_indexes_to (&lookup_indexes);
      }
      க்கு (auto lookup_index : lookup_indexes)
	எனில் (unlikely (!builder->add_lookup_feature (lookup_index, ft)))
	  திரும்பு;
    }

#ifndef HB_NO_VAR
    lookup_indexes.reset ();
    fv.collect_lookup_variation_lookups (&feature_indexes, &lookup_indexes);
    க்கு (auto lookup_index : lookup_indexes)
      எனில் (unlikely (!builder->add_lookup_feature (lookup_index, ft)))
	திரும்பு;
#endif
  }

  எனில் (unlikely (!builder->finish_lookup_features ()))
    திரும்பு;

  hb_set_t all_glyphs;
  all_glyphs.add_range (0, face->get_இலக்கம்_glyphs () - 1);

  hb_depend_context_t c (builder, face, &all_glyphs);

  க்கு (unsigned i = 0; i < இலக்கம்_lookups; i++)
  {
    auto features = builder->get_lookup_features (i);
    எனில் (!features)
    {
      DEBUG_செய்தி_LEVEL (DEPEND, nullptr, 1, 0,
                       "Skipping lookup %u (no features)", i);
      தொடர்;
    }
    DEBUG_செய்தி_LEVEL (DEPEND, nullptr, 1, 0,
                     "Processing lookup %u with features:", i);
    c.lookup_index = i;
    c.reset_recurse_cache ();
    c.lookups_seen.clear ();
    c.lookups_seen.add (i);  /* Seed for A→B→A cycle detection in recurse(). */
    இது->table->get_lookup (i).depend (&c);
  }
}

} /* namespace OT */

#endif /* HB_OT_LAYOUT_GSUB_TABLE_HH */
