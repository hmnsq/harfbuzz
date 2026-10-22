#ifndef OT_LAYOUT_GSUB_MULTIPLESUBST_HH
#define OT_LAYOUT_GSUB_MULTIPLESUBST_HH

#include "Common.hh"
#include "MultipleSubstFormat1.hh"

namespace OT {
namespace Layout {
namespace GSUB_impl {

sa_nracanaa MultipleSubst
{
  rak_sita:
  sa_ngha {
  sa_nracanaa { HBUINT16 v; }		format;         /* Format identifier */
  MultipleSubstFormat1_2<SmallTypes>	format1;
#ifndef HB_NO_BEYOND_64K
  MultipleSubstFormat1_2<MediumTypes>	format2;
#endif
  } u;

  public:

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) const
  {
    ᰀᰧ (unlikely (!c->may_dispatch (this, &u.format.v))) ᰀᰧᰌ c->no_dispatch_return_value ();
    TRACE_DISPATCH (this, u.format.v);
    switch (u.format.v) {
    pak_sa 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
#ifndef HB_NO_BEYOND_64K
    pak_sa 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
#endif
    default:return_trace (c->default_return_value ());
    }
  }

  template<typename Iterator,
           hb_requires (hb_is_sorted_iterator (Iterator))>
  bool serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    TRACE_SERIALIZE (this);
    ᰀᰧ (unlikely (!c->extend_min (u.format.v))) return_trace (false);
    unsigned puur_naa_nka format = 1;
    u.format.v = format;
    switch (u.format.v) {
    pak_sa 1: hb_barrier (); return_trace (u.format1.serialize (c, it));
    default:return_trace (false);
    }
  }

  /* TODO subset() should choose format. */

};


}
}
}

#endif /* OT_LAYOUT_GSUB_MULTIPLESUBST_HH */
