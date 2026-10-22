#ifndef OT_LAYOUT_GPOS_PAIRVALUERECORD_HH
#define OT_LAYOUT_GPOS_PAIRVALUERECORD_HH

#ಅನ್ತರ್ಭಾವ "ValueFormat.hh"

ನಾಮಾಕಾಶ OT {
ನಾಮಾಕಾಶ Layout {
ನಾಮಾಕಾಶ GPOS_impl {


ಸಂರಚನಾ pair_ಮೌಲ್ಯ_record_context_t
{
  ಧ್ರುವ ಮೌಲ್ಯ_Base     *base;
  ಧ್ರುವ ಮೌಲ್ಯ_Format   *ಮೌಲ್ಯ_Formats;
  ಧ್ರುವ ಮೌಲ್ಯ_Format   *newFormats;
  unsigned            len1; /* valueFormats[0].get_len() */
  ಧ್ರುವ hb_map_t      *glyph_map;
  ಧ್ರುವ hb_hashmap_t<unsigned, hb_pair_t<unsigned, ಪೂರ್ಣಾಙ್ಕ>> *layout_variation_idx_delta_map;

  ಶೂನ್ಯ copy_ಮೌಲ್ಯಗಳು (hb_serialize_context_t *s, ಧ್ರುವ ಮೌಲ್ಯ *values) ಧ್ರುವ
  {
    ಮೌಲ್ಯ_Formats[0].copy_ಮೌಲ್ಯಗಳು (s, newFormats[0], base, values,
                                layout_variation_idx_delta_map);
    ಮೌಲ್ಯ_Formats[1].copy_ಮೌಲ್ಯಗಳು (s, newFormats[1], base, values + len1,
                                layout_variation_idx_delta_map);
  }
};

template <typename Types>
ಸಂರಚನಾ Pair_ಮೌಲ್ಯ_Record
{
  template <typename Types2>
  friend ಸಂರಚನಾ PairSet;

  ರಕ್ಷಿತ:
  typename Types::HBGlyphID
	        secondGlyph;            /* GlyphID of second glyph in the
                                         * pair--first glyph is listed in the
                                         * Coverage table */
  ಮೌಲ್ಯ_Record   values;                 /* Positioning data for the first glyph
                                         * followed by for second glyph */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (Types::HBGlyphID::static_size, values);

  ಪೂರ್ಣಾಙ್ಕ cmp (hb_codepoint_t k) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ secondGlyph.cmp (k); }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c,
               pair_ಮೌಲ್ಯ_record_context_t *closure) ಧ್ರುವ
  {
    TRACE_SERIALIZE (ಏತತ್);
    auto *s = c->serializer;
    auto *out = s->start_embed (*ಏತತ್);
    ಯದಿ (unlikely (!s->extend_min (out))) return_trace (ಅಸತ್ಯ);

    out->secondGlyph = (*closure->glyph_map)[secondGlyph];

    closure->copy_ಮೌಲ್ಯಗಳು (s, values);

    return_trace (ಸತ್ಯ);
  }

  ಶೂನ್ಯ collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ಧ್ರುವ ಮೌಲ್ಯ_Format *ಮೌಲ್ಯ_Formats,
                                  ಧ್ರುವ ಮೌಲ್ಯ_Base *base) ಧ್ರುವ
  {
    unsigned record1_len = ಮೌಲ್ಯ_Formats[0].get_len ();
    unsigned record2_len = ಮೌಲ್ಯ_Formats[1].get_len ();
    ಧ್ರುವ hb_array_t<ಧ್ರುವ ಮೌಲ್ಯ> ಮೌಲ್ಯಗಳು_array = values.as_array (record1_len + record2_len);

    ಯದಿ (ಮೌಲ್ಯ_Formats[0].has_device ())
      ಮೌಲ್ಯ_Formats[0].collect_variation_indices (c, base, ಮೌಲ್ಯಗಳು_array.sub_array (0, record1_len));

    ಯದಿ (ಮೌಲ್ಯ_Formats[1].has_device ())
      ಮೌಲ್ಯ_Formats[1].collect_variation_indices (c, base, ಮೌಲ್ಯಗಳು_array.sub_array (record1_len, record2_len));
  }

  ತಾರ್ಕಿಕ intersects (ಧ್ರುವ hb_set_t& glyphset) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ glyphset.has(secondGlyph);
  }

  ಧ್ರುವ ಮೌಲ್ಯ* get_ಮೌಲ್ಯಗಳು_1 () ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ &values[0];
  }

  ಧ್ರುವ ಮೌಲ್ಯ* get_ಮೌಲ್ಯಗಳು_2 (ಮೌಲ್ಯ_Format format1) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ &values[format1.get_len ()];
  }
};


}
}
}

#endif  // OT_LAYOUT_GPOS_PAIRVALUERECORD_HH
