#ifndef OT_LAYOUT_GSUB_SUBSTLOOKUPSUBTABLE_HH
#define OT_LAYOUT_GSUB_SUBSTLOOKUPSUBTABLE_HH

#شامل "Common.hh"
#شامل "SingleSubst.hh"
#شامل "MultipleSubst.hh"
#شامل "AlternateSubst.hh"
#شامل "LigatureSubst.hh"
#شامل "ContextSubst.hh"
#شامل "ChainContextSubst.hh"
#شامل "ExtensionSubst.hh"
#شامل "ReverseChainSingleSubst.hh"

فضاینام OT {
فضاینام Layout {
فضاینام GSUB_impl {

ساختار SubstLookupSubTable
{
  friend ساختار ::OT::Lookup;
  friend ساختار SubstLookup;

  محافظتشده:
  اجتماع {
  SingleSubst                   single;
  MultipleSubst                 multiple;
  AlternateSubst                alternate;
  LigatureSubst                 ligature;
  ContextSubst                  context;
  ChainContextSubst             chainContext;
  ExtensionSubst                extension;
  ReverseChainSingleSubst       reverseChainContextSingle;
  } u;
  عمومی:
  DEFINE_SIZE_MIN (0);

  شمارش Type {
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
  typename context_t::return_t dispatch (context_t *c, unsigned صحیح lookup_type, Ts&&... ds) ثابت
  {
    TRACE_DISPATCH (این, lookup_type);
    انتخاب (lookup_type) {
    حالت Single:                hb_barrier (); return_trace (u.single.dispatch (c, std::forward<Ts> (ds)...));
    حالت Multiple:              hb_barrier (); return_trace (u.multiple.dispatch (c, std::forward<Ts> (ds)...));
    حالت Alternate:             hb_barrier (); return_trace (u.alternate.dispatch (c, std::forward<Ts> (ds)...));
    حالت Ligature:              hb_barrier (); return_trace (u.ligature.dispatch (c, std::forward<Ts> (ds)...));
    حالت Context:               hb_barrier (); return_trace (u.context.dispatch (c, std::forward<Ts> (ds)...));
    حالت ChainContext:          hb_barrier (); return_trace (u.chainContext.dispatch (c, std::forward<Ts> (ds)...));
    حالت Extension:             hb_barrier (); return_trace (u.extension.dispatch (c, std::forward<Ts> (ds)...));
    حالت ReverseChainSingle:    hb_barrier (); return_trace (u.reverseChainContextSingle.dispatch (c, std::forward<Ts> (ds)...));
    پیشفرض:                    return_trace (c->default_return_مقدار ());
    }
  }

  منطقی intersects (ثابت hb_set_t *glyphs, unsigned صحیح lookup_type) ثابت
  {
    hb_intersects_context_t c (glyphs);
    بازگشت dispatch (&c, lookup_type);
  }
};


}
}
}

#endif  /* HB_OT_LAYOUT_GSUB_SUBSTLOOKUPSUBTABLE_HH */
