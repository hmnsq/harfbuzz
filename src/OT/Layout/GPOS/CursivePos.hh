#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OT_LAYOUT_GPOS_CURSIVEPOS_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_LAYOUT_GPOS_CURSIVEPOS_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "CursivePosFormat1.hh"

ਨਾਮਾਕਾਸ਼ OT {
ਨਾਮਾਕਾਸ਼ Layout {
ਨਾਮਾਕਾਸ਼ GPOS_impl {

ਬਣਤਰ CursivePos
{
  ਸੁਰੱਖਿਅਤ:
  ਸੰਘ {
  ਬਣਤਰ { HBUINT16 v; } format;        /* Format identifier */
  CursivePosFormat1_2<SmallTypes> format1;
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
  CursivePosFormat1_2<MediumTypes> format2;
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
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਹਾਲਤ 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
#ਜੇ_ਅੰਤ
    ਮੂਲ:return_trace (c->default_return_ਮੁੱਲ ());
    }
  }
};

}
}
}

#ਜੇ_ਅੰਤ /* OT_LAYOUT_GPOS_CURSIVEPOS_HH */
