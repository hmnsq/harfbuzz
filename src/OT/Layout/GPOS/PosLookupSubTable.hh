#جيڪڏهن_وصف_نه_ٿيل OT_LAYOUT_GPOS_POSLOOKUPSUBTABLE_HH
#وصف_ڪريو OT_LAYOUT_GPOS_POSLOOKUPSUBTABLE_HH

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
  دوست بنية ::OT::Lookup;
  دوست بنية PosLookup;

  تعداد Type {
    Single              = 1,
    Pair                = 2,
    Cursive             = 3,
    Mark_بنياد            = 4,
    MarkLig             = 5,
    MarkMark            = 6,
    Context             = 7,
    ChainContext        = 8,
    Extension           = 9
  };

  سانچو <قسم_نالو context_t, قسم_نالو ...Ts>
  قسم_نالو context_t::return_t dispatch (context_t *c, بنانشان صحيح lookup_type, Ts&&... ds) ثابت
  {
    TRACE_DISPATCH (هذا, lookup_type);
    بدل (lookup_type) {
    حالة Single:                hb_barrier (); return_trace (u.single.dispatch (c, std::forward<Ts> (ds)...));
    حالة Pair:                  hb_barrier (); return_trace (u.pair.dispatch (c, std::forward<Ts> (ds)...));
    حالة Cursive:               hb_barrier (); return_trace (u.cursive.dispatch (c, std::forward<Ts> (ds)...));
    حالة Mark_بنياد:              hb_barrier (); return_trace (u.mark_بنياد.dispatch (c, std::forward<Ts> (ds)...));
    حالة MarkLig:               hb_barrier (); return_trace (u.markLig.dispatch (c, std::forward<Ts> (ds)...));
    حالة MarkMark:              hb_barrier (); return_trace (u.markMark.dispatch (c, std::forward<Ts> (ds)...));
    حالة Context:               hb_barrier (); return_trace (u.context.dispatch (c, std::forward<Ts> (ds)...));
    حالة ChainContext:          hb_barrier (); return_trace (u.chainContext.dispatch (c, std::forward<Ts> (ds)...));
    حالة Extension:             hb_barrier (); return_trace (u.extension.dispatch (c, std::forward<Ts> (ds)...));
    افتراضي:                    return_trace (c->default_return_قدر ());
    }
  }

  منطقي intersects (ثابت hb_سيٽ_t *glyphs, بنانشان صحيح lookup_type) ثابت
  {
    hb_intersects_context_t c (glyphs);
    أرجع dispatch (&c, lookup_type);
  }

  محمي:
  اتحاد {
  SinglePos             single;
  PairPos               pair;
  CursivePos            cursive;
  Mark_بنياد_Pos           mark_بنياد;
  MarkLigPos            markLig;
  MarkMarkPos           markMark;
  ContextPos            context;
  ChainContextPos       chainContext;
  ExtensionPos          extension;
  } u;
  عام:
  DEFINE_سائيز_MIN (0);
};

}
}
}

#جيڪڏهن_پورو  /* HB_OT_LAYOUT_GPOS_POSLOOKUPSUBTABLE_HH */
