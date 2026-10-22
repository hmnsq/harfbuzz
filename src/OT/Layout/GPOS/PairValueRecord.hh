#ifndef OT_LAYOUT_GPOS_PAIRVALUERECORD_HH
#define OT_LAYOUT_GPOS_PAIRVALUERECORD_HH

#অন্তর্ভাব "ValueFormat.hh"

নামাকাশ OT {
নামাকাশ Layout {
নামাকাশ GPOS_impl {


কাঠামো pair_মান_record_context_t
{
  ধ্রুবক মান_Base     *base;
  ধ্রুবক মান_Format   *মান_Formats;
  ধ্রুবক মান_Format   *newFormats;
  unsigned            len1; /* valueFormats[0].get_len() */
  ধ্রুবক hb_map_t      *glyph_map;
  ধ্রুবক hb_hashmap_t<unsigned, hb_pair_t<unsigned, পূর্ণসংখ্যা>> *layout_variation_idx_delta_map;

  শূন্য copy_মানসমূহ (hb_serialize_context_t *s, ধ্রুবক মান *values) ধ্রুবক
  {
    মান_Formats[0].copy_মানসমূহ (s, newFormats[0], base, values,
                                layout_variation_idx_delta_map);
    মান_Formats[1].copy_মানসমূহ (s, newFormats[1], base, values + len1,
                                layout_variation_idx_delta_map);
  }
};

template <typename Types>
কাঠামো Pair_মান_Record
{
  template <typename Types2>
  friend কাঠামো PairSet;

  সুরক্ষিত:
  typename Types::HBGlyphID
	        secondGlyph;            /* GlyphID of second glyph in the
                                         * pair--first glyph is listed in the
                                         * Coverage table */
  মান_Record   values;                 /* Positioning data for the first glyph
                                         * followed by for second glyph */
  সর্বজনীন:
  DEFINE_আকার_ARRAY (Types::HBGlyphID::static_আকার, values);

  পূর্ণসংখ্যা cmp (hb_codepoint_t k) ধ্রুবক
  { ফেরত secondGlyph.cmp (k); }

  যৌক্তিক subset (hb_subset_context_t *c,
               pair_মান_record_context_t *closure) ধ্রুবক
  {
    TRACE_SERIALIZE (এই);
    auto *s = c->serializer;
    auto *out = s->start_embed (*এই);
    যদি (unlikely (!s->extend_min (out))) return_trace (মিথ্যা);

    out->secondGlyph = (*closure->glyph_map)[secondGlyph];

    closure->copy_মানসমূহ (s, values);

    return_trace (সত্য);
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ধ্রুবক মান_Format *মান_Formats,
                                  ধ্রুবক মান_Base *base) ধ্রুবক
  {
    unsigned record1_len = মান_Formats[0].get_len ();
    unsigned record2_len = মান_Formats[1].get_len ();
    ধ্রুবক hb_array_t<ধ্রুবক মান> মানসমূহ_array = values.as_array (record1_len + record2_len);

    যদি (মান_Formats[0].has_device ())
      মান_Formats[0].collect_variation_indices (c, base, মানসমূহ_array.sub_array (0, record1_len));

    যদি (মান_Formats[1].has_device ())
      মান_Formats[1].collect_variation_indices (c, base, মানসমূহ_array.sub_array (record1_len, record2_len));
  }

  যৌক্তিক intersects (ধ্রুবক hb_set_t& glyphset) ধ্রুবক
  {
    ফেরত glyphset.has(secondGlyph);
  }

  ধ্রুবক মান* get_মানসমূহ_1 () ধ্রুবক
  {
    ফেরত &values[0];
  }

  ধ্রুবক মান* get_মানসমূহ_2 (মান_Format format1) ধ্রুবক
  {
    ফেরত &values[format1.get_len ()];
  }
};


}
}
}

#endif  // OT_LAYOUT_GPOS_PAIRVALUERECORD_HH
