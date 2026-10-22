#ifndef OT_LAYOUT_GPOS_POSLOOKUPSUBTABLE_HH
#define OT_LAYOUT_GPOS_POSLOOKUPSUBTABLE_HH

#अन्तर्भाव "SinglePos.hh"
#अन्तर्भाव "PairPos.hh"
#अन्तर्भाव "CursivePos.hh"
#अन्तर्भाव "MarkBasePos.hh"
#अन्तर्भाव "MarkLigPos.hh"
#अन्तर्भाव "MarkMarkPos.hh"
#अन्तर्भाव "ContextPos.hh"
#अन्तर्भाव "ChainContextPos.hh"
#अन्तर्भाव "ExtensionPos.hh"

नामाकाश OT {
नामाकाश Layout {
नामाकाश GPOS_impl {

संरचना PosLookupSubTable
{
  मित्र संरचना ::OT::Lookup;
  मित्र संरचना PosLookup;

  गणना Type {
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

  ढाँचा <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, अचिन्हित पूर्णांक lookup_type, Ts&&... ds) स्थिरांक
  {
    TRACE_DISPATCH (हे, lookup_type);
    निवड (lookup_type) {
    स्थिती Single:                hb_barrier (); return_trace (u.single.dispatch (c, std::forward<Ts> (ds)...));
    स्थिती Pair:                  hb_barrier (); return_trace (u.pair.dispatch (c, std::forward<Ts> (ds)...));
    स्थिती Cursive:               hb_barrier (); return_trace (u.cursive.dispatch (c, std::forward<Ts> (ds)...));
    स्थिती MarkBase:              hb_barrier (); return_trace (u.markBase.dispatch (c, std::forward<Ts> (ds)...));
    स्थिती MarkLig:               hb_barrier (); return_trace (u.markLig.dispatch (c, std::forward<Ts> (ds)...));
    स्थिती MarkMark:              hb_barrier (); return_trace (u.markMark.dispatch (c, std::forward<Ts> (ds)...));
    स्थिती Context:               hb_barrier (); return_trace (u.context.dispatch (c, std::forward<Ts> (ds)...));
    स्थिती ChainContext:          hb_barrier (); return_trace (u.chainContext.dispatch (c, std::forward<Ts> (ds)...));
    स्थिती Extension:             hb_barrier (); return_trace (u.extension.dispatch (c, std::forward<Ts> (ds)...));
    पूर्वनिर्धारित:                    return_trace (c->default_return_मूल्य ());
    }
  }

  तार्किक intersects (स्थिरांक hb_set_t *glyphs, अचिन्हित पूर्णांक lookup_type) स्थिरांक
  {
    hb_intersects_context_t c (glyphs);
    परत dispatch (&c, lookup_type);
  }

  संरक्षित:
  संघ {
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
  सार्वजनिक:
  DEFINE_SIZE_MIN (0);
};

}
}
}

#endif  /* HB_OT_LAYOUT_GPOS_POSLOOKUPSUBTABLE_HH */
