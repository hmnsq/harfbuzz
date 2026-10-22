#ifndef OT_LAYOUT_GPOS_MARKLIGPOS_HH
#define OT_LAYOUT_GPOS_MARKLIGPOS_HH

#ಅನ್ತರ್ಭಾವ "MarkLigPosFormat1.hh"

ನಾಮಾಕಾಶ OT {
ನಾಮಾಕಾಶ Layout {
ನಾಮಾಕಾಶ GPOS_impl {

ಸಂರಚನಾ MarkLigPos
{
  ರಕ್ಷಿತ:
  ಸಙ್ಘ {
  ಸಂರಚನಾ { HBUINT16 v; }		format;         /* Format identifier */
  MarkLigPosFormat1_2<SmallTypes>	format1;
#ifndef HB_NO_BEYOND_64K
  MarkLigPosFormat1_2<MediumTypes>	format2;
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
};

}
}
}

#endif /* OT_LAYOUT_GPOS_MARKLIGPOS_HH */
