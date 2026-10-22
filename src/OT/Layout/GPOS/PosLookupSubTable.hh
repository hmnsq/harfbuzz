#ifndef OT_LAYOUT_GPOS_POSLOOKUPSUBTABLE_HH
#define OT_LAYOUT_GPOS_POSLOOKUPSUBTABLE_HH

#অন্তর্ভাব "SinglePos.hh"
#অন্তর্ভাব "PairPos.hh"
#অন্তর্ভাব "CursivePos.hh"
#অন্তর্ভাব "MarkBasePos.hh"
#অন্তর্ভাব "MarkLigPos.hh"
#অন্তর্ভাব "MarkMarkPos.hh"
#অন্তর্ভাব "ContextPos.hh"
#অন্তর্ভাব "ChainContextPos.hh"
#অন্তর্ভাব "ExtensionPos.hh"

নামাকাশ OT {
নামাকাশ Layout {
নামাকাশ GPOS_impl {

কাঠামো PosLookupSubTable
{
  friend কাঠামো ::OT::Lookup;
  friend কাঠামো PosLookup;

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
  typename context_t::return_t dispatch (context_t *c, unsigned পূর্ণসংখ্যা lookup_type, Ts&&... ds) ধ্রুবক
  {
    TRACE_DISPATCH (এই, lookup_type);
    নির্বাচন (lookup_type) {
    ক্ষেত্র Single:                hb_barrier (); return_trace (u.single.dispatch (c, std::forward<Ts> (ds)...));
    ক্ষেত্র Pair:                  hb_barrier (); return_trace (u.pair.dispatch (c, std::forward<Ts> (ds)...));
    ক্ষেত্র Cursive:               hb_barrier (); return_trace (u.cursive.dispatch (c, std::forward<Ts> (ds)...));
    ক্ষেত্র MarkBase:              hb_barrier (); return_trace (u.markBase.dispatch (c, std::forward<Ts> (ds)...));
    ক্ষেত্র MarkLig:               hb_barrier (); return_trace (u.markLig.dispatch (c, std::forward<Ts> (ds)...));
    ক্ষেত্র MarkMark:              hb_barrier (); return_trace (u.markMark.dispatch (c, std::forward<Ts> (ds)...));
    ক্ষেত্র Context:               hb_barrier (); return_trace (u.context.dispatch (c, std::forward<Ts> (ds)...));
    ক্ষেত্র ChainContext:          hb_barrier (); return_trace (u.chainContext.dispatch (c, std::forward<Ts> (ds)...));
    ক্ষেত্র Extension:             hb_barrier (); return_trace (u.extension.dispatch (c, std::forward<Ts> (ds)...));
    পূর্বনির্ধারিত:                    return_trace (c->default_return_মান ());
    }
  }

  যৌক্তিক intersects (ধ্রুবক hb_set_t *glyphs, unsigned পূর্ণসংখ্যা lookup_type) ধ্রুবক
  {
    hb_intersects_context_t c (glyphs);
    ফেরত dispatch (&c, lookup_type);
  }

  সুরক্ষিত:
  সংঘ {
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
  সর্বজনীন:
  DEFINE_আকার_MIN (0);
};

}
}
}

#endif  /* HB_OT_LAYOUT_GPOS_POSLOOKUPSUBTABLE_HH */
