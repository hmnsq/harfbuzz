#جے_بنا_تعریف OT_LAYOUT_GSUB_GSUB_HH
#تعریف_کرو OT_LAYOUT_GSUB_GSUB_HH

#ضمن "../../../hb-ot-layout-gsubgpos.hh"
#ضمن "Common.hh"
#ضمن "SubstLookup.hh"

نطاق OT {

ورتوں Layout::GSUB_impl::SubstLookup;

نطاق Layout {

/*
 * GSUB -- Glyph Substitution
 * https://docs.microsoft.com/en-us/typography/opentype/spec/gsub
 */

بنية GSUB : GSUBGPOS
{
  ورتوں Lookup = SubstLookup;

  ساكن constexpr hb_ٹیگ_t جدول_ٹیگ = HB_OT_ٹیگ_GSUB;

  ثابت SubstLookup& get_lookup (بنانشانی صحيح i) ثابت
  { أرجع static_cast<ثابت SubstLookup &> (GSUBGPOS::get_lookup (i)); }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    hb_subset_layout_context_t l (c, جدول_ٹیگ);
    أرجع GSUBGPOS::subset<SubstLookup> (&l);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (GSUBGPOS::sanitize<SubstLookup> (c));
  }

  HB_اندرونی منطقي is_blocklisted (hb_blob_t *blob,
                                   hb_face_t *face) ثابت;

  فراغ گھیرا_lookups (hb_face_t      *face,
                        ثابت hb_سیٹ_t *glyphs,
                        hb_سیٹ_t       *lookup_indexes /* IN/OUT */) ثابت
  { GSUBGPOS::گھیرا_lookups<SubstLookup> (face, glyphs, lookup_indexes); }

  تعريفنوع GSUBGPOS::accelerator_t<GSUB> accelerator_t;
};


}

بنية GSUB_accelerator_t : Layout::GSUB::accelerator_t {
  GSUB_accelerator_t (hb_face_t *face) : Layout::GSUB::accelerator_t (face) {}
  HB_اندرونی فراغ depend (hb_depend_انکڑے_builder_t *builder, hb_face_t *face) ثابت;
};


}

#جے_انت  /* OT_LAYOUT_GSUB_GSUB_HH */
