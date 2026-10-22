#اگر_غیر_تعریف_شدہ OT_LAYOUT_GSUB_GSUB_HH
#تعریف_کرو OT_LAYOUT_GSUB_GSUB_HH

#شامل "../../../hb-ot-layout-gsubgpos.hh"
#شامل "Common.hh"
#شامل "SubstLookup.hh"

نامخلا OT {

استعمال Layout::GSUB_impl::SubstLookup;

نامخلا Layout {

/*
 * GSUB -- Glyph Substitution
 * https://docs.microsoft.com/en-us/typography/opentype/spec/gsub
 */

ساخت GSUB : GSUBGPOS
{
  استعمال Lookup = SubstLookup;

  ساکن constexpr hb_لیبل_t جدول_لیبل = HB_OT_لیبل_GSUB;

  ثابت SubstLookup& get_lookup (غیرعلامتی صحیح i) ثابت
  { واپس static_cast<ثابت SubstLookup &> (GSUBGPOS::get_lookup (i)); }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    hb_subset_layout_context_t l (c, جدول_لیبل);
    واپس GSUBGPOS::subset<SubstLookup> (&l);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (GSUBGPOS::sanitize<SubstLookup> (c));
  }

  HB_اندرونی منطقی is_blocklisted (hb_blob_t *blob,
                                   hb_face_t *face) ثابت;

  خالی احاطہ_lookups (hb_face_t      *face,
                        ثابت hb_سیٹ_t *glyphs,
                        hb_سیٹ_t       *lookup_indexes /* IN/OUT */) ثابت
  { GSUBGPOS::احاطہ_lookups<SubstLookup> (face, glyphs, lookup_indexes); }

  قسمتعریف GSUBGPOS::accelerator_t<GSUB> accelerator_t;
};


}

ساخت GSUB_accelerator_t : Layout::GSUB::accelerator_t {
  GSUB_accelerator_t (hb_face_t *face) : Layout::GSUB::accelerator_t (face) {}
  HB_اندرونی خالی depend (hb_depend_ڈیٹا_builder_t *builder, hb_face_t *face) ثابت;
};


}

#اگر_ختم  /* OT_LAYOUT_GSUB_GSUB_HH */
