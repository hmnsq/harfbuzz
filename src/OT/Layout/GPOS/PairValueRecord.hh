#ifndef OT_LAYOUT_GPOS_PAIRVALUERECORD_HH
#define OT_LAYOUT_GPOS_PAIRVALUERECORD_HH

#অন্তর্ভাৱ "ValueFormat.hh"

নামাকাশ OT {
নামাকাশ Layout {
নামাকাশ GPOS_impl {


সংরচনা pair_মান_record_context_t
{
  ধ্রুৱ মান_Base     *base;
  ধ্রুৱ মান_Format   *মান_Formats;
  ধ্রুৱ মান_Format   *newFormats;
  unsigned            len1; /* valueFormats[0].get_len() */
  ধ্রুৱ hb_map_t      *glyph_map;
  ধ্রুৱ hb_hashmap_t<unsigned, hb_pair_t<unsigned, পূর্ণাঙ্ক>> *layout_variation_idx_delta_map;

  শূন্য copy_মানসমূহ (hb_serialize_context_t *s, ধ্রুৱ মান *values) ধ্রুৱ
  {
    মান_Formats[0].copy_মানসমূহ (s, newFormats[0], base, values,
                                layout_variation_idx_delta_map);
    মান_Formats[1].copy_মানসমূহ (s, newFormats[1], base, values + len1,
                                layout_variation_idx_delta_map);
  }
};

template <typename Types>
সংরচনা Pair_মান_Record
{
  template <typename Types2>
  friend সংরচনা PairSet;

  রক্ষিত:
  typename Types::HBGlyphID
	        secondGlyph;            /* GlyphID of second glyph in the
                                         * pair--first glyph is listed in the
                                         * Coverage table */
  মান_Record   values;                 /* Positioning data for the first glyph
                                         * followed by for second glyph */
  সার্ৱজনিক:
  DEFINE_SIZE_ARRAY (Types::HBGlyphID::static_size, values);

  পূর্ণাঙ্ক cmp (hb_codepoint_t k) ধ্রুৱ
  { ঘূৰাই secondGlyph.cmp (k); }

  তার্কিক subset (hb_subset_context_t *c,
               pair_মান_record_context_t *closure) ধ্রুৱ
  {
    TRACE_SERIALIZE (এতত্);
    auto *s = c->serializer;
    auto *out = s->start_embed (*এতত্);
    যদি (unlikely (!s->extend_min (out))) return_trace (মিছা);

    out->secondGlyph = (*closure->glyph_map)[secondGlyph];

    closure->copy_মানসমূহ (s, values);

    return_trace (সঁচা);
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ধ্রুৱ মান_Format *মান_Formats,
                                  ধ্রুৱ মান_Base *base) ধ্রুৱ
  {
    unsigned record1_len = মান_Formats[0].get_len ();
    unsigned record2_len = মান_Formats[1].get_len ();
    ধ্রুৱ hb_array_t<ধ্রুৱ মান> মানসমূহ_array = values.as_array (record1_len + record2_len);

    যদি (মান_Formats[0].has_device ())
      মান_Formats[0].collect_variation_indices (c, base, মানসমূহ_array.sub_array (0, record1_len));

    যদি (মান_Formats[1].has_device ())
      মান_Formats[1].collect_variation_indices (c, base, মানসমূহ_array.sub_array (record1_len, record2_len));
  }

  তার্কিক intersects (ধ্রুৱ hb_set_t& glyphset) ধ্রুৱ
  {
    ঘূৰাই glyphset.has(secondGlyph);
  }

  ধ্রুৱ মান* get_মানসমূহ_1 () ধ্রুৱ
  {
    ঘূৰাই &values[0];
  }

  ধ্রুৱ মান* get_মানসমূহ_2 (মান_Format format1) ধ্রুৱ
  {
    ঘূৰাই &values[format1.get_len ()];
  }
};


}
}
}

#endif  // OT_LAYOUT_GPOS_PAIRVALUERECORD_HH
