#ifndef OT_LAYOUT_GPOS_PAIRVALUERECORD_HH
#define OT_LAYOUT_GPOS_PAIRVALUERECORD_HH

#अन्तर्भाव "ValueFormat.hh"

नामाकाश OT {
नामाकाश Layout {
नामाकाश GPOS_impl {


संरचना pair_मानम्_record_context_t
{
  ध्रुव मानम्_Base     *base;
  ध्रुव मानम्_Format   *मानम्_Formats;
  ध्रुव मानम्_Format   *newFormats;
  unsigned            len1; /* valueFormats[0].get_len() */
  ध्रुव hb_map_t      *glyph_map;
  ध्रुव hb_hashmap_t<unsigned, hb_pair_t<unsigned, पूर्णाङ्क>> *layout_variation_idx_delta_map;

  शून्य copy_मानानि (hb_serialize_context_t *s, ध्रुव मानम् *values) ध्रुव
  {
    मानम्_Formats[0].copy_मानानि (s, newFormats[0], base, values,
                                layout_variation_idx_delta_map);
    मानम्_Formats[1].copy_मानानि (s, newFormats[1], base, values + len1,
                                layout_variation_idx_delta_map);
  }
};

template <typename Types>
संरचना Pair_मानम्_Record
{
  template <typename Types2>
  friend संरचना PairSet;

  रक्षित:
  typename Types::HBGlyphID
	        secondGlyph;            /* GlyphID of second glyph in the
                                         * pair--first glyph is listed in the
                                         * Coverage table */
  मानम्_Record   values;                 /* Positioning data for the first glyph
                                         * followed by for second glyph */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (Types::HBGlyphID::static_size, values);

  पूर्णाङ्क cmp (hb_codepoint_t k) ध्रुव
  { पटिनिवत्त secondGlyph.cmp (k); }

  तार्किक subset (hb_subset_context_t *c,
               pair_मानम्_record_context_t *closure) ध्रुव
  {
    TRACE_SERIALIZE (एतत्);
    auto *s = c->serializer;
    auto *out = s->start_embed (*एतत्);
    सचे (unlikely (!s->extend_min (out))) return_trace (मिच्छा);

    out->secondGlyph = (*closure->glyph_map)[secondGlyph];

    closure->copy_मानानि (s, values);

    return_trace (सच्च);
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ध्रुव मानम्_Format *मानम्_Formats,
                                  ध्रुव मानम्_Base *base) ध्रुव
  {
    unsigned record1_len = मानम्_Formats[0].get_len ();
    unsigned record2_len = मानम्_Formats[1].get_len ();
    ध्रुव hb_array_t<ध्रुव मानम्> मानानि_array = values.as_array (record1_len + record2_len);

    सचे (मानम्_Formats[0].has_device ())
      मानम्_Formats[0].collect_variation_indices (c, base, मानानि_array.sub_array (0, record1_len));

    सचे (मानम्_Formats[1].has_device ())
      मानम्_Formats[1].collect_variation_indices (c, base, मानानि_array.sub_array (record1_len, record2_len));
  }

  तार्किक intersects (ध्रुव hb_set_t& glyphset) ध्रुव
  {
    पटिनिवत्त glyphset.has(secondGlyph);
  }

  ध्रुव मानम्* get_मानानि_1 () ध्रुव
  {
    पटिनिवत्त &values[0];
  }

  ध्रुव मानम्* get_मानानि_2 (मानम्_Format format1) ध्रुव
  {
    पटिनिवत्त &values[format1.get_len ()];
  }
};


}
}
}

#endif  // OT_LAYOUT_GPOS_PAIRVALUERECORD_HH
