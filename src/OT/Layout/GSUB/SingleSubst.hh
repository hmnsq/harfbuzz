#که_نه_تعریف_شوی OT_LAYOUT_GSUB_SINGLESUBST_HH
#تعریف_کړه OT_LAYOUT_GSUB_SINGLESUBST_HH

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
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
  SingleSubstFormat1_3<MediumTypes>	format3;
  SingleSubstFormat2_4<MediumTypes>	format4;
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

  کينډۍ<ډول_نوم Iterator,
           hb_requires (hb_is_sorted_سرچینه_د (Iterator,
                                                ثابت hb_codepoint_pair_t))>
  منطقي serialize (hb_serialize_context_t *c,
                  Iterator glyphs)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!c->extend_min (u.format.v))) return_trace (خطأ);
    بینښه format = 2;
    بینښه delta = 0;
    إذا (glyphs)
    {
      format = 1;
      hb_codepoint_t ماسک = 0xFFFFu;

#که_نه_تعریف_شوی HB_NO_BEYOND_64K
       إذا (+ glyphs
	   | hb_map_retains_ترتیبونه (hb_دوهم)
	   | hb_filter ([] (hb_codepoint_t gid) { أرجع gid > 0xFFFFu; }))
       {
	 format += 2;
	 ماسک = 0xFFFFFFu;
       }
#د_که_پای

      خپلکاری get_delta = [=] (hb_codepoint_pair_t _)
                       { أرجع (بینښه) (_.second - _.لومړی) & ماسک; };
      delta = get_delta (*glyphs);
      إذا (!hb_all (++(+glyphs), delta, get_delta)) format += 1;
    }

    u.format.v = format;
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (u.format1.serialize (c,
                                               + glyphs
                                               | hb_map_retains_ترتیبونه (hb_لومړی),
                                               delta));
    حالة 2: hb_barrier (); return_trace (u.format2.serialize (c, glyphs));
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); return_trace (u.format3.serialize (c,
                                               + glyphs
                                               | hb_map_retains_ترتیبونه (hb_لومړی),
                                               delta));
    حالة 4: hb_barrier (); return_trace (u.format4.serialize (c, glyphs));
#د_که_پای
    افتراضي:return_trace (خطأ);
    }
  }
};

کينډۍ<ډول_نوم Iterator>
ساكن فراغ
SingleSubst_serialize (hb_serialize_context_t *c,
                       Iterator it)
{ c->پیل_embed<SingleSubst> ()->serialize (c, it); }

}
}
}

#د_که_پای /* OT_LAYOUT_GSUB_SINGLESUBST_HH */
