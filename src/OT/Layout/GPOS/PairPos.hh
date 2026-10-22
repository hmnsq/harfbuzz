#جے_بنا_تعریف OT_LAYOUT_GPOS_PAIRPOS_HH
#تعریف_کرو OT_LAYOUT_GPOS_PAIRPOS_HH

#ضمن "PairPosFormat1.hh"
#ضمن "PairPosFormat2.hh"

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {

بنية PairPos
{
  محمي:
  اتحاد {
  بنية { HBUINT16 v; }	format;         /* Format identifier */
  PairPosFormat1_3<SmallTypes>	format1;
  PairPosFormat2_4<SmallTypes>	format2;
#جے_بنا_تعریف HB_NO_BEYOND_64K
  PairPosFormat1_3<MediumTypes>	format3;
  PairPosFormat2_4<MediumTypes>	format4;
#جے_انت
  } u;

  عام:
  سانچا <قسم_ناں context_t, قسم_ناں ...Ts>
  قسم_ناں context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    إذا (unlikely (!c->may_dispatch (هذا, &u.format.v))) أرجع c->no_dispatch_return_قدر ();
    TRACE_DISPATCH (هذا, u.format.v);
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    حالة 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
#جے_بنا_تعریف HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    حالة 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
#جے_انت
    افتراضي:return_trace (c->default_return_قدر ());
    }
  }
};

}
}
}

#جے_انت  // OT_LAYOUT_GPOS_PAIRPOS_HH
