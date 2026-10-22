#اگر_تعریف_نشده OT_LAYOUT_GPOS_PAIRVALUERECORD_HH
#تعریف_کن OT_LAYOUT_GPOS_PAIRVALUERECORD_HH

#ضمن "ValueFormat.hh"

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {


بنية pair_مقدار_رکورد_context_t
{
  ثابت مقدار_پایه     *پایه;
  ثابت مقدار_Format   *مقدار_Formats;
  ثابت مقدار_Format   *newFormats;
  بیعلامت            len1; /* valueFormats[0].get_len() */
  ثابت hb_map_t      *glyph_map;
  ثابت hb_hashmap_t<بیعلامت, hb_pair_t<بیعلامت, صحيح>> *layout_variation_idx_delta_map;

  فراغ copy_مقادیر (hb_serialize_context_t *s, ثابت مقدار *values) ثابت
  {
    مقدار_Formats[0].copy_مقادیر (s, newFormats[0], پایه, values,
                                layout_variation_idx_delta_map);
    مقدار_Formats[1].copy_مقادیر (s, newFormats[1], پایه, values + len1,
                                layout_variation_idx_delta_map);
  }
};

قالب <نام_نوع Types>
بنية Pair_مقدار_رکورد
{
  قالب <نام_نوع Types2>
  دوست بنية Pair_مجموعه;

  محمي:
  نام_نوع Types::HBGlyphID
	        دومی_Glyph;            /* GlyphID of second glyph in the
                                         * pair--first glyph is listed in the
                                         * Coverage table */
  مقدار_رکورد   values;                 /* Positioning data for the first glyph
                                         * followed by for second glyph */
  عام:
  DEFINE_اندازه_ARRAY (Types::HBGlyphID::static_اندازه, values);

  صحيح cmp (hb_codepoint_t k) ثابت
  { أرجع دومی_Glyph.cmp (k); }

  منطقي subset (hb_subset_context_t *c,
               pair_مقدار_رکورد_context_t *بستگی) ثابت
  {
    TRACE_SERIALIZE (هذا);
    خودکار *s = c->serializer;
    خودکار *بیرون = s->شروع_embed (*هذا);
    إذا (unlikely (!s->extend_min (بیرون))) return_trace (خطأ);

    بیرون->دومی_Glyph = (*بستگی->glyph_map)[دومی_Glyph];

    بستگی->copy_مقادیر (s, values);

    return_trace (درست);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ثابت مقدار_Format *مقدار_Formats,
                                  ثابت مقدار_پایه *پایه) ثابت
  {
    بیعلامت record1_len = مقدار_Formats[0].get_len ();
    بیعلامت record2_len = مقدار_Formats[1].get_len ();
    ثابت hb_array_t<ثابت مقدار> مقادیر_array = values.as_array (record1_len + record2_len);

    إذا (مقدار_Formats[0].has_device ())
      مقدار_Formats[0].collect_variation_indices (c, پایه, مقادیر_array.sub_array (0, record1_len));

    إذا (مقدار_Formats[1].has_device ())
      مقدار_Formats[1].collect_variation_indices (c, پایه, مقادیر_array.sub_array (record1_len, record2_len));
  }

  منطقي intersects (ثابت hb_مجموعه_t& glyphset) ثابت
  {
    أرجع glyphset.has(دومی_Glyph);
  }

  ثابت مقدار* get_مقادیر_1 () ثابت
  {
    أرجع &values[0];
  }

  ثابت مقدار* get_مقادیر_2 (مقدار_Format format1) ثابت
  {
    أرجع &values[format1.get_len ()];
  }
};


}
}
}

#پایان_اگر  // OT_LAYOUT_GPOS_PAIRVALUERECORD_HH
