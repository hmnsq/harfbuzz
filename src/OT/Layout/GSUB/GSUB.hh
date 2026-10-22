#ifndef OT_LAYOUT_GSUB_GSUB_HH
#define OT_LAYOUT_GSUB_GSUB_HH

#અન્તર્ભાવ "../../../hb-ot-layout-gsubgpos.hh"
#અન્તર્ભાવ "Common.hh"
#અન્તર્ભાવ "SubstLookup.hh"

નામાકાશ OT {

using Layout::GSUB_impl::SubstLookup;

નામાકાશ Layout {

/*
 * GSUB -- Glyph Substitution
 * https://docs.microsoft.com/en-us/typography/opentype/spec/gsub
 */

સંરચના GSUB : GSUBGPOS
{
  using Lookup = SubstLookup;

  સ્થિર constexpr hb_tag_t tableTag = HB_OT_TAG_GSUB;

  અચલ SubstLookup& get_lookup (unsigned પૂર્ણાંક i) અચલ
  { પાછા static_cast<અચલ SubstLookup &> (GSUBGPOS::get_lookup (i)); }

  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    hb_subset_layout_context_t l (c, tableTag);
    પાછા GSUBGPOS::subset<SubstLookup> (&l);
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (GSUBGPOS::sanitize<SubstLookup> (c));
  }

  HB_INTERNAL તાર્કિક is_blocklisted (hb_blob_t *blob,
                                   hb_face_t *face) અચલ;

  શૂન્ય closure_lookups (hb_face_t      *face,
                        અચલ hb_set_t *glyphs,
                        hb_set_t       *lookup_indexes /* IN/OUT */) અચલ
  { GSUBGPOS::closure_lookups<SubstLookup> (face, glyphs, lookup_indexes); }

  પ્રકારવ્યાખ્યા GSUBGPOS::accelerator_t<GSUB> accelerator_t;
};


}

સંરચના GSUB_accelerator_t : Layout::GSUB::accelerator_t {
  GSUB_accelerator_t (hb_face_t *face) : Layout::GSUB::accelerator_t (face) {}
  HB_INTERNAL શૂન્ય depend (hb_depend_data_builder_t *builder, hb_face_t *face) અચલ;
};


}

#endif  /* OT_LAYOUT_GSUB_GSUB_HH */
