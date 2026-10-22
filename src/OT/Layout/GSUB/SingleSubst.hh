#اگر_غیر_تعریف_شدہ OT_LAYOUT_GSUB_SINGLESUBST_HH
#تعریف_کرو OT_LAYOUT_GSUB_SINGLESUBST_HH

#شامل "Common.hh"
#شامل "SingleSubstFormat1.hh"
#شامل "SingleSubstFormat2.hh"

نامخلا OT {
نامخلا Layout {
نامخلا GSUB_impl {

ساخت SingleSubst
{
  محفوظ:
  اتحاد {
  ساخت { HBUINT16 v; }		format;         /* Format identifier */
  SingleSubstFormat1_3<SmallTypes>	format1;
  SingleSubstFormat2_4<SmallTypes>	format2;
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
  SingleSubstFormat1_3<MediumTypes>	format3;
  SingleSubstFormat2_4<MediumTypes>	format4;
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

  سانچہ<قسم_نام Iterator,
           hb_requires (hb_is_sorted_ماخذ_کا (Iterator,
                                                ثابت hb_codepoint_pair_t))>
  منطقی serialize (hb_serialize_context_t *c,
                  Iterator glyphs)
  {
    TRACE_SERIALIZE (یہ);
    اگر (unlikely (!c->extend_min (u.format.v))) return_trace (جھوٹ);
    غیرعلامتی format = 2;
    غیرعلامتی delta = 0;
    اگر (glyphs)
    {
      format = 1;
      hb_codepoint_t نقاب = 0xFFFFu;

#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
       اگر (+ glyphs
	   | hb_map_retains_ترتیب_کاری (hb_دوسرا)
	   | hb_filter ([] (hb_codepoint_t gid) { واپس gid > 0xFFFFu; }))
       {
	 format += 2;
	 نقاب = 0xFFFFFFu;
       }
#اگر_ختم

      خودکار get_delta = [=] (hb_codepoint_pair_t _)
                       { واپس (غیرعلامتی) (_.second - _.پہلا) & نقاب; };
      delta = get_delta (*glyphs);
      اگر (!hb_all (++(+glyphs), delta, get_delta)) format += 1;
    }

    u.format.v = format;
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); return_trace (u.format1.serialize (c,
                                               + glyphs
                                               | hb_map_retains_ترتیب_کاری (hb_پہلا),
                                               delta));
    حالت 2: hb_barrier (); return_trace (u.format2.serialize (c, glyphs));
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    حالت 3: hb_barrier (); return_trace (u.format3.serialize (c,
                                               + glyphs
                                               | hb_map_retains_ترتیب_کاری (hb_پہلا),
                                               delta));
    حالت 4: hb_barrier (); return_trace (u.format4.serialize (c, glyphs));
#اگر_ختم
    طےشدہ:return_trace (جھوٹ);
    }
  }
};

سانچہ<قسم_نام Iterator>
ساکن خالی
SingleSubst_serialize (hb_serialize_context_t *c,
                       Iterator it)
{ c->آغاز_embed<SingleSubst> ()->serialize (c, it); }

}
}
}

#اگر_ختم /* OT_LAYOUT_GSUB_SINGLESUBST_HH */
