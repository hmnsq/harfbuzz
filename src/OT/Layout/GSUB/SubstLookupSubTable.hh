#که_نه_تعریف_شوی OT_LAYOUT_GSUB_SUBSTLOOKUPSUBTABLE_HH
#تعریف_کړه OT_LAYOUT_GSUB_SUBSTLOOKUPSUBTABLE_HH

#ضمن "Common.hh"
#ضمن "SingleSubst.hh"
#ضمن "MultipleSubst.hh"
#ضمن "AlternateSubst.hh"
#ضمن "LigatureSubst.hh"
#ضمن "ContextSubst.hh"
#ضمن "ChainContextSubst.hh"
#ضمن "ExtensionSubst.hh"
#ضمن "ReverseChainSingleSubst.hh"

نطاق OT {
نطاق Layout {
نطاق GSUB_impl {

بنية Subst_Lookup_Sub_جدول
{
  ملګری بنية ::OT::Lookup;
  ملګری بنية SubstLookup;

  محمي:
  اتحاد {
  SingleSubst                   single;
  مضرب_Subst                 مضرب_2;
  AlternateSubst                alternate;
  LigatureSubst                 ligature;
  ContextSubst                  context;
  ChainContextSubst             chainContext;
  ExtensionSubst                extension;
  سرچپه_Chain_Single_Subst       سرچپه_Chain_Context_Single;
  } u;
  عام:
  DEFINE_اندازه_MIN (0);

  تعداد Type {
    Single              = 1,
    مضرب            = 2,
    Alternate           = 3,
    Ligature            = 4,
    Context             = 5,
    ChainContext        = 6,
    Extension           = 7,
    سرچپه_Chain_Single  = 8
  };

  کينډۍ <ډول_نوم context_t, ډول_نوم ...Ts>
  ډول_نوم context_t::return_t dispatch (context_t *c, بینښه صحيح lookup_type, Ts&&... ds) ثابت
  {
    TRACE_DISPATCH (هذا, lookup_type);
    بدل (lookup_type) {
    حالة Single:                hb_barrier (); return_trace (u.single.dispatch (c, std::forward<Ts> (ds)...));
    حالة مضرب:              hb_barrier (); return_trace (u.مضرب_2.dispatch (c, std::forward<Ts> (ds)...));
    حالة Alternate:             hb_barrier (); return_trace (u.alternate.dispatch (c, std::forward<Ts> (ds)...));
    حالة Ligature:              hb_barrier (); return_trace (u.ligature.dispatch (c, std::forward<Ts> (ds)...));
    حالة Context:               hb_barrier (); return_trace (u.context.dispatch (c, std::forward<Ts> (ds)...));
    حالة ChainContext:          hb_barrier (); return_trace (u.chainContext.dispatch (c, std::forward<Ts> (ds)...));
    حالة Extension:             hb_barrier (); return_trace (u.extension.dispatch (c, std::forward<Ts> (ds)...));
    حالة سرچپه_Chain_Single:    hb_barrier (); return_trace (u.سرچپه_Chain_Context_Single.dispatch (c, std::forward<Ts> (ds)...));
    افتراضي:                    return_trace (c->default_return_ارزښت ());
    }
  }

  منطقي intersects (ثابت hb_سېټ_t *glyphs, بینښه صحيح lookup_type) ثابت
  {
    hb_intersects_context_t c (glyphs);
    أرجع dispatch (&c, lookup_type);
  }
};


}
}
}

#د_که_پای  /* HB_OT_LAYOUT_GSUB_SUBSTLOOKUPSUBTABLE_HH */
