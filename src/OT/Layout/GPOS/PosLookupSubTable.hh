#ifndef OT_LAYOUT_GPOS_POSLOOKUPSUBTABLE_HH
#define OT_LAYOUT_GPOS_POSLOOKUPSUBTABLE_HH

#ଅନ୍ତର୍ଭାଵ "SinglePos.hh"
#ଅନ୍ତର୍ଭାଵ "PairPos.hh"
#ଅନ୍ତର୍ଭାଵ "CursivePos.hh"
#ଅନ୍ତର୍ଭାଵ "MarkBasePos.hh"
#ଅନ୍ତର୍ଭାଵ "MarkLigPos.hh"
#ଅନ୍ତର୍ଭାଵ "MarkMarkPos.hh"
#ଅନ୍ତର୍ଭାଵ "ContextPos.hh"
#ଅନ୍ତର୍ଭାଵ "ChainContextPos.hh"
#ଅନ୍ତର୍ଭାଵ "ExtensionPos.hh"

ନାମାକାଶ OT {
ନାମାକାଶ Layout {
ନାମାକାଶ GPOS_impl {

ସଂରଚନା PosLookupSubTable
{
  friend ସଂରଚନା ::OT::Lookup;
  friend ସଂରଚନା PosLookup;

  ଗଣନା Type {
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
  typename context_t::return_t dispatch (context_t *c, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା lookup_type, Ts&&... ds) ଅଚଳ
  {
    TRACE_DISPATCH (ଏହି, lookup_type);
    ବାଛ (lookup_type) {
    ସ୍ଥିତି Single:                hb_barrier (); return_trace (u.single.dispatch (c, std::forward<Ts> (ds)...));
    ସ୍ଥିତି Pair:                  hb_barrier (); return_trace (u.pair.dispatch (c, std::forward<Ts> (ds)...));
    ସ୍ଥିତି Cursive:               hb_barrier (); return_trace (u.cursive.dispatch (c, std::forward<Ts> (ds)...));
    ସ୍ଥିତି MarkBase:              hb_barrier (); return_trace (u.markBase.dispatch (c, std::forward<Ts> (ds)...));
    ସ୍ଥିତି MarkLig:               hb_barrier (); return_trace (u.markLig.dispatch (c, std::forward<Ts> (ds)...));
    ସ୍ଥିତି MarkMark:              hb_barrier (); return_trace (u.markMark.dispatch (c, std::forward<Ts> (ds)...));
    ସ୍ଥିତି Context:               hb_barrier (); return_trace (u.context.dispatch (c, std::forward<Ts> (ds)...));
    ସ୍ଥିତି ChainContext:          hb_barrier (); return_trace (u.chainContext.dispatch (c, std::forward<Ts> (ds)...));
    ସ୍ଥିତି Extension:             hb_barrier (); return_trace (u.extension.dispatch (c, std::forward<Ts> (ds)...));
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:                    return_trace (c->default_return_ମୂଲ୍ୟ ());
    }
  }

  ତାର୍କିକ intersects (ଅଚଳ hb_set_t *glyphs, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା lookup_type) ଅଚଳ
  {
    hb_intersects_context_t c (glyphs);
    ଫେରନ୍ତୁ dispatch (&c, lookup_type);
  }

  ସୁରକ୍ଷିତ:
  ସଂଘ {
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
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_MIN (0);
};

}
}
}

#endif  /* HB_OT_LAYOUT_GPOS_POSLOOKUPSUBTABLE_HH */
