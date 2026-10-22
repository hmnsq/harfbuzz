#ifndef OT_LAYOUT_GSUB_SINGLESUBST_HH
#define OT_LAYOUT_GSUB_SINGLESUBST_HH

#ଅନ୍ତର୍ଭାଵ "Common.hh"
#ଅନ୍ତର୍ଭାଵ "SingleSubstFormat1.hh"
#ଅନ୍ତର୍ଭାଵ "SingleSubstFormat2.hh"

ନାମାକାଶ OT {
ନାମାକାଶ Layout {
ନାମାକାଶ GSUB_impl {

ସଂରଚନା SingleSubst
{
  ସୁରକ୍ଷିତ:
  ସଂଘ {
  ସଂରଚନା { HBUINT16 v; }		format;         /* Format identifier */
  SingleSubstFormat1_3<SmallTypes>	format1;
  SingleSubstFormat2_4<SmallTypes>	format2;
#ifndef HB_NO_BEYOND_64K
  SingleSubstFormat1_3<MediumTypes>	format3;
  SingleSubstFormat2_4<MediumTypes>	format4;
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
    ସ୍ଥିତି 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
#ifndef HB_NO_BEYOND_64K
    ସ୍ଥିତି 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    ସ୍ଥିତି 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
#endif
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:return_trace (c->default_return_ମୂଲ୍ୟ ());
    }
  }

  template<typename Iterator,
           hb_requires (hb_is_sorted_source_of (Iterator,
                                                ଅଚଳ hb_codepoint_pair_t))>
  ତାର୍କିକ serialize (hb_serialize_context_t *c,
                  Iterator glyphs)
  {
    TRACE_SERIALIZE (ଏହି);
    ଯଦି (unlikely (!c->extend_min (u.format.v))) return_trace (ମିଥ୍ୟା);
    unsigned format = 2;
    unsigned delta = 0;
    ଯଦି (glyphs)
    {
      format = 1;
      hb_codepoint_t mask = 0xFFFFu;

#ifndef HB_NO_BEYOND_64K
       ଯଦି (+ glyphs
	   | hb_map_retains_sorting (hb_second)
	   | hb_filter ([] (hb_codepoint_t gid) { ଫେରନ୍ତୁ gid > 0xFFFFu; }))
       {
	 format += 2;
	 mask = 0xFFFFFFu;
       }
#endif

      auto get_delta = [=] (hb_codepoint_pair_t _)
                       { ଫେରନ୍ତୁ (unsigned) (_.second - _.first) & mask; };
      delta = get_delta (*glyphs);
      ଯଦି (!hb_all (++(+glyphs), delta, get_delta)) format += 1;
    }

    u.format.v = format;
    ବାଛ (u.format.v) {
    ସ୍ଥିତି 1: hb_barrier (); return_trace (u.format1.serialize (c,
                                               + glyphs
                                               | hb_map_retains_sorting (hb_first),
                                               delta));
    ସ୍ଥିତି 2: hb_barrier (); return_trace (u.format2.serialize (c, glyphs));
#ifndef HB_NO_BEYOND_64K
    ସ୍ଥିତି 3: hb_barrier (); return_trace (u.format3.serialize (c,
                                               + glyphs
                                               | hb_map_retains_sorting (hb_first),
                                               delta));
    ସ୍ଥିତି 4: hb_barrier (); return_trace (u.format4.serialize (c, glyphs));
#endif
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:return_trace (ମିଥ୍ୟା);
    }
  }
};

template<typename Iterator>
ସ୍ଥିର ଶୂନ୍ୟ
SingleSubst_serialize (hb_serialize_context_t *c,
                       Iterator it)
{ c->start_embed<SingleSubst> ()->serialize (c, it); }

}
}
}

#endif /* OT_LAYOUT_GSUB_SINGLESUBST_HH */
