#ifndef OT_LAYOUT_GSUB_MULTIPLESUBST_HH
#define OT_LAYOUT_GSUB_MULTIPLESUBST_HH

#અન્તર્ભાવ "Common.hh"
#અન્તર્ભાવ "MultipleSubstFormat1.hh"

નામાકાશ OT {
નામાકાશ Layout {
નામાકાશ GSUB_impl {

સંરચના MultipleSubst
{
  સુરક્ષિત:
  સંઘ {
  સંરચના { HBUINT16 v; }		format;         /* Format identifier */
  MultipleSubstFormat1_2<SmallTypes>	format1;
#ifndef HB_NO_BEYOND_64K
  MultipleSubstFormat1_2<MediumTypes>	format2;
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
#ifndef HB_NO_BEYOND_64K
    સ્થિતિ 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
#endif
    મૂળભૂત:return_trace (c->default_return_કિંમત ());
    }
  }

  template<typename Iterator,
           hb_requires (hb_is_sorted_iterator (Iterator))>
  તાર્કિક serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    TRACE_SERIALIZE (આ);
    જો (unlikely (!c->extend_min (u.format.v))) return_trace (ખોટું);
    unsigned પૂર્ણાંક format = 1;
    u.format.v = format;
    પસંદગી (u.format.v) {
    સ્થિતિ 1: hb_barrier (); return_trace (u.format1.serialize (c, it));
    મૂળભૂત:return_trace (ખોટું);
    }
  }

  /* TODO subset() should choose format. */

};


}
}
}

#endif /* OT_LAYOUT_GSUB_MULTIPLESUBST_HH */
