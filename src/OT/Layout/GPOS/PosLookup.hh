#अगर_अघोषित OT_LAYOUT_GPOS_POSLOOKUP_HH
#घोषणा OT_LAYOUT_GPOS_POSLOOKUP_HH

#समावेश "PosLookupSubTable.hh"
#समावेश "../../../hb-ot-layout-common.hh"

namespace OT {
namespace Layout {
namespace GPOS_impl {

काष्ठा PosLookup : Lookup
{
  using SubTable = PosLookupSubTable;

  स्थिर SubTable& get_subtable (अचिन्हित पूर्णांक i) स्थिर
  { वापस Lookup::get_subtable<SubTable> (i); }

  bool is_reverse () स्थिर
  {
    वापस false;
  }

  bool apply (hb_ot_apply_context_t *c) स्थिर
  {
    TRACE_APPLY (यह);
    return_trace (dispatch (c));
  }

  bool intersects (स्थिर hb_set_t *glyphs) स्थिर
  {
    hb_intersects_context_t c (glyphs);
    वापस dispatch (&c);
  }

  hb_collect_glyphs_context_t::return_t collect_glyphs (hb_collect_glyphs_context_t *c) स्थिर
  { वापस dispatch (c); }

  hb_closure_lookups_context_t::return_t closure_lookups (hb_closure_lookups_context_t *c, अचिन्हित this_सूचकांक) स्थिर
  {
    अगर (c->is_lookup_visited (this_सूचकांक))
      वापस hb_closure_lookups_context_t::default_return_मूल्य ();

    c->set_lookup_visited (this_सूचकांक);
    अगर (!intersects (c->glyphs))
    {
      c->set_lookup_inactive (this_सूचकांक);
      वापस hb_closure_lookups_context_t::default_return_मूल्य ();
    }

    hb_closure_lookups_context_t::return_t ret = dispatch (c);
    वापस ret;
  }

  ढाँचा <typename set_t>
  व्योम collect_coverage (set_t *glyphs) स्थिर
  {
    hb_collect_coverage_context_t<set_t> c (glyphs);
    dispatch (&c);
  }

  ढाँचा <typename context_t>
  अटल typename context_t::return_t dispatch_recurse_func (context_t *c, अचिन्हित पूर्णांक lookup_सूचकांक);

  ढाँचा <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) स्थिर
  { वापस Lookup::dispatch<SubTable> (c, std::forward<Ts> (ds)...); }

  bool subset (hb_subset_context_t *c) स्थिर
  { वापस Lookup::subset<SubTable> (c); }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  { वापस Lookup::sanitize<SubTable> (c); }
};

}
}
}

#पूर्ण_अगर  /* OT_LAYOUT_GPOS_POSLOOKUP_HH */
