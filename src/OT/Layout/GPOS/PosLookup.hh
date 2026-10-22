#جيڪڏهن_وصف_نه_ٿيل OT_LAYOUT_GPOS_POSLOOKUP_HH
#وصف_ڪريو OT_LAYOUT_GPOS_POSLOOKUP_HH

#ضمن "PosLookupSubTable.hh"
#ضمن "../../../hb-ot-layout-common.hh"

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {

بنية PosLookup : Lookup
{
  استعمال Sub_جدول = Pos_Lookup_Sub_جدول;

  ثابت Sub_جدول& get_subtable (بنانشان صحيح i) ثابت
  { أرجع Lookup::get_subtable<Sub_جدول> (i); }

  منطقي is_ابتو () ثابت
  {
    أرجع خطأ;
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

  hb_collect_glyphs_context_t::return_t collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  { أرجع dispatch (c); }

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

  سانچو <قسم_نالو سيٽ_t>
  فراغ collect_coverage (سيٽ_t *glyphs) ثابت
  {
    hb_collect_coverage_context_t<سيٽ_t> c (glyphs);
    dispatch (&c);
  }

  سانچو <قسم_نالو context_t>
  ساكن قسم_نالو context_t::return_t dispatch_recurse_func (context_t *c, بنانشان صحيح lookup_اشاريو);

  سانچو <قسم_نالو context_t, قسم_نالو ...Ts>
  قسم_نالو context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  { أرجع Lookup::dispatch<Sub_جدول> (c, std::forward<Ts> (ds)...); }

  منطقي subset (hb_subset_context_t *c) ثابت
  { أرجع Lookup::subset<Sub_جدول> (c); }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  { أرجع Lookup::sanitize<Sub_جدول> (c); }
};

}
}
}

#جيڪڏهن_پورو  /* OT_LAYOUT_GPOS_POSLOOKUP_HH */
