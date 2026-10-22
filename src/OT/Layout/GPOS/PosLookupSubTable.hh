#ifndef OT_LAYOUT_GPOS_POSLOOKUPSUBTABLE_HH
#define OT_LAYOUT_GPOS_POSLOOKUPSUBTABLE_HH

#অন্তর্ভাৱ "SinglePos.hh"
#অন্তর্ভাৱ "PairPos.hh"
#অন্তর্ভাৱ "CursivePos.hh"
#অন্তর্ভাৱ "MarkBasePos.hh"
#অন্তর্ভাৱ "MarkLigPos.hh"
#অন্তর্ভাৱ "MarkMarkPos.hh"
#অন্তর্ভাৱ "ContextPos.hh"
#অন্তর্ভাৱ "ChainContextPos.hh"
#অন্তর্ভাৱ "ExtensionPos.hh"

নামাকাশ OT {
নামাকাশ Layout {
নামাকাশ GPOS_impl {

সংরচনা PosLookupSubTable
{
  friend সংরচনা ::OT::Lookup;
  friend সংরচনা PosLookup;

  গণনা Type {
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
  typename context_t::return_t dispatch (context_t *c, unsigned পূর্ণাঙ্ক lookup_type, Ts&&... ds) ধ্রুৱ
  {
    TRACE_DISPATCH (এতত্, lookup_type);
    ৱিকল্প (lookup_type) {
    পক্ষ Single:                hb_barrier (); return_trace (u.single.dispatch (c, std::forward<Ts> (ds)...));
    পক্ষ Pair:                  hb_barrier (); return_trace (u.pair.dispatch (c, std::forward<Ts> (ds)...));
    পক্ষ Cursive:               hb_barrier (); return_trace (u.cursive.dispatch (c, std::forward<Ts> (ds)...));
    পক্ষ MarkBase:              hb_barrier (); return_trace (u.markBase.dispatch (c, std::forward<Ts> (ds)...));
    পক্ষ MarkLig:               hb_barrier (); return_trace (u.markLig.dispatch (c, std::forward<Ts> (ds)...));
    পক্ষ MarkMark:              hb_barrier (); return_trace (u.markMark.dispatch (c, std::forward<Ts> (ds)...));
    পক্ষ Context:               hb_barrier (); return_trace (u.context.dispatch (c, std::forward<Ts> (ds)...));
    পক্ষ ChainContext:          hb_barrier (); return_trace (u.chainContext.dispatch (c, std::forward<Ts> (ds)...));
    পক্ষ Extension:             hb_barrier (); return_trace (u.extension.dispatch (c, std::forward<Ts> (ds)...));
    স্ৱভাৱ:                    return_trace (c->default_return_মান ());
    }
  }

  তার্কিক intersects (ধ্রুৱ hb_set_t *glyphs, unsigned পূর্ণাঙ্ক lookup_type) ধ্রুৱ
  {
    hb_intersects_context_t c (glyphs);
    ঘূৰাই dispatch (&c, lookup_type);
  }

  রক্ষিত:
  সঙ্ঘ {
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
  সার্ৱজনিক:
  DEFINE_SIZE_MIN (0);
};

}
}
}

#endif  /* HB_OT_LAYOUT_GPOS_POSLOOKUPSUBTABLE_HH */
