#ifndef OT_LAYOUT_GSUB_SUBSTLOOKUPSUBTABLE_HH
#define OT_LAYOUT_GSUB_SUBSTLOOKUPSUBTABLE_HH

#અન્તર્ભાવ "Common.hh"
#અન્તર્ભાવ "SingleSubst.hh"
#અન્તર્ભાવ "MultipleSubst.hh"
#અન્તર્ભાવ "AlternateSubst.hh"
#અન્તર્ભાવ "LigatureSubst.hh"
#અન્તર્ભાવ "ContextSubst.hh"
#અન્તર્ભાવ "ChainContextSubst.hh"
#અન્તર્ભાવ "ExtensionSubst.hh"
#અન્તર્ભાવ "ReverseChainSingleSubst.hh"

નામાકાશ OT {
નામાકાશ Layout {
નામાકાશ GSUB_impl {

સંરચના SubstLookupSubTable
{
  friend સંરચના ::OT::Lookup;
  friend સંરચના SubstLookup;

  સુરક્ષિત:
  સંઘ {
  SingleSubst                   single;
  MultipleSubst                 multiple;
  AlternateSubst                alternate;
  LigatureSubst                 ligature;
  ContextSubst                  context;
  ChainContextSubst             chainContext;
  ExtensionSubst                extension;
  ReverseChainSingleSubst       reverseChainContextSingle;
  } u;
  જાહેર:
  DEFINE_SIZE_MIN (0);

  ગણના Type {
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
  typename context_t::return_t dispatch (context_t *c, unsigned પૂર્ણાંક lookup_type, Ts&&... ds) અચલ
  {
    TRACE_DISPATCH (આ, lookup_type);
    પસંદગી (lookup_type) {
    સ્થિતિ Single:                hb_barrier (); return_trace (u.single.dispatch (c, std::forward<Ts> (ds)...));
    સ્થિતિ Multiple:              hb_barrier (); return_trace (u.multiple.dispatch (c, std::forward<Ts> (ds)...));
    સ્થિતિ Alternate:             hb_barrier (); return_trace (u.alternate.dispatch (c, std::forward<Ts> (ds)...));
    સ્થિતિ Ligature:              hb_barrier (); return_trace (u.ligature.dispatch (c, std::forward<Ts> (ds)...));
    સ્થિતિ Context:               hb_barrier (); return_trace (u.context.dispatch (c, std::forward<Ts> (ds)...));
    સ્થિતિ ChainContext:          hb_barrier (); return_trace (u.chainContext.dispatch (c, std::forward<Ts> (ds)...));
    સ્થિતિ Extension:             hb_barrier (); return_trace (u.extension.dispatch (c, std::forward<Ts> (ds)...));
    સ્થિતિ ReverseChainSingle:    hb_barrier (); return_trace (u.reverseChainContextSingle.dispatch (c, std::forward<Ts> (ds)...));
    મૂળભૂત:                    return_trace (c->default_return_કિંમત ());
    }
  }

  તાર્કિક intersects (અચલ hb_set_t *glyphs, unsigned પૂર્ણાંક lookup_type) અચલ
  {
    hb_intersects_context_t c (glyphs);
    પાછા dispatch (&c, lookup_type);
  }
};


}
}
}

#endif  /* HB_OT_LAYOUT_GSUB_SUBSTLOOKUPSUBTABLE_HH */
