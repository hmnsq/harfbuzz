#ifndef OT_LAYOUT_GSUB_SINGLESUBST_HH
#define OT_LAYOUT_GSUB_SINGLESUBST_HH

#અન્તર્ભાવ "Common.hh"
#અન્તર્ભાવ "SingleSubstFormat1.hh"
#અન્તર્ભાવ "SingleSubstFormat2.hh"

નામાકાશ OT {
નામાકાશ Layout {
નામાકાશ GSUB_impl {

સંરચના SingleSubst
{
  સુરક્ષિત:
  સંઘ {
  સંરચના { HBUINT16 v; }		format;         /* Format identifier */
  SingleSubstFormat1_3<SmallTypes>	format1;
  SingleSubstFormat2_4<SmallTypes>	format2;
#ifndef HB_NO_BEYOND_64K
  SingleSubstFormat1_3<MediumTypes>	format3;
  SingleSubstFormat2_4<MediumTypes>	format4;
#endif
  } u;

  જાહેર:

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) અચલ
  {
    જો (unlikely (!c->may_dispatch (આ, &u.format.v))) પાછા c->no_dispatch_return_કિંમત ();
    TRACE_DISPATCH (આ, u.format.v);
    પસંદગી (u.format.v) {
    સ્થિતિ 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    સ્થિતિ 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
#ifndef HB_NO_BEYOND_64K
    સ્થિતિ 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    સ્થિતિ 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
#endif
    મૂળભૂત:return_trace (c->default_return_કિંમત ());
    }
  }

  template<typename Iterator,
           hb_requires (hb_is_sorted_source_of (Iterator,
                                                અચલ hb_codepoint_pair_t))>
  તાર્કિક serialize (hb_serialize_context_t *c,
                  Iterator glyphs)
  {
    TRACE_SERIALIZE (આ);
    જો (unlikely (!c->extend_min (u.format.v))) return_trace (ખોટું);
    unsigned format = 2;
    unsigned delta = 0;
    જો (glyphs)
    {
      format = 1;
      hb_codepoint_t mask = 0xFFFFu;

#ifndef HB_NO_BEYOND_64K
       જો (+ glyphs
	   | hb_map_retains_sorting (hb_second)
	   | hb_filter ([] (hb_codepoint_t gid) { પાછા gid > 0xFFFFu; }))
       {
	 format += 2;
	 mask = 0xFFFFFFu;
       }
#endif

      auto get_delta = [=] (hb_codepoint_pair_t _)
                       { પાછા (unsigned) (_.second - _.first) & mask; };
      delta = get_delta (*glyphs);
      જો (!hb_all (++(+glyphs), delta, get_delta)) format += 1;
    }

    u.format.v = format;
    પસંદગી (u.format.v) {
    સ્થિતિ 1: hb_barrier (); return_trace (u.format1.serialize (c,
                                               + glyphs
                                               | hb_map_retains_sorting (hb_first),
                                               delta));
    સ્થિતિ 2: hb_barrier (); return_trace (u.format2.serialize (c, glyphs));
#ifndef HB_NO_BEYOND_64K
    સ્થિતિ 3: hb_barrier (); return_trace (u.format3.serialize (c,
                                               + glyphs
                                               | hb_map_retains_sorting (hb_first),
                                               delta));
    સ્થિતિ 4: hb_barrier (); return_trace (u.format4.serialize (c, glyphs));
#endif
    મૂળભૂત:return_trace (ખોટું);
    }
  }
};

template<typename Iterator>
સ્થિર શૂન્ય
SingleSubst_serialize (hb_serialize_context_t *c,
                       Iterator it)
{ c->start_embed<SingleSubst> ()->serialize (c, it); }

}
}
}

#endif /* OT_LAYOUT_GSUB_SINGLESUBST_HH */
