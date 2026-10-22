#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OT_LAYOUT_GSUB_SINGLESUBST_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_LAYOUT_GSUB_SINGLESUBST_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "Common.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "SingleSubstFormat1.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "SingleSubstFormat2.hh"

ਨਾਮਾਕਾਸ਼ OT {
ਨਾਮਾਕਾਸ਼ Layout {
ਨਾਮਾਕਾਸ਼ GSUB_impl {

ਬਣਤਰ SingleSubst
{
  ਸੁਰੱਖਿਅਤ:
  ਸੰਘ {
  ਬਣਤਰ { HBUINT16 v; }		format;         /* Format identifier */
  SingleSubstFormat1_3<SmallTypes>	format1;
  SingleSubstFormat2_4<SmallTypes>	format2;
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
  SingleSubstFormat1_3<MediumTypes>	format3;
  SingleSubstFormat2_4<MediumTypes>	format4;
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

  ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ Iterator,
           hb_requires (hb_is_sorted_ਸਰੋਤ_ਦਾ (Iterator,
                                                ਅਚਲ hb_codepoint_pair_t))>
  ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c,
                  Iterator glyphs)
  {
    TRACE_SERIALIZE (ਇਹ);
    ਜੇ (unlikely (!c->extend_min (u.format.v))) return_trace (ਝੂਠ);
    ਬਿਨਾਨਿਸ਼ਾਨੀ format = 2;
    ਬਿਨਾਨਿਸ਼ਾਨੀ delta = 0;
    ਜੇ (glyphs)
    {
      format = 1;
      hb_codepoint_t ਨਕਾਬ = 0xFFFFu;

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
       ਜੇ (+ glyphs
	   | hb_map_retains_ਛਾਂਟੀ (hb_ਦੂਜਾ)
	   | hb_filter ([] (hb_codepoint_t gid) { ਵਾਪਸ gid > 0xFFFFu; }))
       {
	 format += 2;
	 ਨਕਾਬ = 0xFFFFFFu;
       }
#ਜੇ_ਅੰਤ

      ਸਵੈਚਾਲਿਤ get_delta = [=] (hb_codepoint_pair_t _)
                       { ਵਾਪਸ (ਬਿਨਾਨਿਸ਼ਾਨੀ) (_.second - _.ਪਹਿਲਾ) & ਨਕਾਬ; };
      delta = get_delta (*glyphs);
      ਜੇ (!hb_all (++(+glyphs), delta, get_delta)) format += 1;
    }

    u.format.v = format;
    ਚੋਣ (u.format.v) {
    ਹਾਲਤ 1: hb_barrier (); return_trace (u.format1.serialize (c,
                                               + glyphs
                                               | hb_map_retains_ਛਾਂਟੀ (hb_ਪਹਿਲਾ),
                                               delta));
    ਹਾਲਤ 2: hb_barrier (); return_trace (u.format2.serialize (c, glyphs));
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਹਾਲਤ 3: hb_barrier (); return_trace (u.format3.serialize (c,
                                               + glyphs
                                               | hb_map_retains_ਛਾਂਟੀ (hb_ਪਹਿਲਾ),
                                               delta));
    ਹਾਲਤ 4: hb_barrier (); return_trace (u.format4.serialize (c, glyphs));
#ਜੇ_ਅੰਤ
    ਮੂਲ:return_trace (ਝੂਠ);
    }
  }
};

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ Iterator>
ਸਥਿਰ ਖਾਲੀ
SingleSubst_serialize (hb_serialize_context_t *c,
                       Iterator it)
{ c->ਸ਼ੁਰੂ_embed<SingleSubst> ()->serialize (c, it); }

}
}
}

#ਜੇ_ਅੰਤ /* OT_LAYOUT_GSUB_SINGLESUBST_HH */
