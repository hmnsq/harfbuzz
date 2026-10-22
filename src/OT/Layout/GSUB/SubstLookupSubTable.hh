#ifndef OT_LAYOUT_GSUB_SUBSTLOOKUPSUBTABLE_HH
#define OT_LAYOUT_GSUB_SUBSTLOOKUPSUBTABLE_HH

#ଅନ୍ତର୍ଭାଵ "Common.hh"
#ଅନ୍ତର୍ଭାଵ "SingleSubst.hh"
#ଅନ୍ତର୍ଭାଵ "MultipleSubst.hh"
#ଅନ୍ତର୍ଭାଵ "AlternateSubst.hh"
#ଅନ୍ତର୍ଭାଵ "LigatureSubst.hh"
#ଅନ୍ତର୍ଭାଵ "ContextSubst.hh"
#ଅନ୍ତର୍ଭାଵ "ChainContextSubst.hh"
#ଅନ୍ତର୍ଭାଵ "ExtensionSubst.hh"
#ଅନ୍ତର୍ଭାଵ "ReverseChainSingleSubst.hh"

ନାମାକାଶ OT {
ନାମାକାଶ Layout {
ନାମାକାଶ GSUB_impl {

ସଂରଚନା SubstLookupSubTable
{
  friend ସଂରଚନା ::OT::Lookup;
  friend ସଂରଚନା SubstLookup;

  ସୁରକ୍ଷିତ:
  ସଂଘ {
  SingleSubst                   single;
  MultipleSubst                 multiple;
  AlternateSubst                alternate;
  LigatureSubst                 ligature;
  ContextSubst                  context;
  ChainContextSubst             chainContext;
  ExtensionSubst                extension;
  ReverseChainSingleSubst       reverseChainContextSingle;
  } u;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_MIN (0);

  ଗଣନା Type {
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
  typename context_t::return_t dispatch (context_t *c, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା lookup_type, Ts&&... ds) ଅଚଳ
  {
    TRACE_DISPATCH (ଏହି, lookup_type);
    ବାଛ (lookup_type) {
    ସ୍ଥିତି Single:                hb_barrier (); return_trace (u.single.dispatch (c, std::forward<Ts> (ds)...));
    ସ୍ଥିତି Multiple:              hb_barrier (); return_trace (u.multiple.dispatch (c, std::forward<Ts> (ds)...));
    ସ୍ଥିତି Alternate:             hb_barrier (); return_trace (u.alternate.dispatch (c, std::forward<Ts> (ds)...));
    ସ୍ଥିତି Ligature:              hb_barrier (); return_trace (u.ligature.dispatch (c, std::forward<Ts> (ds)...));
    ସ୍ଥିତି Context:               hb_barrier (); return_trace (u.context.dispatch (c, std::forward<Ts> (ds)...));
    ସ୍ଥିତି ChainContext:          hb_barrier (); return_trace (u.chainContext.dispatch (c, std::forward<Ts> (ds)...));
    ସ୍ଥିତି Extension:             hb_barrier (); return_trace (u.extension.dispatch (c, std::forward<Ts> (ds)...));
    ସ୍ଥିତି ReverseChainSingle:    hb_barrier (); return_trace (u.reverseChainContextSingle.dispatch (c, std::forward<Ts> (ds)...));
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:                    return_trace (c->default_return_ମୂଲ୍ୟ ());
    }
  }

  ତାର୍କିକ intersects (ଅଚଳ hb_set_t *glyphs, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା lookup_type) ଅଚଳ
  {
    hb_intersects_context_t c (glyphs);
    ଫେରନ୍ତୁ dispatch (&c, lookup_type);
  }
};


}
}
}

#endif  /* HB_OT_LAYOUT_GSUB_SUBSTLOOKUPSUBTABLE_HH */
