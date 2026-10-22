#ifndef OT_LAYOUT_GSUB_SUBSTLOOKUPSUBTABLE_HH
#define OT_LAYOUT_GSUB_SUBSTLOOKUPSUBTABLE_HH

#அந்தர்பாவ "Common.hh"
#அந்தர்பாவ "SingleSubst.hh"
#அந்தர்பாவ "MultipleSubst.hh"
#அந்தர்பாவ "AlternateSubst.hh"
#அந்தர்பாவ "LigatureSubst.hh"
#அந்தர்பாவ "ContextSubst.hh"
#அந்தர்பாவ "ChainContextSubst.hh"
#அந்தர்பாவ "ExtensionSubst.hh"
#அந்தர்பாவ "ReverseChainSingleSubst.hh"

நாமாகாஶ OT {
நாமாகாஶ Layout {
நாமாகாஶ GSUB_impl {

கட்டமைப்பு SubstLookupSubTable
{
  friend கட்டமைப்பு ::OT::Lookup;
  friend கட்டமைப்பு SubstLookup;

  பாதுகாக்கப்பட்ட:
  ஒன்றிப்பு {
  SingleSubst                   single;
  MultipleSubst                 multiple;
  AlternateSubst                alternate;
  LigatureSubst                 ligature;
  ContextSubst                  context;
  ChainContextSubst             chainContext;
  ExtensionSubst                extension;
  ReverseChainSingleSubst       reverseChainContextSingle;
  } u;
  பொது:
  DEFINE_SIZE_MIN (0);

  எண்ணிக்கை Type {
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
  typename context_t::return_t dispatch (context_t *c, unsigned முழு lookup_type, Ts&&... ds) மாறிலி
  {
    TRACE_DISPATCH (இது, lookup_type);
    தேர்வு (lookup_type) {
    நிலை Single:                hb_barrier (); return_trace (u.single.dispatch (c, std::forward<Ts> (ds)...));
    நிலை Multiple:              hb_barrier (); return_trace (u.multiple.dispatch (c, std::forward<Ts> (ds)...));
    நிலை Alternate:             hb_barrier (); return_trace (u.alternate.dispatch (c, std::forward<Ts> (ds)...));
    நிலை Ligature:              hb_barrier (); return_trace (u.ligature.dispatch (c, std::forward<Ts> (ds)...));
    நிலை Context:               hb_barrier (); return_trace (u.context.dispatch (c, std::forward<Ts> (ds)...));
    நிலை ChainContext:          hb_barrier (); return_trace (u.chainContext.dispatch (c, std::forward<Ts> (ds)...));
    நிலை Extension:             hb_barrier (); return_trace (u.extension.dispatch (c, std::forward<Ts> (ds)...));
    நிலை ReverseChainSingle:    hb_barrier (); return_trace (u.reverseChainContextSingle.dispatch (c, std::forward<Ts> (ds)...));
    இயல்பு:                    return_trace (c->default_return_மதிப்பு ());
    }
  }

  தருக்க intersects (மாறிலி hb_set_t *glyphs, unsigned முழு lookup_type) மாறிலி
  {
    hb_intersects_context_t c (glyphs);
    திரும்பு dispatch (&c, lookup_type);
  }
};


}
}
}

#endif  /* HB_OT_LAYOUT_GSUB_SUBSTLOOKUPSUBTABLE_HH */
