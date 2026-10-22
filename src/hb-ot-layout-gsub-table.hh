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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OT_LAYOUT_GSUB_ਸਾਰਣੀ_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_LAYOUT_GSUB_ਸਾਰਣੀ_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "OT/Layout/GSUB/GSUB.hh"

ਨਾਮਾਕਾਸ਼ OT {
ਨਾਮਾਕਾਸ਼ Layout {
ਨਾਮਾਕਾਸ਼ GSUB_impl {

// TODO(garretrieger): Move into the new layout directory.
/* Out-of-class implementation for methods recursing */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_LAYOUT
/*static*/ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ ExtensionSubst::is_ਉਲਟ () ਅਚਲ
{
  ਵਾਪਸ SubstLookup::lookup_type_is_ਉਲਟ (get_type ());
}
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ context_t>
/*static*/ ਕਿਸਮ_ਨਾਂ context_t::return_t SubstLookup::dispatch_recurse_func (context_t *c, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ lookup_ਸੂਚਕਾਂਕ)
{
  ਅਚਲ SubstLookup &l = c->face->ਸਾਰਣੀ.GSUB.get_relaxed ()->ਸਾਰਣੀ->get_lookup (lookup_ਸੂਚਕਾਂਕ);
  ਵਾਪਸ l.dispatch (c);
}

/*static*/ ਕਿਸਮ_ਨਾਂ hb_ਘੇਰਾ_context_t::return_t SubstLookup::ਘੇਰਾ_glyphs_recurse_func (hb_ਘੇਰਾ_context_t *c, ਬਿਨਾਨਿਸ਼ਾਨੀ lookup_ਸੂਚਕਾਂਕ, hb_ਸੈੱਟ_t *covered_seq_indices, ਬਿਨਾਨਿਸ਼ਾਨੀ seq_ਸੂਚਕਾਂਕ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਿਰਾ_ਸੂਚਕਾਂਕ)
{
  ਅਚਲ SubstLookup &l = c->face->ਸਾਰਣੀ.GSUB.get_relaxed ()->ਸਾਰਣੀ->get_lookup (lookup_ਸੂਚਕਾਂਕ);
  ਜੇ (l.may_have_non_1to1 ())
      hb_ਸੈੱਟ_ਜੋੜੋ_range (covered_seq_indices, seq_ਸੂਚਕਾਂਕ, ਸਿਰਾ_ਸੂਚਕਾਂਕ);
  ਵਾਪਸ l.dispatch (c);
}

/*static*/ hb_depend_context_t::return_t SubstLookup::depend_glyphs_recurse_func (hb_depend_context_t *c, ਬਿਨਾਨਿਸ਼ਾਨੀ lookup_ਸੂਚਕਾਂਕ, hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਪੰਨਾ_t *covered_seq_indices, ਬਿਨਾਨਿਸ਼ਾਨੀ seq_ਸੂਚਕਾਂਕ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਿਰਾ_ਸੂਚਕਾਂਕ)
{
  ਅਚਲ SubstLookup &l = c->face->ਸਾਰਣੀ.GSUB.get_relaxed ()->ਸਾਰਣੀ->get_lookup (lookup_ਸੂਚਕਾਂਕ);
  /* After a non-1-to-1 lookup (expansion/contraction), subsequent lookups in the same
   * contextual rule see all glyphs, not position-specific ones. This matches closure behavior.
   * Mark sequence positions as covered so later lookups use the full glyph set. */
  ਜੇ (l.may_have_non_1to1 ())
      covered_seq_indices->ਜੋੜੋ_range (seq_ਸੂਚਕਾਂਕ, ਸਿਰਾ_ਸੂਚਕਾਂਕ);

  hb_depend_context_t::recurse_ਕੁੰਜੀ_t key;
  ਜੇ (!c->get_recurse_ਕੁੰਜੀ (lookup_ਸੂਚਕਾਂਕ, &key))
    ਵਾਪਸ hb_ਖਾਲੀ_ਥਾਵਾਂ_t ();

  hb_depend_context_t::return_t ret = l.dispatch (c);
  c->ਅੰਤ_recurse (key);
  ਵਾਪਸ ret;
}

ਸਾਂਚਾ <>
ਸਤਰਅੰਦਰ hb_ਘੇਰਾ_lookups_context_t::return_t
SubstLookup::dispatch_recurse_func<hb_ਘੇਰਾ_lookups_context_t> (hb_ਘੇਰਾ_lookups_context_t *c, ਬਿਨਾਨਿਸ਼ਾਨੀ this_ਸੂਚਕਾਂਕ)
{
  ਅਚਲ SubstLookup &l = c->face->ਸਾਰਣੀ.GSUB.get_relaxed ()->ਸਾਰਣੀ->get_lookup (this_ਸੂਚਕਾਂਕ);
  ਵਾਪਸ l.ਘੇਰਾ_lookups (c, this_ਸੂਚਕਾਂਕ);
}

ਸਾਂਚਾ <>
ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ SubstLookup::dispatch_recurse_func<hb_ot_apply_context_t> (hb_ot_apply_context_t *c, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ lookup_ਸੂਚਕਾਂਕ)
{
  ਸਵੈਚਾਲਿਤ *gsub = c->face->ਸਾਰਣੀ.GSUB.get_relaxed ();
  ਅਚਲ SubstLookup &l = gsub->ਸਾਰਣੀ->get_lookup (lookup_ਸੂਚਕਾਂਕ);
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ saved_lookup_props = c->lookup_props;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ saved_lookup_ਸੂਚਕਾਂਕ = c->lookup_ਸੂਚਕਾਂਕ;
  c->ਸੈੱਟ_lookup_ਸੂਚਕਾਂਕ (lookup_ਸੂਚਕਾਂਕ);
  c->ਸੈੱਟ_lookup_props (l.get_props ());

  uint32_t ਢੇਰ_ਮੇਲ_positions[8];
  hb_ਸਦਿਸ਼_t<uint32_t> saved_ਮੇਲ_positions;
  saved_ਮੇਲ_positions.ਸੈੱਟ_storage (ਢੇਰ_ਮੇਲ_positions);
  hb_ਅਦਲਾ_ਬਦਲੀ (c->ਮੇਲ_positions, saved_ਮੇਲ_positions);

  ਤਾਰਕਿਕ ret = ਝੂਠ;
  ਸਵੈਚਾਲਿਤ *accel = gsub->get_accel (lookup_ਸੂਚਕਾਂਕ);
  ret = accel && accel->apply (c, ਝੂਠ);

  c->ਸੈੱਟ_lookup_ਸੂਚਕਾਂਕ (saved_lookup_ਸੂਚਕਾਂਕ);
  c->ਸੈੱਟ_lookup_props (saved_lookup_props);

  hb_ਅਦਲਾ_ਬਦਲੀ (c->ਮੇਲ_positions, saved_ਮੇਲ_positions);

  ਵਾਪਸ ret;
}
#ਜੇ_ਅੰਤ

} /* namespace GSUB_impl */
} /* namespace Layout */

ਸਤਰਅੰਦਰ ਖਾਲੀ
GSUB_accelerator_t::depend (hb_depend_ਅੰਕੜੇ_builder_t *builder, hb_face_t *face) ਅਚਲ
{
  ਜੇ (!ਇਹ->ਸਾਰਣੀ->has_ਅੰਕੜੇ ()) ਵਾਪਸ;

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_features = ਇਹ->ਸਾਰਣੀ->get_feature_ਗਿਣਤੀ ();
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_lookups  = ਇਹ->ਸਾਰਣੀ->get_lookup_ਗਿਣਤੀ ();

  hb_ਸਦਿਸ਼_t<hb_ਟੈਗ_t> feature_ਟੈਗ_ਸੂਚੀ;
  ਜੇ (!builder->check_success (feature_ਟੈਗ_ਸੂਚੀ.resize (ਅੰਕ_features)))
    ਵਾਪਸ;
  ਇਹ->ਸਾਰਣੀ->get_feature_ਟੈਗ_ਸੂਚੀ (0, &ਅੰਕ_features, feature_ਟੈਗ_ਸੂਚੀ.arrayZ);

  ਜੇ (!builder->init_lookup_features (ਅੰਕ_lookups))
    ਵਾਪਸ;

  hb_ਸਦਿਸ਼_t<hb_ਟੈਗ_t> feature_query_v;
  feature_query_v.resize (2);
  feature_query_v[1] = 0;

  hb_ਸੈੱਟ_t ਵੇਖਿਆ_features;
  hb_ਸੈੱਟ_t feature_indexes, lookup_indexes;

  ਲਈ (ਸਵੈਚਾਲਿਤ ft : feature_ਟੈਗ_ਸੂਚੀ)
  {
    ਜੇ (ਵੇਖਿਆ_features.has (ft)) ਜਾਰੀ;
    ਵੇਖਿਆ_features.ਜੋੜੋ (ft);
    feature_query_v[0] = ft;
    feature_indexes.reset ();
    hb_ot_layout_collect_features (face, HB_OT_ਟੈਗ_GSUB, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ,
                                   feature_query_v.arrayZ, &feature_indexes);
    lookup_indexes.reset ();
    ਲਈ (ਸਵੈਚਾਲਿਤ feature_ਸੂਚਕਾਂਕ : feature_indexes)
      ਇਹ->ਸਾਰਣੀ->get_feature (feature_ਸੂਚਕਾਂਕ).ਜੋੜੋ_lookup_indexes_ਮੰਜ਼ਿਲ (&lookup_indexes);

    ਲਈ (ਸਵੈਚਾਲਿਤ lookup_ਸੂਚਕਾਂਕ : lookup_indexes)
      ਜੇ (unlikely (!builder->ਜੋੜੋ_lookup_feature (lookup_ਸੂਚਕਾਂਕ, ft)))
	ਵਾਪਸ;

    ਸਵੈਚਾਲਿਤ &fv = ਇਹ->ਸਾਰਣੀ->get_feature_variations ();
    ਸਵੈਚਾਲਿਤ fi_ਗਿਣਤੀ = fv.ਰਿਕਾਰਡ_ਗਿਣਤੀ ();
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < fi_ਗਿਣਤੀ; i++)
    {
      lookup_indexes.reset ();
      ਲਈ (ਸਵੈਚਾਲਿਤ feature_ਸੂਚਕਾਂਕ : feature_indexes)
      {
	ਸਵੈਚਾਲਿਤ feature_ptr = fv.ਲੱਭੋ_substitute (i, feature_ਸੂਚਕਾਂਕ);
	ਜੇ (feature_ptr != ਖਾਲੀ_ਸੂਚਕ)
	  feature_ptr->ਜੋੜੋ_lookup_indexes_ਮੰਜ਼ਿਲ (&lookup_indexes);
      }
      ਲਈ (ਸਵੈਚਾਲਿਤ lookup_ਸੂਚਕਾਂਕ : lookup_indexes)
	ਜੇ (unlikely (!builder->ਜੋੜੋ_lookup_feature (lookup_ਸੂਚਕਾਂਕ, ft)))
	  ਵਾਪਸ;
    }

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
    lookup_indexes.reset ();
    fv.collect_lookup_variation_lookups (&feature_indexes, &lookup_indexes);
    ਲਈ (ਸਵੈਚਾਲਿਤ lookup_ਸੂਚਕਾਂਕ : lookup_indexes)
      ਜੇ (unlikely (!builder->ਜੋੜੋ_lookup_feature (lookup_ਸੂਚਕਾਂਕ, ft)))
	ਵਾਪਸ;
#ਜੇ_ਅੰਤ
  }

  ਜੇ (unlikely (!builder->ਅੰਤ_lookup_features ()))
    ਵਾਪਸ;

  hb_ਸੈੱਟ_t all_glyphs;
  all_glyphs.ਜੋੜੋ_range (0, face->get_ਅੰਕ_glyphs () - 1);

  hb_depend_context_t c (builder, face, &all_glyphs);

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਅੰਕ_lookups; i++)
  {
    ਸਵੈਚਾਲਿਤ features = builder->get_lookup_features (i);
    ਜੇ (!features)
    {
      DEBUG_ਸੁਨੇਹਾ_LEVEL (DEPEND, ਖਾਲੀ_ਸੂਚਕ, 1, 0,
                       "Skipping lookup %u (no features)", i);
      ਜਾਰੀ;
    }
    DEBUG_ਸੁਨੇਹਾ_LEVEL (DEPEND, ਖਾਲੀ_ਸੂਚਕ, 1, 0,
                     "Processing lookup %u with features:", i);
    c.lookup_ਸੂਚਕਾਂਕ = i;
    c.reset_recurse_cache ();
    c.lookups_ਵੇਖਿਆ.clear ();
    c.lookups_ਵੇਖਿਆ.ਜੋੜੋ (i);  /* Seed for A→B→A cycle detection in recurse(). */
    ਇਹ->ਸਾਰਣੀ->get_lookup (i).depend (&c);
  }
}

} /* namespace OT */

#ਜੇ_ਅੰਤ /* HB_OT_LAYOUT_GSUB_TABLE_HH */
