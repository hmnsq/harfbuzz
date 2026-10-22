#اگر_تعریف_نشده OT_LAYOUT_GSUB_SUBSTLOOKUP_HH
#تعریف_کن OT_LAYOUT_GSUB_SUBSTLOOKUP_HH

#ضمن "Common.hh"
#ضمن "SubstLookupSubTable.hh"

نطاق OT {
نطاق Layout {
نطاق GSUB_impl {

بنية SubstLookup : Lookup
{
  استفاده Sub_جدول = Subst_Lookup_Sub_جدول;

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  { أرجع Lookup::sanitize<Sub_جدول> (c); }

  ثابت Sub_جدول& get_subtable (بیعلامت صحيح i) ثابت
  { أرجع Lookup::get_subtable<Sub_جدول> (i); }

  ساكن مضمن منطقي lookup_type_is_معکوس (بیعلامت صحيح lookup_type)
  { أرجع lookup_type == Sub_جدول::معکوس_Chain_Single; }

  منطقي is_معکوس () ثابت
  {
    بیعلامت صحيح type = get_type ();
    إذا (unlikely (type == Sub_جدول::Extension) && hb_barrier ())
      أرجع get_subtable (0).u.extension.is_معکوس ();
    أرجع lookup_type_is_معکوس (type);
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

  منطقي intersects (ثابت hb_مجموعه_t *glyphs) ثابت
  {
    hb_intersects_context_t c (glyphs);
    أرجع dispatch (&c);
  }

  hb_بستگی_context_t::return_t بستگی (hb_بستگی_context_t *c, بیعلامت صحيح this_شاخص) ثابت
  {
    إذا (!c->should_بازدید_lookup (this_شاخص))
      أرجع hb_بستگی_context_t::default_return_مقدار ();

    c->مجموعه_recurse_func (dispatch_بستگی_recurse_func);

    hb_بستگی_context_t::return_t ret = dispatch (c);

    c->flush ();

    أرجع ret;
  }

  فراغ depend (hb_depend_context_t *c) ثابت
  {
    c->مجموعه_recurse_func (dispatch_depend_recurse_func);
    dispatch (c);
  }

  hb_بستگی_lookups_context_t::return_t بستگی_lookups (hb_بستگی_lookups_context_t *c, بیعلامت this_شاخص) ثابت
  {
    إذا (c->is_lookup_دیدهشده (this_شاخص))
      أرجع hb_بستگی_lookups_context_t::default_return_مقدار ();

    c->مجموعه_lookup_دیدهشده (this_شاخص);
    إذا (!intersects (c->glyphs))
    {
      c->مجموعه_lookup_inactive (this_شاخص);
      أرجع hb_بستگی_lookups_context_t::default_return_مقدار ();
    }

    hb_بستگی_lookups_context_t::return_t ret = dispatch (c);
    أرجع ret;
  }

  hb_collect_glyphs_context_t::return_t collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    c->مجموعه_recurse_func (dispatch_recurse_func<hb_collect_glyphs_context_t>);
    أرجع dispatch (c);
  }

  قالب <نام_نوع مجموعه_t>
  فراغ collect_coverage (مجموعه_t *glyphs) ثابت
  {
    hb_collect_coverage_context_t<مجموعه_t> c (glyphs);
    dispatch (&c);
  }

  منطقي would_apply (hb_would_apply_context_t *c,
                    ثابت hb_ot_layout_lookup_accelerator_t *accel) ثابت
  {
    إذا (unlikely (!c->len)) أرجع خطأ;
    إذا (!accel->may_have (c->glyphs[0])) أرجع خطأ;
      أرجع dispatch (c);
  }

  قالب<نام_نوع Glyphs, نام_نوع Substitutes,
	   hb_requires (hb_is_sorted_منبع_از (Glyphs,
						ثابت hb_codepoint_t) &&
			hb_is_منبع_از (Substitutes,
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
      c->جمع_کن_link (get_subtables<Sub_جدول> ()[0], c->برداشت_pack ());
      return_trace (درست);
    }
    c->برداشت_discard ();
    return_trace (خطأ);
  }

  قالب<نام_نوع Iterator,
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
      c->جمع_کن_link (get_subtables<Sub_جدول> ()[0], c->برداشت_pack ());
      return_trace (درست);
    }
    c->برداشت_discard ();
    return_trace (خطأ);
  }

  قالب <نام_نوع GlyphID>
  منطقي serialize_alternate (hb_serialize_context_t *c,
                            uint32_t lookup_props,
                            hb_sorted_array_t<GlyphID> glyphs,
                            hb_array_t<ثابت بیعلامت صحيح> alternate_len_list,
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
      c->جمع_کن_link (get_subtables<Sub_جدول> ()[0], c->برداشت_pack ());
      return_trace (درست);
    }
    c->برداشت_discard ();
    return_trace (خطأ);
  }

  قالب <نام_نوع GlyphID>
  منطقي serialize_ligature (hb_serialize_context_t *c,
                           uint32_t lookup_props,
                           hb_sorted_array_t<GlyphID> اولی_glyphs,
                           hb_array_t<ثابت بیعلامت صحيح> ligature_در_هر_اولی_glyph_شمار_list,
                           hb_array_t<GlyphID> ligatures_list,
                           hb_array_t<ثابت بیعلامت صحيح> component_شمار_list,
                           hb_array_t<GlyphID> component_list /* Starting from second for each ligature */)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!Lookup::serialize (c, Sub_جدول::Ligature, lookup_props, 1))) return_trace (خطأ);
    إذا (c->push<Sub_جدول> ()->u.ligature.
        serialize (c,
                   اولی_glyphs,
                   ligature_در_هر_اولی_glyph_شمار_list,
                   ligatures_list,
                   component_شمار_list,
                   component_list))
    {
      c->جمع_کن_link (get_subtables<Sub_جدول> ()[0], c->برداشت_pack ());
      return_trace (درست);
    }
    c->برداشت_discard ();
    return_trace (خطأ);
  }

  قالب <نام_نوع context_t>
  ساكن مضمن نام_نوع context_t::return_t dispatch_recurse_func (context_t *c, بیعلامت صحيح lookup_شاخص);

  ساكن مضمن نام_نوع hb_بستگی_context_t::return_t بستگی_glyphs_recurse_func (hb_بستگی_context_t *c, بیعلامت lookup_شاخص, hb_مجموعه_t *covered_seq_indices, بیعلامت seq_شاخص, بیعلامت انتها_شاخص);


  ساكن مضمن hb_بستگی_context_t::return_t dispatch_بستگی_recurse_func (hb_بستگی_context_t *c, بیعلامت lookup_شاخص, hb_مجموعه_t *covered_seq_indices, بیعلامت seq_شاخص, بیعلامت انتها_شاخص)
  {
    إذا (!c->should_بازدید_lookup (lookup_شاخص))
      أرجع hb_خالی_t ();

    hb_بستگی_context_t::return_t ret = بستگی_glyphs_recurse_func (c, lookup_شاخص, covered_seq_indices, seq_شاخص, انتها_شاخص);

    /* While in theory we should flush here, it will cause timeouts because a recursive
     * lookup can keep growing the glyph set.  Skip, and outer loop will retry up to
     * HB_CLOSURE_MAX_STAGES time, which should be enough for every realistic font. */
    //c->flush ();

    أرجع ret;
  }

  ساكن مضمن hb_depend_context_t::return_t depend_glyphs_recurse_func (hb_depend_context_t *c, بیعلامت lookup_شاخص, hb_بیت_صفحه_t *covered_seq_indices, بیعلامت seq_شاخص, بیعلامت انتها_شاخص);

  ساكن مضمن hb_depend_context_t::return_t dispatch_depend_recurse_func (hb_depend_context_t *c, بیعلامت lookup_شاخص, hb_بیت_صفحه_t *covered_seq_indices, بیعلامت seq_شاخص, بیعلامت انتها_شاخص)
  {
    أرجع depend_glyphs_recurse_func (c, lookup_شاخص, covered_seq_indices, seq_شاخص, انتها_شاخص);
  }

  قالب <نام_نوع context_t, نام_نوع ...Ts>
  نام_نوع context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  { أرجع Lookup::dispatch<Sub_جدول> (c, std::forward<Ts> (ds)...); }

  منطقي subset (hb_subset_context_t *c) ثابت
  { أرجع Lookup::subset<Sub_جدول> (c); }
};


}
}
}

#پایان_اگر  /* OT_LAYOUT_GSUB_SUBSTLOOKUP_HH */
