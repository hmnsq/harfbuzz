#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OT_LAYOUT_GSUB_SUBSTLOOKUPSUBTABLE_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_LAYOUT_GSUB_SUBSTLOOKUPSUBTABLE_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "Common.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "SingleSubst.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "MultipleSubst.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "AlternateSubst.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "LigatureSubst.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "ContextSubst.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "ChainContextSubst.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "ExtensionSubst.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "ReverseChainSingleSubst.hh"

ਨਾਮਾਕਾਸ਼ OT {
ਨਾਮਾਕਾਸ਼ Layout {
ਨਾਮਾਕਾਸ਼ GSUB_impl {

ਬਣਤਰ Subst_Lookup_Sub_ਸਾਰਣੀ
{
  ਮਿੱਤਰ ਬਣਤਰ ::OT::Lookup;
  ਮਿੱਤਰ ਬਣਤਰ SubstLookup;

  ਸੁਰੱਖਿਅਤ:
  ਸੰਘ {
  SingleSubst                   single;
  ਗੁਣਜ_Subst                 ਗੁਣਜ_2;
  AlternateSubst                alternate;
  LigatureSubst                 ligature;
  ContextSubst                  context;
  ChainContextSubst             chainContext;
  ExtensionSubst                extension;
  ਉਲਟ_Chain_Single_Subst       ਉਲਟ_Chain_Context_Single;
  } u;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_MIN (0);

  ਗਿਣਤੀ Type {
    Single              = 1,
    ਗੁਣਜ            = 2,
    Alternate           = 3,
    Ligature            = 4,
    Context             = 5,
    ChainContext        = 6,
    Extension           = 7,
    ਉਲਟ_Chain_Single  = 8
  };

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ context_t, ਕਿਸਮ_ਨਾਂ ...Ts>
  ਕਿਸਮ_ਨਾਂ context_t::return_t dispatch (context_t *c, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ lookup_type, Ts&&... ds) ਅਚਲ
  {
    TRACE_DISPATCH (ਇਹ, lookup_type);
    ਚੋਣ (lookup_type) {
    ਹਾਲਤ Single:                hb_barrier (); return_trace (u.single.dispatch (c, std::forward<Ts> (ds)...));
    ਹਾਲਤ ਗੁਣਜ:              hb_barrier (); return_trace (u.ਗੁਣਜ_2.dispatch (c, std::forward<Ts> (ds)...));
    ਹਾਲਤ Alternate:             hb_barrier (); return_trace (u.alternate.dispatch (c, std::forward<Ts> (ds)...));
    ਹਾਲਤ Ligature:              hb_barrier (); return_trace (u.ligature.dispatch (c, std::forward<Ts> (ds)...));
    ਹਾਲਤ Context:               hb_barrier (); return_trace (u.context.dispatch (c, std::forward<Ts> (ds)...));
    ਹਾਲਤ ChainContext:          hb_barrier (); return_trace (u.chainContext.dispatch (c, std::forward<Ts> (ds)...));
    ਹਾਲਤ Extension:             hb_barrier (); return_trace (u.extension.dispatch (c, std::forward<Ts> (ds)...));
    ਹਾਲਤ ਉਲਟ_Chain_Single:    hb_barrier (); return_trace (u.ਉਲਟ_Chain_Context_Single.dispatch (c, std::forward<Ts> (ds)...));
    ਮੂਲ:                    return_trace (c->default_return_ਮੁੱਲ ());
    }
  }

  ਤਾਰਕਿਕ intersects (ਅਚਲ hb_ਸੈੱਟ_t *glyphs, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ lookup_type) ਅਚਲ
  {
    hb_intersects_context_t c (glyphs);
    ਵਾਪਸ dispatch (&c, lookup_type);
  }
};


}
}
}

#ਜੇ_ਅੰਤ  /* HB_OT_LAYOUT_GSUB_SUBSTLOOKUPSUBTABLE_HH */
