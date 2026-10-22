#अगर_अघोषित OT_LAYOUT_GPOS_MARKBASEPOS_HH
#घोषणा OT_LAYOUT_GPOS_MARKBASEPOS_HH

#समावेश "MarkBasePosFormat1.hh"

namespace OT {
namespace Layout {
namespace GPOS_impl {

काष्ठा MarkBasePos
{
  रक्षित:
  जोड़ {
  काष्ठा { HBUINT16 v; }		format;         /* Format identifier */
  MarkBasePosFormat1_2<SmallTypes>	format1;
#अगर_अघोषित HB_NO_BEYOND_64K
  MarkBasePosFormat1_2<MediumTypes>	format2;
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
#अगर_अघोषित HB_NO_BEYOND_64K
    हाल 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
#पूर्ण_अगर
    शेष:return_trace (c->default_return_मूल्य ());
    }
  }
};

}
}
}

#पूर्ण_अगर /* OT_LAYOUT_GPOS_MARKBASEPOS_HH */
