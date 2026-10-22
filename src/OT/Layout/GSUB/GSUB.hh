#ifndef OT_LAYOUT_GSUB_GSUB_HH
#define OT_LAYOUT_GSUB_GSUB_HH

#அந்தர்பாவ "../../../hb-ot-layout-gsubgpos.hh"
#அந்தர்பாவ "Common.hh"
#அந்தர்பாவ "SubstLookup.hh"

நாமாகாஶ OT {

using Layout::GSUB_impl::SubstLookup;

நாமாகாஶ Layout {

/*
 * GSUB -- Glyph Substitution
 * https://docs.microsoft.com/en-us/typography/opentype/spec/gsub
 */

கட்டமைப்பு GSUB : GSUBGPOS
{
  using Lookup = SubstLookup;

  நிலையான constexpr hb_tag_t tableTag = HB_OT_TAG_GSUB;

  மாறிலி SubstLookup& get_lookup (unsigned முழு i) மாறிலி
  { திரும்பு static_cast<மாறிலி SubstLookup &> (GSUBGPOS::get_lookup (i)); }

  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    hb_subset_layout_context_t l (c, tableTag);
    திரும்பு GSUBGPOS::subset<SubstLookup> (&l);
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (GSUBGPOS::sanitize<SubstLookup> (c));
  }

  HB_INTERNAL தருக்க is_blocklisted (hb_blob_t *blob,
                                   hb_face_t *face) மாறிலி;

  வெறுமை closure_lookups (hb_face_t      *face,
                        மாறிலி hb_set_t *glyphs,
                        hb_set_t       *lookup_indexes /* IN/OUT */) மாறிலி
  { GSUBGPOS::closure_lookups<SubstLookup> (face, glyphs, lookup_indexes); }

  வகைவரை GSUBGPOS::accelerator_t<GSUB> accelerator_t;
};


}

கட்டமைப்பு GSUB_accelerator_t : Layout::GSUB::accelerator_t {
  GSUB_accelerator_t (hb_face_t *face) : Layout::GSUB::accelerator_t (face) {}
  HB_INTERNAL வெறுமை depend (hb_depend_data_builder_t *builder, hb_face_t *face) மாறிலி;
};


}

#endif  /* OT_LAYOUT_GSUB_GSUB_HH */
