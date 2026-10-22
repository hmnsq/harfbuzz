#ifndef OT_LAYOUT_GPOS_MARKLIGPOS_HH
#define OT_LAYOUT_GPOS_MARKLIGPOS_HH

#ضمن "MarkLigPosFormat1.hh"

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {

بنية MarkLigPos
{
  محمي:
  اتحاد {
  بنية { HBUINT16 v; }		format;         /* Format identifier */
  MarkLigPosFormat1_2<SmallTypes>	format1;
#ifndef HB_NO_BEYOND_64K
  MarkLigPosFormat1_2<MediumTypes>	format2;
#endif
  } u;

  عام:
  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    ܐܢ (unlikely (!c->may_dispatch (هذا, &u.format.v))) ܦܢܐ c->no_dispatch_return_value ();
    TRACE_DISPATCH (هذا, u.format.v);
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
#ifndef HB_NO_BEYOND_64K
    حالة 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
#endif
    افتراضي:return_trace (c->default_return_value ());
    }
  }
};

}
}
}

#endif /* OT_LAYOUT_GPOS_MARKLIGPOS_HH */
