#جے_بنا_تعریف OT_LAYOUT_GSUB_SUBSTLOOKUP_HH
#تعریف_کرو OT_LAYOUT_GSUB_SUBSTLOOKUP_HH

#ضمن "Common.hh"
#ضمن "SubstLookupSubTable.hh"

نطاق OT {
نطاق Layout {
نطاق GSUB_impl {

بنية SubstLookup : Lookup
{
  ورتوں Sub_جدول = Subst_Lookup_Sub_جدول;

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  { أرجع Lookup::sanitize<Sub_جدول> (c); }

  ثابت Sub_جدول& get_subtable (بنانشانی صحيح i) ثابت
  { أرجع Lookup::get_subtable<Sub_جدول> (i); }

  ساكن مضمن منطقي lookup_type_is_الٹ (بنانشانی صحيح lookup_type)
  { أرجع lookup_type == Sub_جدول::الٹ_Chain_Single; }

  منطقي is_الٹ () ثابت
  {
    بنانشانی صحيح type = get_type ();
    إذا (unlikely (type == Sub_جدول::Extension) && hb_barrier ())
      أرجع get_subtable (0).u.extension.is_الٹ ();
    أرجع lookup_type_is_الٹ (type);
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

  منطقي intersects (ثابت hb_سیٹ_t *glyphs) ثابت
  {
    hb_intersects_context_t c (glyphs);
    أرجع dispatch (&c);
  }

  hb_گھیرا_context_t::return_t گھیرا (hb_گھیرا_context_t *c, بنانشانی صحيح this_اشاریہ) ثابت
  {
    إذا (!c->should_ویکھو_lookup (this_اشاریہ))
      أرجع hb_گھیرا_context_t::default_return_قدر ();

    c->سیٹ_recurse_func (dispatch_گھیرا_recurse_func);

    hb_گھیرا_context_t::return_t ret = dispatch (c);

    c->flush ();

    أرجع ret;
  }

  فراغ depend (hb_depend_context_t *c) ثابت
  {
    c->سیٹ_recurse_func (dispatch_depend_recurse_func);
    dispatch (c);
  }

  hb_گھیرا_lookups_context_t::return_t گھیرا_lookups (hb_گھیرا_lookups_context_t *c, بنانشانی this_اشاریہ) ثابت
  {
    إذا (c->is_lookup_ویکھیا (this_اشاریہ))
      أرجع hb_گھیرا_lookups_context_t::default_return_قدر ();

    c->سیٹ_lookup_ویکھیا (this_اشاریہ);
    إذا (!intersects (c->glyphs))
    {
      c->سیٹ_lookup_inactive (this_اشاریہ);
      أرجع hb_گھیرا_lookups_context_t::default_return_قدر ();
    }

    hb_گھیرا_lookups_context_t::return_t ret = dispatch (c);
    أرجع ret;
  }

  hb_collect_glyphs_context_t::return_t collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    c->سیٹ_recurse_func (dispatch_recurse_func<hb_collect_glyphs_context_t>);
    أرجع dispatch (c);
  }

  سانچا <قسم_ناں سیٹ_t>
  فراغ collect_coverage (سیٹ_t *glyphs) ثابت
  {
    hb_collect_coverage_context_t<سیٹ_t> c (glyphs);
    dispatch (&c);
  }

  منطقي would_apply (hb_would_apply_context_t *c,
                    ثابت hb_ot_layout_lookup_accelerator_t *accel) ثابت
  {
    إذا (unlikely (!c->len)) أرجع خطأ;
    إذا (!accel->may_have (c->glyphs[0])) أرجع خطأ;
      أرجع dispatch (c);
  }

  سانچا<قسم_ناں Glyphs, قسم_ناں Substitutes,
	   hb_requires (hb_is_sorted_سروت_دا (Glyphs,
						ثابت hb_codepoint_t) &&
			hb_is_سروت_دا (Substitutes,
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
      c->جوڑو_link (get_subtables<Sub_جدول> ()[0], c->نکالو_pack ());
      return_trace (سچ);
    }
    c->نکالو_discard ();
    return_trace (خطأ);
  }

  سانچا<قسم_ناں Iterator,
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
      c->جوڑو_link (get_subtables<Sub_جدول> ()[0], c->نکالو_pack ());
      return_trace (سچ);
    }
    c->نکالو_discard ();
    return_trace (خطأ);
  }

  سانچا <قسم_ناں GlyphID>
  منطقي serialize_alternate (hb_serialize_context_t *c,
                            uint32_t lookup_props,
                            hb_sorted_array_t<GlyphID> glyphs,
                            hb_array_t<ثابت بنانشانی صحيح> alternate_len_list,
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
      c->جوڑو_link (get_subtables<Sub_جدول> ()[0], c->نکالو_pack ());
      return_trace (سچ);
    }
    c->نکالو_discard ();
    return_trace (خطأ);
  }

  سانچا <قسم_ناں GlyphID>
  منطقي serialize_ligature (hb_serialize_context_t *c,
                           uint32_t lookup_props,
                           hb_sorted_array_t<GlyphID> پہلا_glyphs,
                           hb_array_t<ثابت بنانشانی صحيح> ligature_فی_پہلا_glyph_گنتی_list,
                           hb_array_t<GlyphID> ligatures_list,
                           hb_array_t<ثابت بنانشانی صحيح> component_گنتی_list,
                           hb_array_t<GlyphID> component_list /* Starting from second for each ligature */)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!Lookup::serialize (c, Sub_جدول::Ligature, lookup_props, 1))) return_trace (خطأ);
    إذا (c->push<Sub_جدول> ()->u.ligature.
        serialize (c,
                   پہلا_glyphs,
                   ligature_فی_پہلا_glyph_گنتی_list,
                   ligatures_list,
                   component_گنتی_list,
                   component_list))
    {
      c->جوڑو_link (get_subtables<Sub_جدول> ()[0], c->نکالو_pack ());
      return_trace (سچ);
    }
    c->نکالو_discard ();
    return_trace (خطأ);
  }

  سانچا <قسم_ناں context_t>
  ساكن مضمن قسم_ناں context_t::return_t dispatch_recurse_func (context_t *c, بنانشانی صحيح lookup_اشاریہ);

  ساكن مضمن قسم_ناں hb_گھیرا_context_t::return_t گھیرا_glyphs_recurse_func (hb_گھیرا_context_t *c, بنانشانی lookup_اشاریہ, hb_سیٹ_t *covered_seq_indices, بنانشانی seq_اشاریہ, بنانشانی سرا_اشاریہ);


  ساكن مضمن hb_گھیرا_context_t::return_t dispatch_گھیرا_recurse_func (hb_گھیرا_context_t *c, بنانشانی lookup_اشاریہ, hb_سیٹ_t *covered_seq_indices, بنانشانی seq_اشاریہ, بنانشانی سرا_اشاریہ)
  {
    إذا (!c->should_ویکھو_lookup (lookup_اشاریہ))
      أرجع hb_خالی_تھاواں_t ();

    hb_گھیرا_context_t::return_t ret = گھیرا_glyphs_recurse_func (c, lookup_اشاریہ, covered_seq_indices, seq_اشاریہ, سرا_اشاریہ);

    /* While in theory we should flush here, it will cause timeouts because a recursive
     * lookup can keep growing the glyph set.  Skip, and outer loop will retry up to
     * HB_CLOSURE_MAX_STAGES time, which should be enough for every realistic font. */
    //c->flush ();

    أرجع ret;
  }

  ساكن مضمن hb_depend_context_t::return_t depend_glyphs_recurse_func (hb_depend_context_t *c, بنانشانی lookup_اشاریہ, hb_ثنائی_ہندسہ_ورقہ_t *covered_seq_indices, بنانشانی seq_اشاریہ, بنانشانی سرا_اشاریہ);

  ساكن مضمن hb_depend_context_t::return_t dispatch_depend_recurse_func (hb_depend_context_t *c, بنانشانی lookup_اشاریہ, hb_ثنائی_ہندسہ_ورقہ_t *covered_seq_indices, بنانشانی seq_اشاریہ, بنانشانی سرا_اشاریہ)
  {
    أرجع depend_glyphs_recurse_func (c, lookup_اشاریہ, covered_seq_indices, seq_اشاریہ, سرا_اشاریہ);
  }

  سانچا <قسم_ناں context_t, قسم_ناں ...Ts>
  قسم_ناں context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  { أرجع Lookup::dispatch<Sub_جدول> (c, std::forward<Ts> (ds)...); }

  منطقي subset (hb_subset_context_t *c) ثابت
  { أرجع Lookup::subset<Sub_جدول> (c); }
};


}
}
}

#جے_انت  /* OT_LAYOUT_GSUB_SUBSTLOOKUP_HH */
