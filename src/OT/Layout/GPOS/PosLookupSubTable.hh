#अगर_अघोषित OT_LAYOUT_GPOS_POSLOOKUPSUBTABLE_HH
#घोषणा OT_LAYOUT_GPOS_POSLOOKUPSUBTABLE_HH

#समावेश "SinglePos.hh"
#समावेश "PairPos.hh"
#समावेश "CursivePos.hh"
#समावेश "MarkBasePos.hh"
#समावेश "MarkLigPos.hh"
#समावेश "MarkMarkPos.hh"
#समावेश "ContextPos.hh"
#समावेश "ChainContextPos.hh"
#समावेश "ExtensionPos.hh"

namespace OT {
namespace Layout {
namespace GPOS_impl {

काष्ठा PosLookupSubTable
{
  मित्र काष्ठा ::OT::Lookup;
  मित्र काष्ठा PosLookup;

  क्रमागत Type {
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
  typename context_t::return_t dispatch (context_t *c, अचिन्हित पूर्णांक lookup_type, Ts&&... ds) स्थिर
  {
    TRACE_DISPATCH (यह, lookup_type);
    चयन (lookup_type) {
    हाल Single:                hb_barrier (); return_trace (u.single.dispatch (c, std::forward<Ts> (ds)...));
    हाल Pair:                  hb_barrier (); return_trace (u.pair.dispatch (c, std::forward<Ts> (ds)...));
    हाल Cursive:               hb_barrier (); return_trace (u.cursive.dispatch (c, std::forward<Ts> (ds)...));
    हाल MarkBase:              hb_barrier (); return_trace (u.markBase.dispatch (c, std::forward<Ts> (ds)...));
    हाल MarkLig:               hb_barrier (); return_trace (u.markLig.dispatch (c, std::forward<Ts> (ds)...));
    हाल MarkMark:              hb_barrier (); return_trace (u.markMark.dispatch (c, std::forward<Ts> (ds)...));
    हाल Context:               hb_barrier (); return_trace (u.context.dispatch (c, std::forward<Ts> (ds)...));
    हाल ChainContext:          hb_barrier (); return_trace (u.chainContext.dispatch (c, std::forward<Ts> (ds)...));
    हाल Extension:             hb_barrier (); return_trace (u.extension.dispatch (c, std::forward<Ts> (ds)...));
    शेष:                    return_trace (c->default_return_मूल्य ());
    }
  }

  bool intersects (स्थिर hb_set_t *glyphs, अचिन्हित पूर्णांक lookup_type) स्थिर
  {
    hb_intersects_context_t c (glyphs);
    वापस dispatch (&c, lookup_type);
  }

  रक्षित:
  जोड़ {
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
  खुला:
  DEFINE_आकार_MIN (0);
};

}
}
}

#पूर्ण_अगर  /* HB_OT_LAYOUT_GPOS_POSLOOKUPSUBTABLE_HH */
