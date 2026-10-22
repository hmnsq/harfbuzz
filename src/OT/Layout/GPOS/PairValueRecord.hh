#جيڪڏهن_وصف_نه_ٿيل OT_LAYOUT_GPOS_PAIRVALUERECORD_HH
#وصف_ڪريو OT_LAYOUT_GPOS_PAIRVALUERECORD_HH

#ضمن "ValueFormat.hh"

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {


بنية pair_قدر_رڪارڊ_context_t
{
  ثابت قدر_بنياد     *بنياد;
  ثابت قدر_Format   *قدر_Formats;
  ثابت قدر_Format   *newFormats;
  بنانشان            len1; /* valueFormats[0].get_len() */
  ثابت hb_map_t      *glyph_map;
  ثابت hb_hashmap_t<بنانشان, hb_pair_t<بنانشان, صحيح>> *layout_variation_idx_delta_map;

  فراغ copy_قدرون (hb_serialize_context_t *s, ثابت قدر *values) ثابت
  {
    قدر_Formats[0].copy_قدرون (s, newFormats[0], بنياد, values,
                                layout_variation_idx_delta_map);
    قدر_Formats[1].copy_قدرون (s, newFormats[1], بنياد, values + len1,
                                layout_variation_idx_delta_map);
  }
};

سانچو <قسم_نالو Types>
بنية Pair_قدر_رڪارڊ
{
  سانچو <قسم_نالو Types2>
  دوست بنية Pair_سيٽ;

  محمي:
  قسم_نالو Types::HBGlyphID
	        ٻيو_Glyph;            /* GlyphID of second glyph in the
                                         * pair--first glyph is listed in the
                                         * Coverage table */
  قدر_رڪارڊ   values;                 /* Positioning data for the first glyph
                                         * followed by for second glyph */
  عام:
  DEFINE_سائيز_ARRAY (Types::HBGlyphID::static_سائيز, values);

  صحيح cmp (hb_codepoint_t k) ثابت
  { أرجع ٻيو_Glyph.cmp (k); }

  منطقي subset (hb_subset_context_t *c,
               pair_قدر_رڪارڊ_context_t *گهيرو) ثابت
  {
    TRACE_SERIALIZE (هذا);
    خودڪار *s = c->serializer;
    خودڪار *ٻاهريون = s->شروعات_embed (*هذا);
    إذا (unlikely (!s->extend_min (ٻاهريون))) return_trace (خطأ);

    ٻاهريون->ٻيو_Glyph = (*گهيرو->glyph_map)[ٻيو_Glyph];

    گهيرو->copy_قدرون (s, values);

    return_trace (سچ);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ثابت قدر_Format *قدر_Formats,
                                  ثابت قدر_بنياد *بنياد) ثابت
  {
    بنانشان record1_len = قدر_Formats[0].get_len ();
    بنانشان record2_len = قدر_Formats[1].get_len ();
    ثابت hb_array_t<ثابت قدر> قدرون_array = values.as_array (record1_len + record2_len);

    إذا (قدر_Formats[0].has_device ())
      قدر_Formats[0].collect_variation_indices (c, بنياد, قدرون_array.sub_array (0, record1_len));

    إذا (قدر_Formats[1].has_device ())
      قدر_Formats[1].collect_variation_indices (c, بنياد, قدرون_array.sub_array (record1_len, record2_len));
  }

  منطقي intersects (ثابت hb_سيٽ_t& glyphset) ثابت
  {
    أرجع glyphset.has(ٻيو_Glyph);
  }

  ثابت قدر* get_قدرون_1 () ثابت
  {
    أرجع &values[0];
  }

  ثابت قدر* get_قدرون_2 (قدر_Format format1) ثابت
  {
    أرجع &values[format1.get_len ()];
  }
};


}
}
}

#جيڪڏهن_پورو  // OT_LAYOUT_GPOS_PAIRVALUERECORD_HH
