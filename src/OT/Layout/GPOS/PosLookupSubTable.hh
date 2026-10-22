#ifndef OT_LAYOUT_GPOS_POSLOOKUPSUBTABLE_HH
#define OT_LAYOUT_GPOS_POSLOOKUPSUBTABLE_HH

#شامل "SinglePos.hh"
#شامل "PairPos.hh"
#شامل "CursivePos.hh"
#شامل "MarkBasePos.hh"
#شامل "MarkLigPos.hh"
#شامل "MarkMarkPos.hh"
#شامل "ContextPos.hh"
#شامل "ChainContextPos.hh"
#شامل "ExtensionPos.hh"

فضاینام OT {
فضاینام Layout {
فضاینام GPOS_impl {

ساختار PosLookupSubTable
{
  friend ساختار ::OT::Lookup;
  friend ساختار PosLookup;

  شمارش Type {
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
  typename context_t::return_t dispatch (context_t *c, unsigned صحیح lookup_type, Ts&&... ds) ثابت
  {
    TRACE_DISPATCH (این, lookup_type);
    انتخاب (lookup_type) {
    حالت Single:                hb_barrier (); return_trace (u.single.dispatch (c, std::forward<Ts> (ds)...));
    حالت Pair:                  hb_barrier (); return_trace (u.pair.dispatch (c, std::forward<Ts> (ds)...));
    حالت Cursive:               hb_barrier (); return_trace (u.cursive.dispatch (c, std::forward<Ts> (ds)...));
    حالت MarkBase:              hb_barrier (); return_trace (u.markBase.dispatch (c, std::forward<Ts> (ds)...));
    حالت MarkLig:               hb_barrier (); return_trace (u.markLig.dispatch (c, std::forward<Ts> (ds)...));
    حالت MarkMark:              hb_barrier (); return_trace (u.markMark.dispatch (c, std::forward<Ts> (ds)...));
    حالت Context:               hb_barrier (); return_trace (u.context.dispatch (c, std::forward<Ts> (ds)...));
    حالت ChainContext:          hb_barrier (); return_trace (u.chainContext.dispatch (c, std::forward<Ts> (ds)...));
    حالت Extension:             hb_barrier (); return_trace (u.extension.dispatch (c, std::forward<Ts> (ds)...));
    پیشفرض:                    return_trace (c->default_return_مقدار ());
    }
  }

  منطقی intersects (ثابت hb_set_t *glyphs, unsigned صحیح lookup_type) ثابت
  {
    hb_intersects_context_t c (glyphs);
    بازگشت dispatch (&c, lookup_type);
  }

  محافظتشده:
  اجتماع {
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
  عمومی:
  DEFINE_SIZE_MIN (0);
};

}
}
}

#endif  /* HB_OT_LAYOUT_GPOS_POSLOOKUPSUBTABLE_HH */
