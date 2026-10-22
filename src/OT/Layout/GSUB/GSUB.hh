#جيڪڏهن_وصف_نه_ٿيل OT_LAYOUT_GSUB_GSUB_HH
#وصف_ڪريو OT_LAYOUT_GSUB_GSUB_HH

#ضمن "../../../hb-ot-layout-gsubgpos.hh"
#ضمن "Common.hh"
#ضمن "SubstLookup.hh"

نطاق OT {

استعمال Layout::GSUB_impl::SubstLookup;

نطاق Layout {

/*
 * GSUB -- Glyph Substitution
 * https://docs.microsoft.com/en-us/typography/opentype/spec/gsub
 */

بنية GSUB : GSUBGPOS
{
  استعمال Lookup = SubstLookup;

  ساكن constexpr hb_ٽيگ_t جدول_ٽيگ = HB_OT_ٽيگ_GSUB;

  ثابت SubstLookup& get_lookup (بنانشان صحيح i) ثابت
  { أرجع static_cast<ثابت SubstLookup &> (GSUBGPOS::get_lookup (i)); }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    hb_subset_layout_context_t l (c, جدول_ٽيگ);
    أرجع GSUBGPOS::subset<SubstLookup> (&l);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (GSUBGPOS::sanitize<SubstLookup> (c));
  }

  HB_اندروني منطقي is_blocklisted (hb_blob_t *blob,
                                   hb_face_t *face) ثابت;

  فراغ گهيرو_lookups (hb_face_t      *face,
                        ثابت hb_سيٽ_t *glyphs,
                        hb_سيٽ_t       *lookup_indexes /* IN/OUT */) ثابت
  { GSUBGPOS::گهيرو_lookups<SubstLookup> (face, glyphs, lookup_indexes); }

  تعريفنوع GSUBGPOS::accelerator_t<GSUB> accelerator_t;
};


}

بنية GSUB_accelerator_t : Layout::GSUB::accelerator_t {
  GSUB_accelerator_t (hb_face_t *face) : Layout::GSUB::accelerator_t (face) {}
  HB_اندروني فراغ depend (hb_depend_معلومات_builder_t *builder, hb_face_t *face) ثابت;
};


}

#جيڪڏهن_پورو  /* OT_LAYOUT_GSUB_GSUB_HH */
