#अगर_अघोषित OT_LAYOUT_GPOS_PAIRVALUERECORD_HH
#घोषणा OT_LAYOUT_GPOS_PAIRVALUERECORD_HH

#समावेश "ValueFormat.hh"

namespace OT {
namespace Layout {
namespace GPOS_impl {


काष्ठा pair_मूल्य_record_context_t
{
  स्थिर मूल्य_Base     *base;
  स्थिर मूल्य_Format   *मूल्य_Formats;
  स्थिर मूल्य_Format   *newFormats;
  अचिन्हित            len1; /* valueFormats[0].get_len() */
  स्थिर hb_map_t      *glyph_map;
  स्थिर hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णांक>> *layout_variation_idx_delta_map;

  व्योम copy_मूल्यावली (hb_serialize_context_t *s, स्थिर मूल्य *values) स्थिर
  {
    मूल्य_Formats[0].copy_मूल्यावली (s, newFormats[0], base, values,
                                layout_variation_idx_delta_map);
    मूल्य_Formats[1].copy_मूल्यावली (s, newFormats[1], base, values + len1,
                                layout_variation_idx_delta_map);
  }
};

ढाँचा <typename Types>
काष्ठा Pair_मूल्य_Record
{
  ढाँचा <typename Types2>
  मित्र काष्ठा PairSet;

  रक्षित:
  typename Types::HBGlyphID
	        secondGlyph;            /* GlyphID of second glyph in the
                                         * pair--first glyph is listed in the
                                         * Coverage table */
  मूल्य_Record   values;                 /* Positioning data for the first glyph
                                         * followed by for second glyph */
  खुला:
  DEFINE_आकार_ARRAY (Types::HBGlyphID::static_आकार, values);

  पूर्णांक cmp (hb_codepoint_t k) स्थिर
  { वापस secondGlyph.cmp (k); }

  bool subset (hb_subset_context_t *c,
               pair_मूल्य_record_context_t *closure) स्थिर
  {
    TRACE_SERIALIZE (यह);
    स्वतः *s = c->serializer;
    स्वतः *out = s->start_embed (*यह);
    अगर (unlikely (!s->extend_min (out))) return_trace (false);

    out->secondGlyph = (*closure->glyph_map)[secondGlyph];

    closure->copy_मूल्यावली (s, values);

    return_trace (true);
  }

  व्योम collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  स्थिर मूल्य_Format *मूल्य_Formats,
                                  स्थिर मूल्य_Base *base) स्थिर
  {
    अचिन्हित record1_len = मूल्य_Formats[0].get_len ();
    अचिन्हित record2_len = मूल्य_Formats[1].get_len ();
    स्थिर hb_array_t<स्थिर मूल्य> मूल्यावली_array = values.as_array (record1_len + record2_len);

    अगर (मूल्य_Formats[0].has_device ())
      मूल्य_Formats[0].collect_variation_indices (c, base, मूल्यावली_array.sub_array (0, record1_len));

    अगर (मूल्य_Formats[1].has_device ())
      मूल्य_Formats[1].collect_variation_indices (c, base, मूल्यावली_array.sub_array (record1_len, record2_len));
  }

  bool intersects (स्थिर hb_set_t& glyphset) स्थिर
  {
    वापस glyphset.has(secondGlyph);
  }

  स्थिर मूल्य* get_मूल्यावली_1 () स्थिर
  {
    वापस &values[0];
  }

  स्थिर मूल्य* get_मूल्यावली_2 (मूल्य_Format format1) स्थिर
  {
    वापस &values[format1.get_len ()];
  }
};


}
}
}

#पूर्ण_अगर  // OT_LAYOUT_GPOS_PAIRVALUERECORD_HH
