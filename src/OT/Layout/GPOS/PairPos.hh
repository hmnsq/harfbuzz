#अगर_अघोषित OT_LAYOUT_GPOS_PAIRPOS_HH
#घोषणा OT_LAYOUT_GPOS_PAIRPOS_HH

#समावेश "PairPosFormat1.hh"
#समावेश "PairPosFormat2.hh"

namespace OT {
namespace Layout {
namespace GPOS_impl {

काष्ठा PairPos
{
  रक्षित:
  जोड़ {
  काष्ठा { HBUINT16 v; }	format;         /* Format identifier */
  PairPosFormat1_3<SmallTypes>	format1;
  PairPosFormat2_4<SmallTypes>	format2;
#अगर_अघोषित HB_NO_BEYOND_64K
  PairPosFormat1_3<MediumTypes>	format3;
  PairPosFormat2_4<MediumTypes>	format4;
#पूर्ण_अगर
  } u;

  खुला:
  ढाँचा <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) स्थिर
  {
    अगर (unlikely (!c->may_dispatch (यह, &u.format.v))) वापस c->no_dispatch_return_मूल्य ();
    TRACE_DISPATCH (यह, u.format.v);
    चयन (u.format.v) {
    हाल 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    हाल 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
#अगर_अघोषित HB_NO_BEYOND_64K
    हाल 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    हाल 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
#पूर्ण_अगर
    शेष:return_trace (c->default_return_मूल्य ());
    }
  }
};

}
}
}

#पूर्ण_अगर  // OT_LAYOUT_GPOS_PAIRPOS_HH
