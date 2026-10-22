#جيڪڏهن_وصف_نه_ٿيل OT_LAYOUT_GSUB_SUBSTLOOKUP_HH
#وصف_ڪريو OT_LAYOUT_GSUB_SUBSTLOOKUP_HH

#ضمن "Common.hh"
#ضمن "SubstLookupSubTable.hh"

نطاق OT {
نطاق Layout {
نطاق GSUB_impl {

بنية SubstLookup : Lookup
{
  استعمال Sub_جدول = Subst_Lookup_Sub_جدول;

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  { أرجع Lookup::sanitize<Sub_جدول> (c); }

  ثابت Sub_جدول& get_subtable (بنانشان صحيح i) ثابت
  { أرجع Lookup::get_subtable<Sub_جدول> (i); }

  ساكن مضمن منطقي lookup_type_is_ابتو (بنانشان صحيح lookup_type)
  { أرجع lookup_type == Sub_جدول::ابتو_Chain_Single; }

  منطقي is_ابتو () ثابت
  {
    بنانشان صحيح type = get_type ();
    إذا (unlikely (type == Sub_جدول::Extension) && hb_barrier ())
      أرجع get_subtable (0).u.extension.is_ابتو ();
    أرجع lookup_type_is_ابتو (type);
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

  منطقي intersects (ثابت hb_سيٽ_t *glyphs) ثابت
  {
    hb_intersects_context_t c (glyphs);
    أرجع dispatch (&c);
  }

  hb_گهيرو_context_t::return_t گهيرو (hb_گهيرو_context_t *c, بنانشان صحيح this_اشاريو) ثابت
  {
    إذا (!c->should_گهمو_lookup (this_اشاريو))
      أرجع hb_گهيرو_context_t::default_return_قدر ();

    c->سيٽ_recurse_func (dispatch_گهيرو_recurse_func);

    hb_گهيرو_context_t::return_t ret = dispatch (c);

    c->flush ();

    أرجع ret;
  }

  فراغ depend (hb_depend_context_t *c) ثابت
  {
    c->سيٽ_recurse_func (dispatch_depend_recurse_func);
    dispatch (c);
  }

  hb_گهيرو_lookups_context_t::return_t گهيرو_lookups (hb_گهيرو_lookups_context_t *c, بنانشان this_اشاريو) ثابت
  {
    إذا (c->is_lookup_گهميل (this_اشاريو))
      أرجع hb_گهيرو_lookups_context_t::default_return_قدر ();

    c->سيٽ_lookup_گهميل (this_اشاريو);
    إذا (!intersects (c->glyphs))
    {
      c->سيٽ_lookup_inactive (this_اشاريو);
      أرجع hb_گهيرو_lookups_context_t::default_return_قدر ();
    }

    hb_گهيرو_lookups_context_t::return_t ret = dispatch (c);
    أرجع ret;
  }

  hb_collect_glyphs_context_t::return_t collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    c->سيٽ_recurse_func (dispatch_recurse_func<hb_collect_glyphs_context_t>);
    أرجع dispatch (c);
  }

  سانچو <قسم_نالو سيٽ_t>
  فراغ collect_coverage (سيٽ_t *glyphs) ثابت
  {
    hb_collect_coverage_context_t<سيٽ_t> c (glyphs);
    dispatch (&c);
  }

  منطقي would_apply (hb_would_apply_context_t *c,
                    ثابت hb_ot_layout_lookup_accelerator_t *accel) ثابت
  {
    إذا (unlikely (!c->len)) أرجع خطأ;
    إذا (!accel->may_have (c->glyphs[0])) أرجع خطأ;
      أرجع dispatch (c);
  }

  سانچو<قسم_نالو Glyphs, قسم_نالو Substitutes,
	   hb_requires (hb_is_sorted_ذريعو_جو (Glyphs,
						ثابت hb_codepoint_t) &&
			hb_is_ذريعو_جو (Substitutes,
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
      c->جوڙيو_link (get_subtables<Sub_جدول> ()[0], c->ڪڍو_pack ());
      return_trace (سچ);
    }
    c->ڪڍو_discard ();
    return_trace (خطأ);
  }

  سانچو<قسم_نالو Iterator,
           hb_requires (hb_is_sorted_iterator (Iterator))>
  منطقي serialize (hb_serialize_context_t *c,
		  uint32_t lookup_props,
		  Iterator it)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!Lookup::serialize (c, Sub_جدول::گڻو, lookup_props, 1))) return_trace (خطأ);
    إذا (c->push<Sub_جدول> ()->u.گڻو_2.
        serialize (c, it))
    {
      c->جوڙيو_link (get_subtables<Sub_جدول> ()[0], c->ڪڍو_pack ());
      return_trace (سچ);
    }
    c->ڪڍو_discard ();
    return_trace (خطأ);
  }

  سانچو <قسم_نالو GlyphID>
  منطقي serialize_alternate (hb_serialize_context_t *c,
                            uint32_t lookup_props,
                            hb_sorted_array_t<GlyphID> glyphs,
                            hb_array_t<ثابت بنانشان صحيح> alternate_len_list,
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
      c->جوڙيو_link (get_subtables<Sub_جدول> ()[0], c->ڪڍو_pack ());
      return_trace (سچ);
    }
    c->ڪڍو_discard ();
    return_trace (خطأ);
  }

  سانچو <قسم_نالو GlyphID>
  منطقي serialize_ligature (hb_serialize_context_t *c,
                           uint32_t lookup_props,
                           hb_sorted_array_t<GlyphID> پهريون_glyphs,
                           hb_array_t<ثابت بنانشان صحيح> ligature_في_پهريون_glyph_ڳڻپ_list,
                           hb_array_t<GlyphID> ligatures_list,
                           hb_array_t<ثابت بنانشان صحيح> component_ڳڻپ_list,
                           hb_array_t<GlyphID> component_list /* Starting from second for each ligature */)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!Lookup::serialize (c, Sub_جدول::Ligature, lookup_props, 1))) return_trace (خطأ);
    إذا (c->push<Sub_جدول> ()->u.ligature.
        serialize (c,
                   پهريون_glyphs,
                   ligature_في_پهريون_glyph_ڳڻپ_list,
                   ligatures_list,
                   component_ڳڻپ_list,
                   component_list))
    {
      c->جوڙيو_link (get_subtables<Sub_جدول> ()[0], c->ڪڍو_pack ());
      return_trace (سچ);
    }
    c->ڪڍو_discard ();
    return_trace (خطأ);
  }

  سانچو <قسم_نالو context_t>
  ساكن مضمن قسم_نالو context_t::return_t dispatch_recurse_func (context_t *c, بنانشان صحيح lookup_اشاريو);

  ساكن مضمن قسم_نالو hb_گهيرو_context_t::return_t گهيرو_glyphs_recurse_func (hb_گهيرو_context_t *c, بنانشان lookup_اشاريو, hb_سيٽ_t *covered_seq_indices, بنانشان seq_اشاريو, بنانشان پڇاڙي_اشاريو);


  ساكن مضمن hb_گهيرو_context_t::return_t dispatch_گهيرو_recurse_func (hb_گهيرو_context_t *c, بنانشان lookup_اشاريو, hb_سيٽ_t *covered_seq_indices, بنانشان seq_اشاريو, بنانشان پڇاڙي_اشاريو)
  {
    إذا (!c->should_گهمو_lookup (lookup_اشاريو))
      أرجع hb_خالي_جايون_t ();

    hb_گهيرو_context_t::return_t ret = گهيرو_glyphs_recurse_func (c, lookup_اشاريو, covered_seq_indices, seq_اشاريو, پڇاڙي_اشاريو);

    /* While in theory we should flush here, it will cause timeouts because a recursive
     * lookup can keep growing the glyph set.  Skip, and outer loop will retry up to
     * HB_CLOSURE_MAX_STAGES time, which should be enough for every realistic font. */
    //c->flush ();

    أرجع ret;
  }

  ساكن مضمن hb_depend_context_t::return_t depend_glyphs_recurse_func (hb_depend_context_t *c, بنانشان lookup_اشاريو, hb_ٻٽو_انگ_صفحو_t *covered_seq_indices, بنانشان seq_اشاريو, بنانشان پڇاڙي_اشاريو);

  ساكن مضمن hb_depend_context_t::return_t dispatch_depend_recurse_func (hb_depend_context_t *c, بنانشان lookup_اشاريو, hb_ٻٽو_انگ_صفحو_t *covered_seq_indices, بنانشان seq_اشاريو, بنانشان پڇاڙي_اشاريو)
  {
    أرجع depend_glyphs_recurse_func (c, lookup_اشاريو, covered_seq_indices, seq_اشاريو, پڇاڙي_اشاريو);
  }

  سانچو <قسم_نالو context_t, قسم_نالو ...Ts>
  قسم_نالو context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  { أرجع Lookup::dispatch<Sub_جدول> (c, std::forward<Ts> (ds)...); }

  منطقي subset (hb_subset_context_t *c) ثابت
  { أرجع Lookup::subset<Sub_جدول> (c); }
};


}
}
}

#جيڪڏهن_پورو  /* OT_LAYOUT_GSUB_SUBSTLOOKUP_HH */
