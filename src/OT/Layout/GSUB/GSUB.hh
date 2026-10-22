#اگر_تعریف_نشده OT_LAYOUT_GSUB_GSUB_HH
#تعریف_کن OT_LAYOUT_GSUB_GSUB_HH

#ضمن "../../../hb-ot-layout-gsubgpos.hh"
#ضمن "Common.hh"
#ضمن "SubstLookup.hh"

نطاق OT {

استفاده Layout::GSUB_impl::SubstLookup;

نطاق Layout {

/*
 * GSUB -- Glyph Substitution
 * https://docs.microsoft.com/en-us/typography/opentype/spec/gsub
 */

بنية GSUB : GSUBGPOS
{
  استفاده Lookup = SubstLookup;

  ساكن constexpr hb_برچسب_t جدول_برچسب = HB_OT_برچسب_GSUB;

  ثابت SubstLookup& get_lookup (بیعلامت صحيح i) ثابت
  { أرجع static_cast<ثابت SubstLookup &> (GSUBGPOS::get_lookup (i)); }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    hb_subset_layout_context_t l (c, جدول_برچسب);
    أرجع GSUBGPOS::subset<SubstLookup> (&l);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (GSUBGPOS::sanitize<SubstLookup> (c));
  }

  HB_داخلی منطقي is_blocklisted (hb_blob_t *blob,
                                   hb_face_t *face) ثابت;

  فراغ بستگی_lookups (hb_face_t      *face,
                        ثابت hb_مجموعه_t *glyphs,
                        hb_مجموعه_t       *lookup_indexes /* IN/OUT */) ثابت
  { GSUBGPOS::بستگی_lookups<SubstLookup> (face, glyphs, lookup_indexes); }

  تعريفنوع GSUBGPOS::accelerator_t<GSUB> accelerator_t;
};


}

بنية GSUB_accelerator_t : Layout::GSUB::accelerator_t {
  GSUB_accelerator_t (hb_face_t *face) : Layout::GSUB::accelerator_t (face) {}
  HB_داخلی فراغ depend (hb_depend_داده_builder_t *builder, hb_face_t *face) ثابت;
};


}

#پایان_اگر  /* OT_LAYOUT_GSUB_GSUB_HH */
