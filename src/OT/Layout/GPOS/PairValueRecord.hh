#ifndef OT_LAYOUT_GPOS_PAIRVALUERECORD_HH
#define OT_LAYOUT_GPOS_PAIRVALUERECORD_HH

#അന്തര്ഭാവ "ValueFormat.hh"

നാമാകാശ OT {
നാമാകാശ Layout {
നാമാകാശ GPOS_impl {


ഘടന pair_മൂല്യം_record_context_t
{
  സ്ഥിരം മൂല്യം_Base     *base;
  സ്ഥിരം മൂല്യം_Format   *മൂല്യം_Formats;
  സ്ഥിരം മൂല്യം_Format   *newFormats;
  unsigned            len1; /* valueFormats[0].get_len() */
  സ്ഥിരം hb_map_t      *glyph_map;
  സ്ഥിരം hb_hashmap_t<unsigned, hb_pair_t<unsigned, പൂർണ്ണസംഖ്യ>> *layout_variation_idx_delta_map;

  ശൂന്യം copy_മൂല്യങ്ങൾ (hb_serialize_context_t *s, സ്ഥിരം മൂല്യം *values) സ്ഥിരം
  {
    മൂല്യം_Formats[0].copy_മൂല്യങ്ങൾ (s, newFormats[0], base, values,
                                layout_variation_idx_delta_map);
    മൂല്യം_Formats[1].copy_മൂല്യങ്ങൾ (s, newFormats[1], base, values + len1,
                                layout_variation_idx_delta_map);
  }
};

template <typename Types>
ഘടന Pair_മൂല്യം_Record
{
  template <typename Types2>
  friend ഘടന PairSet;

  സംരക്ഷിത:
  typename Types::HBGlyphID
	        secondGlyph;            /* GlyphID of second glyph in the
                                         * pair--first glyph is listed in the
                                         * Coverage table */
  മൂല്യം_Record   values;                 /* Positioning data for the first glyph
                                         * followed by for second glyph */
  പൊതു:
  DEFINE_SIZE_ARRAY (Types::HBGlyphID::static_size, values);

  പൂർണ്ണസംഖ്യ cmp (hb_codepoint_t k) സ്ഥിരം
  { തിരികെ secondGlyph.cmp (k); }

  തർക്കം subset (hb_subset_context_t *c,
               pair_മൂല്യം_record_context_t *closure) സ്ഥിരം
  {
    TRACE_SERIALIZE (ഇത്);
    auto *s = c->serializer;
    auto *out = s->start_embed (*ഇത്);
    എങ്കിൽ (unlikely (!s->extend_min (out))) return_trace (അസത്യം);

    out->secondGlyph = (*closure->glyph_map)[secondGlyph];

    closure->copy_മൂല്യങ്ങൾ (s, values);

    return_trace (സത്യം);
  }

  ശൂന്യം collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  സ്ഥിരം മൂല്യം_Format *മൂല്യം_Formats,
                                  സ്ഥിരം മൂല്യം_Base *base) സ്ഥിരം
  {
    unsigned record1_len = മൂല്യം_Formats[0].get_len ();
    unsigned record2_len = മൂല്യം_Formats[1].get_len ();
    സ്ഥിരം hb_array_t<സ്ഥിരം മൂല്യം> മൂല്യങ്ങൾ_array = values.as_array (record1_len + record2_len);

    എങ്കിൽ (മൂല്യം_Formats[0].has_device ())
      മൂല്യം_Formats[0].collect_variation_indices (c, base, മൂല്യങ്ങൾ_array.sub_array (0, record1_len));

    എങ്കിൽ (മൂല്യം_Formats[1].has_device ())
      മൂല്യം_Formats[1].collect_variation_indices (c, base, മൂല്യങ്ങൾ_array.sub_array (record1_len, record2_len));
  }

  തർക്കം intersects (സ്ഥിരം hb_set_t& glyphset) സ്ഥിരം
  {
    തിരികെ glyphset.has(secondGlyph);
  }

  സ്ഥിരം മൂല്യം* get_മൂല്യങ്ങൾ_1 () സ്ഥിരം
  {
    തിരികെ &values[0];
  }

  സ്ഥിരം മൂല്യം* get_മൂല്യങ്ങൾ_2 (മൂല്യം_Format format1) സ്ഥിരം
  {
    തിരികെ &values[format1.get_len ()];
  }
};


}
}
}

#endif  // OT_LAYOUT_GPOS_PAIRVALUERECORD_HH
