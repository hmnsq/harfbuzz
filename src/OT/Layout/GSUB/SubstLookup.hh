#اگر_غیر_تعریف_شدہ OT_LAYOUT_GSUB_SUBSTLOOKUP_HH
#تعریف_کرو OT_LAYOUT_GSUB_SUBSTLOOKUP_HH

#شامل "Common.hh"
#شامل "SubstLookupSubTable.hh"

نامخلا OT {
نامخلا Layout {
نامخلا GSUB_impl {

ساخت SubstLookup : Lookup
{
  استعمال Sub_جدول = Subst_Lookup_Sub_جدول;

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  { واپس Lookup::sanitize<Sub_جدول> (c); }

  ثابت Sub_جدول& get_subtable (غیرعلامتی صحیح i) ثابت
  { واپس Lookup::get_subtable<Sub_جدول> (i); }

  ساکن اندرونی منطقی lookup_type_is_الٹ (غیرعلامتی صحیح lookup_type)
  { واپس lookup_type == Sub_جدول::الٹ_Chain_Single; }

  منطقی is_الٹ () ثابت
  {
    غیرعلامتی صحیح type = get_type ();
    اگر (unlikely (type == Sub_جدول::Extension) && hb_barrier ())
      واپس get_subtable (0).u.extension.is_الٹ ();
    واپس lookup_type_is_الٹ (type);
  }

  منطقی may_have_non_1to1 () ثابت
  {
    hb_have_non_1to1_context_t c;
    واپس dispatch (&c);
  }

  منطقی apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (یہ);
    return_trace (dispatch (c));
  }

  منطقی intersects (ثابت hb_سیٹ_t *glyphs) ثابت
  {
    hb_intersects_context_t c (glyphs);
    واپس dispatch (&c);
  }

  hb_احاطہ_context_t::return_t احاطہ (hb_احاطہ_context_t *c, غیرعلامتی صحیح this_اشاریہ) ثابت
  {
    اگر (!c->should_دورہ_lookup (this_اشاریہ))
      واپس hb_احاطہ_context_t::default_return_قدر ();

    c->سیٹ_recurse_func (dispatch_احاطہ_recurse_func);

    hb_احاطہ_context_t::return_t ret = dispatch (c);

    c->flush ();

    واپس ret;
  }

  خالی depend (hb_depend_context_t *c) ثابت
  {
    c->سیٹ_recurse_func (dispatch_depend_recurse_func);
    dispatch (c);
  }

  hb_احاطہ_lookups_context_t::return_t احاطہ_lookups (hb_احاطہ_lookups_context_t *c, غیرعلامتی this_اشاریہ) ثابت
  {
    اگر (c->is_lookup_دیکھا (this_اشاریہ))
      واپس hb_احاطہ_lookups_context_t::default_return_قدر ();

    c->سیٹ_lookup_دیکھا (this_اشاریہ);
    اگر (!intersects (c->glyphs))
    {
      c->سیٹ_lookup_inactive (this_اشاریہ);
      واپس hb_احاطہ_lookups_context_t::default_return_قدر ();
    }

    hb_احاطہ_lookups_context_t::return_t ret = dispatch (c);
    واپس ret;
  }

  hb_collect_glyphs_context_t::return_t collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    c->سیٹ_recurse_func (dispatch_recurse_func<hb_collect_glyphs_context_t>);
    واپس dispatch (c);
  }

  سانچہ <قسم_نام سیٹ_t>
  خالی collect_coverage (سیٹ_t *glyphs) ثابت
  {
    hb_collect_coverage_context_t<سیٹ_t> c (glyphs);
    dispatch (&c);
  }

  منطقی would_apply (hb_would_apply_context_t *c,
                    ثابت hb_ot_layout_lookup_accelerator_t *accel) ثابت
  {
    اگر (unlikely (!c->len)) واپس جھوٹ;
    اگر (!accel->may_have (c->glyphs[0])) واپس جھوٹ;
      واپس dispatch (c);
  }

  سانچہ<قسم_نام Glyphs, قسم_نام Substitutes,
	   hb_requires (hb_is_sorted_ماخذ_کا (Glyphs,
						ثابت hb_codepoint_t) &&
			hb_is_ماخذ_کا (Substitutes,
					 ثابت hb_codepoint_t))>
  منطقی serialize_single (hb_serialize_context_t *c,
                         uint32_t lookup_props,
                         Glyphs glyphs,
                         Substitutes substitutes)
  {
    TRACE_SERIALIZE (یہ);
    اگر (unlikely (!Lookup::serialize (c, Sub_جدول::Single, lookup_props, 1))) return_trace (جھوٹ);
    اگر (c->push<Sub_جدول> ()->u.single.serialize (c, hb_zip (glyphs, substitutes)))
    {
      c->جوڑو_link (get_subtables<Sub_جدول> ()[0], c->نکالو_pack ());
      return_trace (سچ);
    }
    c->نکالو_discard ();
    return_trace (جھوٹ);
  }

  سانچہ<قسم_نام Iterator,
           hb_requires (hb_is_sorted_iterator (Iterator))>
  منطقی serialize (hb_serialize_context_t *c,
		  uint32_t lookup_props,
		  Iterator it)
  {
    TRACE_SERIALIZE (یہ);
    اگر (unlikely (!Lookup::serialize (c, Sub_جدول::مضرب, lookup_props, 1))) return_trace (جھوٹ);
    اگر (c->push<Sub_جدول> ()->u.مضرب_2.
        serialize (c, it))
    {
      c->جوڑو_link (get_subtables<Sub_جدول> ()[0], c->نکالو_pack ());
      return_trace (سچ);
    }
    c->نکالو_discard ();
    return_trace (جھوٹ);
  }

  سانچہ <قسم_نام GlyphID>
  منطقی serialize_alternate (hb_serialize_context_t *c,
                            uint32_t lookup_props,
                            hb_sorted_array_t<GlyphID> glyphs,
                            hb_array_t<ثابت غیرعلامتی صحیح> alternate_len_list,
                            hb_array_t<GlyphID> alternate_glyphs_list)
  {
    TRACE_SERIALIZE (یہ);
    اگر (unlikely (!Lookup::serialize (c, Sub_جدول::Alternate, lookup_props, 1))) return_trace (جھوٹ);

    اگر (c->push<Sub_جدول> ()->u.alternate.
        serialize (c,
                   glyphs,
                   alternate_len_list,
                   alternate_glyphs_list))
    {
      c->جوڑو_link (get_subtables<Sub_جدول> ()[0], c->نکالو_pack ());
      return_trace (سچ);
    }
    c->نکالو_discard ();
    return_trace (جھوٹ);
  }

  سانچہ <قسم_نام GlyphID>
  منطقی serialize_ligature (hb_serialize_context_t *c,
                           uint32_t lookup_props,
                           hb_sorted_array_t<GlyphID> پہلا_glyphs,
                           hb_array_t<ثابت غیرعلامتی صحیح> ligature_فی_پہلا_glyph_گنتی_list,
                           hb_array_t<GlyphID> ligatures_list,
                           hb_array_t<ثابت غیرعلامتی صحیح> component_گنتی_list,
                           hb_array_t<GlyphID> component_list /* Starting from second for each ligature */)
  {
    TRACE_SERIALIZE (یہ);
    اگر (unlikely (!Lookup::serialize (c, Sub_جدول::Ligature, lookup_props, 1))) return_trace (جھوٹ);
    اگر (c->push<Sub_جدول> ()->u.ligature.
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
    return_trace (جھوٹ);
  }

  سانچہ <قسم_نام context_t>
  ساکن اندرونی قسم_نام context_t::return_t dispatch_recurse_func (context_t *c, غیرعلامتی صحیح lookup_اشاریہ);

  ساکن اندرونی قسم_نام hb_احاطہ_context_t::return_t احاطہ_glyphs_recurse_func (hb_احاطہ_context_t *c, غیرعلامتی lookup_اشاریہ, hb_سیٹ_t *covered_seq_indices, غیرعلامتی seq_اشاریہ, غیرعلامتی انتہا_اشاریہ);


  ساکن اندرونی hb_احاطہ_context_t::return_t dispatch_احاطہ_recurse_func (hb_احاطہ_context_t *c, غیرعلامتی lookup_اشاریہ, hb_سیٹ_t *covered_seq_indices, غیرعلامتی seq_اشاریہ, غیرعلامتی انتہا_اشاریہ)
  {
    اگر (!c->should_دورہ_lookup (lookup_اشاریہ))
      واپس hb_خالی_خانے_t ();

    hb_احاطہ_context_t::return_t ret = احاطہ_glyphs_recurse_func (c, lookup_اشاریہ, covered_seq_indices, seq_اشاریہ, انتہا_اشاریہ);

    /* While in theory we should flush here, it will cause timeouts because a recursive
     * lookup can keep growing the glyph set.  Skip, and outer loop will retry up to
     * HB_CLOSURE_MAX_STAGES time, which should be enough for every realistic font. */
    //c->flush ();

    واپس ret;
  }

  ساکن اندرونی hb_depend_context_t::return_t depend_glyphs_recurse_func (hb_depend_context_t *c, غیرعلامتی lookup_اشاریہ, hb_ثنائی_ہندسہ_صفحہ_t *covered_seq_indices, غیرعلامتی seq_اشاریہ, غیرعلامتی انتہا_اشاریہ);

  ساکن اندرونی hb_depend_context_t::return_t dispatch_depend_recurse_func (hb_depend_context_t *c, غیرعلامتی lookup_اشاریہ, hb_ثنائی_ہندسہ_صفحہ_t *covered_seq_indices, غیرعلامتی seq_اشاریہ, غیرعلامتی انتہا_اشاریہ)
  {
    واپس depend_glyphs_recurse_func (c, lookup_اشاریہ, covered_seq_indices, seq_اشاریہ, انتہا_اشاریہ);
  }

  سانچہ <قسم_نام context_t, قسم_نام ...Ts>
  قسم_نام context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  { واپس Lookup::dispatch<Sub_جدول> (c, std::forward<Ts> (ds)...); }

  منطقی subset (hb_subset_context_t *c) ثابت
  { واپس Lookup::subset<Sub_جدول> (c); }
};


}
}
}

#اگر_ختم  /* OT_LAYOUT_GSUB_SUBSTLOOKUP_HH */
