#ifndef OT_LAYOUT_GPOS_POSLOOKUPSUBTABLE_HH
#define OT_LAYOUT_GPOS_POSLOOKUPSUBTABLE_HH

#அந்தர்பாவ "SinglePos.hh"
#அந்தர்பாவ "PairPos.hh"
#அந்தர்பாவ "CursivePos.hh"
#அந்தர்பாவ "MarkBasePos.hh"
#அந்தர்பாவ "MarkLigPos.hh"
#அந்தர்பாவ "MarkMarkPos.hh"
#அந்தர்பாவ "ContextPos.hh"
#அந்தர்பாவ "ChainContextPos.hh"
#அந்தர்பாவ "ExtensionPos.hh"

நாமாகாஶ OT {
நாமாகாஶ Layout {
நாமாகாஶ GPOS_impl {

கட்டமைப்பு PosLookupSubTable
{
  friend கட்டமைப்பு ::OT::Lookup;
  friend கட்டமைப்பு PosLookup;

  எண்ணிக்கை Type {
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
  typename context_t::return_t dispatch (context_t *c, unsigned முழு lookup_type, Ts&&... ds) மாறிலி
  {
    TRACE_DISPATCH (இது, lookup_type);
    தேர்வு (lookup_type) {
    நிலை Single:                hb_barrier (); return_trace (u.single.dispatch (c, std::forward<Ts> (ds)...));
    நிலை Pair:                  hb_barrier (); return_trace (u.pair.dispatch (c, std::forward<Ts> (ds)...));
    நிலை Cursive:               hb_barrier (); return_trace (u.cursive.dispatch (c, std::forward<Ts> (ds)...));
    நிலை MarkBase:              hb_barrier (); return_trace (u.markBase.dispatch (c, std::forward<Ts> (ds)...));
    நிலை MarkLig:               hb_barrier (); return_trace (u.markLig.dispatch (c, std::forward<Ts> (ds)...));
    நிலை MarkMark:              hb_barrier (); return_trace (u.markMark.dispatch (c, std::forward<Ts> (ds)...));
    நிலை Context:               hb_barrier (); return_trace (u.context.dispatch (c, std::forward<Ts> (ds)...));
    நிலை ChainContext:          hb_barrier (); return_trace (u.chainContext.dispatch (c, std::forward<Ts> (ds)...));
    நிலை Extension:             hb_barrier (); return_trace (u.extension.dispatch (c, std::forward<Ts> (ds)...));
    இயல்பு:                    return_trace (c->default_return_மதிப்பு ());
    }
  }

  தருக்க intersects (மாறிலி hb_set_t *glyphs, unsigned முழு lookup_type) மாறிலி
  {
    hb_intersects_context_t c (glyphs);
    திரும்பு dispatch (&c, lookup_type);
  }

  பாதுகாக்கப்பட்ட:
  ஒன்றிப்பு {
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
  பொது:
  DEFINE_SIZE_MIN (0);
};

}
}
}

#endif  /* HB_OT_LAYOUT_GPOS_POSLOOKUPSUBTABLE_HH */
