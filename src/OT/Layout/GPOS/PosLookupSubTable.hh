#ifndef OT_LAYOUT_GPOS_POSLOOKUPSUBTABLE_HH
#define OT_LAYOUT_GPOS_POSLOOKUPSUBTABLE_HH

#ಅನ್ತರ್ಭಾವ "SinglePos.hh"
#ಅನ್ತರ್ಭಾವ "PairPos.hh"
#ಅನ್ತರ್ಭಾವ "CursivePos.hh"
#ಅನ್ತರ್ಭಾವ "MarkBasePos.hh"
#ಅನ್ತರ್ಭಾವ "MarkLigPos.hh"
#ಅನ್ತರ್ಭಾವ "MarkMarkPos.hh"
#ಅನ್ತರ್ಭಾವ "ContextPos.hh"
#ಅನ್ತರ್ಭಾವ "ChainContextPos.hh"
#ಅನ್ತರ್ಭಾವ "ExtensionPos.hh"

ನಾಮಾಕಾಶ OT {
ನಾಮಾಕಾಶ Layout {
ನಾಮಾಕಾಶ GPOS_impl {

ಸಂರಚನಾ PosLookupSubTable
{
  friend ಸಂರಚನಾ ::OT::Lookup;
  friend ಸಂರಚನಾ PosLookup;

  ಗಣನಾ Type {
    Single              = 1,
    Pair                = 2,
    Cursive             = 3,
    MarkBase            = 4,
    MarkLig             = 5,
    MarkMark            = 6,
    Context             = 7,
    ChainContext        = 8,
    Extension           = 9
  };

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, unsigned ಪೂರ್ಣಾಙ್ಕ lookup_type, Ts&&... ds) ಧ್ರುವ
  {
    TRACE_DISPATCH (ಏತತ್, lookup_type);
    ವಿಕಲ್ಪ (lookup_type) {
    ಪಕ್ಷ Single:                hb_barrier (); return_trace (u.single.dispatch (c, std::forward<Ts> (ds)...));
    ಪಕ್ಷ Pair:                  hb_barrier (); return_trace (u.pair.dispatch (c, std::forward<Ts> (ds)...));
    ಪಕ್ಷ Cursive:               hb_barrier (); return_trace (u.cursive.dispatch (c, std::forward<Ts> (ds)...));
    ಪಕ್ಷ MarkBase:              hb_barrier (); return_trace (u.markBase.dispatch (c, std::forward<Ts> (ds)...));
    ಪಕ್ಷ MarkLig:               hb_barrier (); return_trace (u.markLig.dispatch (c, std::forward<Ts> (ds)...));
    ಪಕ್ಷ MarkMark:              hb_barrier (); return_trace (u.markMark.dispatch (c, std::forward<Ts> (ds)...));
    ಪಕ್ಷ Context:               hb_barrier (); return_trace (u.context.dispatch (c, std::forward<Ts> (ds)...));
    ಪಕ್ಷ ChainContext:          hb_barrier (); return_trace (u.chainContext.dispatch (c, std::forward<Ts> (ds)...));
    ಪಕ್ಷ Extension:             hb_barrier (); return_trace (u.extension.dispatch (c, std::forward<Ts> (ds)...));
    ಸ್ವಭಾವ:                    return_trace (c->default_return_ಮೌಲ್ಯ ());
    }
  }

  ತಾರ್ಕಿಕ intersects (ಧ್ರುವ hb_set_t *glyphs, unsigned ಪೂರ್ಣಾಙ್ಕ lookup_type) ಧ್ರುವ
  {
    hb_intersects_context_t c (glyphs);
    ಪ್ರತ್ಯಾಗಮ dispatch (&c, lookup_type);
  }

  ರಕ್ಷಿತ:
  ಸಙ್ಘ {
  SinglePos             single;
  PairPos               pair;
  CursivePos            cursive;
  MarkBasePos           markBase;
  MarkLigPos            markLig;
  MarkMarkPos           markMark;
  ContextPos            context;
  ChainContextPos       chainContext;
  ExtensionPos          extension;
  } u;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_MIN (0);
};

}
}
}

#endif  /* HB_OT_LAYOUT_GPOS_POSLOOKUPSUBTABLE_HH */
