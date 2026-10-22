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

#അന്തര്ഭാവ "OT/Layout/GSUB/GSUB.hh"

നാമാകാശ OT {
നാമാകാശ Layout {
നാമാകാശ GSUB_impl {

// TODO(garretrieger): Move into the new layout directory.
/* Out-of-class implementation for methods recursing */

#ifndef HB_NO_OT_LAYOUT
/*static*/ വരിയിൽ തർക്കം ExtensionSubst::is_reverse () സ്ഥിരം
{
  തിരികെ SubstLookup::lookup_type_is_reverse (get_type ());
}
template <typename context_t>
/*static*/ typename context_t::return_t SubstLookup::dispatch_recurse_func (context_t *c, unsigned പൂർണ്ണസംഖ്യ lookup_index)
{
  സ്ഥിരം SubstLookup &l = c->face->table.GSUB.get_relaxed ()->table->get_lookup (lookup_index);
  തിരികെ l.dispatch (c);
}

/*static*/ typename hb_closure_context_t::return_t SubstLookup::closure_glyphs_recurse_func (hb_closure_context_t *c, unsigned lookup_index, hb_set_t *covered_seq_indices, unsigned seq_index, unsigned end_index)
{
  സ്ഥിരം SubstLookup &l = c->face->table.GSUB.get_relaxed ()->table->get_lookup (lookup_index);
  എങ്കിൽ (l.may_have_non_1to1 ())
      hb_set_add_range (covered_seq_indices, seq_index, end_index);
  തിരികെ l.dispatch (c);
}

/*static*/ hb_depend_context_t::return_t SubstLookup::depend_glyphs_recurse_func (hb_depend_context_t *c, unsigned lookup_index, hb_bit_page_t *covered_seq_indices, unsigned seq_index, unsigned end_index)
{
  സ്ഥിരം SubstLookup &l = c->face->table.GSUB.get_relaxed ()->table->get_lookup (lookup_index);
  /* After a non-1-to-1 lookup (expansion/contraction), subsequent lookups in the same
   * contextual rule see all glyphs, not position-specific ones. This matches closure behavior.
   * Mark sequence positions as covered so later lookups use the full glyph set. */
  എങ്കിൽ (l.may_have_non_1to1 ())
      covered_seq_indices->add_range (seq_index, end_index);

  hb_depend_context_t::recurse_താക്കോൽ_t key;
  എങ്കിൽ (!c->get_recurse_താക്കോൽ (lookup_index, &key))
    തിരികെ hb_empty_t ();

  hb_depend_context_t::return_t ret = l.dispatch (c);
  c->finish_recurse (key);
  തിരികെ ret;
}

template <>
വരിയിൽ hb_closure_lookups_context_t::return_t
SubstLookup::dispatch_recurse_func<hb_closure_lookups_context_t> (hb_closure_lookups_context_t *c, unsigned this_index)
{
  സ്ഥിരം SubstLookup &l = c->face->table.GSUB.get_relaxed ()->table->get_lookup (this_index);
  തിരികെ l.closure_lookups (c, this_index);
}

template <>
വരിയിൽ തർക്കം SubstLookup::dispatch_recurse_func<hb_ot_apply_context_t> (hb_ot_apply_context_t *c, unsigned പൂർണ്ണസംഖ്യ lookup_index)
{
  auto *gsub = c->face->table.GSUB.get_relaxed ();
  സ്ഥിരം SubstLookup &l = gsub->table->get_lookup (lookup_index);
  unsigned പൂർണ്ണസംഖ്യ saved_lookup_props = c->lookup_props;
  unsigned പൂർണ്ണസംഖ്യ saved_lookup_index = c->lookup_index;
  c->set_lookup_index (lookup_index);
  c->set_lookup_props (l.get_props ());

  uint32_t അട്ടി_match_positions[8];
  hb_vector_t<uint32_t> saved_match_positions;
  saved_match_positions.set_storage (അട്ടി_match_positions);
  hb_swap (c->match_positions, saved_match_positions);

  തർക്കം ret = അസത്യം;
  auto *accel = gsub->get_accel (lookup_index);
  ret = accel && accel->apply (c, അസത്യം);

  c->set_lookup_index (saved_lookup_index);
  c->set_lookup_props (saved_lookup_props);

  hb_swap (c->match_positions, saved_match_positions);

  തിരികെ ret;
}
#endif

} /* namespace GSUB_impl */
} /* namespace Layout */

വരിയിൽ ശൂന്യം
GSUB_accelerator_t::depend (hb_depend_data_builder_t *builder, hb_face_t *face) സ്ഥിരം
{
  എങ്കിൽ (!ഇത്->table->has_data ()) തിരികെ;

  unsigned അക്കം_features = ഇത്->table->get_feature_എണ്ണം ();
  unsigned അക്കം_lookups  = ഇത്->table->get_lookup_എണ്ണം ();

  hb_vector_t<hb_tag_t> feature_tags;
  എങ്കിൽ (!builder->check_success (feature_tags.resize (അക്കം_features)))
    തിരികെ;
  ഇത്->table->get_feature_tags (0, &അക്കം_features, feature_tags.arrayZ);

  എങ്കിൽ (!builder->init_lookup_features (അക്കം_lookups))
    തിരികെ;

  hb_vector_t<hb_tag_t> feature_query_v;
  feature_query_v.resize (2);
  feature_query_v[1] = 0;

  hb_set_t seen_features;
  hb_set_t feature_indexes, lookup_indexes;

  വേണ്ടി (auto ft : feature_tags)
  {
    എങ്കിൽ (seen_features.has (ft)) തുടരുക;
    seen_features.add (ft);
    feature_query_v[0] = ft;
    feature_indexes.reset ();
    hb_ot_layout_collect_features (face, HB_OT_TAG_GSUB, nullptr, nullptr,
                                   feature_query_v.arrayZ, &feature_indexes);
    lookup_indexes.reset ();
    വേണ്ടി (auto feature_index : feature_indexes)
      ഇത്->table->get_feature (feature_index).add_lookup_indexes_to (&lookup_indexes);

    വേണ്ടി (auto lookup_index : lookup_indexes)
      എങ്കിൽ (unlikely (!builder->add_lookup_feature (lookup_index, ft)))
	തിരികെ;

    auto &fv = ഇത്->table->get_feature_variations ();
    auto fi_എണ്ണം = fv.record_എണ്ണം ();
    വേണ്ടി (unsigned i = 0; i < fi_എണ്ണം; i++)
    {
      lookup_indexes.reset ();
      വേണ്ടി (auto feature_index : feature_indexes)
      {
	auto feature_ptr = fv.find_substitute (i, feature_index);
	എങ്കിൽ (feature_ptr != nullptr)
	  feature_ptr->add_lookup_indexes_to (&lookup_indexes);
      }
      വേണ്ടി (auto lookup_index : lookup_indexes)
	എങ്കിൽ (unlikely (!builder->add_lookup_feature (lookup_index, ft)))
	  തിരികെ;
    }

#ifndef HB_NO_VAR
    lookup_indexes.reset ();
    fv.collect_lookup_variation_lookups (&feature_indexes, &lookup_indexes);
    വേണ്ടി (auto lookup_index : lookup_indexes)
      എങ്കിൽ (unlikely (!builder->add_lookup_feature (lookup_index, ft)))
	തിരികെ;
#endif
  }

  എങ്കിൽ (unlikely (!builder->finish_lookup_features ()))
    തിരികെ;

  hb_set_t all_glyphs;
  all_glyphs.add_range (0, face->get_അക്കം_glyphs () - 1);

  hb_depend_context_t c (builder, face, &all_glyphs);

  വേണ്ടി (unsigned i = 0; i < അക്കം_lookups; i++)
  {
    auto features = builder->get_lookup_features (i);
    എങ്കിൽ (!features)
    {
      DEBUG_സന്ദേശം_LEVEL (DEPEND, nullptr, 1, 0,
                       "Skipping lookup %u (no features)", i);
      തുടരുക;
    }
    DEBUG_സന്ദേശം_LEVEL (DEPEND, nullptr, 1, 0,
                     "Processing lookup %u with features:", i);
    c.lookup_index = i;
    c.reset_recurse_cache ();
    c.lookups_seen.clear ();
    c.lookups_seen.add (i);  /* Seed for A→B→A cycle detection in recurse(). */
    ഇത്->table->get_lookup (i).depend (&c);
  }
}

} /* namespace OT */

#endif /* HB_OT_LAYOUT_GSUB_TABLE_HH */
