#که_نه_تعریف_شوی OT_LAYOUT_GPOS_POSLOOKUP_HH
#تعریف_کړه OT_LAYOUT_GPOS_POSLOOKUP_HH

#ضمن "PosLookupSubTable.hh"
#ضمن "../../../hb-ot-layout-common.hh"

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {

بنية PosLookup : Lookup
{
  کارول Sub_جدول = Pos_Lookup_Sub_جدول;

  ثابت Sub_جدول& get_subtable (بینښه صحيح i) ثابت
  { أرجع Lookup::get_subtable<Sub_جدول> (i); }

  منطقي is_سرچپه () ثابت
  {
    أرجع خطأ;
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

  hb_collect_glyphs_context_t::return_t collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  { أرجع dispatch (c); }

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

  کينډۍ <ډول_نوم سېټ_t>
  فراغ collect_coverage (سېټ_t *glyphs) ثابت
  {
    hb_collect_coverage_context_t<سېټ_t> c (glyphs);
    dispatch (&c);
  }

  کينډۍ <ډول_نوم context_t>
  ساكن ډول_نوم context_t::return_t dispatch_recurse_func (context_t *c, بینښه صحيح lookup_شاخص);

  کينډۍ <ډول_نوم context_t, ډول_نوم ...Ts>
  ډول_نوم context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  { أرجع Lookup::dispatch<Sub_جدول> (c, std::forward<Ts> (ds)...); }

  منطقي subset (hb_subset_context_t *c) ثابت
  { أرجع Lookup::subset<Sub_جدول> (c); }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  { أرجع Lookup::sanitize<Sub_جدول> (c); }
};

}
}
}

#د_که_پای  /* OT_LAYOUT_GPOS_POSLOOKUP_HH */
