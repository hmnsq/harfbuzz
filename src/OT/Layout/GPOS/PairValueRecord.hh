#ifndef OT_LAYOUT_GPOS_PAIRVALUERECORD_HH
#define OT_LAYOUT_GPOS_PAIRVALUERECORD_HH

#ضمن "ValueFormat.hh"

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {


بنية pair_قىممەت_record_context_t
{
  ثابت قىممەت_Base     *base;
  ثابت قىممەت_Format   *قىممەت_Formats;
  ثابت قىممەت_Format   *newFormats;
  unsigned            len1; /* valueFormats[0].get_len() */
  ثابت hb_map_t      *glyph_map;
  ثابت hb_hashmap_t<unsigned, hb_pair_t<unsigned, صحيح>> *layout_variation_idx_delta_map;

  فراغ copy_قىممەتلەر (hb_serialize_context_t *s, ثابت قىممەت *values) ثابت
  {
    قىممەت_Formats[0].copy_قىممەتلەر (s, newFormats[0], base, values,
                                layout_variation_idx_delta_map);
    قىممەت_Formats[1].copy_قىممەتلەر (s, newFormats[1], base, values + len1,
                                layout_variation_idx_delta_map);
  }
};

template <typename Types>
بنية Pair_قىممەت_Record
{
  template <typename Types2>
  friend بنية PairSet;

  محمي:
  typename Types::HBGlyphID
	        secondGlyph;            /* GlyphID of second glyph in the
                                         * pair--first glyph is listed in the
                                         * Coverage table */
  قىممەت_Record   values;                 /* Positioning data for the first glyph
                                         * followed by for second glyph */
  عام:
  DEFINE_SIZE_ARRAY (Types::HBGlyphID::static_size, values);

  صحيح cmp (hb_codepoint_t k) ثابت
  { أرجع secondGlyph.cmp (k); }

  منطقي subset (hb_subset_context_t *c,
               pair_قىممەت_record_context_t *closure) ثابت
  {
    TRACE_SERIALIZE (هذا);
    auto *s = c->serializer;
    auto *out = s->start_embed (*هذا);
    إذا (unlikely (!s->extend_min (out))) return_trace (خطأ);

    out->secondGlyph = (*closure->glyph_map)[secondGlyph];

    closure->copy_قىممەتلەر (s, values);

    return_trace (true);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ثابت قىممەت_Format *قىممەت_Formats,
                                  ثابت قىممەت_Base *base) ثابت
  {
    unsigned record1_len = قىممەت_Formats[0].get_len ();
    unsigned record2_len = قىممەت_Formats[1].get_len ();
    ثابت hb_array_t<ثابت قىممەت> قىممەتلەر_array = values.as_array (record1_len + record2_len);

    إذا (قىممەت_Formats[0].has_device ())
      قىممەت_Formats[0].collect_variation_indices (c, base, قىممەتلەر_array.sub_array (0, record1_len));

    إذا (قىممەت_Formats[1].has_device ())
      قىممەت_Formats[1].collect_variation_indices (c, base, قىممەتلەر_array.sub_array (record1_len, record2_len));
  }

  منطقي intersects (ثابت hb_set_t& glyphset) ثابت
  {
    أرجع glyphset.has(secondGlyph);
  }

  ثابت قىممەت* get_قىممەتلەر_1 () ثابت
  {
    أرجع &values[0];
  }

  ثابت قىممەت* get_قىممەتلەر_2 (قىممەت_Format format1) ثابت
  {
    أرجع &values[format1.get_len ()];
  }
};


}
}
}

#endif  // OT_LAYOUT_GPOS_PAIRVALUERECORD_HH
