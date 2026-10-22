#ifndef OT_LAYOUT_GSUB_ALTERNATESUBST_HH
#define OT_LAYOUT_GSUB_ALTERNATESUBST_HH

#ଅନ୍ତର୍ଭାଵ "AlternateSubstFormat1.hh"
#ଅନ୍ତର୍ଭାଵ "Common.hh"

ନାମାକାଶ OT {
ନାମାକାଶ Layout {
ନାମାକାଶ GSUB_impl {

ସଂରଚନା AlternateSubst
{
  ସୁରକ୍ଷିତ:
  ସଂଘ {
  ସଂରଚନା { HBUINT16 v; }		format;         /* Format identifier */
  AlternateSubstFormat1_2<SmallTypes>	format1;
#ifndef HB_NO_BEYOND_64K
  AlternateSubstFormat1_2<MediumTypes>	format2;
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

  /* TODO This function is unused and should be done using iterators. While
   * at it perhaps using iterator of arrays of hb_codepoint_t instead. */
  template <typename GlyphID>
  ତାର୍କିକ serialize (hb_serialize_context_t *c,
                  hb_sorted_array_t<GlyphID> glyphs,
                  hb_array_t<ଅଚଳ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା> alternate_len_list,
                  hb_array_t<GlyphID> alternate_glyphs_list)
  {
    TRACE_SERIALIZE (ଏହି);
    ଯଦି (unlikely (!c->extend_min (u.format.v))) return_trace (ମିଥ୍ୟା);
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା format = 1;
    u.format.v = format;
    ବାଛ (u.format.v) {
    ସ୍ଥିତି 1: hb_barrier (); return_trace (u.format1.serialize (c, glyphs, alternate_len_list, alternate_glyphs_list));
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:return_trace (ମିଥ୍ୟା);
    }
  }

  /* TODO subset() should choose format. */

};

}
}
}

#endif  /* OT_LAYOUT_GSUB_ALTERNATESUBST_HH */
