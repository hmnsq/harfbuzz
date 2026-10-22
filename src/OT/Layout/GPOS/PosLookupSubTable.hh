#ifndef OT_LAYOUT_GPOS_POSLOOKUPSUBTABLE_HH
#define OT_LAYOUT_GPOS_POSLOOKUPSUBTABLE_HH

#അന്തര്ഭാവ "SinglePos.hh"
#അന്തര്ഭാവ "PairPos.hh"
#അന്തര്ഭാവ "CursivePos.hh"
#അന്തര്ഭാവ "MarkBasePos.hh"
#അന്തര്ഭാവ "MarkLigPos.hh"
#അന്തര്ഭാവ "MarkMarkPos.hh"
#അന്തര്ഭാവ "ContextPos.hh"
#അന്തര്ഭാവ "ChainContextPos.hh"
#അന്തര്ഭാവ "ExtensionPos.hh"

നാമാകാശ OT {
നാമാകാശ Layout {
നാമാകാശ GPOS_impl {

ഘടന PosLookupSubTable
{
  friend ഘടന ::OT::Lookup;
  friend ഘടന PosLookup;

  എണ്ണൽ Type {
    Single              = 1,
    Pair                = 2,
    Cursive             = 3,
    MarkBase            = 4,
    MarkLig             = 5,
    MarkMark            = 6,
    Context             = 7,
    ChainContext        = 8,
    Extension           = 9
  };

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, unsigned പൂർണ്ണസംഖ്യ lookup_type, Ts&&... ds) സ്ഥിരം
  {
    TRACE_DISPATCH (ഇത്, lookup_type);
    തിരഞ്ഞെടുപ്പ് (lookup_type) {
    അവസ്ഥ Single:                hb_barrier (); return_trace (u.single.dispatch (c, std::forward<Ts> (ds)...));
    അവസ്ഥ Pair:                  hb_barrier (); return_trace (u.pair.dispatch (c, std::forward<Ts> (ds)...));
    അവസ്ഥ Cursive:               hb_barrier (); return_trace (u.cursive.dispatch (c, std::forward<Ts> (ds)...));
    അവസ്ഥ MarkBase:              hb_barrier (); return_trace (u.markBase.dispatch (c, std::forward<Ts> (ds)...));
    അവസ്ഥ MarkLig:               hb_barrier (); return_trace (u.markLig.dispatch (c, std::forward<Ts> (ds)...));
    അവസ്ഥ MarkMark:              hb_barrier (); return_trace (u.markMark.dispatch (c, std::forward<Ts> (ds)...));
    അവസ്ഥ Context:               hb_barrier (); return_trace (u.context.dispatch (c, std::forward<Ts> (ds)...));
    അവസ്ഥ ChainContext:          hb_barrier (); return_trace (u.chainContext.dispatch (c, std::forward<Ts> (ds)...));
    അവസ്ഥ Extension:             hb_barrier (); return_trace (u.extension.dispatch (c, std::forward<Ts> (ds)...));
    സ്ഥിരസ്ഥിതി:                    return_trace (c->default_return_മൂല്യം ());
    }
  }

  തർക്കം intersects (സ്ഥിരം hb_set_t *glyphs, unsigned പൂർണ്ണസംഖ്യ lookup_type) സ്ഥിരം
  {
    hb_intersects_context_t c (glyphs);
    തിരികെ dispatch (&c, lookup_type);
  }

  സംരക്ഷിത:
  സംഘം {
  SinglePos             single;
  PairPos               pair;
  CursivePos            cursive;
  MarkBasePos           markBase;
  MarkLigPos            markLig;
  MarkMarkPos           markMark;
  ContextPos            context;
  ChainContextPos       chainContext;
  ExtensionPos          extension;
  } u;
  പൊതു:
  DEFINE_SIZE_MIN (0);
};

}
}
}

#endif  /* HB_OT_LAYOUT_GPOS_POSLOOKUPSUBTABLE_HH */
