#ifndef OT_LAYOUT_GSUB_LIGATURESUBST_HH
#define OT_LAYOUT_GSUB_LIGATURESUBST_HH

#अन्तर्भाव "Common.hh"
#अन्तर्भाव "LigatureSubstFormat1.hh"

नामाकाश OT {
नामाकाश Layout {
नामाकाश GSUB_impl {

संरचना LigatureSubst
{
  रक्षित:
  सङ्घ {
  संरचना { HBUINT16 v; }		format;         /* Format identifier */
  LigatureSubstFormat1_2<SmallTypes>	format1;
#ifndef HB_NO_BEYOND_64K
  LigatureSubstFormat1_2<MediumTypes>	format2;
#endif
  } u;

  सार्वजनिक:
  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ध्रुव
  {
    जइ (unlikely (!c->may_dispatch (एतत्, &u.format.v))) प्रत्यागम c->no_dispatch_return_मानम् ();
    TRACE_DISPATCH (एतत्, u.format.v);
    विकल्प (u.format.v) {
    पक्ष 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
#ifndef HB_NO_BEYOND_64K
    पक्ष 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
#endif
    स्वभाव:return_trace (c->default_return_मानम् ());
    }
  }

  /* TODO This function should be done using iterators. While at it perhaps
   * using iterator of arrays of hb_codepoint_t instead. */
  template <typename GlyphID>
  तार्किक serialize (hb_serialize_context_t *c,
                  hb_sorted_array_t<GlyphID> first_glyphs,
                  hb_array_t<ध्रुव unsigned पूर्णाङ्क> ligature_per_first_glyph_गणनम्_list,
                  hb_array_t<GlyphID> ligatures_list,
                  hb_array_t<ध्रुव unsigned पूर्णाङ्क> component_गणनम्_list,
                  hb_array_t<GlyphID> component_list /* Starting from second for each ligature */)
  {
    TRACE_SERIALIZE (एतत्);
    जइ (unlikely (!c->extend_min (u.format.v))) return_trace (मिच्छ);
    unsigned पूर्णाङ्क format = 1;
    u.format.v = format;
    विकल्प (u.format.v) {
    पक्ष 1: hb_barrier (); return_trace (u.format1.serialize (c,
                                               first_glyphs,
                                               ligature_per_first_glyph_गणनम्_list,
                                               ligatures_list,
                                               component_गणनम्_list,
                                               component_list));
    स्वभाव:return_trace (मिच्छ);
    }
  }

  /* TODO subset() should choose format. */

};


}
}
}

#endif  /* OT_LAYOUT_GSUB_LIGATURESUBST_HH */
