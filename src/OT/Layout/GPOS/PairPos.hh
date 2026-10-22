#ifndef OT_LAYOUT_GPOS_PAIRPOS_HH
#define OT_LAYOUT_GPOS_PAIRPOS_HH

#അന്തര്ഭാവ "PairPosFormat1.hh"
#അന്തര്ഭാവ "PairPosFormat2.hh"

നാമാകാശ OT {
നാമാകാശ Layout {
നാമാകാശ GPOS_impl {

ഘടന PairPos
{
  സംരക്ഷിത:
  സംഘം {
  ഘടന { HBUINT16 v; }	format;         /* Format identifier */
  PairPosFormat1_3<SmallTypes>	format1;
  PairPosFormat2_4<SmallTypes>	format2;
#ifndef HB_NO_BEYOND_64K
  PairPosFormat1_3<MediumTypes>	format3;
  PairPosFormat2_4<MediumTypes>	format4;
#endif
  } u;

  പൊതു:
  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) സ്ഥിരം
  {
    എങ്കിൽ (unlikely (!c->may_dispatch (ഇത്, &u.format.v))) തിരികെ c->no_dispatch_return_മൂല്യം ();
    TRACE_DISPATCH (ഇത്, u.format.v);
    തിരഞ്ഞെടുപ്പ് (u.format.v) {
    അവസ്ഥ 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    അവസ്ഥ 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
#ifndef HB_NO_BEYOND_64K
    അവസ്ഥ 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    അവസ്ഥ 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
#endif
    സ്ഥിരസ്ഥിതി:return_trace (c->default_return_മൂല്യം ());
    }
  }
};

}
}
}

#endif  // OT_LAYOUT_GPOS_PAIRPOS_HH
