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

#अन्तर्भाव "OT/Layout/GSUB/GSUB.hh"

नामाकाश OT {
नामाकाश Layout {
नामाकाश GSUB_impl {

// TODO(garretrieger): Move into the new layout directory.
/* Out-of-class implementation for methods recursing */

#ifndef HB_NO_OT_LAYOUT
/*static*/ अन्तःस्थ तार्किक ExtensionSubst::is_reverse () अचर
{
  फर्काऊ SubstLookup::lookup_type_is_reverse (get_type ());
}
ढाँचा <typename context_t>
/*static*/ typename context_t::return_t SubstLookup::dispatch_recurse_func (context_t *c, अचिन्हित पूर्णाङ्क lookup_index)
{
  अचर SubstLookup &l = c->face->table.GSUB.get_relaxed ()->table->get_lookup (lookup_index);
  फर्काऊ l.dispatch (c);
}

/*static*/ typename hb_closure_context_t::return_t SubstLookup::closure_glyphs_recurse_func (hb_closure_context_t *c, अचिन्हित lookup_index, hb_set_t *covered_seq_indices, अचिन्हित seq_index, अचिन्हित end_index)
{
  अचर SubstLookup &l = c->face->table.GSUB.get_relaxed ()->table->get_lookup (lookup_index);
  यदि (l.may_have_non_1to1 ())
      hb_set_add_range (covered_seq_indices, seq_index, end_index);
  फर्काऊ l.dispatch (c);
}

/*static*/ hb_depend_context_t::return_t SubstLookup::depend_glyphs_recurse_func (hb_depend_context_t *c, अचिन्हित lookup_index, hb_bit_page_t *covered_seq_indices, अचिन्हित seq_index, अचिन्हित end_index)
{
  अचर SubstLookup &l = c->face->table.GSUB.get_relaxed ()->table->get_lookup (lookup_index);
  /* After a non-1-to-1 lookup (expansion/contraction), subsequent lookups in the same
   * contextual rule see all glyphs, not position-specific ones. This matches closure behavior.
   * Mark sequence positions as covered so later lookups use the full glyph set. */
  यदि (l.may_have_non_1to1 ())
      covered_seq_indices->add_range (seq_index, end_index);

  hb_depend_context_t::recurse_साँचो_t key;
  यदि (!c->get_recurse_साँचो (lookup_index, &key))
    फर्काऊ hb_empty_t ();

  hb_depend_context_t::return_t ret = l.dispatch (c);
  c->finish_recurse (key);
  फर्काऊ ret;
}

ढाँचा <>
अन्तःस्थ hb_closure_lookups_context_t::return_t
SubstLookup::dispatch_recurse_func<hb_closure_lookups_context_t> (hb_closure_lookups_context_t *c, अचिन्हित this_index)
{
  अचर SubstLookup &l = c->face->table.GSUB.get_relaxed ()->table->get_lookup (this_index);
  फर्काऊ l.closure_lookups (c, this_index);
}

ढाँचा <>
अन्तःस्थ तार्किक SubstLookup::dispatch_recurse_func<hb_ot_apply_context_t> (hb_ot_apply_context_t *c, अचिन्हित पूर्णाङ्क lookup_index)
{
  स्वतः *gsub = c->face->table.GSUB.get_relaxed ();
  अचर SubstLookup &l = gsub->table->get_lookup (lookup_index);
  अचिन्हित पूर्णाङ्क saved_lookup_props = c->lookup_props;
  अचिन्हित पूर्णाङ्क saved_lookup_index = c->lookup_index;
  c->set_lookup_index (lookup_index);
  c->set_lookup_props (l.get_props ());

  uint32_t थाक_match_positions[8];
  hb_vector_t<uint32_t> saved_match_positions;
  saved_match_positions.set_storage (थाक_match_positions);
  hb_swap (c->match_positions, saved_match_positions);

  तार्किक ret = असत्य;
  स्वतः *accel = gsub->get_accel (lookup_index);
  ret = accel && accel->apply (c, असत्य);

  c->set_lookup_index (saved_lookup_index);
  c->set_lookup_props (saved_lookup_props);

  hb_swap (c->match_positions, saved_match_positions);

  फर्काऊ ret;
}
#endif

} /* namespace GSUB_impl */
} /* namespace Layout */

अन्तःस्थ शून्य
GSUB_accelerator_t::depend (hb_depend_data_builder_t *builder, hb_face_t *face) अचर
{
  यदि (!एतत्->table->has_data ()) फर्काऊ;

  अचिन्हित अङ्क_features = एतत्->table->get_feature_गन्ती ();
  अचिन्हित अङ्क_lookups  = एतत्->table->get_lookup_गन्ती ();

  hb_vector_t<hb_tag_t> feature_tags;
  यदि (!builder->check_success (feature_tags.resize (अङ्क_features)))
    फर्काऊ;
  एतत्->table->get_feature_tags (0, &अङ्क_features, feature_tags.arrayZ);

  यदि (!builder->init_lookup_features (अङ्क_lookups))
    फर्काऊ;

  hb_vector_t<hb_tag_t> feature_query_v;
  feature_query_v.resize (2);
  feature_query_v[1] = 0;

  hb_set_t seen_features;
  hb_set_t feature_indexes, lookup_indexes;

  लागि (स्वतः ft : feature_tags)
  {
    यदि (seen_features.has (ft)) जारी;
    seen_features.add (ft);
    feature_query_v[0] = ft;
    feature_indexes.reset ();
    hb_ot_layout_collect_features (face, HB_OT_TAG_GSUB, nullptr, nullptr,
                                   feature_query_v.arrayZ, &feature_indexes);
    lookup_indexes.reset ();
    लागि (स्वतः feature_index : feature_indexes)
      एतत्->table->get_feature (feature_index).add_lookup_indexes_to (&lookup_indexes);

    लागि (स्वतः lookup_index : lookup_indexes)
      यदि (unlikely (!builder->add_lookup_feature (lookup_index, ft)))
	फर्काऊ;

    स्वतः &fv = एतत्->table->get_feature_variations ();
    स्वतः fi_गन्ती = fv.record_गन्ती ();
    लागि (अचिन्हित i = 0; i < fi_गन्ती; i++)
    {
      lookup_indexes.reset ();
      लागि (स्वतः feature_index : feature_indexes)
      {
	स्वतः feature_ptr = fv.find_substitute (i, feature_index);
	यदि (feature_ptr != nullptr)
	  feature_ptr->add_lookup_indexes_to (&lookup_indexes);
      }
      लागि (स्वतः lookup_index : lookup_indexes)
	यदि (unlikely (!builder->add_lookup_feature (lookup_index, ft)))
	  फर्काऊ;
    }

#ifndef HB_NO_VAR
    lookup_indexes.reset ();
    fv.collect_lookup_variation_lookups (&feature_indexes, &lookup_indexes);
    लागि (स्वतः lookup_index : lookup_indexes)
      यदि (unlikely (!builder->add_lookup_feature (lookup_index, ft)))
	फर्काऊ;
#endif
  }

  यदि (unlikely (!builder->finish_lookup_features ()))
    फर्काऊ;

  hb_set_t all_glyphs;
  all_glyphs.add_range (0, face->get_अङ्क_glyphs () - 1);

  hb_depend_context_t c (builder, face, &all_glyphs);

  लागि (अचिन्हित i = 0; i < अङ्क_lookups; i++)
  {
    स्वतः features = builder->get_lookup_features (i);
    यदि (!features)
    {
      DEBUG_सन्देश_LEVEL (DEPEND, nullptr, 1, 0,
                       "Skipping lookup %u (no features)", i);
      जारी;
    }
    DEBUG_सन्देश_LEVEL (DEPEND, nullptr, 1, 0,
                     "Processing lookup %u with features:", i);
    c.lookup_index = i;
    c.reset_recurse_cache ();
    c.lookups_seen.clear ();
    c.lookups_seen.add (i);  /* Seed for A→B→A cycle detection in recurse(). */
    एतत्->table->get_lookup (i).depend (&c);
  }
}

} /* namespace OT */

#endif /* HB_OT_LAYOUT_GSUB_TABLE_HH */
