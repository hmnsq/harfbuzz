#ifndef OT_LAYOUT_GPOS_PAIRVALUERECORD_HH
#define OT_LAYOUT_GPOS_PAIRVALUERECORD_HH

#ଅନ୍ତର୍ଭାଵ "ValueFormat.hh"

ନାମାକାଶ OT {
ନାମାକାଶ Layout {
ନାମାକାଶ GPOS_impl {


ସଂରଚନା pair_ମୂଲ୍ୟ_record_context_t
{
  ଅଚଳ ମୂଲ୍ୟ_Base     *base;
  ଅଚଳ ମୂଲ୍ୟ_Format   *ମୂଲ୍ୟ_Formats;
  ଅଚଳ ମୂଲ୍ୟ_Format   *newFormats;
  unsigned            len1; /* valueFormats[0].get_len() */
  ଅଚଳ hb_map_t      *glyph_map;
  ଅଚଳ hb_hashmap_t<unsigned, hb_pair_t<unsigned, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>> *layout_variation_idx_delta_map;

  ଶୂନ୍ୟ copy_ମୂଲ୍ୟଗୁଡ଼ିକ (hb_serialize_context_t *s, ଅଚଳ ମୂଲ୍ୟ *values) ଅଚଳ
  {
    ମୂଲ୍ୟ_Formats[0].copy_ମୂଲ୍ୟଗୁଡ଼ିକ (s, newFormats[0], base, values,
                                layout_variation_idx_delta_map);
    ମୂଲ୍ୟ_Formats[1].copy_ମୂଲ୍ୟଗୁଡ଼ିକ (s, newFormats[1], base, values + len1,
                                layout_variation_idx_delta_map);
  }
};

template <typename Types>
ସଂରଚନା Pair_ମୂଲ୍ୟ_Record
{
  template <typename Types2>
  friend ସଂରଚନା PairSet;

  ସୁରକ୍ଷିତ:
  typename Types::HBGlyphID
	        secondGlyph;            /* GlyphID of second glyph in the
                                         * pair--first glyph is listed in the
                                         * Coverage table */
  ମୂଲ୍ୟ_Record   values;                 /* Positioning data for the first glyph
                                         * followed by for second glyph */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (Types::HBGlyphID::static_size, values);

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmp (hb_codepoint_t k) ଅଚଳ
  { ଫେରନ୍ତୁ secondGlyph.cmp (k); }

  ତାର୍କିକ subset (hb_subset_context_t *c,
               pair_ମୂଲ୍ୟ_record_context_t *closure) ଅଚଳ
  {
    TRACE_SERIALIZE (ଏହି);
    auto *s = c->serializer;
    auto *out = s->start_embed (*ଏହି);
    ଯଦି (unlikely (!s->extend_min (out))) return_trace (ମିଥ୍ୟା);

    out->secondGlyph = (*closure->glyph_map)[secondGlyph];

    closure->copy_ମୂଲ୍ୟଗୁଡ଼ିକ (s, values);

    return_trace (ସତ୍ୟ);
  }

  ଶୂନ୍ୟ collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ଅଚଳ ମୂଲ୍ୟ_Format *ମୂଲ୍ୟ_Formats,
                                  ଅଚଳ ମୂଲ୍ୟ_Base *base) ଅଚଳ
  {
    unsigned record1_len = ମୂଲ୍ୟ_Formats[0].get_len ();
    unsigned record2_len = ମୂଲ୍ୟ_Formats[1].get_len ();
    ଅଚଳ hb_array_t<ଅଚଳ ମୂଲ୍ୟ> ମୂଲ୍ୟଗୁଡ଼ିକ_array = values.as_array (record1_len + record2_len);

    ଯଦି (ମୂଲ୍ୟ_Formats[0].has_device ())
      ମୂଲ୍ୟ_Formats[0].collect_variation_indices (c, base, ମୂଲ୍ୟଗୁଡ଼ିକ_array.sub_array (0, record1_len));

    ଯଦି (ମୂଲ୍ୟ_Formats[1].has_device ())
      ମୂଲ୍ୟ_Formats[1].collect_variation_indices (c, base, ମୂଲ୍ୟଗୁଡ଼ିକ_array.sub_array (record1_len, record2_len));
  }

  ତାର୍କିକ intersects (ଅଚଳ hb_set_t& glyphset) ଅଚଳ
  {
    ଫେରନ୍ତୁ glyphset.has(secondGlyph);
  }

  ଅଚଳ ମୂଲ୍ୟ* get_ମୂଲ୍ୟଗୁଡ଼ିକ_1 () ଅଚଳ
  {
    ଫେରନ୍ତୁ &values[0];
  }

  ଅଚଳ ମୂଲ୍ୟ* get_ମୂଲ୍ୟଗୁଡ଼ିକ_2 (ମୂଲ୍ୟ_Format format1) ଅଚଳ
  {
    ଫେରନ୍ତୁ &values[format1.get_len ()];
  }
};


}
}
}

#endif  // OT_LAYOUT_GPOS_PAIRVALUERECORD_HH
