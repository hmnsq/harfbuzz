#جيڪڏهن_وصف_نه_ٿيل OT_LAYOUT_GSUB_SUBSTLOOKUPSUBTABLE_HH
#وصف_ڪريو OT_LAYOUT_GSUB_SUBSTLOOKUPSUBTABLE_HH

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
  دوست بنية ::OT::Lookup;
  دوست بنية SubstLookup;

  محمي:
  اتحاد {
  SingleSubst                   single;
  گڻو_Subst                 گڻو_2;
  AlternateSubst                alternate;
  LigatureSubst                 ligature;
  ContextSubst                  context;
  ChainContextSubst             chainContext;
  ExtensionSubst                extension;
  ابتو_Chain_Single_Subst       ابتو_Chain_Context_Single;
  } u;
  عام:
  DEFINE_سائيز_MIN (0);

  تعداد Type {
    Single              = 1,
    گڻو            = 2,
    Alternate           = 3,
    Ligature            = 4,
    Context             = 5,
    ChainContext        = 6,
    Extension           = 7,
    ابتو_Chain_Single  = 8
  };

  سانچو <قسم_نالو context_t, قسم_نالو ...Ts>
  قسم_نالو context_t::return_t dispatch (context_t *c, بنانشان صحيح lookup_type, Ts&&... ds) ثابت
  {
    TRACE_DISPATCH (هذا, lookup_type);
    بدل (lookup_type) {
    حالة Single:                hb_barrier (); return_trace (u.single.dispatch (c, std::forward<Ts> (ds)...));
    حالة گڻو:              hb_barrier (); return_trace (u.گڻو_2.dispatch (c, std::forward<Ts> (ds)...));
    حالة Alternate:             hb_barrier (); return_trace (u.alternate.dispatch (c, std::forward<Ts> (ds)...));
    حالة Ligature:              hb_barrier (); return_trace (u.ligature.dispatch (c, std::forward<Ts> (ds)...));
    حالة Context:               hb_barrier (); return_trace (u.context.dispatch (c, std::forward<Ts> (ds)...));
    حالة ChainContext:          hb_barrier (); return_trace (u.chainContext.dispatch (c, std::forward<Ts> (ds)...));
    حالة Extension:             hb_barrier (); return_trace (u.extension.dispatch (c, std::forward<Ts> (ds)...));
    حالة ابتو_Chain_Single:    hb_barrier (); return_trace (u.ابتو_Chain_Context_Single.dispatch (c, std::forward<Ts> (ds)...));
    افتراضي:                    return_trace (c->default_return_قدر ());
    }
  }

  منطقي intersects (ثابت hb_سيٽ_t *glyphs, بنانشان صحيح lookup_type) ثابت
  {
    hb_intersects_context_t c (glyphs);
    أرجع dispatch (&c, lookup_type);
  }
};


}
}
}

#جيڪڏهن_پورو  /* HB_OT_LAYOUT_GSUB_SUBSTLOOKUPSUBTABLE_HH */
