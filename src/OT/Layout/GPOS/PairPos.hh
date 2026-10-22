#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OT_LAYOUT_GPOS_PAIRPOS_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_LAYOUT_GPOS_PAIRPOS_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "PairPosFormat1.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "PairPosFormat2.hh"

ਨਾਮਾਕਾਸ਼ OT {
ਨਾਮਾਕਾਸ਼ Layout {
ਨਾਮਾਕਾਸ਼ GPOS_impl {

ਬਣਤਰ PairPos
{
  ਸੁਰੱਖਿਅਤ:
  ਸੰਘ {
  ਬਣਤਰ { HBUINT16 v; }	format;         /* Format identifier */
  PairPosFormat1_3<SmallTypes>	format1;
  PairPosFormat2_4<SmallTypes>	format2;
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
  PairPosFormat1_3<MediumTypes>	format3;
  PairPosFormat2_4<MediumTypes>	format4;
#ਜੇ_ਅੰਤ
  } u;

  ਜਨਤਕ:
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ context_t, ਕਿਸਮ_ਨਾਂ ...Ts>
  ਕਿਸਮ_ਨਾਂ context_t::return_t dispatch (context_t *c, Ts&&... ds) ਅਚਲ
  {
    ਜੇ (unlikely (!c->may_dispatch (ਇਹ, &u.format.v))) ਵਾਪਸ c->no_dispatch_return_ਮੁੱਲ ();
    TRACE_DISPATCH (ਇਹ, u.format.v);
    ਚੋਣ (u.format.v) {
    ਹਾਲਤ 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    ਹਾਲਤ 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਹਾਲਤ 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    ਹਾਲਤ 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
#ਜੇ_ਅੰਤ
    ਮੂਲ:return_trace (c->default_return_ਮੁੱਲ ());
    }
  }
};

}
}
}

#ਜੇ_ਅੰਤ  // OT_LAYOUT_GPOS_PAIRPOS_HH
