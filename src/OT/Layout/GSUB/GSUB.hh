#ifndef OT_LAYOUT_GSUB_GSUB_HH
#define OT_LAYOUT_GSUB_GSUB_HH

#ଅନ୍ତର୍ଭାଵ "../../../hb-ot-layout-gsubgpos.hh"
#ଅନ୍ତର୍ଭାଵ "Common.hh"
#ଅନ୍ତର୍ଭାଵ "SubstLookup.hh"

ନାମାକାଶ OT {

using Layout::GSUB_impl::SubstLookup;

ନାମାକାଶ Layout {

/*
 * GSUB -- Glyph Substitution
 * https://docs.microsoft.com/en-us/typography/opentype/spec/gsub
 */

ସଂରଚନା GSUB : GSUBGPOS
{
  using Lookup = SubstLookup;

  ସ୍ଥିର constexpr hb_tag_t tableTag = HB_OT_TAG_GSUB;

  ଅଚଳ SubstLookup& get_lookup (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i) ଅଚଳ
  { ଫେରନ୍ତୁ static_cast<ଅଚଳ SubstLookup &> (GSUBGPOS::get_lookup (i)); }

  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  {
    hb_subset_layout_context_t l (c, tableTag);
    ଫେରନ୍ତୁ GSUBGPOS::subset<SubstLookup> (&l);
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (GSUBGPOS::sanitize<SubstLookup> (c));
  }

  HB_INTERNAL ତାର୍କିକ is_blocklisted (hb_blob_t *blob,
                                   hb_face_t *face) ଅଚଳ;

  ଶୂନ୍ୟ closure_lookups (hb_face_t      *face,
                        ଅଚଳ hb_set_t *glyphs,
                        hb_set_t       *lookup_indexes /* IN/OUT */) ଅଚଳ
  { GSUBGPOS::closure_lookups<SubstLookup> (face, glyphs, lookup_indexes); }

  ପ୍ରକାରବ୍ୟାଖ୍ୟା GSUBGPOS::accelerator_t<GSUB> accelerator_t;
};


}

ସଂରଚନା GSUB_accelerator_t : Layout::GSUB::accelerator_t {
  GSUB_accelerator_t (hb_face_t *face) : Layout::GSUB::accelerator_t (face) {}
  HB_INTERNAL ଶୂନ୍ୟ depend (hb_depend_data_builder_t *builder, hb_face_t *face) ଅଚଳ;
};


}

#endif  /* OT_LAYOUT_GSUB_GSUB_HH */
