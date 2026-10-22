#ifndef OT_LAYOUT_GSUB_GSUB_HH
#define OT_LAYOUT_GSUB_GSUB_HH

#অন্তর্ভাব "../../../hb-ot-layout-gsubgpos.hh"
#অন্তর্ভাব "Common.hh"
#অন্তর্ভাব "SubstLookup.hh"

নামাকাশ OT {

using Layout::GSUB_impl::SubstLookup;

নামাকাশ Layout {

/*
 * GSUB -- Glyph Substitution
 * https://docs.microsoft.com/en-us/typography/opentype/spec/gsub
 */

কাঠামো GSUB : GSUBGPOS
{
  using Lookup = SubstLookup;

  স্থির constexpr hb_tag_t tableTag = HB_OT_TAG_GSUB;

  ধ্রুবক SubstLookup& get_lookup (unsigned পূর্ণসংখ্যা i) ধ্রুবক
  { ফেরত static_cast<ধ্রুবক SubstLookup &> (GSUBGPOS::get_lookup (i)); }

  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  {
    hb_subset_layout_context_t l (c, tableTag);
    ফেরত GSUBGPOS::subset<SubstLookup> (&l);
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (GSUBGPOS::sanitize<SubstLookup> (c));
  }

  HB_INTERNAL যৌক্তিক is_blocklisted (hb_blob_t *blob,
                                   hb_face_t *face) ধ্রুবক;

  শূন্য closure_lookups (hb_face_t      *face,
                        ধ্রুবক hb_set_t *glyphs,
                        hb_set_t       *lookup_indexes /* IN/OUT */) ধ্রুবক
  { GSUBGPOS::closure_lookups<SubstLookup> (face, glyphs, lookup_indexes); }

  প্রকারলেখ GSUBGPOS::accelerator_t<GSUB> accelerator_t;
};


}

কাঠামো GSUB_accelerator_t : Layout::GSUB::accelerator_t {
  GSUB_accelerator_t (hb_face_t *face) : Layout::GSUB::accelerator_t (face) {}
  HB_INTERNAL শূন্য depend (hb_depend_তথ্য_builder_t *builder, hb_face_t *face) ধ্রুবক;
};


}

#endif  /* OT_LAYOUT_GSUB_GSUB_HH */
