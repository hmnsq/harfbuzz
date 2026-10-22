#ifndef OT_LAYOUT_GSUB_SUBSTLOOKUPSUBTABLE_HH
#define OT_LAYOUT_GSUB_SUBSTLOOKUPSUBTABLE_HH

#ಅನ್ತರ್ಭಾವ "Common.hh"
#ಅನ್ತರ್ಭಾವ "SingleSubst.hh"
#ಅನ್ತರ್ಭಾವ "MultipleSubst.hh"
#ಅನ್ತರ್ಭಾವ "AlternateSubst.hh"
#ಅನ್ತರ್ಭಾವ "LigatureSubst.hh"
#ಅನ್ತರ್ಭಾವ "ContextSubst.hh"
#ಅನ್ತರ್ಭಾವ "ChainContextSubst.hh"
#ಅನ್ತರ್ಭಾವ "ExtensionSubst.hh"
#ಅನ್ತರ್ಭಾವ "ReverseChainSingleSubst.hh"

ನಾಮಾಕಾಶ OT {
ನಾಮಾಕಾಶ Layout {
ನಾಮಾಕಾಶ GSUB_impl {

ಸಂರಚನಾ SubstLookupSubTable
{
  friend ಸಂರಚನಾ ::OT::Lookup;
  friend ಸಂರಚನಾ SubstLookup;

  ರಕ್ಷಿತ:
  ಸಙ್ಘ {
  SingleSubst                   single;
  MultipleSubst                 multiple;
  AlternateSubst                alternate;
  LigatureSubst                 ligature;
  ContextSubst                  context;
  ChainContextSubst             chainContext;
  ExtensionSubst                extension;
  ReverseChainSingleSubst       reverseChainContextSingle;
  } u;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_MIN (0);

  ಗಣನಾ Type {
    Single              = 1,
    Multiple            = 2,
    Alternate           = 3,
    Ligature            = 4,
    Context             = 5,
    ChainContext        = 6,
    Extension           = 7,
    ReverseChainSingle  = 8
  };

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, unsigned ಪೂರ್ಣಾಙ್ಕ lookup_type, Ts&&... ds) ಧ್ರುವ
  {
    TRACE_DISPATCH (ಏತತ್, lookup_type);
    ವಿಕಲ್ಪ (lookup_type) {
    ಪಕ್ಷ Single:                hb_barrier (); return_trace (u.single.dispatch (c, std::forward<Ts> (ds)...));
    ಪಕ್ಷ Multiple:              hb_barrier (); return_trace (u.multiple.dispatch (c, std::forward<Ts> (ds)...));
    ಪಕ್ಷ Alternate:             hb_barrier (); return_trace (u.alternate.dispatch (c, std::forward<Ts> (ds)...));
    ಪಕ್ಷ Ligature:              hb_barrier (); return_trace (u.ligature.dispatch (c, std::forward<Ts> (ds)...));
    ಪಕ್ಷ Context:               hb_barrier (); return_trace (u.context.dispatch (c, std::forward<Ts> (ds)...));
    ಪಕ್ಷ ChainContext:          hb_barrier (); return_trace (u.chainContext.dispatch (c, std::forward<Ts> (ds)...));
    ಪಕ್ಷ Extension:             hb_barrier (); return_trace (u.extension.dispatch (c, std::forward<Ts> (ds)...));
    ಪಕ್ಷ ReverseChainSingle:    hb_barrier (); return_trace (u.reverseChainContextSingle.dispatch (c, std::forward<Ts> (ds)...));
    ಸ್ವಭಾವ:                    return_trace (c->default_return_ಮೌಲ್ಯ ());
    }
  }

  ತಾರ್ಕಿಕ intersects (ಧ್ರುವ hb_set_t *glyphs, unsigned ಪೂರ್ಣಾಙ್ಕ lookup_type) ಧ್ರುವ
  {
    hb_intersects_context_t c (glyphs);
    ಪ್ರತ್ಯಾಗಮ dispatch (&c, lookup_type);
  }
};


}
}
}

#endif  /* HB_OT_LAYOUT_GSUB_SUBSTLOOKUPSUBTABLE_HH */
