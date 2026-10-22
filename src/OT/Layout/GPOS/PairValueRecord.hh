#ifndef OT_LAYOUT_GPOS_PAIRVALUERECORD_HH
#define OT_LAYOUT_GPOS_PAIRVALUERECORD_HH

#अन्तर्भाव "ValueFormat.hh"

नामाकाश OT {
नामाकाश Layout {
नामाकाश GPOS_impl {


संरचना pair_मूल्य_record_context_t
{
  ध्रुव मूल्य_Base     *base;
  ध्रुव मूल्य_Format   *मूल्य_Formats;
  ध्रुव मूल्य_Format   *newFormats;
  unsigned            len1; /* valueFormats[0].get_len() */
  ध्रुव hb_map_t      *glyph_map;
  ध्रुव hb_hashmap_t<unsigned, hb_pair_t<unsigned, पूर्णाङ्क>> *layout_variation_idx_delta_map;

  शून्य copy_मूल्यावली (hb_serialize_context_t *s, ध्रुव मूल्य *values) ध्रुव
  {
    मूल्य_Formats[0].copy_मूल्यावली (s, newFormats[0], base, values,
                                layout_variation_idx_delta_map);
    मूल्य_Formats[1].copy_मूल्यावली (s, newFormats[1], base, values + len1,
                                layout_variation_idx_delta_map);
  }
};

template <typename Types>
संरचना Pair_मूल्य_Record
{
  template <typename Types2>
  friend संरचना PairSet;

  रक्षित:
  typename Types::HBGlyphID
	        secondGlyph;            /* GlyphID of second glyph in the
                                         * pair--first glyph is listed in the
                                         * Coverage table */
  मूल्य_Record   values;                 /* Positioning data for the first glyph
                                         * followed by for second glyph */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (Types::HBGlyphID::static_size, values);

  पूर्णाङ्क cmp (hb_codepoint_t k) ध्रुव
  { प्रत्यागम secondGlyph.cmp (k); }

  तार्किक subset (hb_subset_context_t *c,
               pair_मूल्य_record_context_t *closure) ध्रुव
  {
    TRACE_SERIALIZE (एतत्);
    auto *s = c->serializer;
    auto *out = s->start_embed (*एतत्);
    यदि (unlikely (!s->extend_min (out))) return_trace (असत्य);

    out->secondGlyph = (*closure->glyph_map)[secondGlyph];

    closure->copy_मूल्यावली (s, values);

    return_trace (सत्य);
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ध्रुव मूल्य_Format *मूल्य_Formats,
                                  ध्रुव मूल्य_Base *base) ध्रुव
  {
    unsigned record1_len = मूल्य_Formats[0].get_len ();
    unsigned record2_len = मूल्य_Formats[1].get_len ();
    ध्रुव hb_array_t<ध्रुव मूल्य> मूल्यावली_array = values.as_array (record1_len + record2_len);

    यदि (मूल्य_Formats[0].has_device ())
      मूल्य_Formats[0].collect_variation_indices (c, base, मूल्यावली_array.sub_array (0, record1_len));

    यदि (मूल्य_Formats[1].has_device ())
      मूल्य_Formats[1].collect_variation_indices (c, base, मूल्यावली_array.sub_array (record1_len, record2_len));
  }

  तार्किक intersects (ध्रुव hb_set_t& glyphset) ध्रुव
  {
    प्रत्यागम glyphset.has(secondGlyph);
  }

  ध्रुव मूल्य* get_मूल्यावली_1 () ध्रुव
  {
    प्रत्यागम &values[0];
  }

  ध्रुव मूल्य* get_मूल्यावली_2 (मूल्य_Format format1) ध्रुव
  {
    प्रत्यागम &values[format1.get_len ()];
  }
};


}
}
}

#endif  // OT_LAYOUT_GPOS_PAIRVALUERECORD_HH
