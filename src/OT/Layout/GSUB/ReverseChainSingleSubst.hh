#اگر_تعریف_نشده OT_LAYOUT_GSUB_REVERSECHAINSINGLESUBST_HH
#تعریف_کن OT_LAYOUT_GSUB_REVERSECHAINSINGLESUBST_HH

#ضمن "Common.hh"
#ضمن "ReverseChainSingleSubstFormat1.hh"

نطاق OT {
نطاق Layout {
نطاق GSUB_impl {

بنية معکوس_Chain_Single_Subst
{
  محمي:
  اتحاد {
  بنية { HBUINT16 v; }                format;         /* Format identifier */
  معکوس_Chain_Single_Subst_Format1_2<SmallTypes>
                                        format1;
#اگر_تعریف_نشده HB_NO_BEYOND_64K
  معکوس_Chain_Single_Subst_Format1_2<MediumTypes>
                                        format2;
#پایان_اگر
  } u;

  عام:
  قالب <نام_نوع context_t, نام_نوع ...Ts>
  نام_نوع context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    إذا (unlikely (!c->may_dispatch (هذا, &u.format.v))) أرجع c->no_dispatch_return_مقدار ();
    TRACE_DISPATCH (هذا, u.format.v);
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    حالة 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
#پایان_اگر
    افتراضي:return_trace (c->default_return_مقدار ());
    }
  }
};

}
}
}

#پایان_اگر  /* HB_OT_LAYOUT_GSUB_REVERSECHAINSINGLESUBST_HH */
