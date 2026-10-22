#ifndef OT_LAYOUT_GSUB_MULTIPLESUBST_HH
#define OT_LAYOUT_GSUB_MULTIPLESUBST_HH

#ಅನ್ತರ್ಭಾವ "Common.hh"
#ಅನ್ತರ್ಭಾವ "MultipleSubstFormat1.hh"

ನಾಮಾಕಾಶ OT {
ನಾಮಾಕಾಶ Layout {
ನಾಮಾಕಾಶ GSUB_impl {

ಸಂರಚನಾ MultipleSubst
{
  ರಕ್ಷಿತ:
  ಸಙ್ಘ {
  ಸಂರಚನಾ { HBUINT16 v; }		format;         /* Format identifier */
  MultipleSubstFormat1_2<SmallTypes>	format1;
#ifndef HB_NO_BEYOND_64K
  MultipleSubstFormat1_2<MediumTypes>	format2;
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
#ifndef HB_NO_BEYOND_64K
    ಪಕ್ಷ 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
#endif
    ಸ್ವಭಾವ:return_trace (c->default_return_ಮೌಲ್ಯ ());
    }
  }

  template<typename Iterator,
           hb_requires (hb_is_sorted_iterator (Iterator))>
  ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    TRACE_SERIALIZE (ಏತತ್);
    ಯದಿ (unlikely (!c->extend_min (u.format.v))) return_trace (ಅಸತ್ಯ);
    unsigned ಪೂರ್ಣಾಙ್ಕ format = 1;
    u.format.v = format;
    ವಿಕಲ್ಪ (u.format.v) {
    ಪಕ್ಷ 1: hb_barrier (); return_trace (u.format1.serialize (c, it));
    ಸ್ವಭಾವ:return_trace (ಅಸತ್ಯ);
    }
  }

  /* TODO subset() should choose format. */

};


}
}
}

#endif /* OT_LAYOUT_GSUB_MULTIPLESUBST_HH */
