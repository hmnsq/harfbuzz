#ifndef OT_LAYOUT_GPOS_PAIRVALUERECORD_HH
#define OT_LAYOUT_GPOS_PAIRVALUERECORD_HH

#شامل "ValueFormat.hh"

فضاینام OT {
فضاینام Layout {
فضاینام GPOS_impl {


ساختار pair_مقدار_record_context_t
{
  ثابت مقدار_Base     *base;
  ثابت مقدار_Format   *مقدار_Formats;
  ثابت مقدار_Format   *newFormats;
  unsigned            len1; /* valueFormats[0].get_len() */
  ثابت hb_map_t      *glyph_map;
  ثابت hb_hashmap_t<unsigned, hb_pair_t<unsigned, صحیح>> *layout_variation_idx_delta_map;

  تهی copy_مقادیر (hb_serialize_context_t *s, ثابت مقدار *values) ثابت
  {
    مقدار_Formats[0].copy_مقادیر (s, newFormats[0], base, values,
                                layout_variation_idx_delta_map);
    مقدار_Formats[1].copy_مقادیر (s, newFormats[1], base, values + len1,
                                layout_variation_idx_delta_map);
  }
};

template <typename Types>
ساختار Pair_مقدار_Record
{
  template <typename Types2>
  friend ساختار PairSet;

  محافظتشده:
  typename Types::HBGlyphID
	        secondGlyph;            /* GlyphID of second glyph in the
                                         * pair--first glyph is listed in the
                                         * Coverage table */
  مقدار_Record   values;                 /* Positioning data for the first glyph
                                         * followed by for second glyph */
  عمومی:
  DEFINE_SIZE_ARRAY (Types::HBGlyphID::static_size, values);

  صحیح cmp (hb_codepoint_t k) ثابت
  { بازگشت secondGlyph.cmp (k); }

  منطقی subset (hb_subset_context_t *c,
               pair_مقدار_record_context_t *closure) ثابت
  {
    TRACE_SERIALIZE (این);
    auto *s = c->serializer;
    auto *out = s->start_embed (*این);
    اگر (unlikely (!s->extend_min (out))) return_trace (نادرست);

    out->secondGlyph = (*closure->glyph_map)[secondGlyph];

    closure->copy_مقادیر (s, values);

    return_trace (درست);
  }

  تهی collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ثابت مقدار_Format *مقدار_Formats,
                                  ثابت مقدار_Base *base) ثابت
  {
    unsigned record1_len = مقدار_Formats[0].get_len ();
    unsigned record2_len = مقدار_Formats[1].get_len ();
    ثابت hb_array_t<ثابت مقدار> مقادیر_array = values.as_array (record1_len + record2_len);

    اگر (مقدار_Formats[0].has_device ())
      مقدار_Formats[0].collect_variation_indices (c, base, مقادیر_array.sub_array (0, record1_len));

    اگر (مقدار_Formats[1].has_device ())
      مقدار_Formats[1].collect_variation_indices (c, base, مقادیر_array.sub_array (record1_len, record2_len));
  }

  منطقی intersects (ثابت hb_set_t& glyphset) ثابت
  {
    بازگشت glyphset.has(secondGlyph);
  }

  ثابت مقدار* get_مقادیر_1 () ثابت
  {
    بازگشت &values[0];
  }

  ثابت مقدار* get_مقادیر_2 (مقدار_Format format1) ثابت
  {
    بازگشت &values[format1.get_len ()];
  }
};


}
}
}

#endif  // OT_LAYOUT_GPOS_PAIRVALUERECORD_HH
