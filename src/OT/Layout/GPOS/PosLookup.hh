#جے_بنا_تعریف OT_LAYOUT_GPOS_POSLOOKUP_HH
#تعریف_کرو OT_LAYOUT_GPOS_POSLOOKUP_HH

#ضمن "PosLookupSubTable.hh"
#ضمن "../../../hb-ot-layout-common.hh"

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {

بنية PosLookup : Lookup
{
  ورتوں Sub_جدول = Pos_Lookup_Sub_جدول;

  ثابت Sub_جدول& get_subtable (بنانشانی صحيح i) ثابت
  { أرجع Lookup::get_subtable<Sub_جدول> (i); }

  منطقي is_الٹ () ثابت
  {
    أرجع خطأ;
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

  hb_collect_glyphs_context_t::return_t collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  { أرجع dispatch (c); }

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

  سانچا <قسم_ناں سیٹ_t>
  فراغ collect_coverage (سیٹ_t *glyphs) ثابت
  {
    hb_collect_coverage_context_t<سیٹ_t> c (glyphs);
    dispatch (&c);
  }

  سانچا <قسم_ناں context_t>
  ساكن قسم_ناں context_t::return_t dispatch_recurse_func (context_t *c, بنانشانی صحيح lookup_اشاریہ);

  سانچا <قسم_ناں context_t, قسم_ناں ...Ts>
  قسم_ناں context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  { أرجع Lookup::dispatch<Sub_جدول> (c, std::forward<Ts> (ds)...); }

  منطقي subset (hb_subset_context_t *c) ثابت
  { أرجع Lookup::subset<Sub_جدول> (c); }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  { أرجع Lookup::sanitize<Sub_جدول> (c); }
};

}
}
}

#جے_انت  /* OT_LAYOUT_GPOS_POSLOOKUP_HH */
