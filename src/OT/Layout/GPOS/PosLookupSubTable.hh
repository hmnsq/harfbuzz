#ifndef OT_LAYOUT_GPOS_POSLOOKUPSUBTABLE_HH
#define OT_LAYOUT_GPOS_POSLOOKUPSUBTABLE_HH

#અન્તર્ભાવ "SinglePos.hh"
#અન્તર્ભાવ "PairPos.hh"
#અન્તર્ભાવ "CursivePos.hh"
#અન્તર્ભાવ "MarkBasePos.hh"
#અન્તર્ભાવ "MarkLigPos.hh"
#અન્તર્ભાવ "MarkMarkPos.hh"
#અન્તર્ભાવ "ContextPos.hh"
#અન્તર્ભાવ "ChainContextPos.hh"
#અન્તર્ભાવ "ExtensionPos.hh"

નામાકાશ OT {
નામાકાશ Layout {
નામાકાશ GPOS_impl {

સંરચના PosLookupSubTable
{
  friend સંરચના ::OT::Lookup;
  friend સંરચના PosLookup;

  ગણના Type {
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
  typename context_t::return_t dispatch (context_t *c, unsigned પૂર્ણાંક lookup_type, Ts&&... ds) અચલ
  {
    TRACE_DISPATCH (આ, lookup_type);
    પસંદગી (lookup_type) {
    સ્થિતિ Single:                hb_barrier (); return_trace (u.single.dispatch (c, std::forward<Ts> (ds)...));
    સ્થિતિ Pair:                  hb_barrier (); return_trace (u.pair.dispatch (c, std::forward<Ts> (ds)...));
    સ્થિતિ Cursive:               hb_barrier (); return_trace (u.cursive.dispatch (c, std::forward<Ts> (ds)...));
    સ્થિતિ MarkBase:              hb_barrier (); return_trace (u.markBase.dispatch (c, std::forward<Ts> (ds)...));
    સ્થિતિ MarkLig:               hb_barrier (); return_trace (u.markLig.dispatch (c, std::forward<Ts> (ds)...));
    સ્થિતિ MarkMark:              hb_barrier (); return_trace (u.markMark.dispatch (c, std::forward<Ts> (ds)...));
    સ્થિતિ Context:               hb_barrier (); return_trace (u.context.dispatch (c, std::forward<Ts> (ds)...));
    સ્થિતિ ChainContext:          hb_barrier (); return_trace (u.chainContext.dispatch (c, std::forward<Ts> (ds)...));
    સ્થિતિ Extension:             hb_barrier (); return_trace (u.extension.dispatch (c, std::forward<Ts> (ds)...));
    મૂળભૂત:                    return_trace (c->default_return_કિંમત ());
    }
  }

  તાર્કિક intersects (અચલ hb_set_t *glyphs, unsigned પૂર્ણાંક lookup_type) અચલ
  {
    hb_intersects_context_t c (glyphs);
    પાછા dispatch (&c, lookup_type);
  }

  સુરક્ષિત:
  સંઘ {
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
  જાહેર:
  DEFINE_SIZE_MIN (0);
};

}
}
}

#endif  /* HB_OT_LAYOUT_GPOS_POSLOOKUPSUBTABLE_HH */
