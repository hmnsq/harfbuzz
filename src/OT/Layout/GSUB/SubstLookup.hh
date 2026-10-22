#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OT_LAYOUT_GSUB_SUBSTLOOKUP_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_LAYOUT_GSUB_SUBSTLOOKUP_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "Common.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "SubstLookupSubTable.hh"

ਨਾਮਾਕਾਸ਼ OT {
ਨਾਮਾਕਾਸ਼ Layout {
ਨਾਮਾਕਾਸ਼ GSUB_impl {

ਬਣਤਰ SubstLookup : Lookup
{
  ਵਰਤੋਂ Sub_ਸਾਰਣੀ = Subst_Lookup_Sub_ਸਾਰਣੀ;

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  { ਵਾਪਸ Lookup::sanitize<Sub_ਸਾਰਣੀ> (c); }

  ਅਚਲ Sub_ਸਾਰਣੀ& get_subtable (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i) ਅਚਲ
  { ਵਾਪਸ Lookup::get_subtable<Sub_ਸਾਰਣੀ> (i); }

  ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ lookup_type_is_ਉਲਟ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ lookup_type)
  { ਵਾਪਸ lookup_type == Sub_ਸਾਰਣੀ::ਉਲਟ_Chain_Single; }

  ਤਾਰਕਿਕ is_ਉਲਟ () ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ type = get_type ();
    ਜੇ (unlikely (type == Sub_ਸਾਰਣੀ::Extension) && hb_barrier ())
      ਵਾਪਸ get_subtable (0).u.extension.is_ਉਲਟ ();
    ਵਾਪਸ lookup_type_is_ਉਲਟ (type);
  }

  ਤਾਰਕਿਕ may_have_non_1to1 () ਅਚਲ
  {
    hb_have_non_1to1_context_t c;
    ਵਾਪਸ dispatch (&c);
  }

  ਤਾਰਕਿਕ apply (hb_ot_apply_context_t *c) ਅਚਲ
  {
    TRACE_APPLY (ਇਹ);
    return_trace (dispatch (c));
  }

  ਤਾਰਕਿਕ intersects (ਅਚਲ hb_ਸੈੱਟ_t *glyphs) ਅਚਲ
  {
    hb_intersects_context_t c (glyphs);
    ਵਾਪਸ dispatch (&c);
  }

  hb_ਘੇਰਾ_context_t::return_t ਘੇਰਾ (hb_ਘੇਰਾ_context_t *c, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ this_ਸੂਚਕਾਂਕ) ਅਚਲ
  {
    ਜੇ (!c->should_ਦੇਖੋ_lookup (this_ਸੂਚਕਾਂਕ))
      ਵਾਪਸ hb_ਘੇਰਾ_context_t::default_return_ਮੁੱਲ ();

    c->ਸੈੱਟ_recurse_func (dispatch_ਘੇਰਾ_recurse_func);

    hb_ਘੇਰਾ_context_t::return_t ret = dispatch (c);

    c->flush ();

    ਵਾਪਸ ret;
  }

  ਖਾਲੀ depend (hb_depend_context_t *c) ਅਚਲ
  {
    c->ਸੈੱਟ_recurse_func (dispatch_depend_recurse_func);
    dispatch (c);
  }

  hb_ਘੇਰਾ_lookups_context_t::return_t ਘੇਰਾ_lookups (hb_ਘੇਰਾ_lookups_context_t *c, ਬਿਨਾਨਿਸ਼ਾਨੀ this_ਸੂਚਕਾਂਕ) ਅਚਲ
  {
    ਜੇ (c->is_lookup_ਦੇਖਿਆ (this_ਸੂਚਕਾਂਕ))
      ਵਾਪਸ hb_ਘੇਰਾ_lookups_context_t::default_return_ਮੁੱਲ ();

    c->ਸੈੱਟ_lookup_ਦੇਖਿਆ (this_ਸੂਚਕਾਂਕ);
    ਜੇ (!intersects (c->glyphs))
    {
      c->ਸੈੱਟ_lookup_inactive (this_ਸੂਚਕਾਂਕ);
      ਵਾਪਸ hb_ਘੇਰਾ_lookups_context_t::default_return_ਮੁੱਲ ();
    }

    hb_ਘੇਰਾ_lookups_context_t::return_t ret = dispatch (c);
    ਵਾਪਸ ret;
  }

  hb_collect_glyphs_context_t::return_t collect_glyphs (hb_collect_glyphs_context_t *c) ਅਚਲ
  {
    c->ਸੈੱਟ_recurse_func (dispatch_recurse_func<hb_collect_glyphs_context_t>);
    ਵਾਪਸ dispatch (c);
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਸੈੱਟ_t>
  ਖਾਲੀ collect_coverage (ਸੈੱਟ_t *glyphs) ਅਚਲ
  {
    hb_collect_coverage_context_t<ਸੈੱਟ_t> c (glyphs);
    dispatch (&c);
  }

  ਤਾਰਕਿਕ would_apply (hb_would_apply_context_t *c,
                    ਅਚਲ hb_ot_layout_lookup_accelerator_t *accel) ਅਚਲ
  {
    ਜੇ (unlikely (!c->len)) ਵਾਪਸ ਝੂਠ;
    ਜੇ (!accel->may_have (c->glyphs[0])) ਵਾਪਸ ਝੂਠ;
      ਵਾਪਸ dispatch (c);
  }

  ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ Glyphs, ਕਿਸਮ_ਨਾਂ Substitutes,
	   hb_requires (hb_is_sorted_ਸਰੋਤ_ਦਾ (Glyphs,
						ਅਚਲ hb_codepoint_t) &&
			hb_is_ਸਰੋਤ_ਦਾ (Substitutes,
					 ਅਚਲ hb_codepoint_t))>
  ਤਾਰਕਿਕ serialize_single (hb_serialize_context_t *c,
                         uint32_t lookup_props,
                         Glyphs glyphs,
                         Substitutes substitutes)
  {
    TRACE_SERIALIZE (ਇਹ);
    ਜੇ (unlikely (!Lookup::serialize (c, Sub_ਸਾਰਣੀ::Single, lookup_props, 1))) return_trace (ਝੂਠ);
    ਜੇ (c->push<Sub_ਸਾਰਣੀ> ()->u.single.serialize (c, hb_zip (glyphs, substitutes)))
    {
      c->ਜੋੜੋ_link (get_subtables<Sub_ਸਾਰਣੀ> ()[0], c->ਕੱਢੋ_pack ());
      return_trace (ਸੱਚ);
    }
    c->ਕੱਢੋ_discard ();
    return_trace (ਝੂਠ);
  }

  ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ Iterator,
           hb_requires (hb_is_sorted_iterator (Iterator))>
  ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c,
		  uint32_t lookup_props,
		  Iterator it)
  {
    TRACE_SERIALIZE (ਇਹ);
    ਜੇ (unlikely (!Lookup::serialize (c, Sub_ਸਾਰਣੀ::ਗੁਣਜ, lookup_props, 1))) return_trace (ਝੂਠ);
    ਜੇ (c->push<Sub_ਸਾਰਣੀ> ()->u.ਗੁਣਜ_2.
        serialize (c, it))
    {
      c->ਜੋੜੋ_link (get_subtables<Sub_ਸਾਰਣੀ> ()[0], c->ਕੱਢੋ_pack ());
      return_trace (ਸੱਚ);
    }
    c->ਕੱਢੋ_discard ();
    return_trace (ਝੂਠ);
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ GlyphID>
  ਤਾਰਕਿਕ serialize_alternate (hb_serialize_context_t *c,
                            uint32_t lookup_props,
                            hb_sorted_array_t<GlyphID> glyphs,
                            hb_array_t<ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ> alternate_len_list,
                            hb_array_t<GlyphID> alternate_glyphs_list)
  {
    TRACE_SERIALIZE (ਇਹ);
    ਜੇ (unlikely (!Lookup::serialize (c, Sub_ਸਾਰਣੀ::Alternate, lookup_props, 1))) return_trace (ਝੂਠ);

    ਜੇ (c->push<Sub_ਸਾਰਣੀ> ()->u.alternate.
        serialize (c,
                   glyphs,
                   alternate_len_list,
                   alternate_glyphs_list))
    {
      c->ਜੋੜੋ_link (get_subtables<Sub_ਸਾਰਣੀ> ()[0], c->ਕੱਢੋ_pack ());
      return_trace (ਸੱਚ);
    }
    c->ਕੱਢੋ_discard ();
    return_trace (ਝੂਠ);
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ GlyphID>
  ਤਾਰਕਿਕ serialize_ligature (hb_serialize_context_t *c,
                           uint32_t lookup_props,
                           hb_sorted_array_t<GlyphID> ਪਹਿਲਾ_glyphs,
                           hb_array_t<ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ> ligature_ਪ੍ਰਤੀ_ਪਹਿਲਾ_glyph_ਗਿਣਤੀ_list,
                           hb_array_t<GlyphID> ligatures_list,
                           hb_array_t<ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ> component_ਗਿਣਤੀ_list,
                           hb_array_t<GlyphID> component_list /* Starting from second for each ligature */)
  {
    TRACE_SERIALIZE (ਇਹ);
    ਜੇ (unlikely (!Lookup::serialize (c, Sub_ਸਾਰਣੀ::Ligature, lookup_props, 1))) return_trace (ਝੂਠ);
    ਜੇ (c->push<Sub_ਸਾਰਣੀ> ()->u.ligature.
        serialize (c,
                   ਪਹਿਲਾ_glyphs,
                   ligature_ਪ੍ਰਤੀ_ਪਹਿਲਾ_glyph_ਗਿਣਤੀ_list,
                   ligatures_list,
                   component_ਗਿਣਤੀ_list,
                   component_list))
    {
      c->ਜੋੜੋ_link (get_subtables<Sub_ਸਾਰਣੀ> ()[0], c->ਕੱਢੋ_pack ());
      return_trace (ਸੱਚ);
    }
    c->ਕੱਢੋ_discard ();
    return_trace (ਝੂਠ);
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ context_t>
  ਸਥਿਰ ਸਤਰਅੰਦਰ ਕਿਸਮ_ਨਾਂ context_t::return_t dispatch_recurse_func (context_t *c, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ lookup_ਸੂਚਕਾਂਕ);

  ਸਥਿਰ ਸਤਰਅੰਦਰ ਕਿਸਮ_ਨਾਂ hb_ਘੇਰਾ_context_t::return_t ਘੇਰਾ_glyphs_recurse_func (hb_ਘੇਰਾ_context_t *c, ਬਿਨਾਨਿਸ਼ਾਨੀ lookup_ਸੂਚਕਾਂਕ, hb_ਸੈੱਟ_t *covered_seq_indices, ਬਿਨਾਨਿਸ਼ਾਨੀ seq_ਸੂਚਕਾਂਕ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਿਰਾ_ਸੂਚਕਾਂਕ);


  ਸਥਿਰ ਸਤਰਅੰਦਰ hb_ਘੇਰਾ_context_t::return_t dispatch_ਘੇਰਾ_recurse_func (hb_ਘੇਰਾ_context_t *c, ਬਿਨਾਨਿਸ਼ਾਨੀ lookup_ਸੂਚਕਾਂਕ, hb_ਸੈੱਟ_t *covered_seq_indices, ਬਿਨਾਨਿਸ਼ਾਨੀ seq_ਸੂਚਕਾਂਕ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਿਰਾ_ਸੂਚਕਾਂਕ)
  {
    ਜੇ (!c->should_ਦੇਖੋ_lookup (lookup_ਸੂਚਕਾਂਕ))
      ਵਾਪਸ hb_ਖਾਲੀ_ਥਾਵਾਂ_t ();

    hb_ਘੇਰਾ_context_t::return_t ret = ਘੇਰਾ_glyphs_recurse_func (c, lookup_ਸੂਚਕਾਂਕ, covered_seq_indices, seq_ਸੂਚਕਾਂਕ, ਸਿਰਾ_ਸੂਚਕਾਂਕ);

    /* While in theory we should flush here, it will cause timeouts because a recursive
     * lookup can keep growing the glyph set.  Skip, and outer loop will retry up to
     * HB_CLOSURE_MAX_STAGES time, which should be enough for every realistic font. */
    //c->flush ();

    ਵਾਪਸ ret;
  }

  ਸਥਿਰ ਸਤਰਅੰਦਰ hb_depend_context_t::return_t depend_glyphs_recurse_func (hb_depend_context_t *c, ਬਿਨਾਨਿਸ਼ਾਨੀ lookup_ਸੂਚਕਾਂਕ, hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਪੰਨਾ_t *covered_seq_indices, ਬਿਨਾਨਿਸ਼ਾਨੀ seq_ਸੂਚਕਾਂਕ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਿਰਾ_ਸੂਚਕਾਂਕ);

  ਸਥਿਰ ਸਤਰਅੰਦਰ hb_depend_context_t::return_t dispatch_depend_recurse_func (hb_depend_context_t *c, ਬਿਨਾਨਿਸ਼ਾਨੀ lookup_ਸੂਚਕਾਂਕ, hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਪੰਨਾ_t *covered_seq_indices, ਬਿਨਾਨਿਸ਼ਾਨੀ seq_ਸੂਚਕਾਂਕ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਿਰਾ_ਸੂਚਕਾਂਕ)
  {
    ਵਾਪਸ depend_glyphs_recurse_func (c, lookup_ਸੂਚਕਾਂਕ, covered_seq_indices, seq_ਸੂਚਕਾਂਕ, ਸਿਰਾ_ਸੂਚਕਾਂਕ);
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ context_t, ਕਿਸਮ_ਨਾਂ ...Ts>
  ਕਿਸਮ_ਨਾਂ context_t::return_t dispatch (context_t *c, Ts&&... ds) ਅਚਲ
  { ਵਾਪਸ Lookup::dispatch<Sub_ਸਾਰਣੀ> (c, std::forward<Ts> (ds)...); }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  { ਵਾਪਸ Lookup::subset<Sub_ਸਾਰਣੀ> (c); }
};


}
}
}

#ਜੇ_ਅੰਤ  /* OT_LAYOUT_GSUB_SUBSTLOOKUP_HH */
