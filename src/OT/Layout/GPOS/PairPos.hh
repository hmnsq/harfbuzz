#ifndef OT_LAYOUT_GPOS_PAIRPOS_HH
#define OT_LAYOUT_GPOS_PAIRPOS_HH

#شامل "PairPosFormat1.hh"
#شامل "PairPosFormat2.hh"

فضاینام OT {
فضاینام Layout {
فضاینام GPOS_impl {

ساختار PairPos
{
  محافظتشده:
  اجتماع {
  ساختار { HBUINT16 v; }	format;         /* Format identifier */
  PairPosFormat1_3<SmallTypes>	format1;
  PairPosFormat2_4<SmallTypes>	format2;
#ifndef HB_NO_BEYOND_64K
  PairPosFormat1_3<MediumTypes>	format3;
  PairPosFormat2_4<MediumTypes>	format4;
#endif
  } u;

  عمومی:
  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    اگر (unlikely (!c->may_dispatch (این, &u.format.v))) بازگشت c->no_dispatch_return_مقدار ();
    TRACE_DISPATCH (این, u.format.v);
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    حالت 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
#ifndef HB_NO_BEYOND_64K
    حالت 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    حالت 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
#endif
    پیشفرض:return_trace (c->default_return_مقدار ());
    }
  }
};

}
}
}

#endif  // OT_LAYOUT_GPOS_PAIRPOS_HH
