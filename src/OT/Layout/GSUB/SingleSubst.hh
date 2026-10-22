#ifndef OT_LAYOUT_GSUB_SINGLESUBST_HH
#define OT_LAYOUT_GSUB_SINGLESUBST_HH

#شامل "Common.hh"
#شامل "SingleSubstFormat1.hh"
#شامل "SingleSubstFormat2.hh"

فضاینام OT {
فضاینام Layout {
فضاینام GSUB_impl {

ساختار SingleSubst
{
  محافظتشده:
  اجتماع {
  ساختار { HBUINT16 v; }		format;         /* Format identifier */
  SingleSubstFormat1_3<SmallTypes>	format1;
  SingleSubstFormat2_4<SmallTypes>	format2;
#ifndef HB_NO_BEYOND_64K
  SingleSubstFormat1_3<MediumTypes>	format3;
  SingleSubstFormat2_4<MediumTypes>	format4;
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

  template<typename Iterator,
           hb_requires (hb_is_sorted_source_of (Iterator,
                                                ثابت hb_codepoint_pair_t))>
  منطقی serialize (hb_serialize_context_t *c,
                  Iterator glyphs)
  {
    TRACE_SERIALIZE (این);
    اگر (unlikely (!c->extend_min (u.format.v))) return_trace (نادرست);
    unsigned format = 2;
    unsigned delta = 0;
    اگر (glyphs)
    {
      format = 1;
      hb_codepoint_t mask = 0xFFFFu;

#ifndef HB_NO_BEYOND_64K
       اگر (+ glyphs
	   | hb_map_retains_sorting (hb_second)
	   | hb_filter ([] (hb_codepoint_t gid) { بازگشت gid > 0xFFFFu; }))
       {
	 format += 2;
	 mask = 0xFFFFFFu;
       }
#endif

      auto get_delta = [=] (hb_codepoint_pair_t _)
                       { بازگشت (unsigned) (_.second - _.first) & mask; };
      delta = get_delta (*glyphs);
      اگر (!hb_all (++(+glyphs), delta, get_delta)) format += 1;
    }

    u.format.v = format;
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); return_trace (u.format1.serialize (c,
                                               + glyphs
                                               | hb_map_retains_sorting (hb_first),
                                               delta));
    حالت 2: hb_barrier (); return_trace (u.format2.serialize (c, glyphs));
#ifndef HB_NO_BEYOND_64K
    حالت 3: hb_barrier (); return_trace (u.format3.serialize (c,
                                               + glyphs
                                               | hb_map_retains_sorting (hb_first),
                                               delta));
    حالت 4: hb_barrier (); return_trace (u.format4.serialize (c, glyphs));
#endif
    پیشفرض:return_trace (نادرست);
    }
  }
};

template<typename Iterator>
ایستا تهی
SingleSubst_serialize (hb_serialize_context_t *c,
                       Iterator it)
{ c->start_embed<SingleSubst> ()->serialize (c, it); }

}
}
}

#endif /* OT_LAYOUT_GSUB_SINGLESUBST_HH */
