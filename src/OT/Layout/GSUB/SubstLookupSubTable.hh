#अगर_अघोषित OT_LAYOUT_GSUB_SUBSTLOOKUPSUBTABLE_HH
#घोषणा OT_LAYOUT_GSUB_SUBSTLOOKUPSUBTABLE_HH

#समावेश "Common.hh"
#समावेश "SingleSubst.hh"
#समावेश "MultipleSubst.hh"
#समावेश "AlternateSubst.hh"
#समावेश "LigatureSubst.hh"
#समावेश "ContextSubst.hh"
#समावेश "ChainContextSubst.hh"
#समावेश "ExtensionSubst.hh"
#समावेश "ReverseChainSingleSubst.hh"

namespace OT {
namespace Layout {
namespace GSUB_impl {

काष्ठा SubstLookupSubTable
{
  मित्र काष्ठा ::OT::Lookup;
  मित्र काष्ठा SubstLookup;

  रक्षित:
  जोड़ {
  SingleSubst                   single;
  MultipleSubst                 multiple;
  AlternateSubst                alternate;
  LigatureSubst                 ligature;
  ContextSubst                  context;
  ChainContextSubst             chainContext;
  ExtensionSubst                extension;
  ReverseChainSingleSubst       reverseChainContextSingle;
  } u;
  खुला:
  DEFINE_आकार_MIN (0);

  क्रमागत Type {
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
  typename context_t::return_t dispatch (context_t *c, अचिन्हित पूर्णांक lookup_type, Ts&&... ds) स्थिर
  {
    TRACE_DISPATCH (यह, lookup_type);
    चयन (lookup_type) {
    हाल Single:                hb_barrier (); return_trace (u.single.dispatch (c, std::forward<Ts> (ds)...));
    हाल Multiple:              hb_barrier (); return_trace (u.multiple.dispatch (c, std::forward<Ts> (ds)...));
    हाल Alternate:             hb_barrier (); return_trace (u.alternate.dispatch (c, std::forward<Ts> (ds)...));
    हाल Ligature:              hb_barrier (); return_trace (u.ligature.dispatch (c, std::forward<Ts> (ds)...));
    हाल Context:               hb_barrier (); return_trace (u.context.dispatch (c, std::forward<Ts> (ds)...));
    हाल ChainContext:          hb_barrier (); return_trace (u.chainContext.dispatch (c, std::forward<Ts> (ds)...));
    हाल Extension:             hb_barrier (); return_trace (u.extension.dispatch (c, std::forward<Ts> (ds)...));
    हाल ReverseChainSingle:    hb_barrier (); return_trace (u.reverseChainContextSingle.dispatch (c, std::forward<Ts> (ds)...));
    शेष:                    return_trace (c->default_return_मूल्य ());
    }
  }

  bool intersects (स्थिर hb_set_t *glyphs, अचिन्हित पूर्णांक lookup_type) स्थिर
  {
    hb_intersects_context_t c (glyphs);
    वापस dispatch (&c, lookup_type);
  }
};


}
}
}

#पूर्ण_अगर  /* HB_OT_LAYOUT_GSUB_SUBSTLOOKUPSUBTABLE_HH */
