#جيڪڏهن_وصف_نه_ٿيل OT_LAYOUT_GPOS_CURSIVEPOS_HH
#وصف_ڪريو OT_LAYOUT_GPOS_CURSIVEPOS_HH

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
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
  CursivePosFormat1_2<MediumTypes> format2;
#جيڪڏهن_پورو
  } u;

  عام:
  سانچو <قسم_نالو context_t, قسم_نالو ...Ts>
  قسم_نالو context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    إذا (unlikely (!c->may_dispatch (هذا, &u.format.v))) أرجع c->no_dispatch_return_قدر ();
    TRACE_DISPATCH (هذا, u.format.v);
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    حالة 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
#جيڪڏهن_پورو
    افتراضي:return_trace (c->default_return_قدر ());
    }
  }
};

}
}
}

#جيڪڏهن_پورو /* OT_LAYOUT_GPOS_CURSIVEPOS_HH */
