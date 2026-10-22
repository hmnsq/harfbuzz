#ifndef OT_LAYOUT_GPOS_PAIRVALUERECORD_HH
#define OT_LAYOUT_GPOS_PAIRVALUERECORD_HH

#अन्तर्भाव "ValueFormat.hh"

नामाकाश OT {
नामाकाश Layout {
नामाकाश GPOS_impl {


संरचना pair_मूल्य_record_context_t
{
  स्थिरांक मूल्य_Base     *base;
  स्थिरांक मूल्य_Format   *मूल्य_Formats;
  स्थिरांक मूल्य_Format   *newFormats;
  अचिन्हित            len1; /* valueFormats[0].get_len() */
  स्थिरांक hb_map_t      *glyph_map;
  स्थिरांक hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णांक>> *layout_variation_idx_delta_map;

  शून्य copy_मूल्ये (hb_serialize_context_t *s, स्थिरांक मूल्य *values) स्थिरांक
  {
    मूल्य_Formats[0].copy_मूल्ये (s, newFormats[0], base, values,
                                layout_variation_idx_delta_map);
    मूल्य_Formats[1].copy_मूल्ये (s, newFormats[1], base, values + len1,
                                layout_variation_idx_delta_map);
  }
};

ढाँचा <typename Types>
संरचना Pair_मूल्य_Record
{
  ढाँचा <typename Types2>
  मित्र संरचना PairSet;

  संरक्षित:
  typename Types::HBGlyphID
	        secondGlyph;            /* GlyphID of second glyph in the
                                         * pair--first glyph is listed in the
                                         * Coverage table */
  मूल्य_Record   values;                 /* Positioning data for the first glyph
                                         * followed by for second glyph */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (Types::HBGlyphID::static_size, values);

  पूर्णांक cmp (hb_codepoint_t k) स्थिरांक
  { परत secondGlyph.cmp (k); }

  तार्किक subset (hb_subset_context_t *c,
               pair_मूल्य_record_context_t *closure) स्थिरांक
  {
    TRACE_SERIALIZE (हे);
    स्वतः *s = c->serializer;
    स्वतः *out = s->start_embed (*हे);
    जर (unlikely (!s->extend_min (out))) return_trace (असत्य);

    out->secondGlyph = (*closure->glyph_map)[secondGlyph];

    closure->copy_मूल्ये (s, values);

    return_trace (सत्य);
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  स्थिरांक मूल्य_Format *मूल्य_Formats,
                                  स्थिरांक मूल्य_Base *base) स्थिरांक
  {
    अचिन्हित record1_len = मूल्य_Formats[0].get_len ();
    अचिन्हित record2_len = मूल्य_Formats[1].get_len ();
    स्थिरांक hb_array_t<स्थिरांक मूल्य> मूल्ये_array = values.as_array (record1_len + record2_len);

    जर (मूल्य_Formats[0].has_device ())
      मूल्य_Formats[0].collect_variation_indices (c, base, मूल्ये_array.sub_array (0, record1_len));

    जर (मूल्य_Formats[1].has_device ())
      मूल्य_Formats[1].collect_variation_indices (c, base, मूल्ये_array.sub_array (record1_len, record2_len));
  }

  तार्किक intersects (स्थिरांक hb_set_t& glyphset) स्थिरांक
  {
    परत glyphset.has(secondGlyph);
  }

  स्थिरांक मूल्य* get_मूल्ये_1 () स्थिरांक
  {
    परत &values[0];
  }

  स्थिरांक मूल्य* get_मूल्ये_2 (मूल्य_Format format1) स्थिरांक
  {
    परत &values[format1.get_len ()];
  }
};


}
}
}

#endif  // OT_LAYOUT_GPOS_PAIRVALUERECORD_HH
