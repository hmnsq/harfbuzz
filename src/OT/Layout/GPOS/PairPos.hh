#ifndef OT_LAYOUT_GPOS_PAIRPOS_HH
#define OT_LAYOUT_GPOS_PAIRPOS_HH

#ಅನ್ತರ್ಭಾವ "PairPosFormat1.hh"
#ಅನ್ತರ್ಭಾವ "PairPosFormat2.hh"

ನಾಮಾಕಾಶ OT {
ನಾಮಾಕಾಶ Layout {
ನಾಮಾಕಾಶ GPOS_impl {

ಸಂರಚನಾ PairPos
{
  ರಕ್ಷಿತ:
  ಸಙ್ಘ {
  ಸಂರಚನಾ { HBUINT16 v; }	format;         /* Format identifier */
  PairPosFormat1_3<SmallTypes>	format1;
  PairPosFormat2_4<SmallTypes>	format2;
#ifndef HB_NO_BEYOND_64K
  PairPosFormat1_3<MediumTypes>	format3;
  PairPosFormat2_4<MediumTypes>	format4;
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
};

}
}
}

#endif  // OT_LAYOUT_GPOS_PAIRPOS_HH
