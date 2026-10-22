#ifndef OT_LAYOUT_GSUB_GSUB_HH
#define OT_LAYOUT_GSUB_GSUB_HH

#ಅನ್ತರ್ಭಾವ "../../../hb-ot-layout-gsubgpos.hh"
#ಅನ್ತರ್ಭಾವ "Common.hh"
#ಅನ್ತರ್ಭಾವ "SubstLookup.hh"

ನಾಮಾಕಾಶ OT {

using Layout::GSUB_impl::SubstLookup;

ನಾಮಾಕಾಶ Layout {

/*
 * GSUB -- Glyph Substitution
 * https://docs.microsoft.com/en-us/typography/opentype/spec/gsub
 */

ಸಂರಚನಾ GSUB : GSUBGPOS
{
  using Lookup = SubstLookup;

  ಸ್ಥಿರ constexpr hb_tag_t tableTag = HB_OT_TAG_GSUB;

  ಧ್ರುವ SubstLookup& get_lookup (unsigned ಪೂರ್ಣಾಙ್ಕ i) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ static_cast<ಧ್ರುವ SubstLookup &> (GSUBGPOS::get_lookup (i)); }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    hb_subset_layout_context_t l (c, tableTag);
    ಪ್ರತ್ಯಾಗಮ GSUBGPOS::subset<SubstLookup> (&l);
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (GSUBGPOS::sanitize<SubstLookup> (c));
  }

  HB_INTERNAL ತಾರ್ಕಿಕ is_blocklisted (hb_blob_t *blob,
                                   hb_face_t *face) ಧ್ರುವ;

  ಶೂನ್ಯ closure_lookups (hb_face_t      *face,
                        ಧ್ರುವ hb_set_t *glyphs,
                        hb_set_t       *lookup_indexes /* IN/OUT */) ಧ್ರುವ
  { GSUBGPOS::closure_lookups<SubstLookup> (face, glyphs, lookup_indexes); }

  ಪ್ರಕಾರನಿರ್ದೇಶ GSUBGPOS::accelerator_t<GSUB> accelerator_t;
};


}

ಸಂರಚನಾ GSUB_accelerator_t : Layout::GSUB::accelerator_t {
  GSUB_accelerator_t (hb_face_t *face) : Layout::GSUB::accelerator_t (face) {}
  HB_INTERNAL ಶೂನ್ಯ depend (hb_depend_data_builder_t *builder, hb_face_t *face) ಧ್ರುವ;
};


}

#endif  /* OT_LAYOUT_GSUB_GSUB_HH */
