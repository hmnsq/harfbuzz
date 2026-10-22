#ifndef OT_LAYOUT_GSUB_REVERSECHAINSINGLESUBST_HH
#define OT_LAYOUT_GSUB_REVERSECHAINSINGLESUBST_HH

#അന്തര്ഭാവ "Common.hh"
#അന്തര്ഭാവ "ReverseChainSingleSubstFormat1.hh"

നാമാകാശ OT {
നാമാകാശ Layout {
നാമാകാശ GSUB_impl {

ഘടന ReverseChainSingleSubst
{
  സംരക്ഷിത:
  സംഘം {
  ഘടന { HBUINT16 v; }                format;         /* Format identifier */
  ReverseChainSingleSubstFormat1_2<SmallTypes>
                                        format1;
#ifndef HB_NO_BEYOND_64K
  ReverseChainSingleSubstFormat1_2<MediumTypes>
                                        format2;
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
#ifndef HB_NO_BEYOND_64K
    അവസ്ഥ 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
#endif
    സ്ഥിരസ്ഥിതി:return_trace (c->default_return_മൂല്യം ());
    }
  }
};

}
}
}

#endif  /* HB_OT_LAYOUT_GSUB_REVERSECHAINSINGLESUBST_HH */
