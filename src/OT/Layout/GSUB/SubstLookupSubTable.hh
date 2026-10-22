#ifndef OT_LAYOUT_GSUB_SUBSTLOOKUPSUBTABLE_HH
#define OT_LAYOUT_GSUB_SUBSTLOOKUPSUBTABLE_HH

#അന്തര്ഭാവ "Common.hh"
#അന്തര്ഭാവ "SingleSubst.hh"
#അന്തര്ഭാവ "MultipleSubst.hh"
#അന്തര്ഭാവ "AlternateSubst.hh"
#അന്തര്ഭാവ "LigatureSubst.hh"
#അന്തര്ഭാവ "ContextSubst.hh"
#അന്തര്ഭാവ "ChainContextSubst.hh"
#അന്തര്ഭാവ "ExtensionSubst.hh"
#അന്തര്ഭാവ "ReverseChainSingleSubst.hh"

നാമാകാശ OT {
നാമാകാശ Layout {
നാമാകാശ GSUB_impl {

ഘടന SubstLookupSubTable
{
  friend ഘടന ::OT::Lookup;
  friend ഘടന SubstLookup;

  സംരക്ഷിത:
  സംഘം {
  SingleSubst                   single;
  MultipleSubst                 multiple;
  AlternateSubst                alternate;
  LigatureSubst                 ligature;
  ContextSubst                  context;
  ChainContextSubst             chainContext;
  ExtensionSubst                extension;
  ReverseChainSingleSubst       reverseChainContextSingle;
  } u;
  പൊതു:
  DEFINE_SIZE_MIN (0);

  എണ്ണൽ Type {
    Single              = 1,
    Multiple            = 2,
    Alternate           = 3,
    Ligature            = 4,
    Context             = 5,
    ChainContext        = 6,
    Extension           = 7,
    ReverseChainSingle  = 8
  };

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, unsigned പൂർണ്ണസംഖ്യ lookup_type, Ts&&... ds) സ്ഥിരം
  {
    TRACE_DISPATCH (ഇത്, lookup_type);
    തിരഞ്ഞെടുപ്പ് (lookup_type) {
    അവസ്ഥ Single:                hb_barrier (); return_trace (u.single.dispatch (c, std::forward<Ts> (ds)...));
    അവസ്ഥ Multiple:              hb_barrier (); return_trace (u.multiple.dispatch (c, std::forward<Ts> (ds)...));
    അവസ്ഥ Alternate:             hb_barrier (); return_trace (u.alternate.dispatch (c, std::forward<Ts> (ds)...));
    അവസ്ഥ Ligature:              hb_barrier (); return_trace (u.ligature.dispatch (c, std::forward<Ts> (ds)...));
    അവസ്ഥ Context:               hb_barrier (); return_trace (u.context.dispatch (c, std::forward<Ts> (ds)...));
    അവസ്ഥ ChainContext:          hb_barrier (); return_trace (u.chainContext.dispatch (c, std::forward<Ts> (ds)...));
    അവസ്ഥ Extension:             hb_barrier (); return_trace (u.extension.dispatch (c, std::forward<Ts> (ds)...));
    അവസ്ഥ ReverseChainSingle:    hb_barrier (); return_trace (u.reverseChainContextSingle.dispatch (c, std::forward<Ts> (ds)...));
    സ്ഥിരസ്ഥിതി:                    return_trace (c->default_return_മൂല്യം ());
    }
  }

  തർക്കം intersects (സ്ഥിരം hb_set_t *glyphs, unsigned പൂർണ്ണസംഖ്യ lookup_type) സ്ഥിരം
  {
    hb_intersects_context_t c (glyphs);
    തിരികെ dispatch (&c, lookup_type);
  }
};


}
}
}

#endif  /* HB_OT_LAYOUT_GSUB_SUBSTLOOKUPSUBTABLE_HH */
