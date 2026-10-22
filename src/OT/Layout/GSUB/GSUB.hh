#ifndef OT_LAYOUT_GSUB_GSUB_HH
#define OT_LAYOUT_GSUB_GSUB_HH

#অন্তর্ভাৱ "../../../hb-ot-layout-gsubgpos.hh"
#অন্তর্ভাৱ "Common.hh"
#অন্তর্ভাৱ "SubstLookup.hh"

নামাকাশ OT {

using Layout::GSUB_impl::SubstLookup;

নামাকাশ Layout {

/*
 * GSUB -- Glyph Substitution
 * https://docs.microsoft.com/en-us/typography/opentype/spec/gsub
 */

সংরচনা GSUB : GSUBGPOS
{
  using Lookup = SubstLookup;

  স্থির constexpr hb_tag_t tableTag = HB_OT_TAG_GSUB;

  ধ্রুৱ SubstLookup& get_lookup (unsigned পূর্ণাঙ্ক i) ধ্রুৱ
  { ঘূৰাই static_cast<ধ্রুৱ SubstLookup &> (GSUBGPOS::get_lookup (i)); }

  তার্কিক subset (hb_subset_context_t *c) ধ্রুৱ
  {
    hb_subset_layout_context_t l (c, tableTag);
    ঘূৰাই GSUBGPOS::subset<SubstLookup> (&l);
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (GSUBGPOS::sanitize<SubstLookup> (c));
  }

  HB_INTERNAL তার্কিক is_blocklisted (hb_blob_t *blob,
                                   hb_face_t *face) ধ্রুৱ;

  শূন্য closure_lookups (hb_face_t      *face,
                        ধ্রুৱ hb_set_t *glyphs,
                        hb_set_t       *lookup_indexes /* IN/OUT */) ধ্রুৱ
  { GSUBGPOS::closure_lookups<SubstLookup> (face, glyphs, lookup_indexes); }

  প্রকারনির্দেশ GSUBGPOS::accelerator_t<GSUB> accelerator_t;
};


}

সংরচনা GSUB_accelerator_t : Layout::GSUB::accelerator_t {
  GSUB_accelerator_t (hb_face_t *face) : Layout::GSUB::accelerator_t (face) {}
  HB_INTERNAL শূন্য depend (hb_depend_data_builder_t *builder, hb_face_t *face) ধ্রুৱ;
};


}

#endif  /* OT_LAYOUT_GSUB_GSUB_HH */
