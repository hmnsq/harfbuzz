#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OT_LAYOUT_GSUB_GSUB_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_LAYOUT_GSUB_GSUB_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "../../../hb-ot-layout-gsubgpos.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "Common.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "SubstLookup.hh"

ਨਾਮਾਕਾਸ਼ OT {

ਵਰਤੋਂ Layout::GSUB_impl::SubstLookup;

ਨਾਮਾਕਾਸ਼ Layout {

/*
 * GSUB -- Glyph Substitution
 * https://docs.microsoft.com/en-us/typography/opentype/spec/gsub
 */

ਬਣਤਰ GSUB : GSUBGPOS
{
  ਵਰਤੋਂ Lookup = SubstLookup;

  ਸਥਿਰ constexpr hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ = HB_OT_ਟੈਗ_GSUB;

  ਅਚਲ SubstLookup& get_lookup (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i) ਅਚਲ
  { ਵਾਪਸ static_cast<ਅਚਲ SubstLookup &> (GSUBGPOS::get_lookup (i)); }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    hb_subset_layout_context_t l (c, ਸਾਰਣੀ_ਟੈਗ);
    ਵਾਪਸ GSUBGPOS::subset<SubstLookup> (&l);
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (GSUBGPOS::sanitize<SubstLookup> (c));
  }

  HB_ਅੰਦਰੂਨੀ ਤਾਰਕਿਕ is_blocklisted (hb_blob_t *blob,
                                   hb_face_t *face) ਅਚਲ;

  ਖਾਲੀ ਘੇਰਾ_lookups (hb_face_t      *face,
                        ਅਚਲ hb_ਸੈੱਟ_t *glyphs,
                        hb_ਸੈੱਟ_t       *lookup_indexes /* IN/OUT */) ਅਚਲ
  { GSUBGPOS::ਘੇਰਾ_lookups<SubstLookup> (face, glyphs, lookup_indexes); }

  ਕਿਸਮਪਰਿਭਾਸ਼ਾ GSUBGPOS::accelerator_t<GSUB> accelerator_t;
};


}

ਬਣਤਰ GSUB_accelerator_t : Layout::GSUB::accelerator_t {
  GSUB_accelerator_t (hb_face_t *face) : Layout::GSUB::accelerator_t (face) {}
  HB_ਅੰਦਰੂਨੀ ਖਾਲੀ depend (hb_depend_ਅੰਕੜੇ_builder_t *builder, hb_face_t *face) ਅਚਲ;
};


}

#ਜੇ_ਅੰਤ  /* OT_LAYOUT_GSUB_GSUB_HH */
