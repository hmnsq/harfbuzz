#اگر_غیر_تعریف_شدہ OT_LAYOUT_GPOS_PAIRVALUERECORD_HH
#تعریف_کرو OT_LAYOUT_GPOS_PAIRVALUERECORD_HH

#شامل "ValueFormat.hh"

نامخلا OT {
نامخلا Layout {
نامخلا GPOS_impl {


ساخت pair_قدر_ریکارڈ_context_t
{
  ثابت قدر_بنیاد     *بنیاد;
  ثابت قدر_Format   *قدر_Formats;
  ثابت قدر_Format   *newFormats;
  غیرعلامتی            len1; /* valueFormats[0].get_len() */
  ثابت hb_map_t      *glyph_map;
  ثابت hb_hashmap_t<غیرعلامتی, hb_pair_t<غیرعلامتی, صحیح>> *layout_variation_idx_delta_map;

  خالی copy_اقدار (hb_serialize_context_t *s, ثابت قدر *values) ثابت
  {
    قدر_Formats[0].copy_اقدار (s, newFormats[0], بنیاد, values,
                                layout_variation_idx_delta_map);
    قدر_Formats[1].copy_اقدار (s, newFormats[1], بنیاد, values + len1,
                                layout_variation_idx_delta_map);
  }
};

سانچہ <قسم_نام Types>
ساخت Pair_قدر_ریکارڈ
{
  سانچہ <قسم_نام Types2>
  دوست ساخت Pair_سیٹ;

  محفوظ:
  قسم_نام Types::HBGlyphID
	        دوسرا_Glyph;            /* GlyphID of second glyph in the
                                         * pair--first glyph is listed in the
                                         * Coverage table */
  قدر_ریکارڈ   values;                 /* Positioning data for the first glyph
                                         * followed by for second glyph */
  عوامی:
  DEFINE_سائز_ARRAY (Types::HBGlyphID::static_سائز, values);

  صحیح cmp (hb_codepoint_t k) ثابت
  { واپس دوسرا_Glyph.cmp (k); }

  منطقی subset (hb_subset_context_t *c,
               pair_قدر_ریکارڈ_context_t *احاطہ) ثابت
  {
    TRACE_SERIALIZE (یہ);
    خودکار *s = c->serializer;
    خودکار *برآمد = s->آغاز_embed (*یہ);
    اگر (unlikely (!s->extend_min (برآمد))) return_trace (جھوٹ);

    برآمد->دوسرا_Glyph = (*احاطہ->glyph_map)[دوسرا_Glyph];

    احاطہ->copy_اقدار (s, values);

    return_trace (سچ);
  }

  خالی collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ثابت قدر_Format *قدر_Formats,
                                  ثابت قدر_بنیاد *بنیاد) ثابت
  {
    غیرعلامتی record1_len = قدر_Formats[0].get_len ();
    غیرعلامتی record2_len = قدر_Formats[1].get_len ();
    ثابت hb_array_t<ثابت قدر> اقدار_array = values.as_array (record1_len + record2_len);

    اگر (قدر_Formats[0].has_device ())
      قدر_Formats[0].collect_variation_indices (c, بنیاد, اقدار_array.sub_array (0, record1_len));

    اگر (قدر_Formats[1].has_device ())
      قدر_Formats[1].collect_variation_indices (c, بنیاد, اقدار_array.sub_array (record1_len, record2_len));
  }

  منطقی intersects (ثابت hb_سیٹ_t& glyphset) ثابت
  {
    واپس glyphset.has(دوسرا_Glyph);
  }

  ثابت قدر* get_اقدار_1 () ثابت
  {
    واپس &values[0];
  }

  ثابت قدر* get_اقدار_2 (قدر_Format format1) ثابت
  {
    واپس &values[format1.get_len ()];
  }
};


}
}
}

#اگر_ختم  // OT_LAYOUT_GPOS_PAIRVALUERECORD_HH
