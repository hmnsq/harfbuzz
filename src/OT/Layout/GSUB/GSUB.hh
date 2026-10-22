#که_نه_تعریف_شوی OT_LAYOUT_GSUB_GSUB_HH
#تعریف_کړه OT_LAYOUT_GSUB_GSUB_HH

#ضمن "../../../hb-ot-layout-gsubgpos.hh"
#ضمن "Common.hh"
#ضمن "SubstLookup.hh"

نطاق OT {

کارول Layout::GSUB_impl::SubstLookup;

نطاق Layout {

/*
 * GSUB -- Glyph Substitution
 * https://docs.microsoft.com/en-us/typography/opentype/spec/gsub
 */

بنية GSUB : GSUBGPOS
{
  کارول Lookup = SubstLookup;

  ساكن constexpr hb_نښان_t جدول_نښان = HB_OT_نښان_GSUB;

  ثابت SubstLookup& get_lookup (بینښه صحيح i) ثابت
  { أرجع static_cast<ثابت SubstLookup &> (GSUBGPOS::get_lookup (i)); }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    hb_subset_layout_context_t l (c, جدول_نښان);
    أرجع GSUBGPOS::subset<SubstLookup> (&l);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (GSUBGPOS::sanitize<SubstLookup> (c));
  }

  HB_دننی منطقي is_blocklisted (hb_blob_t *blob,
                                   hb_face_t *face) ثابت;

  فراغ تړون_lookups (hb_face_t      *face,
                        ثابت hb_سېټ_t *glyphs,
                        hb_سېټ_t       *lookup_indexes /* IN/OUT */) ثابت
  { GSUBGPOS::تړون_lookups<SubstLookup> (face, glyphs, lookup_indexes); }

  تعريفنوع GSUBGPOS::accelerator_t<GSUB> accelerator_t;
};


}

بنية GSUB_accelerator_t : Layout::GSUB::accelerator_t {
  GSUB_accelerator_t (hb_face_t *face) : Layout::GSUB::accelerator_t (face) {}
  HB_دننی فراغ depend (hb_depend_معلومات_builder_t *builder, hb_face_t *face) ثابت;
};


}

#د_که_پای  /* OT_LAYOUT_GSUB_GSUB_HH */
