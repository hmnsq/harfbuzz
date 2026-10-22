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

#నిర్వచితం_కాకపోతే HB_OT_LAYOUT_GSUB_పట్టిక_HH
#నిర్వచించు HB_OT_LAYOUT_GSUB_పట్టిక_HH

#అన్తర్భావ "OT/Layout/GSUB/GSUB.hh"

నామాకాశ OT {
నామాకాశ Layout {
నామాకాశ GSUB_impl {

// TODO(garretrieger): Move into the new layout directory.
/* Out-of-class implementation for methods recursing */

#నిర్వచితం_కాకపోతే HB_NO_OT_LAYOUT
/*static*/ అంతఃవరుస తార్కిక ExtensionSubst::is_తిరగేయి () స్థిరరాశి
{
  తిరిగి SubstLookup::lookup_type_is_తిరగేయి (get_type ());
}
మూస <రకం_పేరు context_t>
/*static*/ రకం_పేరు context_t::return_t SubstLookup::dispatch_recurse_func (context_t *c, గుర్తులేని పూర్ణాంకం lookup_సూచీ)
{
  స్థిరరాశి SubstLookup &l = c->face->పట్టిక.GSUB.get_relaxed ()->పట్టిక->get_lookup (lookup_సూచీ);
  తిరిగి l.dispatch (c);
}

/*static*/ రకం_పేరు hb_సంవృతం_context_t::return_t SubstLookup::సంవృతం_glyphs_recurse_func (hb_సంవృతం_context_t *c, గుర్తులేని lookup_సూచీ, hb_సమితి_t *covered_seq_indices, గుర్తులేని seq_సూచీ, గుర్తులేని చివర_సూచీ)
{
  స్థిరరాశి SubstLookup &l = c->face->పట్టిక.GSUB.get_relaxed ()->పట్టిక->get_lookup (lookup_సూచీ);
  ఒకవేళ (l.may_have_non_1to1 ())
      hb_సమితి_కూడు_range (covered_seq_indices, seq_సూచీ, చివర_సూచీ);
  తిరిగి l.dispatch (c);
}

/*static*/ hb_depend_context_t::return_t SubstLookup::depend_glyphs_recurse_func (hb_depend_context_t *c, గుర్తులేని lookup_సూచీ, hb_బిట్టు_పుట_t *covered_seq_indices, గుర్తులేని seq_సూచీ, గుర్తులేని చివర_సూచీ)
{
  స్థిరరాశి SubstLookup &l = c->face->పట్టిక.GSUB.get_relaxed ()->పట్టిక->get_lookup (lookup_సూచీ);
  /* After a non-1-to-1 lookup (expansion/contraction), subsequent lookups in the same
   * contextual rule see all glyphs, not position-specific ones. This matches closure behavior.
   * Mark sequence positions as covered so later lookups use the full glyph set. */
  ఒకవేళ (l.may_have_non_1to1 ())
      covered_seq_indices->కూడు_range (seq_సూచీ, చివర_సూచీ);

  hb_depend_context_t::recurse_కీలకం_t key;
  ఒకవేళ (!c->get_recurse_కీలకం (lookup_సూచీ, &key))
    తిరిగి hb_ఖాళీలు_t ();

  hb_depend_context_t::return_t ret = l.dispatch (c);
  c->ముగింపు_recurse (key);
  తిరిగి ret;
}

మూస <>
అంతఃవరుస hb_సంవృతం_lookups_context_t::return_t
SubstLookup::dispatch_recurse_func<hb_సంవృతం_lookups_context_t> (hb_సంవృతం_lookups_context_t *c, గుర్తులేని this_సూచీ)
{
  స్థిరరాశి SubstLookup &l = c->face->పట్టిక.GSUB.get_relaxed ()->పట్టిక->get_lookup (this_సూచీ);
  తిరిగి l.సంవృతం_lookups (c, this_సూచీ);
}

మూస <>
అంతఃవరుస తార్కిక SubstLookup::dispatch_recurse_func<hb_ot_apply_context_t> (hb_ot_apply_context_t *c, గుర్తులేని పూర్ణాంకం lookup_సూచీ)
{
  స్వయంచాలక *gsub = c->face->పట్టిక.GSUB.get_relaxed ();
  స్థిరరాశి SubstLookup &l = gsub->పట్టిక->get_lookup (lookup_సూచీ);
  గుర్తులేని పూర్ణాంకం saved_lookup_props = c->lookup_props;
  గుర్తులేని పూర్ణాంకం saved_lookup_సూచీ = c->lookup_సూచీ;
  c->సమితి_lookup_సూచీ (lookup_సూచీ);
  c->సమితి_lookup_props (l.get_props ());

  uint32_t దొంతర_సరిపోలిక_positions[8];
  hb_సదిశ_t<uint32_t> saved_సరిపోలిక_positions;
  saved_సరిపోలిక_positions.సమితి_storage (దొంతర_సరిపోలిక_positions);
  hb_మార్పిడి (c->సరిపోలిక_positions, saved_సరిపోలిక_positions);

  తార్కిక ret = అబద్ధం;
  స్వయంచాలక *accel = gsub->get_accel (lookup_సూచీ);
  ret = accel && accel->apply (c, అబద్ధం);

  c->సమితి_lookup_సూచీ (saved_lookup_సూచీ);
  c->సమితి_lookup_props (saved_lookup_props);

  hb_మార్పిడి (c->సరిపోలిక_positions, saved_సరిపోలిక_positions);

  తిరిగి ret;
}
#అయితే_ముగింపు

} /* namespace GSUB_impl */
} /* namespace Layout */

అంతఃవరుస శూన్య
GSUB_accelerator_t::depend (hb_depend_దత్తాంశం_builder_t *builder, hb_face_t *face) స్థిరరాశి
{
  ఒకవేళ (!ఇది->పట్టిక->has_దత్తాంశం ()) తిరిగి;

  గుర్తులేని అంకె_features = ఇది->పట్టిక->get_feature_లెక్క ();
  గుర్తులేని అంకె_lookups  = ఇది->పట్టిక->get_lookup_లెక్క ();

  hb_సదిశ_t<hb_ట్యాగ్_t> feature_ట్యాగులు;
  ఒకవేళ (!builder->check_success (feature_ట్యాగులు.resize (అంకె_features)))
    తిరిగి;
  ఇది->పట్టిక->get_feature_ట్యాగులు (0, &అంకె_features, feature_ట్యాగులు.arrayZ);

  ఒకవేళ (!builder->init_lookup_features (అంకె_lookups))
    తిరిగి;

  hb_సదిశ_t<hb_ట్యాగ్_t> feature_query_v;
  feature_query_v.resize (2);
  feature_query_v[1] = 0;

  hb_సమితి_t చూసిన_features;
  hb_సమితి_t feature_indexes, lookup_indexes;

  కొరకు (స్వయంచాలక ft : feature_ట్యాగులు)
  {
    ఒకవేళ (చూసిన_features.has (ft)) కొనసాగు;
    చూసిన_features.కూడు (ft);
    feature_query_v[0] = ft;
    feature_indexes.reset ();
    hb_ot_layout_collect_features (face, HB_OT_ట్యాగ్_GSUB, శూన్యసూచిక, శూన్యసూచిక,
                                   feature_query_v.arrayZ, &feature_indexes);
    lookup_indexes.reset ();
    కొరకు (స్వయంచాలక feature_సూచీ : feature_indexes)
      ఇది->పట్టిక->get_feature (feature_సూచీ).కూడు_lookup_indexes_గమ్యం (&lookup_indexes);

    కొరకు (స్వయంచాలక lookup_సూచీ : lookup_indexes)
      ఒకవేళ (unlikely (!builder->కూడు_lookup_feature (lookup_సూచీ, ft)))
	తిరిగి;

    స్వయంచాలక &fv = ఇది->పట్టిక->get_feature_variations ();
    స్వయంచాలక fi_లెక్క = fv.రికార్డు_లెక్క ();
    కొరకు (గుర్తులేని i = 0; i < fi_లెక్క; i++)
    {
      lookup_indexes.reset ();
      కొరకు (స్వయంచాలక feature_సూచీ : feature_indexes)
      {
	స్వయంచాలక feature_ptr = fv.కనుగొను_substitute (i, feature_సూచీ);
	ఒకవేళ (feature_ptr != శూన్యసూచిక)
	  feature_ptr->కూడు_lookup_indexes_గమ్యం (&lookup_indexes);
      }
      కొరకు (స్వయంచాలక lookup_సూచీ : lookup_indexes)
	ఒకవేళ (unlikely (!builder->కూడు_lookup_feature (lookup_సూచీ, ft)))
	  తిరిగి;
    }

#నిర్వచితం_కాకపోతే HB_NO_VAR
    lookup_indexes.reset ();
    fv.collect_lookup_variation_lookups (&feature_indexes, &lookup_indexes);
    కొరకు (స్వయంచాలక lookup_సూచీ : lookup_indexes)
      ఒకవేళ (unlikely (!builder->కూడు_lookup_feature (lookup_సూచీ, ft)))
	తిరిగి;
#అయితే_ముగింపు
  }

  ఒకవేళ (unlikely (!builder->ముగింపు_lookup_features ()))
    తిరిగి;

  hb_సమితి_t all_glyphs;
  all_glyphs.కూడు_range (0, face->get_అంకె_glyphs () - 1);

  hb_depend_context_t c (builder, face, &all_glyphs);

  కొరకు (గుర్తులేని i = 0; i < అంకె_lookups; i++)
  {
    స్వయంచాలక features = builder->get_lookup_features (i);
    ఒకవేళ (!features)
    {
      DEBUG_సందేశం_LEVEL (DEPEND, శూన్యసూచిక, 1, 0,
                       "Skipping lookup %u (no features)", i);
      కొనసాగు;
    }
    DEBUG_సందేశం_LEVEL (DEPEND, శూన్యసూచిక, 1, 0,
                     "Processing lookup %u with features:", i);
    c.lookup_సూచీ = i;
    c.reset_recurse_cache ();
    c.lookups_చూసిన.clear ();
    c.lookups_చూసిన.కూడు (i);  /* Seed for A→B→A cycle detection in recurse(). */
    ఇది->పట్టిక->get_lookup (i).depend (&c);
  }
}

} /* namespace OT */

#అయితే_ముగింపు /* HB_OT_LAYOUT_GSUB_TABLE_HH */
