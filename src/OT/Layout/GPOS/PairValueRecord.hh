#ifndef OT_LAYOUT_GPOS_PAIRVALUERECORD_HH
#define OT_LAYOUT_GPOS_PAIRVALUERECORD_HH

#અન્તર્ભાવ "ValueFormat.hh"

નામાકાશ OT {
નામાકાશ Layout {
નામાકાશ GPOS_impl {


સંરચના pair_કિંમત_record_context_t
{
  અચલ કિંમત_Base     *base;
  અચલ કિંમત_Format   *કિંમત_Formats;
  અચલ કિંમત_Format   *newFormats;
  unsigned            len1; /* valueFormats[0].get_len() */
  અચલ hb_map_t      *glyph_map;
  અચલ hb_hashmap_t<unsigned, hb_pair_t<unsigned, પૂર્ણાંક>> *layout_variation_idx_delta_map;

  શૂન્ય copy_કિંમતો (hb_serialize_context_t *s, અચલ કિંમત *values) અચલ
  {
    કિંમત_Formats[0].copy_કિંમતો (s, newFormats[0], base, values,
                                layout_variation_idx_delta_map);
    કિંમત_Formats[1].copy_કિંમતો (s, newFormats[1], base, values + len1,
                                layout_variation_idx_delta_map);
  }
};

template <typename Types>
સંરચના Pair_કિંમત_Record
{
  template <typename Types2>
  friend સંરચના PairSet;

  સુરક્ષિત:
  typename Types::HBGlyphID
	        secondGlyph;            /* GlyphID of second glyph in the
                                         * pair--first glyph is listed in the
                                         * Coverage table */
  કિંમત_Record   values;                 /* Positioning data for the first glyph
                                         * followed by for second glyph */
  જાહેર:
  DEFINE_SIZE_ARRAY (Types::HBGlyphID::static_size, values);

  પૂર્ણાંક cmp (hb_codepoint_t k) અચલ
  { પાછા secondGlyph.cmp (k); }

  તાર્કિક subset (hb_subset_context_t *c,
               pair_કિંમત_record_context_t *closure) અચલ
  {
    TRACE_SERIALIZE (આ);
    auto *s = c->serializer;
    auto *out = s->start_embed (*આ);
    જો (unlikely (!s->extend_min (out))) return_trace (ખોટું);

    out->secondGlyph = (*closure->glyph_map)[secondGlyph];

    closure->copy_કિંમતો (s, values);

    return_trace (સાચું);
  }

  શૂન્ય collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  અચલ કિંમત_Format *કિંમત_Formats,
                                  અચલ કિંમત_Base *base) અચલ
  {
    unsigned record1_len = કિંમત_Formats[0].get_len ();
    unsigned record2_len = કિંમત_Formats[1].get_len ();
    અચલ hb_array_t<અચલ કિંમત> કિંમતો_array = values.as_array (record1_len + record2_len);

    જો (કિંમત_Formats[0].has_device ())
      કિંમત_Formats[0].collect_variation_indices (c, base, કિંમતો_array.sub_array (0, record1_len));

    જો (કિંમત_Formats[1].has_device ())
      કિંમત_Formats[1].collect_variation_indices (c, base, કિંમતો_array.sub_array (record1_len, record2_len));
  }

  તાર્કિક intersects (અચલ hb_set_t& glyphset) અચલ
  {
    પાછા glyphset.has(secondGlyph);
  }

  અચલ કિંમત* get_કિંમતો_1 () અચલ
  {
    પાછા &values[0];
  }

  અચલ કિંમત* get_કિંમતો_2 (કિંમત_Format format1) અચલ
  {
    પાછા &values[format1.get_len ()];
  }
};


}
}
}

#endif  // OT_LAYOUT_GPOS_PAIRVALUERECORD_HH
