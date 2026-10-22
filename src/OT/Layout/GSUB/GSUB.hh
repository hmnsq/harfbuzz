#अगर_अघोषित OT_LAYOUT_GSUB_GSUB_HH
#घोषणा OT_LAYOUT_GSUB_GSUB_HH

#समावेश "../../../hb-ot-layout-gsubgpos.hh"
#समावेश "Common.hh"
#समावेश "SubstLookup.hh"

namespace OT {

using Layout::GSUB_impl::SubstLookup;

namespace Layout {

/*
 * GSUB -- Glyph Substitution
 * https://docs.microsoft.com/en-us/typography/opentype/spec/gsub
 */

काष्ठा GSUB : GSUBGPOS
{
  using Lookup = SubstLookup;

  अटल constexpr hb_tag_t tableTag = HB_OT_TAG_GSUB;

  स्थिर SubstLookup& get_lookup (अचिन्हित पूर्णांक i) स्थिर
  { वापस static_cast<स्थिर SubstLookup &> (GSUBGPOS::get_lookup (i)); }

  bool subset (hb_subset_context_t *c) स्थिर
  {
    hb_subset_layout_context_t l (c, tableTag);
    वापस GSUBGPOS::subset<SubstLookup> (&l);
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (GSUBGPOS::sanitize<SubstLookup> (c));
  }

  HB_INTERNAL bool is_blocklisted (hb_blob_t *blob,
                                   hb_face_t *face) स्थिर;

  व्योम closure_lookups (hb_face_t      *face,
                        स्थिर hb_set_t *glyphs,
                        hb_set_t       *lookup_indexes /* IN/OUT */) स्थिर
  { GSUBGPOS::closure_lookups<SubstLookup> (face, glyphs, lookup_indexes); }

  प्रकार GSUBGPOS::accelerator_t<GSUB> accelerator_t;
};


}

काष्ठा GSUB_accelerator_t : Layout::GSUB::accelerator_t {
  GSUB_accelerator_t (hb_face_t *face) : Layout::GSUB::accelerator_t (face) {}
  HB_INTERNAL व्योम depend (hb_depend_तथ्य_builder_t *builder, hb_face_t *face) स्थिर;
};


}

#पूर्ण_अगर  /* OT_LAYOUT_GSUB_GSUB_HH */
