#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OT_LAYOUT_GSUB_MULTIPLESUBST_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_LAYOUT_GSUB_MULTIPLESUBST_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "Common.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "MultipleSubstFormat1.hh"

ਨਾਮਾਕਾਸ਼ OT {
ਨਾਮਾਕਾਸ਼ Layout {
ਨਾਮਾਕਾਸ਼ GSUB_impl {

ਬਣਤਰ ਗੁਣਜ_Subst
{
  ਸੁਰੱਖਿਅਤ:
  ਸੰਘ {
  ਬਣਤਰ { HBUINT16 v; }		format;         /* Format identifier */
  ਗੁਣਜ_Subst_Format1_2<SmallTypes>	format1;
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
  ਗੁਣਜ_Subst_Format1_2<MediumTypes>	format2;
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

  ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ Iterator,
           hb_requires (hb_is_sorted_iterator (Iterator))>
  ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    TRACE_SERIALIZE (ਇਹ);
    ਜੇ (unlikely (!c->extend_min (u.format.v))) return_trace (ਝੂਠ);
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ format = 1;
    u.format.v = format;
    ਚੋਣ (u.format.v) {
    ਹਾਲਤ 1: hb_barrier (); return_trace (u.format1.serialize (c, it));
    ਮੂਲ:return_trace (ਝੂਠ);
    }
  }

  /* TODO subset() should choose format. */

};


}
}
}

#ਜੇ_ਅੰਤ /* OT_LAYOUT_GSUB_MULTIPLESUBST_HH */
