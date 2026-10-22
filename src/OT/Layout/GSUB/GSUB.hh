#ifndef OT_LAYOUT_GSUB_GSUB_HH
#define OT_LAYOUT_GSUB_GSUB_HH

#अन्तर्भाव "../../../hb-ot-layout-gsubgpos.hh"
#अन्तर्भाव "Common.hh"
#अन्तर्भाव "SubstLookup.hh"

नामाकाश OT {

using Layout::GSUB_impl::SubstLookup;

नामाकाश Layout {

/*
 * GSUB -- Glyph Substitution
 * https://docs.microsoft.com/en-us/typography/opentype/spec/gsub
 */

संरचना GSUB : GSUBGPOS
{
  using Lookup = SubstLookup;

  स्थिर constexpr hb_tag_t tableTag = HB_OT_TAG_GSUB;

  अचर SubstLookup& get_lookup (अचिन्हित पूर्णाङ्क i) अचर
  { फर्काऊ static_cast<अचर SubstLookup &> (GSUBGPOS::get_lookup (i)); }

  तार्किक subset (hb_subset_context_t *c) अचर
  {
    hb_subset_layout_context_t l (c, tableTag);
    फर्काऊ GSUBGPOS::subset<SubstLookup> (&l);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (GSUBGPOS::sanitize<SubstLookup> (c));
  }

  HB_INTERNAL तार्किक is_blocklisted (hb_blob_t *blob,
                                   hb_face_t *face) अचर;

  शून्य closure_lookups (hb_face_t      *face,
                        अचर hb_set_t *glyphs,
                        hb_set_t       *lookup_indexes /* IN/OUT */) अचर
  { GSUBGPOS::closure_lookups<SubstLookup> (face, glyphs, lookup_indexes); }

  प्रकारनिर्देश GSUBGPOS::accelerator_t<GSUB> accelerator_t;
};


}

संरचना GSUB_accelerator_t : Layout::GSUB::accelerator_t {
  GSUB_accelerator_t (hb_face_t *face) : Layout::GSUB::accelerator_t (face) {}
  HB_INTERNAL शून्य depend (hb_depend_data_builder_t *builder, hb_face_t *face) अचर;
};


}

#endif  /* OT_LAYOUT_GSUB_GSUB_HH */
