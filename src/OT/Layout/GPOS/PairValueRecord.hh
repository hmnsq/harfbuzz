#ifndef OT_LAYOUT_GPOS_PAIRVALUERECORD_HH
#define OT_LAYOUT_GPOS_PAIRVALUERECORD_HH

#ضمن "ValueFormat.hh"

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {


بنية pair_بەها_record_context_t
{
  ثابت بەها_Base     *base;
  ثابت بەها_Format   *بەها_Formats;
  ثابت بەها_Format   *newFormats;
  unsigned            len1; /* valueFormats[0].get_len() */
  ثابت hb_map_t      *glyph_map;
  ثابت hb_hashmap_t<unsigned, hb_pair_t<unsigned, صحيح>> *layout_variation_idx_delta_map;

  فراغ copy_بەهاکان (hb_serialize_context_t *s, ثابت بەها *values) ثابت
  {
    بەها_Formats[0].copy_بەهاکان (s, newFormats[0], base, values,
                                layout_variation_idx_delta_map);
    بەها_Formats[1].copy_بەهاکان (s, newFormats[1], base, values + len1,
                                layout_variation_idx_delta_map);
  }
};

template <typename Types>
بنية Pair_بەها_Record
{
  template <typename Types2>
  friend بنية PairSet;

  محمي:
  typename Types::HBGlyphID
	        secondGlyph;            /* GlyphID of second glyph in the
                                         * pair--first glyph is listed in the
                                         * Coverage table */
  بەها_Record   values;                 /* Positioning data for the first glyph
                                         * followed by for second glyph */
  عام:
  DEFINE_SIZE_ARRAY (Types::HBGlyphID::static_size, values);

  صحيح cmp (hb_codepoint_t k) ثابت
  { أرجع secondGlyph.cmp (k); }

  منطقي subset (hb_subset_context_t *c,
               pair_بەها_record_context_t *closure) ثابت
  {
    TRACE_SERIALIZE (هذا);
    auto *s = c->serializer;
    auto *out = s->start_embed (*هذا);
    إذا (unlikely (!s->extend_min (out))) return_trace (خطأ);

    out->secondGlyph = (*closure->glyph_map)[secondGlyph];

    closure->copy_بەهاکان (s, values);

    return_trace (true);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ثابت بەها_Format *بەها_Formats,
                                  ثابت بەها_Base *base) ثابت
  {
    unsigned record1_len = بەها_Formats[0].get_len ();
    unsigned record2_len = بەها_Formats[1].get_len ();
    ثابت hb_array_t<ثابت بەها> بەهاکان_array = values.as_array (record1_len + record2_len);

    إذا (بەها_Formats[0].has_device ())
      بەها_Formats[0].collect_variation_indices (c, base, بەهاکان_array.sub_array (0, record1_len));

    إذا (بەها_Formats[1].has_device ())
      بەها_Formats[1].collect_variation_indices (c, base, بەهاکان_array.sub_array (record1_len, record2_len));
  }

  منطقي intersects (ثابت hb_set_t& glyphset) ثابت
  {
    أرجع glyphset.has(secondGlyph);
  }

  ثابت بەها* get_بەهاکان_1 () ثابت
  {
    أرجع &values[0];
  }

  ثابت بەها* get_بەهاکان_2 (بەها_Format format1) ثابت
  {
    أرجع &values[format1.get_len ()];
  }
};


}
}
}

#endif  // OT_LAYOUT_GPOS_PAIRVALUERECORD_HH
