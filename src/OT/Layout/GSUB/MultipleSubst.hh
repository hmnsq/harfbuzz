#ifndef OT_LAYOUT_GSUB_MULTIPLESUBST_HH
#define OT_LAYOUT_GSUB_MULTIPLESUBST_HH

#شامل "Common.hh"
#شامل "MultipleSubstFormat1.hh"

فضاینام OT {
فضاینام Layout {
فضاینام GSUB_impl {

ساختار MultipleSubst
{
  محافظتشده:
  اجتماع {
  ساختار { HBUINT16 v; }		format;         /* Format identifier */
  MultipleSubstFormat1_2<SmallTypes>	format1;
#ifndef HB_NO_BEYOND_64K
  MultipleSubstFormat1_2<MediumTypes>	format2;
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

  template<typename Iterator,
           hb_requires (hb_is_sorted_iterator (Iterator))>
  منطقی serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    TRACE_SERIALIZE (این);
    اگر (unlikely (!c->extend_min (u.format.v))) return_trace (نادرست);
    unsigned صحیح format = 1;
    u.format.v = format;
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); return_trace (u.format1.serialize (c, it));
    پیشفرض:return_trace (نادرست);
    }
  }

  /* TODO subset() should choose format. */

};


}
}
}

#endif /* OT_LAYOUT_GSUB_MULTIPLESUBST_HH */
