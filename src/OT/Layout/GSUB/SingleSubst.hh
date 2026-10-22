#اگر_تعریف_نشده OT_LAYOUT_GSUB_SINGLESUBST_HH
#تعریف_کن OT_LAYOUT_GSUB_SINGLESUBST_HH

#ضمن "Common.hh"
#ضمن "SingleSubstFormat1.hh"
#ضمن "SingleSubstFormat2.hh"

نطاق OT {
نطاق Layout {
نطاق GSUB_impl {

بنية SingleSubst
{
  محمي:
  اتحاد {
  بنية { HBUINT16 v; }		format;         /* Format identifier */
  SingleSubstFormat1_3<SmallTypes>	format1;
  SingleSubstFormat2_4<SmallTypes>	format2;
#اگر_تعریف_نشده HB_NO_BEYOND_64K
  SingleSubstFormat1_3<MediumTypes>	format3;
  SingleSubstFormat2_4<MediumTypes>	format4;
#پایان_اگر
  } u;

  عام:

  قالب <نام_نوع context_t, نام_نوع ...Ts>
  نام_نوع context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    إذا (unlikely (!c->may_dispatch (هذا, &u.format.v))) أرجع c->no_dispatch_return_مقدار ();
    TRACE_DISPATCH (هذا, u.format.v);
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    حالة 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    حالة 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
#پایان_اگر
    افتراضي:return_trace (c->default_return_مقدار ());
    }
  }

  قالب<نام_نوع Iterator,
           hb_requires (hb_is_sorted_منبع_از (Iterator,
                                                ثابت hb_codepoint_pair_t))>
  منطقي serialize (hb_serialize_context_t *c,
                  Iterator glyphs)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!c->extend_min (u.format.v))) return_trace (خطأ);
    بیعلامت format = 2;
    بیعلامت delta = 0;
    إذا (glyphs)
    {
      format = 1;
      hb_codepoint_t ماسک = 0xFFFFu;

#اگر_تعریف_نشده HB_NO_BEYOND_64K
       إذا (+ glyphs
	   | hb_map_retains_ترتیبدهی (hb_دومی)
	   | hb_filter ([] (hb_codepoint_t gid) { أرجع gid > 0xFFFFu; }))
       {
	 format += 2;
	 ماسک = 0xFFFFFFu;
       }
#پایان_اگر

      خودکار get_delta = [=] (hb_codepoint_pair_t _)
                       { أرجع (بیعلامت) (_.second - _.اولی) & ماسک; };
      delta = get_delta (*glyphs);
      إذا (!hb_all (++(+glyphs), delta, get_delta)) format += 1;
    }

    u.format.v = format;
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (u.format1.serialize (c,
                                               + glyphs
                                               | hb_map_retains_ترتیبدهی (hb_اولی),
                                               delta));
    حالة 2: hb_barrier (); return_trace (u.format2.serialize (c, glyphs));
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); return_trace (u.format3.serialize (c,
                                               + glyphs
                                               | hb_map_retains_ترتیبدهی (hb_اولی),
                                               delta));
    حالة 4: hb_barrier (); return_trace (u.format4.serialize (c, glyphs));
#پایان_اگر
    افتراضي:return_trace (خطأ);
    }
  }
};

قالب<نام_نوع Iterator>
ساكن فراغ
SingleSubst_serialize (hb_serialize_context_t *c,
                       Iterator it)
{ c->شروع_embed<SingleSubst> ()->serialize (c, it); }

}
}
}

#پایان_اگر /* OT_LAYOUT_GSUB_SINGLESUBST_HH */
