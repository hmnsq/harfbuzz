#ifndef OT_LAYOUT_GSUB_SUBSTLOOKUPSUBTABLE_HH
#define OT_LAYOUT_GSUB_SUBSTLOOKUPSUBTABLE_HH

#অন্তর্ভাব "Common.hh"
#অন্তর্ভাব "SingleSubst.hh"
#অন্তর্ভাব "MultipleSubst.hh"
#অন্তর্ভাব "AlternateSubst.hh"
#অন্তর্ভাব "LigatureSubst.hh"
#অন্তর্ভাব "ContextSubst.hh"
#অন্তর্ভাব "ChainContextSubst.hh"
#অন্তর্ভাব "ExtensionSubst.hh"
#অন্তর্ভাব "ReverseChainSingleSubst.hh"

নামাকাশ OT {
নামাকাশ Layout {
নামাকাশ GSUB_impl {

কাঠামো SubstLookupSubTable
{
  friend কাঠামো ::OT::Lookup;
  friend কাঠামো SubstLookup;

  সুরক্ষিত:
  সংঘ {
  SingleSubst                   single;
  MultipleSubst                 multiple;
  AlternateSubst                alternate;
  LigatureSubst                 ligature;
  ContextSubst                  context;
  ChainContextSubst             chainContext;
  ExtensionSubst                extension;
  ReverseChainSingleSubst       reverseChainContextSingle;
  } u;
  সর্বজনীন:
  DEFINE_আকার_MIN (0);

  গণনা Type {
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
  typename context_t::return_t dispatch (context_t *c, unsigned পূর্ণসংখ্যা lookup_type, Ts&&... ds) ধ্রুবক
  {
    TRACE_DISPATCH (এই, lookup_type);
    নির্বাচন (lookup_type) {
    ক্ষেত্র Single:                hb_barrier (); return_trace (u.single.dispatch (c, std::forward<Ts> (ds)...));
    ক্ষেত্র Multiple:              hb_barrier (); return_trace (u.multiple.dispatch (c, std::forward<Ts> (ds)...));
    ক্ষেত্র Alternate:             hb_barrier (); return_trace (u.alternate.dispatch (c, std::forward<Ts> (ds)...));
    ক্ষেত্র Ligature:              hb_barrier (); return_trace (u.ligature.dispatch (c, std::forward<Ts> (ds)...));
    ক্ষেত্র Context:               hb_barrier (); return_trace (u.context.dispatch (c, std::forward<Ts> (ds)...));
    ক্ষেত্র ChainContext:          hb_barrier (); return_trace (u.chainContext.dispatch (c, std::forward<Ts> (ds)...));
    ক্ষেত্র Extension:             hb_barrier (); return_trace (u.extension.dispatch (c, std::forward<Ts> (ds)...));
    ক্ষেত্র ReverseChainSingle:    hb_barrier (); return_trace (u.reverseChainContextSingle.dispatch (c, std::forward<Ts> (ds)...));
    পূর্বনির্ধারিত:                    return_trace (c->default_return_মান ());
    }
  }

  যৌক্তিক intersects (ধ্রুবক hb_set_t *glyphs, unsigned পূর্ণসংখ্যা lookup_type) ধ্রুবক
  {
    hb_intersects_context_t c (glyphs);
    ফেরত dispatch (&c, lookup_type);
  }
};


}
}
}

#endif  /* HB_OT_LAYOUT_GSUB_SUBSTLOOKUPSUBTABLE_HH */
