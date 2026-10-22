#నిర్వచితం_కాకపోతే OT_LAYOUT_GSUB_GSUB_HH
#నిర్వచించు OT_LAYOUT_GSUB_GSUB_HH

#అన్తర్భావ "../../../hb-ot-layout-gsubgpos.hh"
#అన్తర్భావ "Common.hh"
#అన్తర్భావ "SubstLookup.hh"

నామాకాశ OT {

ఉపయోగించి Layout::GSUB_impl::SubstLookup;

నామాకాశ Layout {

/*
 * GSUB -- Glyph Substitution
 * https://docs.microsoft.com/en-us/typography/opentype/spec/gsub
 */

నిర్మాణం GSUB : GSUBGPOS
{
  ఉపయోగించి Lookup = SubstLookup;

  స్థిర constexpr hb_ట్యాగ్_t పట్టిక_ట్యాగ్ = HB_OT_ట్యాగ్_GSUB;

  స్థిరరాశి SubstLookup& get_lookup (గుర్తులేని పూర్ణాంకం i) స్థిరరాశి
  { తిరిగి static_cast<స్థిరరాశి SubstLookup &> (GSUBGPOS::get_lookup (i)); }

  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    hb_subset_layout_context_t l (c, పట్టిక_ట్యాగ్);
    తిరిగి GSUBGPOS::subset<SubstLookup> (&l);
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (GSUBGPOS::sanitize<SubstLookup> (c));
  }

  HB_అంతర్గత తార్కిక is_blocklisted (hb_blob_t *blob,
                                   hb_face_t *face) స్థిరరాశి;

  శూన్య సంవృతం_lookups (hb_face_t      *face,
                        స్థిరరాశి hb_సమితి_t *glyphs,
                        hb_సమితి_t       *lookup_indexes /* IN/OUT */) స్థిరరాశి
  { GSUBGPOS::సంవృతం_lookups<SubstLookup> (face, glyphs, lookup_indexes); }

  రకనిర్వచనం GSUBGPOS::accelerator_t<GSUB> accelerator_t;
};


}

నిర్మాణం GSUB_accelerator_t : Layout::GSUB::accelerator_t {
  GSUB_accelerator_t (hb_face_t *face) : Layout::GSUB::accelerator_t (face) {}
  HB_అంతర్గత శూన్య depend (hb_depend_దత్తాంశం_builder_t *builder, hb_face_t *face) స్థిరరాశి;
};


}

#అయితే_ముగింపు  /* OT_LAYOUT_GSUB_GSUB_HH */
