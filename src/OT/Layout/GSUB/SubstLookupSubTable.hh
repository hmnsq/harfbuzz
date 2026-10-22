#ifndef OT_LAYOUT_GSUB_SUBSTLOOKUPSUBTABLE_HH
#define OT_LAYOUT_GSUB_SUBSTLOOKUPSUBTABLE_HH

#অন্তর্ভাৱ "Common.hh"
#অন্তর্ভাৱ "SingleSubst.hh"
#অন্তর্ভাৱ "MultipleSubst.hh"
#অন্তর্ভাৱ "AlternateSubst.hh"
#অন্তর্ভাৱ "LigatureSubst.hh"
#অন্তর্ভাৱ "ContextSubst.hh"
#অন্তর্ভাৱ "ChainContextSubst.hh"
#অন্তর্ভাৱ "ExtensionSubst.hh"
#অন্তর্ভাৱ "ReverseChainSingleSubst.hh"

নামাকাশ OT {
নামাকাশ Layout {
নামাকাশ GSUB_impl {

সংরচনা SubstLookupSubTable
{
  friend সংরচনা ::OT::Lookup;
  friend সংরচনা SubstLookup;

  রক্ষিত:
  সঙ্ঘ {
  SingleSubst                   single;
  MultipleSubst                 multiple;
  AlternateSubst                alternate;
  LigatureSubst                 ligature;
  ContextSubst                  context;
  ChainContextSubst             chainContext;
  ExtensionSubst                extension;
  ReverseChainSingleSubst       reverseChainContextSingle;
  } u;
  সার্ৱজনিক:
  DEFINE_SIZE_MIN (0);

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
  typename context_t::return_t dispatch (context_t *c, unsigned পূর্ণাঙ্ক lookup_type, Ts&&... ds) ধ্রুৱ
  {
    TRACE_DISPATCH (এতত্, lookup_type);
    ৱিকল্প (lookup_type) {
    পক্ষ Single:                hb_barrier (); return_trace (u.single.dispatch (c, std::forward<Ts> (ds)...));
    পক্ষ Multiple:              hb_barrier (); return_trace (u.multiple.dispatch (c, std::forward<Ts> (ds)...));
    পক্ষ Alternate:             hb_barrier (); return_trace (u.alternate.dispatch (c, std::forward<Ts> (ds)...));
    পক্ষ Ligature:              hb_barrier (); return_trace (u.ligature.dispatch (c, std::forward<Ts> (ds)...));
    পক্ষ Context:               hb_barrier (); return_trace (u.context.dispatch (c, std::forward<Ts> (ds)...));
    পক্ষ ChainContext:          hb_barrier (); return_trace (u.chainContext.dispatch (c, std::forward<Ts> (ds)...));
    পক্ষ Extension:             hb_barrier (); return_trace (u.extension.dispatch (c, std::forward<Ts> (ds)...));
    পক্ষ ReverseChainSingle:    hb_barrier (); return_trace (u.reverseChainContextSingle.dispatch (c, std::forward<Ts> (ds)...));
    স্ৱভাৱ:                    return_trace (c->default_return_মান ());
    }
  }

  তার্কিক intersects (ধ্রুৱ hb_set_t *glyphs, unsigned পূর্ণাঙ্ক lookup_type) ধ্রুৱ
  {
    hb_intersects_context_t c (glyphs);
    ঘূৰাই dispatch (&c, lookup_type);
  }
};


}
}
}

#endif  /* HB_OT_LAYOUT_GSUB_SUBSTLOOKUPSUBTABLE_HH */
