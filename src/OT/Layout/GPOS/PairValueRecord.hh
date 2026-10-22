#که_نه_تعریف_شوی OT_LAYOUT_GPOS_PAIRVALUERECORD_HH
#تعریف_کړه OT_LAYOUT_GPOS_PAIRVALUERECORD_HH

#ضمن "ValueFormat.hh"

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {


بنية pair_ارزښت_ریکارډ_context_t
{
  ثابت ارزښت_بنسټ     *بنسټ;
  ثابت ارزښت_Format   *ارزښت_Formats;
  ثابت ارزښت_Format   *newFormats;
  بینښه            len1; /* valueFormats[0].get_len() */
  ثابت hb_map_t      *glyph_map;
  ثابت hb_hashmap_t<بینښه, hb_pair_t<بینښه, صحيح>> *layout_variation_idx_delta_map;

  فراغ copy_ارزښتونه (hb_serialize_context_t *s, ثابت ارزښت *values) ثابت
  {
    ارزښت_Formats[0].copy_ارزښتونه (s, newFormats[0], بنسټ, values,
                                layout_variation_idx_delta_map);
    ارزښت_Formats[1].copy_ارزښتونه (s, newFormats[1], بنسټ, values + len1,
                                layout_variation_idx_delta_map);
  }
};

کينډۍ <ډول_نوم Types>
بنية Pair_ارزښت_ریکارډ
{
  کينډۍ <ډول_نوم Types2>
  ملګری بنية Pair_سېټ;

  محمي:
  ډول_نوم Types::HBGlyphID
	        دوهم_Glyph;            /* GlyphID of second glyph in the
                                         * pair--first glyph is listed in the
                                         * Coverage table */
  ارزښت_ریکارډ   values;                 /* Positioning data for the first glyph
                                         * followed by for second glyph */
  عام:
  DEFINE_اندازه_ARRAY (Types::HBGlyphID::static_اندازه, values);

  صحيح cmp (hb_codepoint_t k) ثابت
  { أرجع دوهم_Glyph.cmp (k); }

  منطقي subset (hb_subset_context_t *c,
               pair_ارزښت_ریکارډ_context_t *تړون) ثابت
  {
    TRACE_SERIALIZE (هذا);
    خپلکاری *s = c->serializer;
    خپلکاری *بهر = s->پیل_embed (*هذا);
    إذا (unlikely (!s->extend_min (بهر))) return_trace (خطأ);

    بهر->دوهم_Glyph = (*تړون->glyph_map)[دوهم_Glyph];

    تړون->copy_ارزښتونه (s, values);

    return_trace (ریښتیا);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ثابت ارزښت_Format *ارزښت_Formats,
                                  ثابت ارزښت_بنسټ *بنسټ) ثابت
  {
    بینښه record1_len = ارزښت_Formats[0].get_len ();
    بینښه record2_len = ارزښت_Formats[1].get_len ();
    ثابت hb_array_t<ثابت ارزښت> ارزښتونه_array = values.as_array (record1_len + record2_len);

    إذا (ارزښت_Formats[0].has_device ())
      ارزښت_Formats[0].collect_variation_indices (c, بنسټ, ارزښتونه_array.sub_array (0, record1_len));

    إذا (ارزښت_Formats[1].has_device ())
      ارزښت_Formats[1].collect_variation_indices (c, بنسټ, ارزښتونه_array.sub_array (record1_len, record2_len));
  }

  منطقي intersects (ثابت hb_سېټ_t& glyphset) ثابت
  {
    أرجع glyphset.has(دوهم_Glyph);
  }

  ثابت ارزښت* get_ارزښتونه_1 () ثابت
  {
    أرجع &values[0];
  }

  ثابت ارزښت* get_ارزښتونه_2 (ارزښت_Format format1) ثابت
  {
    أرجع &values[format1.get_len ()];
  }
};


}
}
}

#د_که_پای  // OT_LAYOUT_GPOS_PAIRVALUERECORD_HH
