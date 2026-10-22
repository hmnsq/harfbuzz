#ifndef OT_LAYOUT_GSUB_GSUB_HH
#define OT_LAYOUT_GSUB_GSUB_HH

#അന്തര്ഭാവ "../../../hb-ot-layout-gsubgpos.hh"
#അന്തര്ഭാവ "Common.hh"
#അന്തര്ഭാവ "SubstLookup.hh"

നാമാകാശ OT {

using Layout::GSUB_impl::SubstLookup;

നാമാകാശ Layout {

/*
 * GSUB -- Glyph Substitution
 * https://docs.microsoft.com/en-us/typography/opentype/spec/gsub
 */

ഘടന GSUB : GSUBGPOS
{
  using Lookup = SubstLookup;

  സ്ഥിതം constexpr hb_tag_t tableTag = HB_OT_TAG_GSUB;

  സ്ഥിരം SubstLookup& get_lookup (unsigned പൂർണ്ണസംഖ്യ i) സ്ഥിരം
  { തിരികെ static_cast<സ്ഥിരം SubstLookup &> (GSUBGPOS::get_lookup (i)); }

  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  {
    hb_subset_layout_context_t l (c, tableTag);
    തിരികെ GSUBGPOS::subset<SubstLookup> (&l);
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (GSUBGPOS::sanitize<SubstLookup> (c));
  }

  HB_INTERNAL തർക്കം is_blocklisted (hb_blob_t *blob,
                                   hb_face_t *face) സ്ഥിരം;

  ശൂന്യം closure_lookups (hb_face_t      *face,
                        സ്ഥിരം hb_set_t *glyphs,
                        hb_set_t       *lookup_indexes /* IN/OUT */) സ്ഥിരം
  { GSUBGPOS::closure_lookups<SubstLookup> (face, glyphs, lookup_indexes); }

  തരനിർവചനം GSUBGPOS::accelerator_t<GSUB> accelerator_t;
};


}

ഘടന GSUB_accelerator_t : Layout::GSUB::accelerator_t {
  GSUB_accelerator_t (hb_face_t *face) : Layout::GSUB::accelerator_t (face) {}
  HB_INTERNAL ശൂന്യം depend (hb_depend_data_builder_t *builder, hb_face_t *face) സ്ഥിരം;
};


}

#endif  /* OT_LAYOUT_GSUB_GSUB_HH */
