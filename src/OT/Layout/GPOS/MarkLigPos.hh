#ifndef OT_LAYOUT_GPOS_MARKLIGPOS_HH
#define OT_LAYOUT_GPOS_MARKLIGPOS_HH

#شامل "MarkLigPosFormat1.hh"

فضاینام OT {
فضاینام Layout {
فضاینام GPOS_impl {

ساختار MarkLigPos
{
  محافظتشده:
  اجتماع {
  ساختار { HBUINT16 v; }		format;         /* Format identifier */
  MarkLigPosFormat1_2<SmallTypes>	format1;
#ifndef HB_NO_BEYOND_64K
  MarkLigPosFormat1_2<MediumTypes>	format2;
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
#ifndef HB_NO_BEYOND_64K
    حالت 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
#endif
    پیشفرض:return_trace (c->default_return_مقدار ());
    }
  }
};

}
}
}

#endif /* OT_LAYOUT_GPOS_MARKLIGPOS_HH */
