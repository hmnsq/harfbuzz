#ifndef OT_LAYOUT_GSUB_GSUB_HH
#define OT_LAYOUT_GSUB_GSUB_HH

#ضمن "../../../hb-ot-layout-gsubgpos.hh"
#ضمن "Common.hh"
#ضمن "SubstLookup.hh"

نطاق OT {

using Layout::GSUB_impl::SubstLookup;

نطاق Layout {

/*
 * GSUB -- Glyph Substitution
 * https://docs.microsoft.com/en-us/typography/opentype/spec/gsub
 */

بنية GSUB : GSUBGPOS
{
  using Lookup = SubstLookup;

  ساكن constexpr hb_tag_t tableTag = HB_OT_TAG_GSUB;

  ثابت SubstLookup& get_lookup (unsigned صحيح i) ثابت
  { החזר static_cast<ثابت SubstLookup &> (GSUBGPOS::get_lookup (i)); }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    hb_subset_layout_context_t l (c, tableTag);
    החזר GSUBGPOS::subset<SubstLookup> (&l);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (GSUBGPOS::sanitize<SubstLookup> (c));
  }

  HB_INTERNAL منطقي is_blocklisted (hb_blob_t *blob,
                                   hb_face_t *face) ثابت;

  فراغ closure_lookups (hb_face_t      *face,
                        ثابت hb_set_t *glyphs,
                        hb_set_t       *lookup_indexes /* IN/OUT */) ثابت
  { GSUBGPOS::closure_lookups<SubstLookup> (face, glyphs, lookup_indexes); }

  تعريفنوع GSUBGPOS::accelerator_t<GSUB> accelerator_t;
};


}

بنية GSUB_accelerator_t : Layout::GSUB::accelerator_t {
  GSUB_accelerator_t (hb_face_t *face) : Layout::GSUB::accelerator_t (face) {}
  HB_INTERNAL فراغ depend (hb_depend_data_builder_t *builder, hb_face_t *face) ثابت;
};


}

#endif  /* OT_LAYOUT_GSUB_GSUB_HH */
