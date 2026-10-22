#ifndef OT_LAYOUT_GPOS_PAIRVALUERECORD_HH
#define OT_LAYOUT_GPOS_PAIRVALUERECORD_HH

#अन्तर्भाव "ValueFormat.hh"

नामाकाश OT {
नामाकाश Layout {
नामाकाश GPOS_impl {


संरचना pair_मान_record_context_t
{
  अचर मान_Base     *base;
  अचर मान_Format   *मान_Formats;
  अचर मान_Format   *newFormats;
  अचिन्हित            len1; /* valueFormats[0].get_len() */
  अचर hb_map_t      *glyph_map;
  अचर hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णाङ्क>> *layout_variation_idx_delta_map;

  शून्य copy_मानहरू (hb_serialize_context_t *s, अचर मान *values) अचर
  {
    मान_Formats[0].copy_मानहरू (s, newFormats[0], base, values,
                                layout_variation_idx_delta_map);
    मान_Formats[1].copy_मानहरू (s, newFormats[1], base, values + len1,
                                layout_variation_idx_delta_map);
  }
};

ढाँचा <typename Types>
संरचना Pair_मान_Record
{
  ढाँचा <typename Types2>
  मित्र संरचना PairSet;

  रक्षित:
  typename Types::HBGlyphID
	        secondGlyph;            /* GlyphID of second glyph in the
                                         * pair--first glyph is listed in the
                                         * Coverage table */
  मान_Record   values;                 /* Positioning data for the first glyph
                                         * followed by for second glyph */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (Types::HBGlyphID::static_size, values);

  पूर्णाङ्क cmp (hb_codepoint_t k) अचर
  { फर्काऊ secondGlyph.cmp (k); }

  तार्किक subset (hb_subset_context_t *c,
               pair_मान_record_context_t *closure) अचर
  {
    TRACE_SERIALIZE (एतत्);
    स्वतः *s = c->serializer;
    स्वतः *out = s->start_embed (*एतत्);
    यदि (unlikely (!s->extend_min (out))) return_trace (असत्य);

    out->secondGlyph = (*closure->glyph_map)[secondGlyph];

    closure->copy_मानहरू (s, values);

    return_trace (सत्य);
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  अचर मान_Format *मान_Formats,
                                  अचर मान_Base *base) अचर
  {
    अचिन्हित record1_len = मान_Formats[0].get_len ();
    अचिन्हित record2_len = मान_Formats[1].get_len ();
    अचर hb_array_t<अचर मान> मानहरू_array = values.as_array (record1_len + record2_len);

    यदि (मान_Formats[0].has_device ())
      मान_Formats[0].collect_variation_indices (c, base, मानहरू_array.sub_array (0, record1_len));

    यदि (मान_Formats[1].has_device ())
      मान_Formats[1].collect_variation_indices (c, base, मानहरू_array.sub_array (record1_len, record2_len));
  }

  तार्किक intersects (अचर hb_set_t& glyphset) अचर
  {
    फर्काऊ glyphset.has(secondGlyph);
  }

  अचर मान* get_मानहरू_1 () अचर
  {
    फर्काऊ &values[0];
  }

  अचर मान* get_मानहरू_2 (मान_Format format1) अचर
  {
    फर्काऊ &values[format1.get_len ()];
  }
};


}
}
}

#endif  // OT_LAYOUT_GPOS_PAIRVALUERECORD_HH
