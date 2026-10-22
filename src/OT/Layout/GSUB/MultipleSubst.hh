#ifndef OT_LAYOUT_GSUB_MULTIPLESUBST_HH
#define OT_LAYOUT_GSUB_MULTIPLESUBST_HH

#ଅନ୍ତର୍ଭାଵ "Common.hh"
#ଅନ୍ତର୍ଭାଵ "MultipleSubstFormat1.hh"

ନାମାକାଶ OT {
ନାମାକାଶ Layout {
ନାମାକାଶ GSUB_impl {

ସଂରଚନା MultipleSubst
{
  ସୁରକ୍ଷିତ:
  ସଂଘ {
  ସଂରଚନା { HBUINT16 v; }		format;         /* Format identifier */
  MultipleSubstFormat1_2<SmallTypes>	format1;
#ifndef HB_NO_BEYOND_64K
  MultipleSubstFormat1_2<MediumTypes>	format2;
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

  template<typename Iterator,
           hb_requires (hb_is_sorted_iterator (Iterator))>
  ତାର୍କିକ serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    TRACE_SERIALIZE (ଏହି);
    ଯଦି (unlikely (!c->extend_min (u.format.v))) return_trace (ମିଥ୍ୟା);
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା format = 1;
    u.format.v = format;
    ବାଛ (u.format.v) {
    ସ୍ଥିତି 1: hb_barrier (); return_trace (u.format1.serialize (c, it));
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:return_trace (ମିଥ୍ୟା);
    }
  }

  /* TODO subset() should choose format. */

};


}
}
}

#endif /* OT_LAYOUT_GSUB_MULTIPLESUBST_HH */
