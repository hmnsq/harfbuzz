#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OT_LAYOUT_GPOS_PAIRVALUERECORD_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_LAYOUT_GPOS_PAIRVALUERECORD_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "ValueFormat.hh"

ਨਾਮਾਕਾਸ਼ OT {
ਨਾਮਾਕਾਸ਼ Layout {
ਨਾਮਾਕਾਸ਼ GPOS_impl {


ਬਣਤਰ pair_ਮੁੱਲ_ਰਿਕਾਰਡ_context_t
{
  ਅਚਲ ਮੁੱਲ_ਅਧਾਰ     *ਅਧਾਰ;
  ਅਚਲ ਮੁੱਲ_Format   *ਮੁੱਲ_Formats;
  ਅਚਲ ਮੁੱਲ_Format   *newFormats;
  ਬਿਨਾਨਿਸ਼ਾਨੀ            len1; /* valueFormats[0].get_len() */
  ਅਚਲ hb_map_t      *glyph_map;
  ਅਚਲ hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਪੂਰਨ>> *layout_variation_idx_delta_map;

  ਖਾਲੀ copy_ਮੁੱਲਾਂ (hb_serialize_context_t *s, ਅਚਲ ਮੁੱਲ *values) ਅਚਲ
  {
    ਮੁੱਲ_Formats[0].copy_ਮੁੱਲਾਂ (s, newFormats[0], ਅਧਾਰ, values,
                                layout_variation_idx_delta_map);
    ਮੁੱਲ_Formats[1].copy_ਮੁੱਲਾਂ (s, newFormats[1], ਅਧਾਰ, values + len1,
                                layout_variation_idx_delta_map);
  }
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Types>
ਬਣਤਰ Pair_ਮੁੱਲ_ਰਿਕਾਰਡ
{
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Types2>
  ਮਿੱਤਰ ਬਣਤਰ Pair_ਸੈੱਟ;

  ਸੁਰੱਖਿਅਤ:
  ਕਿਸਮ_ਨਾਂ Types::HBGlyphID
	        ਦੂਜਾ_Glyph;            /* GlyphID of second glyph in the
                                         * pair--first glyph is listed in the
                                         * Coverage table */
  ਮੁੱਲ_ਰਿਕਾਰਡ   values;                 /* Positioning data for the first glyph
                                         * followed by for second glyph */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (Types::HBGlyphID::static_ਮਾਪ, values);

  ਪੂਰਨ cmp (hb_codepoint_t k) ਅਚਲ
  { ਵਾਪਸ ਦੂਜਾ_Glyph.cmp (k); }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c,
               pair_ਮੁੱਲ_ਰਿਕਾਰਡ_context_t *ਘੇਰਾ) ਅਚਲ
  {
    TRACE_SERIALIZE (ਇਹ);
    ਸਵੈਚਾਲਿਤ *s = c->serializer;
    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = s->ਸ਼ੁਰੂ_embed (*ਇਹ);
    ਜੇ (unlikely (!s->extend_min (ਬਾਹਰਲਾ))) return_trace (ਝੂਠ);

    ਬਾਹਰਲਾ->ਦੂਜਾ_Glyph = (*ਘੇਰਾ->glyph_map)[ਦੂਜਾ_Glyph];

    ਘੇਰਾ->copy_ਮੁੱਲਾਂ (s, values);

    return_trace (ਸੱਚ);
  }

  ਖਾਲੀ collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ਅਚਲ ਮੁੱਲ_Format *ਮੁੱਲ_Formats,
                                  ਅਚਲ ਮੁੱਲ_ਅਧਾਰ *ਅਧਾਰ) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ record1_len = ਮੁੱਲ_Formats[0].get_len ();
    ਬਿਨਾਨਿਸ਼ਾਨੀ record2_len = ਮੁੱਲ_Formats[1].get_len ();
    ਅਚਲ hb_array_t<ਅਚਲ ਮੁੱਲ> ਮੁੱਲਾਂ_array = values.as_array (record1_len + record2_len);

    ਜੇ (ਮੁੱਲ_Formats[0].has_device ())
      ਮੁੱਲ_Formats[0].collect_variation_indices (c, ਅਧਾਰ, ਮੁੱਲਾਂ_array.sub_array (0, record1_len));

    ਜੇ (ਮੁੱਲ_Formats[1].has_device ())
      ਮੁੱਲ_Formats[1].collect_variation_indices (c, ਅਧਾਰ, ਮੁੱਲਾਂ_array.sub_array (record1_len, record2_len));
  }

  ਤਾਰਕਿਕ intersects (ਅਚਲ hb_ਸੈੱਟ_t& glyphset) ਅਚਲ
  {
    ਵਾਪਸ glyphset.has(ਦੂਜਾ_Glyph);
  }

  ਅਚਲ ਮੁੱਲ* get_ਮੁੱਲਾਂ_1 () ਅਚਲ
  {
    ਵਾਪਸ &values[0];
  }

  ਅਚਲ ਮੁੱਲ* get_ਮੁੱਲਾਂ_2 (ਮੁੱਲ_Format format1) ਅਚਲ
  {
    ਵਾਪਸ &values[format1.get_len ()];
  }
};


}
}
}

#ਜੇ_ਅੰਤ  // OT_LAYOUT_GPOS_PAIRVALUERECORD_HH
