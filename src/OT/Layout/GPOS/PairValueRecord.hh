#ifndef OT_LAYOUT_GPOS_PAIRVALUERECORD_HH
#define OT_LAYOUT_GPOS_PAIRVALUERECORD_HH

#ضمن "ValueFormat.hh"

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {


بنية pair_قدر_record_context_t
{
  ثابت قدر_Base     *base;
  ثابت قدر_Format   *قدر_Formats;
  ثابت قدر_Format   *newFormats;
  unsigned            len1; /* valueFormats[0].get_len() */
  ثابت hb_map_t      *glyph_map;
  ثابت hb_hashmap_t<unsigned, hb_pair_t<unsigned, صحيح>> *layout_variation_idx_delta_map;

  فراغ copy_اقدار (hb_serialize_context_t *s, ثابت قدر *values) ثابت
  {
    قدر_Formats[0].copy_اقدار (s, newFormats[0], base, values,
                                layout_variation_idx_delta_map);
    قدر_Formats[1].copy_اقدار (s, newFormats[1], base, values + len1,
                                layout_variation_idx_delta_map);
  }
};

template <typename Types>
بنية Pair_قدر_Record
{
  template <typename Types2>
  friend بنية PairSet;

  محمي:
  typename Types::HBGlyphID
	        secondGlyph;            /* GlyphID of second glyph in the
                                         * pair--first glyph is listed in the
                                         * Coverage table */
  قدر_Record   values;                 /* Positioning data for the first glyph
                                         * followed by for second glyph */
  عام:
  DEFINE_SIZE_ARRAY (Types::HBGlyphID::static_size, values);

  صحيح cmp (hb_codepoint_t k) ثابت
  { أرجع secondGlyph.cmp (k); }

  منطقي subset (hb_subset_context_t *c,
               pair_قدر_record_context_t *closure) ثابت
  {
    TRACE_SERIALIZE (هذا);
    auto *s = c->serializer;
    auto *out = s->start_embed (*هذا);
    إذا (unlikely (!s->extend_min (out))) return_trace (خطأ);

    out->secondGlyph = (*closure->glyph_map)[secondGlyph];

    closure->copy_اقدار (s, values);

    return_trace (true);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ثابت قدر_Format *قدر_Formats,
                                  ثابت قدر_Base *base) ثابت
  {
    unsigned record1_len = قدر_Formats[0].get_len ();
    unsigned record2_len = قدر_Formats[1].get_len ();
    ثابت hb_array_t<ثابت قدر> اقدار_array = values.as_array (record1_len + record2_len);

    إذا (قدر_Formats[0].has_device ())
      قدر_Formats[0].collect_variation_indices (c, base, اقدار_array.sub_array (0, record1_len));

    إذا (قدر_Formats[1].has_device ())
      قدر_Formats[1].collect_variation_indices (c, base, اقدار_array.sub_array (record1_len, record2_len));
  }

  منطقي intersects (ثابت hb_set_t& glyphset) ثابت
  {
    أرجع glyphset.has(secondGlyph);
  }

  ثابت قدر* get_اقدار_1 () ثابت
  {
    أرجع &values[0];
  }

  ثابت قدر* get_اقدار_2 (قدر_Format format1) ثابت
  {
    أرجع &values[format1.get_len ()];
  }
};


}
}
}

#endif  // OT_LAYOUT_GPOS_PAIRVALUERECORD_HH
