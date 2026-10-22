#جے_بنا_تعریف OT_LAYOUT_GSUB_SUBSTLOOKUPSUBTABLE_HH
#تعریف_کرو OT_LAYOUT_GSUB_SUBSTLOOKUPSUBTABLE_HH

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
  یار بنية ::OT::Lookup;
  یار بنية SubstLookup;

  محمي:
  اتحاد {
  SingleSubst                   single;
  مضرب_Subst                 مضرب_2;
  AlternateSubst                alternate;
  LigatureSubst                 ligature;
  ContextSubst                  context;
  ChainContextSubst             chainContext;
  ExtensionSubst                extension;
  الٹ_Chain_Single_Subst       الٹ_Chain_Context_Single;
  } u;
  عام:
  DEFINE_ناپ_MIN (0);

  تعداد Type {
    Single              = 1,
    مضرب            = 2,
    Alternate           = 3,
    Ligature            = 4,
    Context             = 5,
    ChainContext        = 6,
    Extension           = 7,
    الٹ_Chain_Single  = 8
  };

  سانچا <قسم_ناں context_t, قسم_ناں ...Ts>
  قسم_ناں context_t::return_t dispatch (context_t *c, بنانشانی صحيح lookup_type, Ts&&... ds) ثابت
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
    حالة الٹ_Chain_Single:    hb_barrier (); return_trace (u.الٹ_Chain_Context_Single.dispatch (c, std::forward<Ts> (ds)...));
    افتراضي:                    return_trace (c->default_return_قدر ());
    }
  }

  منطقي intersects (ثابت hb_سیٹ_t *glyphs, بنانشانی صحيح lookup_type) ثابت
  {
    hb_intersects_context_t c (glyphs);
    أرجع dispatch (&c, lookup_type);
  }
};


}
}
}

#جے_انت  /* HB_OT_LAYOUT_GSUB_SUBSTLOOKUPSUBTABLE_HH */
