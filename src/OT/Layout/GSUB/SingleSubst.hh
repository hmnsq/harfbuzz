#ifndef OT_LAYOUT_GSUB_SINGLESUBST_HH
#define OT_LAYOUT_GSUB_SINGLESUBST_HH

#ಅನ್ತರ್ಭಾವ "Common.hh"
#ಅನ್ತರ್ಭಾವ "SingleSubstFormat1.hh"
#ಅನ್ತರ್ಭಾವ "SingleSubstFormat2.hh"

ನಾಮಾಕಾಶ OT {
ನಾಮಾಕಾಶ Layout {
ನಾಮಾಕಾಶ GSUB_impl {

ಸಂರಚನಾ SingleSubst
{
  ರಕ್ಷಿತ:
  ಸಙ್ಘ {
  ಸಂರಚನಾ { HBUINT16 v; }		format;         /* Format identifier */
  SingleSubstFormat1_3<SmallTypes>	format1;
  SingleSubstFormat2_4<SmallTypes>	format2;
#ifndef HB_NO_BEYOND_64K
  SingleSubstFormat1_3<MediumTypes>	format3;
  SingleSubstFormat2_4<MediumTypes>	format4;
#endif
  } u;

  ಸಾರ್ವಜನಿಕ:

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ಧ್ರುವ
  {
    ಯದಿ (unlikely (!c->may_dispatch (ಏತತ್, &u.format.v))) ಪ್ರತ್ಯಾಗಮ c->no_dispatch_return_ಮೌಲ್ಯ ();
    TRACE_DISPATCH (ಏತತ್, u.format.v);
    ವಿಕಲ್ಪ (u.format.v) {
    ಪಕ್ಷ 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    ಪಕ್ಷ 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
#ifndef HB_NO_BEYOND_64K
    ಪಕ್ಷ 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    ಪಕ್ಷ 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
#endif
    ಸ್ವಭಾವ:return_trace (c->default_return_ಮೌಲ್ಯ ());
    }
  }

  template<typename Iterator,
           hb_requires (hb_is_sorted_source_of (Iterator,
                                                ಧ್ರುವ hb_codepoint_pair_t))>
  ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c,
                  Iterator glyphs)
  {
    TRACE_SERIALIZE (ಏತತ್);
    ಯದಿ (unlikely (!c->extend_min (u.format.v))) return_trace (ಅಸತ್ಯ);
    unsigned format = 2;
    unsigned delta = 0;
    ಯದಿ (glyphs)
    {
      format = 1;
      hb_codepoint_t mask = 0xFFFFu;

#ifndef HB_NO_BEYOND_64K
       ಯದಿ (+ glyphs
	   | hb_map_retains_sorting (hb_second)
	   | hb_filter ([] (hb_codepoint_t gid) { ಪ್ರತ್ಯಾಗಮ gid > 0xFFFFu; }))
       {
	 format += 2;
	 mask = 0xFFFFFFu;
       }
#endif

      auto get_delta = [=] (hb_codepoint_pair_t _)
                       { ಪ್ರತ್ಯಾಗಮ (unsigned) (_.second - _.first) & mask; };
      delta = get_delta (*glyphs);
      ಯದಿ (!hb_all (++(+glyphs), delta, get_delta)) format += 1;
    }

    u.format.v = format;
    ವಿಕಲ್ಪ (u.format.v) {
    ಪಕ್ಷ 1: hb_barrier (); return_trace (u.format1.serialize (c,
                                               + glyphs
                                               | hb_map_retains_sorting (hb_first),
                                               delta));
    ಪಕ್ಷ 2: hb_barrier (); return_trace (u.format2.serialize (c, glyphs));
#ifndef HB_NO_BEYOND_64K
    ಪಕ್ಷ 3: hb_barrier (); return_trace (u.format3.serialize (c,
                                               + glyphs
                                               | hb_map_retains_sorting (hb_first),
                                               delta));
    ಪಕ್ಷ 4: hb_barrier (); return_trace (u.format4.serialize (c, glyphs));
#endif
    ಸ್ವಭಾವ:return_trace (ಅಸತ್ಯ);
    }
  }
};

template<typename Iterator>
ಸ್ಥಿರ ಶೂನ್ಯ
SingleSubst_serialize (hb_serialize_context_t *c,
                       Iterator it)
{ c->start_embed<SingleSubst> ()->serialize (c, it); }

}
}
}

#endif /* OT_LAYOUT_GSUB_SINGLESUBST_HH */
