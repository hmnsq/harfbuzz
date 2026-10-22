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

  स्थिरांक SubstLookup& get_lookup (अचिन्हित पूर्णांक i) स्थिरांक
  { परत static_cast<स्थिरांक SubstLookup &> (GSUBGPOS::get_lookup (i)); }

  तार्किक subset (hb_subset_context_t *c) स्थिरांक
  {
    hb_subset_layout_context_t l (c, tableTag);
    परत GSUBGPOS::subset<SubstLookup> (&l);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (GSUBGPOS::sanitize<SubstLookup> (c));
  }

  HB_INTERNAL तार्किक is_blocklisted (hb_blob_t *blob,
                                   hb_face_t *face) स्थिरांक;

  शून्य closure_lookups (hb_face_t      *face,
                        स्थिरांक hb_set_t *glyphs,
                        hb_set_t       *lookup_indexes /* IN/OUT */) स्थिरांक
  { GSUBGPOS::closure_lookups<SubstLookup> (face, glyphs, lookup_indexes); }

  प्रकारव्याख्या GSUBGPOS::accelerator_t<GSUB> accelerator_t;
};


}

संरचना GSUB_accelerator_t : Layout::GSUB::accelerator_t {
  GSUB_accelerator_t (hb_face_t *face) : Layout::GSUB::accelerator_t (face) {}
  HB_INTERNAL शून्य depend (hb_depend_data_builder_t *builder, hb_face_t *face) स्थिरांक;
};


}

#endif  /* OT_LAYOUT_GSUB_GSUB_HH */
