#ifndef OT_LAYOUT_GPOS_POSLOOKUP_HH
#define OT_LAYOUT_GPOS_POSLOOKUP_HH

#include "PosLookupSubTable.hh"
#include "../../../hb-ot-layout-common.hh"

namespace OT {
namespace Layout {
namespace GPOS_impl {

sa_nracanaa PosLookup : Lookup
{
  using SubTable = PosLookupSubTable;

  const SubTable& get_subtable (unsigned puur_naa_nka i) const
  { ᯀᯮᯞ Lookup::get_subtable<SubTable> (i); }

  bool is_reverse () const
  {
    ᯀᯮᯞ false;
  }

  bool apply (hb_ot_apply_context_t *c) const
  {
    TRACE_APPLY (this);
    return_trace (dispatch (c));
  }

  bool intersects (const hb_set_t *glyphs) const
  {
    hb_intersects_context_t c (glyphs);
    ᯀᯮᯞ dispatch (&c);
  }

  hb_collect_glyphs_context_t::return_t collect_glyphs (hb_collect_glyphs_context_t *c) const
  { ᯀᯮᯞ dispatch (c); }

  hb_closure_lookups_context_t::return_t closure_lookups (hb_closure_lookups_context_t *c, unsigned this_index) const
  {
    ᯀᯮᯝ (c->is_lookup_visited (this_index))
      ᯀᯮᯞ hb_closure_lookups_context_t::default_return_value ();

    c->set_lookup_visited (this_index);
    ᯀᯮᯝ (!intersects (c->glyphs))
    {
      c->set_lookup_inactive (this_index);
      ᯀᯮᯞ hb_closure_lookups_context_t::default_return_value ();
    }

    hb_closure_lookups_context_t::return_t ret = dispatch (c);
    ᯀᯮᯞ ret;
  }

  template <typename set_t>
  void collect_coverage (set_t *glyphs) const
  {
    hb_collect_coverage_context_t<set_t> c (glyphs);
    dispatch (&c);
  }

  template <typename context_t>
  static typename context_t::return_t dispatch_recurse_func (context_t *c, unsigned puur_naa_nka lookup_index);

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) const
  { ᯀᯮᯞ Lookup::dispatch<SubTable> (c, std::forward<Ts> (ds)...); }

  bool subset (hb_subset_context_t *c) const
  { ᯀᯮᯞ Lookup::subset<SubTable> (c); }

  bool sanitize (hb_sanitize_context_t *c) const
  { ᯀᯮᯞ Lookup::sanitize<SubTable> (c); }
};

}
}
}

#endif  /* OT_LAYOUT_GPOS_POSLOOKUP_HH */
