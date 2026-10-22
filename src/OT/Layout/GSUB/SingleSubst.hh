#جيڪڏهن_وصف_نه_ٿيل OT_LAYOUT_GSUB_SINGLESUBST_HH
#وصف_ڪريو OT_LAYOUT_GSUB_SINGLESUBST_HH

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
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
  SingleSubstFormat1_3<MediumTypes>	format3;
  SingleSubstFormat2_4<MediumTypes>	format4;
#جيڪڏهن_پورو
  } u;

  عام:

  سانچو <قسم_نالو context_t, قسم_نالو ...Ts>
  قسم_نالو context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    إذا (unlikely (!c->may_dispatch (هذا, &u.format.v))) أرجع c->no_dispatch_return_قدر ();
    TRACE_DISPATCH (هذا, u.format.v);
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    حالة 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    حالة 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
#جيڪڏهن_پورو
    افتراضي:return_trace (c->default_return_قدر ());
    }
  }

  سانچو<قسم_نالو Iterator,
           hb_requires (hb_is_sorted_ذريعو_جو (Iterator,
                                                ثابت hb_codepoint_pair_t))>
  منطقي serialize (hb_serialize_context_t *c,
                  Iterator glyphs)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!c->extend_min (u.format.v))) return_trace (خطأ);
    بنانشان format = 2;
    بنانشان delta = 0;
    إذا (glyphs)
    {
      format = 1;
      hb_codepoint_t نقاب = 0xFFFFu;

#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
       إذا (+ glyphs
	   | hb_map_retains_ترتيب_ڏيڻ (hb_ٻيو)
	   | hb_filter ([] (hb_codepoint_t gid) { أرجع gid > 0xFFFFu; }))
       {
	 format += 2;
	 نقاب = 0xFFFFFFu;
       }
#جيڪڏهن_پورو

      خودڪار get_delta = [=] (hb_codepoint_pair_t _)
                       { أرجع (بنانشان) (_.second - _.پهريون) & نقاب; };
      delta = get_delta (*glyphs);
      إذا (!hb_all (++(+glyphs), delta, get_delta)) format += 1;
    }

    u.format.v = format;
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (u.format1.serialize (c,
                                               + glyphs
                                               | hb_map_retains_ترتيب_ڏيڻ (hb_پهريون),
                                               delta));
    حالة 2: hb_barrier (); return_trace (u.format2.serialize (c, glyphs));
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); return_trace (u.format3.serialize (c,
                                               + glyphs
                                               | hb_map_retains_ترتيب_ڏيڻ (hb_پهريون),
                                               delta));
    حالة 4: hb_barrier (); return_trace (u.format4.serialize (c, glyphs));
#جيڪڏهن_پورو
    افتراضي:return_trace (خطأ);
    }
  }
};

سانچو<قسم_نالو Iterator>
ساكن فراغ
SingleSubst_serialize (hb_serialize_context_t *c,
                       Iterator it)
{ c->شروعات_embed<SingleSubst> ()->serialize (c, it); }

}
}
}

#جيڪڏهن_پورو /* OT_LAYOUT_GSUB_SINGLESUBST_HH */
