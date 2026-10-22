#اگر_غیر_تعریف_شدہ OT_LAYOUT_GPOS_PAIRPOS_HH
#تعریف_کرو OT_LAYOUT_GPOS_PAIRPOS_HH

#شامل "PairPosFormat1.hh"
#شامل "PairPosFormat2.hh"

نامخلا OT {
نامخلا Layout {
نامخلا GPOS_impl {

ساخت PairPos
{
  محفوظ:
  اتحاد {
  ساخت { HBUINT16 v; }	format;         /* Format identifier */
  PairPosFormat1_3<SmallTypes>	format1;
  PairPosFormat2_4<SmallTypes>	format2;
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
  PairPosFormat1_3<MediumTypes>	format3;
  PairPosFormat2_4<MediumTypes>	format4;
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
    حالت 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    حالت 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    حالت 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
#اگر_ختم
    طےشدہ:return_trace (c->default_return_قدر ());
    }
  }
};

}
}
}

#اگر_ختم  // OT_LAYOUT_GPOS_PAIRPOS_HH
