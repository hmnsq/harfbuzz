#ifndef OT_LAYOUT_GPOS_PAIRVALUERECORD_HH
#define OT_LAYOUT_GPOS_PAIRVALUERECORD_HH

#அந்தர்பாவ "ValueFormat.hh"

நாமாகாஶ OT {
நாமாகாஶ Layout {
நாமாகாஶ GPOS_impl {


கட்டமைப்பு pair_மதிப்பு_record_context_t
{
  மாறிலி மதிப்பு_Base     *base;
  மாறிலி மதிப்பு_Format   *மதிப்பு_Formats;
  மாறிலி மதிப்பு_Format   *newFormats;
  unsigned            len1; /* valueFormats[0].get_len() */
  மாறிலி hb_map_t      *glyph_map;
  மாறிலி hb_hashmap_t<unsigned, hb_pair_t<unsigned, முழு>> *layout_variation_idx_delta_map;

  வெறுமை copy_மதிப்புகள் (hb_serialize_context_t *s, மாறிலி மதிப்பு *values) மாறிலி
  {
    மதிப்பு_Formats[0].copy_மதிப்புகள் (s, newFormats[0], base, values,
                                layout_variation_idx_delta_map);
    மதிப்பு_Formats[1].copy_மதிப்புகள் (s, newFormats[1], base, values + len1,
                                layout_variation_idx_delta_map);
  }
};

template <typename Types>
கட்டமைப்பு Pair_மதிப்பு_Record
{
  template <typename Types2>
  friend கட்டமைப்பு PairSet;

  பாதுகாக்கப்பட்ட:
  typename Types::HBGlyphID
	        secondGlyph;            /* GlyphID of second glyph in the
                                         * pair--first glyph is listed in the
                                         * Coverage table */
  மதிப்பு_Record   values;                 /* Positioning data for the first glyph
                                         * followed by for second glyph */
  பொது:
  DEFINE_SIZE_ARRAY (Types::HBGlyphID::static_size, values);

  முழு cmp (hb_codepoint_t k) மாறிலி
  { திரும்பு secondGlyph.cmp (k); }

  தருக்க subset (hb_subset_context_t *c,
               pair_மதிப்பு_record_context_t *closure) மாறிலி
  {
    TRACE_SERIALIZE (இது);
    auto *s = c->serializer;
    auto *out = s->start_embed (*இது);
    எனில் (unlikely (!s->extend_min (out))) return_trace (பொய்);

    out->secondGlyph = (*closure->glyph_map)[secondGlyph];

    closure->copy_மதிப்புகள் (s, values);

    return_trace (உண்மை);
  }

  வெறுமை collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  மாறிலி மதிப்பு_Format *மதிப்பு_Formats,
                                  மாறிலி மதிப்பு_Base *base) மாறிலி
  {
    unsigned record1_len = மதிப்பு_Formats[0].get_len ();
    unsigned record2_len = மதிப்பு_Formats[1].get_len ();
    மாறிலி hb_array_t<மாறிலி மதிப்பு> மதிப்புகள்_array = values.as_array (record1_len + record2_len);

    எனில் (மதிப்பு_Formats[0].has_device ())
      மதிப்பு_Formats[0].collect_variation_indices (c, base, மதிப்புகள்_array.sub_array (0, record1_len));

    எனில் (மதிப்பு_Formats[1].has_device ())
      மதிப்பு_Formats[1].collect_variation_indices (c, base, மதிப்புகள்_array.sub_array (record1_len, record2_len));
  }

  தருக்க intersects (மாறிலி hb_set_t& glyphset) மாறிலி
  {
    திரும்பு glyphset.has(secondGlyph);
  }

  மாறிலி மதிப்பு* get_மதிப்புகள்_1 () மாறிலி
  {
    திரும்பு &values[0];
  }

  மாறிலி மதிப்பு* get_மதிப்புகள்_2 (மதிப்பு_Format format1) மாறிலி
  {
    திரும்பு &values[format1.get_len ()];
  }
};


}
}
}

#endif  // OT_LAYOUT_GPOS_PAIRVALUERECORD_HH
