#ifndef OT_LAYOUT_GSUB_SINGLESUBST_HH
#define OT_LAYOUT_GSUB_SINGLESUBST_HH

#അന്തര്ഭാവ "Common.hh"
#അന്തര്ഭാവ "SingleSubstFormat1.hh"
#അന്തര്ഭാവ "SingleSubstFormat2.hh"

നാമാകാശ OT {
നാമാകാശ Layout {
നാമാകാശ GSUB_impl {

ഘടന SingleSubst
{
  സംരക്ഷിത:
  സംഘം {
  ഘടന { HBUINT16 v; }		format;         /* Format identifier */
  SingleSubstFormat1_3<SmallTypes>	format1;
  SingleSubstFormat2_4<SmallTypes>	format2;
#ifndef HB_NO_BEYOND_64K
  SingleSubstFormat1_3<MediumTypes>	format3;
  SingleSubstFormat2_4<MediumTypes>	format4;
#endif
  } u;

  പൊതു:

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) സ്ഥിരം
  {
    എങ്കിൽ (unlikely (!c->may_dispatch (ഇത്, &u.format.v))) തിരികെ c->no_dispatch_return_മൂല്യം ();
    TRACE_DISPATCH (ഇത്, u.format.v);
    തിരഞ്ഞെടുപ്പ് (u.format.v) {
    അവസ്ഥ 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    അവസ്ഥ 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
#ifndef HB_NO_BEYOND_64K
    അവസ്ഥ 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    അവസ്ഥ 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
#endif
    സ്ഥിരസ്ഥിതി:return_trace (c->default_return_മൂല്യം ());
    }
  }

  template<typename Iterator,
           hb_requires (hb_is_sorted_source_of (Iterator,
                                                സ്ഥിരം hb_codepoint_pair_t))>
  തർക്കം serialize (hb_serialize_context_t *c,
                  Iterator glyphs)
  {
    TRACE_SERIALIZE (ഇത്);
    എങ്കിൽ (unlikely (!c->extend_min (u.format.v))) return_trace (അസത്യം);
    unsigned format = 2;
    unsigned delta = 0;
    എങ്കിൽ (glyphs)
    {
      format = 1;
      hb_codepoint_t mask = 0xFFFFu;

#ifndef HB_NO_BEYOND_64K
       എങ്കിൽ (+ glyphs
	   | hb_map_retains_sorting (hb_second)
	   | hb_filter ([] (hb_codepoint_t gid) { തിരികെ gid > 0xFFFFu; }))
       {
	 format += 2;
	 mask = 0xFFFFFFu;
       }
#endif

      auto get_delta = [=] (hb_codepoint_pair_t _)
                       { തിരികെ (unsigned) (_.second - _.first) & mask; };
      delta = get_delta (*glyphs);
      എങ്കിൽ (!hb_all (++(+glyphs), delta, get_delta)) format += 1;
    }

    u.format.v = format;
    തിരഞ്ഞെടുപ്പ് (u.format.v) {
    അവസ്ഥ 1: hb_barrier (); return_trace (u.format1.serialize (c,
                                               + glyphs
                                               | hb_map_retains_sorting (hb_first),
                                               delta));
    അവസ്ഥ 2: hb_barrier (); return_trace (u.format2.serialize (c, glyphs));
#ifndef HB_NO_BEYOND_64K
    അവസ്ഥ 3: hb_barrier (); return_trace (u.format3.serialize (c,
                                               + glyphs
                                               | hb_map_retains_sorting (hb_first),
                                               delta));
    അവസ്ഥ 4: hb_barrier (); return_trace (u.format4.serialize (c, glyphs));
#endif
    സ്ഥിരസ്ഥിതി:return_trace (അസത്യം);
    }
  }
};

template<typename Iterator>
സ്ഥിതം ശൂന്യം
SingleSubst_serialize (hb_serialize_context_t *c,
                       Iterator it)
{ c->start_embed<SingleSubst> ()->serialize (c, it); }

}
}
}

#endif /* OT_LAYOUT_GSUB_SINGLESUBST_HH */
