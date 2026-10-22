#ifndef OT_LAYOUT_GPOS_PAIRPOS_HH
#define OT_LAYOUT_GPOS_PAIRPOS_HH

#ଅନ୍ତର୍ଭାଵ "PairPosFormat1.hh"
#ଅନ୍ତର୍ଭାଵ "PairPosFormat2.hh"

ନାମାକାଶ OT {
ନାମାକାଶ Layout {
ନାମାକାଶ GPOS_impl {

ସଂରଚନା PairPos
{
  ସୁରକ୍ଷିତ:
  ସଂଘ {
  ସଂରଚନା { HBUINT16 v; }	format;         /* Format identifier */
  PairPosFormat1_3<SmallTypes>	format1;
  PairPosFormat2_4<SmallTypes>	format2;
#ifndef HB_NO_BEYOND_64K
  PairPosFormat1_3<MediumTypes>	format3;
  PairPosFormat2_4<MediumTypes>	format4;
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
};

}
}
}

#endif  // OT_LAYOUT_GPOS_PAIRPOS_HH
