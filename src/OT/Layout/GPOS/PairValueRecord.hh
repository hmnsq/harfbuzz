#جے_بنا_تعریف OT_LAYOUT_GPOS_PAIRVALUERECORD_HH
#تعریف_کرو OT_LAYOUT_GPOS_PAIRVALUERECORD_HH

#ضمن "ValueFormat.hh"

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {


بنية pair_قدر_ریکارڈ_context_t
{
  ثابت قدر_ادھار     *ادھار;
  ثابت قدر_Format   *قدر_Formats;
  ثابت قدر_Format   *newFormats;
  بنانشانی            len1; /* valueFormats[0].get_len() */
  ثابت hb_map_t      *glyph_map;
  ثابت hb_hashmap_t<بنانشانی, hb_pair_t<بنانشانی, صحيح>> *layout_variation_idx_delta_map;

  فراغ copy_اقدار (hb_serialize_context_t *s, ثابت قدر *values) ثابت
  {
    قدر_Formats[0].copy_اقدار (s, newFormats[0], ادھار, values,
                                layout_variation_idx_delta_map);
    قدر_Formats[1].copy_اقدار (s, newFormats[1], ادھار, values + len1,
                                layout_variation_idx_delta_map);
  }
};

سانچا <قسم_ناں Types>
بنية Pair_قدر_ریکارڈ
{
  سانچا <قسم_ناں Types2>
  یار بنية Pair_سیٹ;

  محمي:
  قسم_ناں Types::HBGlyphID
	        دوجا_Glyph;            /* GlyphID of second glyph in the
                                         * pair--first glyph is listed in the
                                         * Coverage table */
  قدر_ریکارڈ   values;                 /* Positioning data for the first glyph
                                         * followed by for second glyph */
  عام:
  DEFINE_ناپ_ARRAY (Types::HBGlyphID::static_ناپ, values);

  صحيح cmp (hb_codepoint_t k) ثابت
  { أرجع دوجا_Glyph.cmp (k); }

  منطقي subset (hb_subset_context_t *c,
               pair_قدر_ریکارڈ_context_t *گھیرا) ثابت
  {
    TRACE_SERIALIZE (هذا);
    خودکار *s = c->serializer;
    خودکار *باہرلا = s->شروع_embed (*هذا);
    إذا (unlikely (!s->extend_min (باہرلا))) return_trace (خطأ);

    باہرلا->دوجا_Glyph = (*گھیرا->glyph_map)[دوجا_Glyph];

    گھیرا->copy_اقدار (s, values);

    return_trace (سچ);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ثابت قدر_Format *قدر_Formats,
                                  ثابت قدر_ادھار *ادھار) ثابت
  {
    بنانشانی record1_len = قدر_Formats[0].get_len ();
    بنانشانی record2_len = قدر_Formats[1].get_len ();
    ثابت hb_array_t<ثابت قدر> اقدار_array = values.as_array (record1_len + record2_len);

    إذا (قدر_Formats[0].has_device ())
      قدر_Formats[0].collect_variation_indices (c, ادھار, اقدار_array.sub_array (0, record1_len));

    إذا (قدر_Formats[1].has_device ())
      قدر_Formats[1].collect_variation_indices (c, ادھار, اقدار_array.sub_array (record1_len, record2_len));
  }

  منطقي intersects (ثابت hb_سیٹ_t& glyphset) ثابت
  {
    أرجع glyphset.has(دوجا_Glyph);
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

#جے_انت  // OT_LAYOUT_GPOS_PAIRVALUERECORD_HH
