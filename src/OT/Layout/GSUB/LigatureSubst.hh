#ifndef OT_LAYOUT_GSUB_LIGATURESUBST_HH
#define OT_LAYOUT_GSUB_LIGATURESUBST_HH

#ଅନ୍ତର୍ଭାଵ "Common.hh"
#ଅନ୍ତର୍ଭାଵ "LigatureSubstFormat1.hh"

ନାମାକାଶ OT {
ନାମାକାଶ Layout {
ନାମାକାଶ GSUB_impl {

ସଂରଚନା LigatureSubst
{
  ସୁରକ୍ଷିତ:
  ସଂଘ {
  ସଂରଚନା { HBUINT16 v; }		format;         /* Format identifier */
  LigatureSubstFormat1_2<SmallTypes>	format1;
#ifndef HB_NO_BEYOND_64K
  LigatureSubstFormat1_2<MediumTypes>	format2;
#endif
  } u;

  ସର୍ବସାଧାରଣ:
  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ଅଚଳ
  {
    ଯଦି (unlikely (!c->may_dispatch (ଏହି, &u.format.v))) ଫେରନ୍ତୁ c->no_dispatch_return_ମୂଲ୍ୟ ();
    TRACE_DISPATCH (ଏହି, u.format.v);
    ବାଛ (u.format.v) {
    ସ୍ଥିତି 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
#ifndef HB_NO_BEYOND_64K
    ସ୍ଥିତି 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
#endif
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:return_trace (c->default_return_ମୂଲ୍ୟ ());
    }
  }

  /* TODO This function should be done using iterators. While at it perhaps
   * using iterator of arrays of hb_codepoint_t instead. */
  template <typename GlyphID>
  ତାର୍କିକ serialize (hb_serialize_context_t *c,
                  hb_sorted_array_t<GlyphID> first_glyphs,
                  hb_array_t<ଅଚଳ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା> ligature_per_first_glyph_ଗଣନା_list,
                  hb_array_t<GlyphID> ligatures_list,
                  hb_array_t<ଅଚଳ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା> component_ଗଣନା_list,
                  hb_array_t<GlyphID> component_list /* Starting from second for each ligature */)
  {
    TRACE_SERIALIZE (ଏହି);
    ଯଦି (unlikely (!c->extend_min (u.format.v))) return_trace (ମିଥ୍ୟା);
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା format = 1;
    u.format.v = format;
    ବାଛ (u.format.v) {
    ସ୍ଥିତି 1: hb_barrier (); return_trace (u.format1.serialize (c,
                                               first_glyphs,
                                               ligature_per_first_glyph_ଗଣନା_list,
                                               ligatures_list,
                                               component_ଗଣନା_list,
                                               component_list));
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:return_trace (ମିଥ୍ୟା);
    }
  }

  /* TODO subset() should choose format. */

};


}
}
}

#endif  /* OT_LAYOUT_GSUB_LIGATURESUBST_HH */
