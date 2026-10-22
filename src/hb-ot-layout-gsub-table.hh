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

#ಅನ್ತರ್ಭಾವ "OT/Layout/GSUB/GSUB.hh"

ನಾಮಾಕಾಶ OT {
ನಾಮಾಕಾಶ Layout {
ನಾಮಾಕಾಶ GSUB_impl {

// TODO(garretrieger): Move into the new layout directory.
/* Out-of-class implementation for methods recursing */

#ifndef HB_NO_OT_LAYOUT
/*static*/ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ ExtensionSubst::is_reverse () ಧ್ರುವ
{
  ಪ್ರತ್ಯಾಗಮ SubstLookup::lookup_type_is_reverse (get_type ());
}
template <typename context_t>
/*static*/ typename context_t::return_t SubstLookup::dispatch_recurse_func (context_t *c, unsigned ಪೂರ್ಣಾಙ್ಕ lookup_index)
{
  ಧ್ರುವ SubstLookup &l = c->face->table.GSUB.get_relaxed ()->table->get_lookup (lookup_index);
  ಪ್ರತ್ಯಾಗಮ l.dispatch (c);
}

/*static*/ typename hb_closure_context_t::return_t SubstLookup::closure_glyphs_recurse_func (hb_closure_context_t *c, unsigned lookup_index, hb_set_t *covered_seq_indices, unsigned seq_index, unsigned end_index)
{
  ಧ್ರುವ SubstLookup &l = c->face->table.GSUB.get_relaxed ()->table->get_lookup (lookup_index);
  ಯದಿ (l.may_have_non_1to1 ())
      hb_set_add_range (covered_seq_indices, seq_index, end_index);
  ಪ್ರತ್ಯಾಗಮ l.dispatch (c);
}

/*static*/ hb_depend_context_t::return_t SubstLookup::depend_glyphs_recurse_func (hb_depend_context_t *c, unsigned lookup_index, hb_bit_page_t *covered_seq_indices, unsigned seq_index, unsigned end_index)
{
  ಧ್ರುವ SubstLookup &l = c->face->table.GSUB.get_relaxed ()->table->get_lookup (lookup_index);
  /* After a non-1-to-1 lookup (expansion/contraction), subsequent lookups in the same
   * contextual rule see all glyphs, not position-specific ones. This matches closure behavior.
   * Mark sequence positions as covered so later lookups use the full glyph set. */
  ಯದಿ (l.may_have_non_1to1 ())
      covered_seq_indices->add_range (seq_index, end_index);

  hb_depend_context_t::recurse_ಕೀಲಿ_t key;
  ಯದಿ (!c->get_recurse_ಕೀಲಿ (lookup_index, &key))
    ಪ್ರತ್ಯಾಗಮ hb_empty_t ();

  hb_depend_context_t::return_t ret = l.dispatch (c);
  c->finish_recurse (key);
  ಪ್ರತ್ಯಾಗಮ ret;
}

template <>
ಅನ್ತಃಸ್ಥ hb_closure_lookups_context_t::return_t
SubstLookup::dispatch_recurse_func<hb_closure_lookups_context_t> (hb_closure_lookups_context_t *c, unsigned this_index)
{
  ಧ್ರುವ SubstLookup &l = c->face->table.GSUB.get_relaxed ()->table->get_lookup (this_index);
  ಪ್ರತ್ಯಾಗಮ l.closure_lookups (c, this_index);
}

template <>
ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ SubstLookup::dispatch_recurse_func<hb_ot_apply_context_t> (hb_ot_apply_context_t *c, unsigned ಪೂರ್ಣಾಙ್ಕ lookup_index)
{
  auto *gsub = c->face->table.GSUB.get_relaxed ();
  ಧ್ರುವ SubstLookup &l = gsub->table->get_lookup (lookup_index);
  unsigned ಪೂರ್ಣಾಙ್ಕ saved_lookup_props = c->lookup_props;
  unsigned ಪೂರ್ಣಾಙ್ಕ saved_lookup_index = c->lookup_index;
  c->set_lookup_index (lookup_index);
  c->set_lookup_props (l.get_props ());

  uint32_t ರಾಶಿ_match_positions[8];
  hb_vector_t<uint32_t> saved_match_positions;
  saved_match_positions.set_storage (ರಾಶಿ_match_positions);
  hb_swap (c->match_positions, saved_match_positions);

  ತಾರ್ಕಿಕ ret = ಅಸತ್ಯ;
  auto *accel = gsub->get_accel (lookup_index);
  ret = accel && accel->apply (c, ಅಸತ್ಯ);

  c->set_lookup_index (saved_lookup_index);
  c->set_lookup_props (saved_lookup_props);

  hb_swap (c->match_positions, saved_match_positions);

  ಪ್ರತ್ಯಾಗಮ ret;
}
#endif

} /* namespace GSUB_impl */
} /* namespace Layout */

ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
GSUB_accelerator_t::depend (hb_depend_data_builder_t *builder, hb_face_t *face) ಧ್ರುವ
{
  ಯದಿ (!ಏತತ್->table->has_data ()) ಪ್ರತ್ಯಾಗಮ;

  unsigned ಅಂಕೆ_features = ಏತತ್->table->get_feature_ಎಣಿಕೆ ();
  unsigned ಅಂಕೆ_lookups  = ಏತತ್->table->get_lookup_ಎಣಿಕೆ ();

  hb_vector_t<hb_tag_t> feature_tags;
  ಯದಿ (!builder->check_success (feature_tags.resize (ಅಂಕೆ_features)))
    ಪ್ರತ್ಯಾಗಮ;
  ಏತತ್->table->get_feature_tags (0, &ಅಂಕೆ_features, feature_tags.arrayZ);

  ಯದಿ (!builder->init_lookup_features (ಅಂಕೆ_lookups))
    ಪ್ರತ್ಯಾಗಮ;

  hb_vector_t<hb_tag_t> feature_query_v;
  feature_query_v.resize (2);
  feature_query_v[1] = 0;

  hb_set_t seen_features;
  hb_set_t feature_indexes, lookup_indexes;

  ಪ್ರತಿ (auto ft : feature_tags)
  {
    ಯದಿ (seen_features.has (ft)) ಅನುವರ್ತ;
    seen_features.add (ft);
    feature_query_v[0] = ft;
    feature_indexes.reset ();
    hb_ot_layout_collect_features (face, HB_OT_TAG_GSUB, nullptr, nullptr,
                                   feature_query_v.arrayZ, &feature_indexes);
    lookup_indexes.reset ();
    ಪ್ರತಿ (auto feature_index : feature_indexes)
      ಏತತ್->table->get_feature (feature_index).add_lookup_indexes_to (&lookup_indexes);

    ಪ್ರತಿ (auto lookup_index : lookup_indexes)
      ಯದಿ (unlikely (!builder->add_lookup_feature (lookup_index, ft)))
	ಪ್ರತ್ಯಾಗಮ;

    auto &fv = ಏತತ್->table->get_feature_variations ();
    auto fi_ಎಣಿಕೆ = fv.record_ಎಣಿಕೆ ();
    ಪ್ರತಿ (unsigned i = 0; i < fi_ಎಣಿಕೆ; i++)
    {
      lookup_indexes.reset ();
      ಪ್ರತಿ (auto feature_index : feature_indexes)
      {
	auto feature_ptr = fv.find_substitute (i, feature_index);
	ಯದಿ (feature_ptr != nullptr)
	  feature_ptr->add_lookup_indexes_to (&lookup_indexes);
      }
      ಪ್ರತಿ (auto lookup_index : lookup_indexes)
	ಯದಿ (unlikely (!builder->add_lookup_feature (lookup_index, ft)))
	  ಪ್ರತ್ಯಾಗಮ;
    }

#ifndef HB_NO_VAR
    lookup_indexes.reset ();
    fv.collect_lookup_variation_lookups (&feature_indexes, &lookup_indexes);
    ಪ್ರತಿ (auto lookup_index : lookup_indexes)
      ಯದಿ (unlikely (!builder->add_lookup_feature (lookup_index, ft)))
	ಪ್ರತ್ಯಾಗಮ;
#endif
  }

  ಯದಿ (unlikely (!builder->finish_lookup_features ()))
    ಪ್ರತ್ಯಾಗಮ;

  hb_set_t all_glyphs;
  all_glyphs.add_range (0, face->get_ಅಂಕೆ_glyphs () - 1);

  hb_depend_context_t c (builder, face, &all_glyphs);

  ಪ್ರತಿ (unsigned i = 0; i < ಅಂಕೆ_lookups; i++)
  {
    auto features = builder->get_lookup_features (i);
    ಯದಿ (!features)
    {
      DEBUG_ಸಂದೇಶ_LEVEL (DEPEND, nullptr, 1, 0,
                       "Skipping lookup %u (no features)", i);
      ಅನುವರ್ತ;
    }
    DEBUG_ಸಂದೇಶ_LEVEL (DEPEND, nullptr, 1, 0,
                     "Processing lookup %u with features:", i);
    c.lookup_index = i;
    c.reset_recurse_cache ();
    c.lookups_seen.clear ();
    c.lookups_seen.add (i);  /* Seed for A→B→A cycle detection in recurse(). */
    ಏತತ್->table->get_lookup (i).depend (&c);
  }
}

} /* namespace OT */

#endif /* HB_OT_LAYOUT_GSUB_TABLE_HH */
