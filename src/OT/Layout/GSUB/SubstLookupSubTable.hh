#ifndef OT_LAYOUT_GSUB_SUBSTLOOKUPSUBTABLE_HH
#define OT_LAYOUT_GSUB_SUBSTLOOKUPSUBTABLE_HH

#अन्तर्भाव "Common.hh"
#अन्तर्भाव "SingleSubst.hh"
#अन्तर्भाव "MultipleSubst.hh"
#अन्तर्भाव "AlternateSubst.hh"
#अन्तर्भाव "LigatureSubst.hh"
#अन्तर्भाव "ContextSubst.hh"
#अन्तर्भाव "ChainContextSubst.hh"
#अन्तर्भाव "ExtensionSubst.hh"
#अन्तर्भाव "ReverseChainSingleSubst.hh"

नामाकाश OT {
नामाकाश Layout {
नामाकाश GSUB_impl {

संरचना SubstLookupSubTable
{
  मित्र संरचना ::OT::Lookup;
  मित्र संरचना SubstLookup;

  संरक्षित:
  संघ {
  SingleSubst                   single;
  MultipleSubst                 multiple;
  AlternateSubst                alternate;
  LigatureSubst                 ligature;
  ContextSubst                  context;
  ChainContextSubst             chainContext;
  ExtensionSubst                extension;
  ReverseChainSingleSubst       reverseChainContextSingle;
  } u;
  सार्वजनिक:
  DEFINE_SIZE_MIN (0);

  गणना Type {
    Single              = 1,
    Multiple            = 2,
    Alternate           = 3,
    Ligature            = 4,
    Context             = 5,
    ChainContext        = 6,
    Extension           = 7,
    ReverseChainSingle  = 8
  };

  ढाँचा <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, अचिन्हित पूर्णांक lookup_type, Ts&&... ds) स्थिरांक
  {
    TRACE_DISPATCH (हे, lookup_type);
    निवड (lookup_type) {
    स्थिती Single:                hb_barrier (); return_trace (u.single.dispatch (c, std::forward<Ts> (ds)...));
    स्थिती Multiple:              hb_barrier (); return_trace (u.multiple.dispatch (c, std::forward<Ts> (ds)...));
    स्थिती Alternate:             hb_barrier (); return_trace (u.alternate.dispatch (c, std::forward<Ts> (ds)...));
    स्थिती Ligature:              hb_barrier (); return_trace (u.ligature.dispatch (c, std::forward<Ts> (ds)...));
    स्थिती Context:               hb_barrier (); return_trace (u.context.dispatch (c, std::forward<Ts> (ds)...));
    स्थिती ChainContext:          hb_barrier (); return_trace (u.chainContext.dispatch (c, std::forward<Ts> (ds)...));
    स्थिती Extension:             hb_barrier (); return_trace (u.extension.dispatch (c, std::forward<Ts> (ds)...));
    स्थिती ReverseChainSingle:    hb_barrier (); return_trace (u.reverseChainContextSingle.dispatch (c, std::forward<Ts> (ds)...));
    पूर्वनिर्धारित:                    return_trace (c->default_return_मूल्य ());
    }
  }

  तार्किक intersects (स्थिरांक hb_set_t *glyphs, अचिन्हित पूर्णांक lookup_type) स्थिरांक
  {
    hb_intersects_context_t c (glyphs);
    परत dispatch (&c, lookup_type);
  }
};


}
}
}

#endif  /* HB_OT_LAYOUT_GSUB_SUBSTLOOKUPSUBTABLE_HH */
