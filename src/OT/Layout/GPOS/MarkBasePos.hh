#ifndef OT_LAYOUT_GPOS_MARKBASEPOS_HH
#define OT_LAYOUT_GPOS_MARKBASEPOS_HH

#ضمن "MarkBasePosFormat1.hh"

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {

بنية MarkBasePos
{
  محمي:
  اتحاد {
  بنية { HBUINT16 v; }		format;         /* Format identifier */
  MarkBasePosFormat1_2<SmallTypes>	format1;
#ifndef HB_NO_BEYOND_64K
  MarkBasePosFormat1_2<MediumTypes>	format2;
#endif
  } u;

  عام:
  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    إذا (unlikely (!c->may_dispatch (هذا, &u.format.v))) أرجع c->no_dispatch_return_قىممەت ();
    TRACE_DISPATCH (هذا, u.format.v);
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
#ifndef HB_NO_BEYOND_64K
    حالة 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
#endif
    افتراضي:return_trace (c->default_return_قىممەت ());
    }
  }
};

}
}
}

#endif /* OT_LAYOUT_GPOS_MARKBASEPOS_HH */
