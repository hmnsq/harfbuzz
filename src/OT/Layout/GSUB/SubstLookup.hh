#که_نه_تعریف_شوی OT_LAYOUT_GSUB_SUBSTLOOKUP_HH
#تعریف_کړه OT_LAYOUT_GSUB_SUBSTLOOKUP_HH

#ضمن "Common.hh"
#ضمن "SubstLookupSubTable.hh"

نطاق OT {
نطاق Layout {
نطاق GSUB_impl {

بنية SubstLookup : Lookup
{
  کارول Sub_جدول = Subst_Lookup_Sub_جدول;

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  { أرجع Lookup::sanitize<Sub_جدول> (c); }

  ثابت Sub_جدول& get_subtable (بینښه صحيح i) ثابت
  { أرجع Lookup::get_subtable<Sub_جدول> (i); }

  ساكن مضمن منطقي lookup_type_is_سرچپه (بینښه صحيح lookup_type)
  { أرجع lookup_type == Sub_جدول::سرچپه_Chain_Single; }

  منطقي is_سرچپه () ثابت
  {
    بینښه صحيح type = get_type ();
    إذا (unlikely (type == Sub_جدول::Extension) && hb_barrier ())
      أرجع get_subtable (0).u.extension.is_سرچپه ();
    أرجع lookup_type_is_سرچپه (type);
  }

  منطقي may_have_non_1to1 () ثابت
  {
    hb_have_non_1to1_context_t c;
    أرجع dispatch (&c);
  }

  منطقي apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);
    return_trace (dispatch (c));
  }

  منطقي intersects (ثابت hb_سېټ_t *glyphs) ثابت
  {
    hb_intersects_context_t c (glyphs);
    أرجع dispatch (&c);
  }

  hb_تړون_context_t::return_t تړون (hb_تړون_context_t *c, بینښه صحيح this_شاخص) ثابت
  {
    إذا (!c->should_لیدنه_lookup (this_شاخص))
      أرجع hb_تړون_context_t::default_return_ارزښت ();

    c->سېټ_recurse_func (dispatch_تړون_recurse_func);

    hb_تړون_context_t::return_t ret = dispatch (c);

    c->flush ();

    أرجع ret;
  }

  فراغ depend (hb_depend_context_t *c) ثابت
  {
    c->سېټ_recurse_func (dispatch_depend_recurse_func);
    dispatch (c);
  }

  hb_تړون_lookups_context_t::return_t تړون_lookups (hb_تړون_lookups_context_t *c, بینښه this_شاخص) ثابت
  {
    إذا (c->is_lookup_لیدل_شوی (this_شاخص))
      أرجع hb_تړون_lookups_context_t::default_return_ارزښت ();

    c->سېټ_lookup_لیدل_شوی (this_شاخص);
    إذا (!intersects (c->glyphs))
    {
      c->سېټ_lookup_inactive (this_شاخص);
      أرجع hb_تړون_lookups_context_t::default_return_ارزښت ();
    }

    hb_تړون_lookups_context_t::return_t ret = dispatch (c);
    أرجع ret;
  }

  hb_collect_glyphs_context_t::return_t collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    c->سېټ_recurse_func (dispatch_recurse_func<hb_collect_glyphs_context_t>);
    أرجع dispatch (c);
  }

  کينډۍ <ډول_نوم سېټ_t>
  فراغ collect_coverage (سېټ_t *glyphs) ثابت
  {
    hb_collect_coverage_context_t<سېټ_t> c (glyphs);
    dispatch (&c);
  }

  منطقي would_apply (hb_would_apply_context_t *c,
                    ثابت hb_ot_layout_lookup_accelerator_t *accel) ثابت
  {
    إذا (unlikely (!c->len)) أرجع خطأ;
    إذا (!accel->may_have (c->glyphs[0])) أرجع خطأ;
      أرجع dispatch (c);
  }

  کينډۍ<ډول_نوم Glyphs, ډول_نوم Substitutes,
	   hb_requires (hb_is_sorted_سرچینه_د (Glyphs,
						ثابت hb_codepoint_t) &&
			hb_is_سرچینه_د (Substitutes,
					 ثابت hb_codepoint_t))>
  منطقي serialize_single (hb_serialize_context_t *c,
                         uint32_t lookup_props,
                         Glyphs glyphs,
                         Substitutes substitutes)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!Lookup::serialize (c, Sub_جدول::Single, lookup_props, 1))) return_trace (خطأ);
    إذا (c->push<Sub_جدول> ()->u.single.serialize (c, hb_zip (glyphs, substitutes)))
    {
      c->جمع_کړه_link (get_subtables<Sub_جدول> ()[0], c->راوباسه_pack ());
      return_trace (ریښتیا);
    }
    c->راوباسه_discard ();
    return_trace (خطأ);
  }

  کينډۍ<ډول_نوم Iterator,
           hb_requires (hb_is_sorted_iterator (Iterator))>
  منطقي serialize (hb_serialize_context_t *c,
		  uint32_t lookup_props,
		  Iterator it)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!Lookup::serialize (c, Sub_جدول::مضرب, lookup_props, 1))) return_trace (خطأ);
    إذا (c->push<Sub_جدول> ()->u.مضرب_2.
        serialize (c, it))
    {
      c->جمع_کړه_link (get_subtables<Sub_جدول> ()[0], c->راوباسه_pack ());
      return_trace (ریښتیا);
    }
    c->راوباسه_discard ();
    return_trace (خطأ);
  }

  کينډۍ <ډول_نوم GlyphID>
  منطقي serialize_alternate (hb_serialize_context_t *c,
                            uint32_t lookup_props,
                            hb_sorted_array_t<GlyphID> glyphs,
                            hb_array_t<ثابت بینښه صحيح> alternate_len_list,
                            hb_array_t<GlyphID> alternate_glyphs_list)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!Lookup::serialize (c, Sub_جدول::Alternate, lookup_props, 1))) return_trace (خطأ);

    إذا (c->push<Sub_جدول> ()->u.alternate.
        serialize (c,
                   glyphs,
                   alternate_len_list,
                   alternate_glyphs_list))
    {
      c->جمع_کړه_link (get_subtables<Sub_جدول> ()[0], c->راوباسه_pack ());
      return_trace (ریښتیا);
    }
    c->راوباسه_discard ();
    return_trace (خطأ);
  }

  کينډۍ <ډول_نوم GlyphID>
  منطقي serialize_ligature (hb_serialize_context_t *c,
                           uint32_t lookup_props,
                           hb_sorted_array_t<GlyphID> لومړی_glyphs,
                           hb_array_t<ثابت بینښه صحيح> ligature_فی_لومړی_glyph_شمېر_list,
                           hb_array_t<GlyphID> ligatures_list,
                           hb_array_t<ثابت بینښه صحيح> component_شمېر_list,
                           hb_array_t<GlyphID> component_list /* Starting from second for each ligature */)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!Lookup::serialize (c, Sub_جدول::Ligature, lookup_props, 1))) return_trace (خطأ);
    إذا (c->push<Sub_جدول> ()->u.ligature.
        serialize (c,
                   لومړی_glyphs,
                   ligature_فی_لومړی_glyph_شمېر_list,
                   ligatures_list,
                   component_شمېر_list,
                   component_list))
    {
      c->جمع_کړه_link (get_subtables<Sub_جدول> ()[0], c->راوباسه_pack ());
      return_trace (ریښتیا);
    }
    c->راوباسه_discard ();
    return_trace (خطأ);
  }

  کينډۍ <ډول_نوم context_t>
  ساكن مضمن ډول_نوم context_t::return_t dispatch_recurse_func (context_t *c, بینښه صحيح lookup_شاخص);

  ساكن مضمن ډول_نوم hb_تړون_context_t::return_t تړون_glyphs_recurse_func (hb_تړون_context_t *c, بینښه lookup_شاخص, hb_سېټ_t *covered_seq_indices, بینښه seq_شاخص, بینښه پای_ټکی_شاخص);


  ساكن مضمن hb_تړون_context_t::return_t dispatch_تړون_recurse_func (hb_تړون_context_t *c, بینښه lookup_شاخص, hb_سېټ_t *covered_seq_indices, بینښه seq_شاخص, بینښه پای_ټکی_شاخص)
  {
    إذا (!c->should_لیدنه_lookup (lookup_شاخص))
      أرجع hb_تشه_t ();

    hb_تړون_context_t::return_t ret = تړون_glyphs_recurse_func (c, lookup_شاخص, covered_seq_indices, seq_شاخص, پای_ټکی_شاخص);

    /* While in theory we should flush here, it will cause timeouts because a recursive
     * lookup can keep growing the glyph set.  Skip, and outer loop will retry up to
     * HB_CLOSURE_MAX_STAGES time, which should be enough for every realistic font. */
    //c->flush ();

    أرجع ret;
  }

  ساكن مضمن hb_depend_context_t::return_t depend_glyphs_recurse_func (hb_depend_context_t *c, بینښه lookup_شاخص, hb_بټ_پاڼه_t *covered_seq_indices, بینښه seq_شاخص, بینښه پای_ټکی_شاخص);

  ساكن مضمن hb_depend_context_t::return_t dispatch_depend_recurse_func (hb_depend_context_t *c, بینښه lookup_شاخص, hb_بټ_پاڼه_t *covered_seq_indices, بینښه seq_شاخص, بینښه پای_ټکی_شاخص)
  {
    أرجع depend_glyphs_recurse_func (c, lookup_شاخص, covered_seq_indices, seq_شاخص, پای_ټکی_شاخص);
  }

  کينډۍ <ډول_نوم context_t, ډول_نوم ...Ts>
  ډول_نوم context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  { أرجع Lookup::dispatch<Sub_جدول> (c, std::forward<Ts> (ds)...); }

  منطقي subset (hb_subset_context_t *c) ثابت
  { أرجع Lookup::subset<Sub_جدول> (c); }
};


}
}
}

#د_که_پای  /* OT_LAYOUT_GSUB_SUBSTLOOKUP_HH */
