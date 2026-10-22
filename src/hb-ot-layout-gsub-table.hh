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

#अगर_अघोषित HB_OT_LAYOUT_GSUB_TABLE_HH
#घोषणा HB_OT_LAYOUT_GSUB_TABLE_HH

#समावेश "OT/Layout/GSUB/GSUB.hh"

namespace OT {
namespace Layout {
namespace GSUB_impl {

// TODO(garretrieger): Move into the new layout directory.
/* Out-of-class implementation for methods recursing */

#अगर_अघोषित HB_NO_OT_LAYOUT
/*static*/ अंतरभूत bool ExtensionSubst::is_reverse () स्थिर
{
  वापस SubstLookup::lookup_type_is_reverse (get_type ());
}
ढाँचा <typename context_t>
/*static*/ typename context_t::return_t SubstLookup::dispatch_recurse_func (context_t *c, अचिन्हित पूर्णांक lookup_सूचकांक)
{
  स्थिर SubstLookup &l = c->face->table.GSUB.get_relaxed ()->table->get_lookup (lookup_सूचकांक);
  वापस l.dispatch (c);
}

/*static*/ typename hb_closure_context_t::return_t SubstLookup::closure_glyphs_recurse_func (hb_closure_context_t *c, अचिन्हित lookup_सूचकांक, hb_set_t *covered_seq_indices, अचिन्हित seq_सूचकांक, अचिन्हित end_सूचकांक)
{
  स्थिर SubstLookup &l = c->face->table.GSUB.get_relaxed ()->table->get_lookup (lookup_सूचकांक);
  अगर (l.may_have_non_1to1 ())
      hb_set_add_range (covered_seq_indices, seq_सूचकांक, end_सूचकांक);
  वापस l.dispatch (c);
}

/*static*/ hb_depend_context_t::return_t SubstLookup::depend_glyphs_recurse_func (hb_depend_context_t *c, अचिन्हित lookup_सूचकांक, hb_bit_page_t *covered_seq_indices, अचिन्हित seq_सूचकांक, अचिन्हित end_सूचकांक)
{
  स्थिर SubstLookup &l = c->face->table.GSUB.get_relaxed ()->table->get_lookup (lookup_सूचकांक);
  /* After a non-1-to-1 lookup (expansion/contraction), subsequent lookups in the same
   * contextual rule see all glyphs, not position-specific ones. This matches closure behavior.
   * Mark sequence positions as covered so later lookups use the full glyph set. */
  अगर (l.may_have_non_1to1 ())
      covered_seq_indices->add_range (seq_सूचकांक, end_सूचकांक);

  hb_depend_context_t::recurse_कुंजी_t key;
  अगर (!c->get_recurse_कुंजी (lookup_सूचकांक, &key))
    वापस hb_empty_t ();

  hb_depend_context_t::return_t ret = l.dispatch (c);
  c->finish_recurse (key);
  वापस ret;
}

ढाँचा <>
अंतरभूत hb_closure_lookups_context_t::return_t
SubstLookup::dispatch_recurse_func<hb_closure_lookups_context_t> (hb_closure_lookups_context_t *c, अचिन्हित this_सूचकांक)
{
  स्थिर SubstLookup &l = c->face->table.GSUB.get_relaxed ()->table->get_lookup (this_सूचकांक);
  वापस l.closure_lookups (c, this_सूचकांक);
}

ढाँचा <>
अंतरभूत bool SubstLookup::dispatch_recurse_func<hb_ot_apply_context_t> (hb_ot_apply_context_t *c, अचिन्हित पूर्णांक lookup_सूचकांक)
{
  स्वतः *gsub = c->face->table.GSUB.get_relaxed ();
  स्थिर SubstLookup &l = gsub->table->get_lookup (lookup_सूचकांक);
  अचिन्हित पूर्णांक saved_lookup_props = c->lookup_props;
  अचिन्हित पूर्णांक saved_lookup_सूचकांक = c->lookup_सूचकांक;
  c->set_lookup_सूचकांक (lookup_सूचकांक);
  c->set_lookup_props (l.get_props ());

  uint32_t ढेर_match_positions[8];
  hb_vector_t<uint32_t> saved_match_positions;
  saved_match_positions.set_storage (ढेर_match_positions);
  hb_swap (c->match_positions, saved_match_positions);

  bool ret = false;
  स्वतः *accel = gsub->get_accel (lookup_सूचकांक);
  ret = accel && accel->apply (c, false);

  c->set_lookup_सूचकांक (saved_lookup_सूचकांक);
  c->set_lookup_props (saved_lookup_props);

  hb_swap (c->match_positions, saved_match_positions);

  वापस ret;
}
#पूर्ण_अगर

} /* namespace GSUB_impl */
} /* namespace Layout */

अंतरभूत व्योम
GSUB_accelerator_t::depend (hb_depend_तथ्य_builder_t *builder, hb_face_t *face) स्थिर
{
  अगर (!यह->table->has_तथ्य ()) वापस;

  अचिन्हित अंक_features = यह->table->get_feature_गिनती ();
  अचिन्हित अंक_lookups  = यह->table->get_lookup_गिनती ();

  hb_vector_t<hb_tag_t> feature_tags;
  अगर (!builder->check_success (feature_tags.resize (अंक_features)))
    वापस;
  यह->table->get_feature_tags (0, &अंक_features, feature_tags.arrayZ);

  अगर (!builder->init_lookup_features (अंक_lookups))
    वापस;

  hb_vector_t<hb_tag_t> feature_query_v;
  feature_query_v.resize (2);
  feature_query_v[1] = 0;

  hb_set_t seen_features;
  hb_set_t feature_indexes, lookup_indexes;

  क्रम (स्वतः ft : feature_tags)
  {
    अगर (seen_features.has (ft)) जारी;
    seen_features.add (ft);
    feature_query_v[0] = ft;
    feature_indexes.reset ();
    hb_ot_layout_collect_features (face, HB_OT_TAG_GSUB, nullptr, nullptr,
                                   feature_query_v.arrayZ, &feature_indexes);
    lookup_indexes.reset ();
    क्रम (स्वतः feature_सूचकांक : feature_indexes)
      यह->table->get_feature (feature_सूचकांक).add_lookup_indexes_to (&lookup_indexes);

    क्रम (स्वतः lookup_सूचकांक : lookup_indexes)
      अगर (unlikely (!builder->add_lookup_feature (lookup_सूचकांक, ft)))
	वापस;

    स्वतः &fv = यह->table->get_feature_variations ();
    स्वतः fi_गिनती = fv.record_गिनती ();
    क्रम (अचिन्हित i = 0; i < fi_गिनती; i++)
    {
      lookup_indexes.reset ();
      क्रम (स्वतः feature_सूचकांक : feature_indexes)
      {
	स्वतः feature_ptr = fv.find_substitute (i, feature_सूचकांक);
	अगर (feature_ptr != nullptr)
	  feature_ptr->add_lookup_indexes_to (&lookup_indexes);
      }
      क्रम (स्वतः lookup_सूचकांक : lookup_indexes)
	अगर (unlikely (!builder->add_lookup_feature (lookup_सूचकांक, ft)))
	  वापस;
    }

#अगर_अघोषित HB_NO_VAR
    lookup_indexes.reset ();
    fv.collect_lookup_variation_lookups (&feature_indexes, &lookup_indexes);
    क्रम (स्वतः lookup_सूचकांक : lookup_indexes)
      अगर (unlikely (!builder->add_lookup_feature (lookup_सूचकांक, ft)))
	वापस;
#पूर्ण_अगर
  }

  अगर (unlikely (!builder->finish_lookup_features ()))
    वापस;

  hb_set_t all_glyphs;
  all_glyphs.add_range (0, face->get_अंक_glyphs () - 1);

  hb_depend_context_t c (builder, face, &all_glyphs);

  क्रम (अचिन्हित i = 0; i < अंक_lookups; i++)
  {
    स्वतः features = builder->get_lookup_features (i);
    अगर (!features)
    {
      DEBUG_संदेश_LEVEL (DEPEND, nullptr, 1, 0,
                       "Skipping lookup %u (no features)", i);
      जारी;
    }
    DEBUG_संदेश_LEVEL (DEPEND, nullptr, 1, 0,
                     "Processing lookup %u with features:", i);
    c.lookup_सूचकांक = i;
    c.reset_recurse_cache ();
    c.lookups_seen.सफा ();
    c.lookups_seen.add (i);  /* Seed for A→B→A cycle detection in recurse(). */
    यह->table->get_lookup (i).depend (&c);
  }
}

} /* namespace OT */

#पूर्ण_अगर /* HB_OT_LAYOUT_GSUB_TABLE_HH */
