#اگر_غیر_تعریف_شدہ OT_LAYOUT_GPOS_MARKMARKPOS_HH
#تعریف_کرو OT_LAYOUT_GPOS_MARKMARKPOS_HH

#شامل "MarkMarkPosFormat1.hh"

نامخلا OT {
نامخلا Layout {
نامخلا GPOS_impl {

ساخت MarkMarkPos
{
  محفوظ:
  اتحاد {
  ساخت { HBUINT16 v; }		format;         /* Format identifier */
  MarkMarkPosFormat1_2<SmallTypes>	format1;
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
  MarkMarkPosFormat1_2<MediumTypes>	format2;
#اگر_ختم
  } u;

  عوامی:
  سانچہ <قسم_نام context_t, قسم_نام ...Ts>
  قسم_نام context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    اگر (unlikely (!c->may_dispatch (یہ, &u.format.v))) واپس c->no_dispatch_return_قدر ();
    TRACE_DISPATCH (یہ, u.format.v);
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    حالت 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
#اگر_ختم
    طےشدہ:return_trace (c->default_return_قدر ());
    }
  }
};


}
}
}

#اگر_ختم /* OT_LAYOUT_GPOS_MARKMARKPOS_HH */
