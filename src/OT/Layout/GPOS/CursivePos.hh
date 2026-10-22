#که_نه_تعریف_شوی OT_LAYOUT_GPOS_CURSIVEPOS_HH
#تعریف_کړه OT_LAYOUT_GPOS_CURSIVEPOS_HH

#ضمن "CursivePosFormat1.hh"

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {

بنية CursivePos
{
  محمي:
  اتحاد {
  بنية { HBUINT16 v; } format;        /* Format identifier */
  CursivePosFormat1_2<SmallTypes> format1;
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
  CursivePosFormat1_2<MediumTypes> format2;
#د_که_پای
  } u;

  عام:
  کينډۍ <ډول_نوم context_t, ډول_نوم ...Ts>
  ډول_نوم context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    إذا (unlikely (!c->may_dispatch (هذا, &u.format.v))) أرجع c->no_dispatch_return_ارزښت ();
    TRACE_DISPATCH (هذا, u.format.v);
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    حالة 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
#د_که_پای
    افتراضي:return_trace (c->default_return_ارزښت ());
    }
  }
};

}
}
}

#د_که_پای /* OT_LAYOUT_GPOS_CURSIVEPOS_HH */
