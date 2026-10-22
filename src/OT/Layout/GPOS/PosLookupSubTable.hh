#ifndef OT_LAYOUT_GPOS_POSLOOKUPSUBTABLE_HH
#define OT_LAYOUT_GPOS_POSLOOKUPSUBTABLE_HH

#अन्तर्भाव "SinglePos.hh"
#अन्तर्भाव "PairPos.hh"
#अन्तर्भाव "CursivePos.hh"
#अन्तर्भाव "MarkBasePos.hh"
#अन्तर्भाव "MarkLigPos.hh"
#अन्तर्भाव "MarkMarkPos.hh"
#अन्तर्भाव "ContextPos.hh"
#अन्तर्भाव "ChainContextPos.hh"
#अन्तर्भाव "ExtensionPos.hh"

नामाकाश OT {
नामाकाश Layout {
नामाकाश GPOS_impl {

संरचना PosLookupSubTable
{
  friend संरचना ::OT::Lookup;
  friend संरचना PosLookup;

  गणना Type {
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
  typename context_t::return_t dispatch (context_t *c, unsigned पूर्णाङ्क lookup_type, Ts&&... ds) ध्रुव
  {
    TRACE_DISPATCH (एतत्, lookup_type);
    विकल्प (lookup_type) {
    पक्ष Single:                hb_barrier (); return_trace (u.single.dispatch (c, std::forward<Ts> (ds)...));
    पक्ष Pair:                  hb_barrier (); return_trace (u.pair.dispatch (c, std::forward<Ts> (ds)...));
    पक्ष Cursive:               hb_barrier (); return_trace (u.cursive.dispatch (c, std::forward<Ts> (ds)...));
    पक्ष MarkBase:              hb_barrier (); return_trace (u.markBase.dispatch (c, std::forward<Ts> (ds)...));
    पक्ष MarkLig:               hb_barrier (); return_trace (u.markLig.dispatch (c, std::forward<Ts> (ds)...));
    पक्ष MarkMark:              hb_barrier (); return_trace (u.markMark.dispatch (c, std::forward<Ts> (ds)...));
    पक्ष Context:               hb_barrier (); return_trace (u.context.dispatch (c, std::forward<Ts> (ds)...));
    पक्ष ChainContext:          hb_barrier (); return_trace (u.chainContext.dispatch (c, std::forward<Ts> (ds)...));
    पक्ष Extension:             hb_barrier (); return_trace (u.extension.dispatch (c, std::forward<Ts> (ds)...));
    स्वभाव:                    return_trace (c->default_return_मूल्य ());
    }
  }

  तार्किक intersects (ध्रुव hb_set_t *glyphs, unsigned पूर्णाङ्क lookup_type) ध्रुव
  {
    hb_intersects_context_t c (glyphs);
    प्रत्यागम dispatch (&c, lookup_type);
  }

  रक्षित:
  सङ्घ {
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
  सार्वजनिक:
  DEFINE_SIZE_MIN (0);
};

}
}
}

#endif  /* HB_OT_LAYOUT_GPOS_POSLOOKUPSUBTABLE_HH */
