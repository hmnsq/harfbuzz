#اگر_تعریف_نشده OT_LAYOUT_GPOS_POSLOOKUP_HH
#تعریف_کن OT_LAYOUT_GPOS_POSLOOKUP_HH

#ضمن "PosLookupSubTable.hh"
#ضمن "../../../hb-ot-layout-common.hh"

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {

بنية PosLookup : Lookup
{
  استفاده Sub_جدول = Pos_Lookup_Sub_جدول;

  ثابت Sub_جدول& get_subtable (بیعلامت صحيح i) ثابت
  { أرجع Lookup::get_subtable<Sub_جدول> (i); }

  منطقي is_معکوس () ثابت
  {
    أرجع خطأ;
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

  hb_collect_glyphs_context_t::return_t collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  { أرجع dispatch (c); }

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

  قالب <نام_نوع مجموعه_t>
  فراغ collect_coverage (مجموعه_t *glyphs) ثابت
  {
    hb_collect_coverage_context_t<مجموعه_t> c (glyphs);
    dispatch (&c);
  }

  قالب <نام_نوع context_t>
  ساكن نام_نوع context_t::return_t dispatch_recurse_func (context_t *c, بیعلامت صحيح lookup_شاخص);

  قالب <نام_نوع context_t, نام_نوع ...Ts>
  نام_نوع context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  { أرجع Lookup::dispatch<Sub_جدول> (c, std::forward<Ts> (ds)...); }

  منطقي subset (hb_subset_context_t *c) ثابت
  { أرجع Lookup::subset<Sub_جدول> (c); }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  { أرجع Lookup::sanitize<Sub_جدول> (c); }
};

}
}
}

#پایان_اگر  /* OT_LAYOUT_GPOS_POSLOOKUP_HH */
