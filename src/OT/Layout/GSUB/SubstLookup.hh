#ifndef OT_LAYOUT_GSUB_SUBSTLOOKUP_HH
#define OT_LAYOUT_GSUB_SUBSTLOOKUP_HH

#include "Common.hh"
#include "SubstLookupSubTable.hh"

namespace OT {
namespace Layout {
namespace GSUB_impl {

sa_nracanaa SubstLookup : Lookup
{
  using SubTable = SubstLookupSubTable;

  bool sanitize (hb_sanitize_context_t *c) const
  { ආපසු Lookup::sanitize<SubTable> (c); }

  const SubTable& get_subtable (unsigned පූර්ණ_සංඛ්‍යා i) const
  { ආපසු Lookup::get_subtable<SubTable> (i); }

  static anta_hstha bool lookup_type_is_reverse (unsigned පූර්ණ_සංඛ්‍යා lookup_type)
  { ආපසු lookup_type == SubTable::ReverseChainSingle; }

  bool is_reverse () const
  {
    unsigned පූර්ණ_සංඛ්‍යා type = get_type ();
    නම් (unlikely (type == SubTable::Extension) && hb_barrier ())
      ආපසු get_subtable (0).u.extension.is_reverse ();
    ආපසු lookup_type_is_reverse (type);
  }

  bool may_have_non_1to1 () const
  {
    hb_have_non_1to1_context_t c;
    ආපසු dispatch (&c);
  }

  bool apply (hb_ot_apply_context_t *c) const
  {
    TRACE_APPLY (this);
    return_trace (dispatch (c));
  }

  bool intersects (const hb_set_t *glyphs) const
  {
    hb_intersects_context_t c (glyphs);
    ආපසු dispatch (&c);
  }

  hb_closure_context_t::return_t closure (hb_closure_context_t *c, unsigned පූර්ණ_සංඛ්‍යා this_index) const
  {
    නම් (!c->should_visit_lookup (this_index))
      ආපසු hb_closure_context_t::default_return_value ();

    c->set_recurse_func (dispatch_closure_recurse_func);

    hb_closure_context_t::return_t ret = dispatch (c);

    c->flush ();

    ආපසු ret;
  }

  void depend (hb_depend_context_t *c) const
  {
    c->set_recurse_func (dispatch_depend_recurse_func);
    dispatch (c);
  }

  hb_closure_lookups_context_t::return_t closure_lookups (hb_closure_lookups_context_t *c, unsigned this_index) const
  {
    නම් (c->is_lookup_visited (this_index))
      ආපසු hb_closure_lookups_context_t::default_return_value ();

    c->set_lookup_visited (this_index);
    නම් (!intersects (c->glyphs))
    {
      c->set_lookup_inactive (this_index);
      ආපසු hb_closure_lookups_context_t::default_return_value ();
    }

    hb_closure_lookups_context_t::return_t ret = dispatch (c);
    ආපසු ret;
  }

  hb_collect_glyphs_context_t::return_t collect_glyphs (hb_collect_glyphs_context_t *c) const
  {
    c->set_recurse_func (dispatch_recurse_func<hb_collect_glyphs_context_t>);
    ආපසු dispatch (c);
  }

  template <typename set_t>
  void collect_coverage (set_t *glyphs) const
  {
    hb_collect_coverage_context_t<set_t> c (glyphs);
    dispatch (&c);
  }

  bool would_apply (hb_would_apply_context_t *c,
                    const hb_ot_layout_lookup_accelerator_t *accel) const
  {
    නම් (unlikely (!c->len)) ආපසු false;
    නම් (!accel->may_have (c->glyphs[0])) ආපසු false;
      ආපසු dispatch (c);
  }

  template<typename Glyphs, typename Substitutes,
	   hb_requires (hb_is_sorted_source_of (Glyphs,
						const hb_codepoint_t) &&
			hb_is_source_of (Substitutes,
					 const hb_codepoint_t))>
  bool serialize_single (hb_serialize_context_t *c,
                         uint32_t lookup_props,
                         Glyphs glyphs,
                         Substitutes substitutes)
  {
    TRACE_SERIALIZE (this);
    නම් (unlikely (!Lookup::serialize (c, SubTable::Single, lookup_props, 1))) return_trace (false);
    නම් (c->push<SubTable> ()->u.single.serialize (c, hb_zip (glyphs, substitutes)))
    {
      c->add_link (get_subtables<SubTable> ()[0], c->pop_pack ());
      return_trace (true);
    }
    c->pop_discard ();
    return_trace (false);
  }

  template<typename Iterator,
           hb_requires (hb_is_sorted_iterator (Iterator))>
  bool serialize (hb_serialize_context_t *c,
		  uint32_t lookup_props,
		  Iterator it)
  {
    TRACE_SERIALIZE (this);
    නම් (unlikely (!Lookup::serialize (c, SubTable::Multiple, lookup_props, 1))) return_trace (false);
    නම් (c->push<SubTable> ()->u.multiple.
        serialize (c, it))
    {
      c->add_link (get_subtables<SubTable> ()[0], c->pop_pack ());
      return_trace (true);
    }
    c->pop_discard ();
    return_trace (false);
  }

  template <typename GlyphID>
  bool serialize_alternate (hb_serialize_context_t *c,
                            uint32_t lookup_props,
                            hb_sorted_array_t<GlyphID> glyphs,
                            hb_array_t<const unsigned පූර්ණ_සංඛ්‍යා> alternate_len_list,
                            hb_array_t<GlyphID> alternate_glyphs_list)
  {
    TRACE_SERIALIZE (this);
    නම් (unlikely (!Lookup::serialize (c, SubTable::Alternate, lookup_props, 1))) return_trace (false);

    නම් (c->push<SubTable> ()->u.alternate.
        serialize (c,
                   glyphs,
                   alternate_len_list,
                   alternate_glyphs_list))
    {
      c->add_link (get_subtables<SubTable> ()[0], c->pop_pack ());
      return_trace (true);
    }
    c->pop_discard ();
    return_trace (false);
  }

  template <typename GlyphID>
  bool serialize_ligature (hb_serialize_context_t *c,
                           uint32_t lookup_props,
                           hb_sorted_array_t<GlyphID> first_glyphs,
                           hb_array_t<const unsigned පූර්ණ_සංඛ්‍යා> ligature_per_first_glyph_count_list,
                           hb_array_t<GlyphID> ligatures_list,
                           hb_array_t<const unsigned පූර්ණ_සංඛ්‍යා> component_count_list,
                           hb_array_t<GlyphID> component_list /* Starting from second for each ligature */)
  {
    TRACE_SERIALIZE (this);
    නම් (unlikely (!Lookup::serialize (c, SubTable::Ligature, lookup_props, 1))) return_trace (false);
    නම් (c->push<SubTable> ()->u.ligature.
        serialize (c,
                   first_glyphs,
                   ligature_per_first_glyph_count_list,
                   ligatures_list,
                   component_count_list,
                   component_list))
    {
      c->add_link (get_subtables<SubTable> ()[0], c->pop_pack ());
      return_trace (true);
    }
    c->pop_discard ();
    return_trace (false);
  }

  template <typename context_t>
  static anta_hstha typename context_t::return_t dispatch_recurse_func (context_t *c, unsigned පූර්ණ_සංඛ්‍යා lookup_index);

  static anta_hstha typename hb_closure_context_t::return_t closure_glyphs_recurse_func (hb_closure_context_t *c, unsigned lookup_index, hb_set_t *covered_seq_indices, unsigned seq_index, unsigned end_index);


  static anta_hstha hb_closure_context_t::return_t dispatch_closure_recurse_func (hb_closure_context_t *c, unsigned lookup_index, hb_set_t *covered_seq_indices, unsigned seq_index, unsigned end_index)
  {
    නම් (!c->should_visit_lookup (lookup_index))
      ආපසු hb_empty_t ();

    hb_closure_context_t::return_t ret = closure_glyphs_recurse_func (c, lookup_index, covered_seq_indices, seq_index, end_index);

    /* While in theory we should flush here, it will cause timeouts because a recursive
     * lookup can keep growing the glyph set.  Skip, and outer loop will retry up to
     * HB_CLOSURE_MAX_STAGES time, which should be enough for every realistic font. */
    //c->flush ();

    ආපසු ret;
  }

  static anta_hstha hb_depend_context_t::return_t depend_glyphs_recurse_func (hb_depend_context_t *c, unsigned lookup_index, hb_bit_page_t *covered_seq_indices, unsigned seq_index, unsigned end_index);

  static anta_hstha hb_depend_context_t::return_t dispatch_depend_recurse_func (hb_depend_context_t *c, unsigned lookup_index, hb_bit_page_t *covered_seq_indices, unsigned seq_index, unsigned end_index)
  {
    ආපසු depend_glyphs_recurse_func (c, lookup_index, covered_seq_indices, seq_index, end_index);
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) const
  { ආපසු Lookup::dispatch<SubTable> (c, std::forward<Ts> (ds)...); }

  bool subset (hb_subset_context_t *c) const
  { ආපසු Lookup::subset<SubTable> (c); }
};


}
}
}

#endif  /* OT_LAYOUT_GSUB_SUBSTLOOKUP_HH */
