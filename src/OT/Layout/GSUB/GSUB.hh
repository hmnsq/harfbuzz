#ifndef OT_LAYOUT_GSUB_GSUB_HH
#define OT_LAYOUT_GSUB_GSUB_HH

#شامل "../../../hb-ot-layout-gsubgpos.hh"
#شامل "Common.hh"
#شامل "SubstLookup.hh"

فضاینام OT {

using Layout::GSUB_impl::SubstLookup;

فضاینام Layout {

/*
 * GSUB -- Glyph Substitution
 * https://docs.microsoft.com/en-us/typography/opentype/spec/gsub
 */

ساختار GSUB : GSUBGPOS
{
  using Lookup = SubstLookup;

  ایستا constexpr hb_tag_t tableTag = HB_OT_TAG_GSUB;

  ثابت SubstLookup& get_lookup (unsigned صحیح i) ثابت
  { بازگشت static_cast<ثابت SubstLookup &> (GSUBGPOS::get_lookup (i)); }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    hb_subset_layout_context_t l (c, tableTag);
    بازگشت GSUBGPOS::subset<SubstLookup> (&l);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (GSUBGPOS::sanitize<SubstLookup> (c));
  }

  HB_INTERNAL منطقی is_blocklisted (hb_blob_t *blob,
                                   hb_face_t *face) ثابت;

  تهی closure_lookups (hb_face_t      *face,
                        ثابت hb_set_t *glyphs,
                        hb_set_t       *lookup_indexes /* IN/OUT */) ثابت
  { GSUBGPOS::closure_lookups<SubstLookup> (face, glyphs, lookup_indexes); }

  تعریفنوع GSUBGPOS::accelerator_t<GSUB> accelerator_t;
};


}

ساختار GSUB_accelerator_t : Layout::GSUB::accelerator_t {
  GSUB_accelerator_t (hb_face_t *face) : Layout::GSUB::accelerator_t (face) {}
  HB_INTERNAL تهی depend (hb_depend_data_builder_t *builder, hb_face_t *face) ثابت;
};


}

#endif  /* OT_LAYOUT_GSUB_GSUB_HH */
