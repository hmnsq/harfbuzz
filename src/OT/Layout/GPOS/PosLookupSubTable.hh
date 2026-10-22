#اگر_غیر_تعریف_شدہ OT_LAYOUT_GPOS_POSLOOKUPSUBTABLE_HH
#تعریف_کرو OT_LAYOUT_GPOS_POSLOOKUPSUBTABLE_HH

#شامل "SinglePos.hh"
#شامل "PairPos.hh"
#شامل "CursivePos.hh"
#شامل "MarkBasePos.hh"
#شامل "MarkLigPos.hh"
#شامل "MarkMarkPos.hh"
#شامل "ContextPos.hh"
#شامل "ChainContextPos.hh"
#شامل "ExtensionPos.hh"

نامخلا OT {
نامخلا Layout {
نامخلا GPOS_impl {

ساخت Pos_Lookup_Sub_جدول
{
  دوست ساخت ::OT::Lookup;
  دوست ساخت PosLookup;

  شمار Type {
    Single              = 1,
    Pair                = 2,
    Cursive             = 3,
    Mark_بنیاد            = 4,
    MarkLig             = 5,
    MarkMark            = 6,
    Context             = 7,
    ChainContext        = 8,
    Extension           = 9
  };

  سانچہ <قسم_نام context_t, قسم_نام ...Ts>
  قسم_نام context_t::return_t dispatch (context_t *c, غیرعلامتی صحیح lookup_type, Ts&&... ds) ثابت
  {
    TRACE_DISPATCH (یہ, lookup_type);
    انتخاب (lookup_type) {
    حالت Single:                hb_barrier (); return_trace (u.single.dispatch (c, std::forward<Ts> (ds)...));
    حالت Pair:                  hb_barrier (); return_trace (u.pair.dispatch (c, std::forward<Ts> (ds)...));
    حالت Cursive:               hb_barrier (); return_trace (u.cursive.dispatch (c, std::forward<Ts> (ds)...));
    حالت Mark_بنیاد:              hb_barrier (); return_trace (u.mark_بنیاد.dispatch (c, std::forward<Ts> (ds)...));
    حالت MarkLig:               hb_barrier (); return_trace (u.markLig.dispatch (c, std::forward<Ts> (ds)...));
    حالت MarkMark:              hb_barrier (); return_trace (u.markMark.dispatch (c, std::forward<Ts> (ds)...));
    حالت Context:               hb_barrier (); return_trace (u.context.dispatch (c, std::forward<Ts> (ds)...));
    حالت ChainContext:          hb_barrier (); return_trace (u.chainContext.dispatch (c, std::forward<Ts> (ds)...));
    حالت Extension:             hb_barrier (); return_trace (u.extension.dispatch (c, std::forward<Ts> (ds)...));
    طےشدہ:                    return_trace (c->default_return_قدر ());
    }
  }

  منطقی intersects (ثابت hb_سیٹ_t *glyphs, غیرعلامتی صحیح lookup_type) ثابت
  {
    hb_intersects_context_t c (glyphs);
    واپس dispatch (&c, lookup_type);
  }

  محفوظ:
  اتحاد {
  SinglePos             single;
  PairPos               pair;
  CursivePos            cursive;
  Mark_بنیاد_Pos           mark_بنیاد;
  MarkLigPos            markLig;
  MarkMarkPos           markMark;
  ContextPos            context;
  ChainContextPos       chainContext;
  ExtensionPos          extension;
  } u;
  عوامی:
  DEFINE_سائز_MIN (0);
};

}
}
}

#اگر_ختم  /* HB_OT_LAYOUT_GPOS_POSLOOKUPSUBTABLE_HH */
