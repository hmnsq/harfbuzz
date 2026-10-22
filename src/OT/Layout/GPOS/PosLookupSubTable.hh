#که_نه_تعریف_شوی OT_LAYOUT_GPOS_POSLOOKUPSUBTABLE_HH
#تعریف_کړه OT_LAYOUT_GPOS_POSLOOKUPSUBTABLE_HH

#ضمن "SinglePos.hh"
#ضمن "PairPos.hh"
#ضمن "CursivePos.hh"
#ضمن "MarkBasePos.hh"
#ضمن "MarkLigPos.hh"
#ضمن "MarkMarkPos.hh"
#ضمن "ContextPos.hh"
#ضمن "ChainContextPos.hh"
#ضمن "ExtensionPos.hh"

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {

بنية Pos_Lookup_Sub_جدول
{
  ملګری بنية ::OT::Lookup;
  ملګری بنية PosLookup;

  تعداد Type {
    Single              = 1,
    Pair                = 2,
    Cursive             = 3,
    Mark_بنسټ            = 4,
    MarkLig             = 5,
    MarkMark            = 6,
    Context             = 7,
    ChainContext        = 8,
    Extension           = 9
  };

  کينډۍ <ډول_نوم context_t, ډول_نوم ...Ts>
  ډول_نوم context_t::return_t dispatch (context_t *c, بینښه صحيح lookup_type, Ts&&... ds) ثابت
  {
    TRACE_DISPATCH (هذا, lookup_type);
    بدل (lookup_type) {
    حالة Single:                hb_barrier (); return_trace (u.single.dispatch (c, std::forward<Ts> (ds)...));
    حالة Pair:                  hb_barrier (); return_trace (u.pair.dispatch (c, std::forward<Ts> (ds)...));
    حالة Cursive:               hb_barrier (); return_trace (u.cursive.dispatch (c, std::forward<Ts> (ds)...));
    حالة Mark_بنسټ:              hb_barrier (); return_trace (u.mark_بنسټ.dispatch (c, std::forward<Ts> (ds)...));
    حالة MarkLig:               hb_barrier (); return_trace (u.markLig.dispatch (c, std::forward<Ts> (ds)...));
    حالة MarkMark:              hb_barrier (); return_trace (u.markMark.dispatch (c, std::forward<Ts> (ds)...));
    حالة Context:               hb_barrier (); return_trace (u.context.dispatch (c, std::forward<Ts> (ds)...));
    حالة ChainContext:          hb_barrier (); return_trace (u.chainContext.dispatch (c, std::forward<Ts> (ds)...));
    حالة Extension:             hb_barrier (); return_trace (u.extension.dispatch (c, std::forward<Ts> (ds)...));
    افتراضي:                    return_trace (c->default_return_ارزښت ());
    }
  }

  منطقي intersects (ثابت hb_سېټ_t *glyphs, بینښه صحيح lookup_type) ثابت
  {
    hb_intersects_context_t c (glyphs);
    أرجع dispatch (&c, lookup_type);
  }

  محمي:
  اتحاد {
  SinglePos             single;
  PairPos               pair;
  CursivePos            cursive;
  Mark_بنسټ_Pos           mark_بنسټ;
  MarkLigPos            markLig;
  MarkMarkPos           markMark;
  ContextPos            context;
  ChainContextPos       chainContext;
  ExtensionPos          extension;
  } u;
  عام:
  DEFINE_اندازه_MIN (0);
};

}
}
}

#د_که_پای  /* HB_OT_LAYOUT_GPOS_POSLOOKUPSUBTABLE_HH */
