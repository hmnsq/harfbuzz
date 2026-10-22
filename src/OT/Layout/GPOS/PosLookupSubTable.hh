#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OT_LAYOUT_GPOS_POSLOOKUPSUBTABLE_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_LAYOUT_GPOS_POSLOOKUPSUBTABLE_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "SinglePos.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "PairPos.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "CursivePos.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "MarkBasePos.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "MarkLigPos.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "MarkMarkPos.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "ContextPos.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "ChainContextPos.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "ExtensionPos.hh"

ਨਾਮਾਕਾਸ਼ OT {
ਨਾਮਾਕਾਸ਼ Layout {
ਨਾਮਾਕਾਸ਼ GPOS_impl {

ਬਣਤਰ Pos_Lookup_Sub_ਸਾਰਣੀ
{
  ਮਿੱਤਰ ਬਣਤਰ ::OT::Lookup;
  ਮਿੱਤਰ ਬਣਤਰ PosLookup;

  ਗਿਣਤੀ Type {
    Single              = 1,
    Pair                = 2,
    Cursive             = 3,
    Mark_ਅਧਾਰ            = 4,
    MarkLig             = 5,
    MarkMark            = 6,
    Context             = 7,
    ChainContext        = 8,
    Extension           = 9
  };

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ context_t, ਕਿਸਮ_ਨਾਂ ...Ts>
  ਕਿਸਮ_ਨਾਂ context_t::return_t dispatch (context_t *c, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ lookup_type, Ts&&... ds) ਅਚਲ
  {
    TRACE_DISPATCH (ਇਹ, lookup_type);
    ਚੋਣ (lookup_type) {
    ਹਾਲਤ Single:                hb_barrier (); return_trace (u.single.dispatch (c, std::forward<Ts> (ds)...));
    ਹਾਲਤ Pair:                  hb_barrier (); return_trace (u.pair.dispatch (c, std::forward<Ts> (ds)...));
    ਹਾਲਤ Cursive:               hb_barrier (); return_trace (u.cursive.dispatch (c, std::forward<Ts> (ds)...));
    ਹਾਲਤ Mark_ਅਧਾਰ:              hb_barrier (); return_trace (u.mark_ਅਧਾਰ.dispatch (c, std::forward<Ts> (ds)...));
    ਹਾਲਤ MarkLig:               hb_barrier (); return_trace (u.markLig.dispatch (c, std::forward<Ts> (ds)...));
    ਹਾਲਤ MarkMark:              hb_barrier (); return_trace (u.markMark.dispatch (c, std::forward<Ts> (ds)...));
    ਹਾਲਤ Context:               hb_barrier (); return_trace (u.context.dispatch (c, std::forward<Ts> (ds)...));
    ਹਾਲਤ ChainContext:          hb_barrier (); return_trace (u.chainContext.dispatch (c, std::forward<Ts> (ds)...));
    ਹਾਲਤ Extension:             hb_barrier (); return_trace (u.extension.dispatch (c, std::forward<Ts> (ds)...));
    ਮੂਲ:                    return_trace (c->default_return_ਮੁੱਲ ());
    }
  }

  ਤਾਰਕਿਕ intersects (ਅਚਲ hb_ਸੈੱਟ_t *glyphs, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ lookup_type) ਅਚਲ
  {
    hb_intersects_context_t c (glyphs);
    ਵਾਪਸ dispatch (&c, lookup_type);
  }

  ਸੁਰੱਖਿਅਤ:
  ਸੰਘ {
  SinglePos             single;
  PairPos               pair;
  CursivePos            cursive;
  Mark_ਅਧਾਰ_Pos           mark_ਅਧਾਰ;
  MarkLigPos            markLig;
  MarkMarkPos           markMark;
  ContextPos            context;
  ChainContextPos       chainContext;
  ExtensionPos          extension;
  } u;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_MIN (0);
};

}
}
}

#ਜੇ_ਅੰਤ  /* HB_OT_LAYOUT_GPOS_POSLOOKUPSUBTABLE_HH */
