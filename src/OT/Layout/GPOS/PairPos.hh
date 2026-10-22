#که_نه_تعریف_شوی OT_LAYOUT_GPOS_PAIRPOS_HH
#تعریف_کړه OT_LAYOUT_GPOS_PAIRPOS_HH

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
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
  PairPosFormat1_3<MediumTypes>	format3;
  PairPosFormat2_4<MediumTypes>	format4;
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
    حالة 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    حالة 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
#د_که_پای
    افتراضي:return_trace (c->default_return_ارزښت ());
    }
  }
};

}
}
}

#د_که_پای  // OT_LAYOUT_GPOS_PAIRPOS_HH
