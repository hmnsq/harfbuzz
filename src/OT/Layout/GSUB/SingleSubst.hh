#جے_بنا_تعریف OT_LAYOUT_GSUB_SINGLESUBST_HH
#تعریف_کرو OT_LAYOUT_GSUB_SINGLESUBST_HH

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
#جے_بنا_تعریف HB_NO_BEYOND_64K
  SingleSubstFormat1_3<MediumTypes>	format3;
  SingleSubstFormat2_4<MediumTypes>	format4;
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

  سانچا<قسم_ناں Iterator,
           hb_requires (hb_is_sorted_سروت_دا (Iterator,
                                                ثابت hb_codepoint_pair_t))>
  منطقي serialize (hb_serialize_context_t *c,
                  Iterator glyphs)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!c->extend_min (u.format.v))) return_trace (خطأ);
    بنانشانی format = 2;
    بنانشانی delta = 0;
    إذا (glyphs)
    {
      format = 1;
      hb_codepoint_t نقاب = 0xFFFFu;

#جے_بنا_تعریف HB_NO_BEYOND_64K
       إذا (+ glyphs
	   | hb_map_retains_چھانٹی (hb_دوجا)
	   | hb_filter ([] (hb_codepoint_t gid) { أرجع gid > 0xFFFFu; }))
       {
	 format += 2;
	 نقاب = 0xFFFFFFu;
       }
#جے_انت

      خودکار get_delta = [=] (hb_codepoint_pair_t _)
                       { أرجع (بنانشانی) (_.second - _.پہلا) & نقاب; };
      delta = get_delta (*glyphs);
      إذا (!hb_all (++(+glyphs), delta, get_delta)) format += 1;
    }

    u.format.v = format;
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (u.format1.serialize (c,
                                               + glyphs
                                               | hb_map_retains_چھانٹی (hb_پہلا),
                                               delta));
    حالة 2: hb_barrier (); return_trace (u.format2.serialize (c, glyphs));
#جے_بنا_تعریف HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); return_trace (u.format3.serialize (c,
                                               + glyphs
                                               | hb_map_retains_چھانٹی (hb_پہلا),
                                               delta));
    حالة 4: hb_barrier (); return_trace (u.format4.serialize (c, glyphs));
#جے_انت
    افتراضي:return_trace (خطأ);
    }
  }
};

سانچا<قسم_ناں Iterator>
ساكن فراغ
SingleSubst_serialize (hb_serialize_context_t *c,
                       Iterator it)
{ c->شروع_embed<SingleSubst> ()->serialize (c, it); }

}
}
}

#جے_انت /* OT_LAYOUT_GSUB_SINGLESUBST_HH */
