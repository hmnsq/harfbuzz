#اگر_غیر_تعریف_شدہ OT_LAYOUT_GPOS_POSLOOKUP_HH
#تعریف_کرو OT_LAYOUT_GPOS_POSLOOKUP_HH

#شامل "PosLookupSubTable.hh"
#شامل "../../../hb-ot-layout-common.hh"

نامخلا OT {
نامخلا Layout {
نامخلا GPOS_impl {

ساخت PosLookup : Lookup
{
  استعمال Sub_جدول = Pos_Lookup_Sub_جدول;

  ثابت Sub_جدول& get_subtable (غیرعلامتی صحیح i) ثابت
  { واپس Lookup::get_subtable<Sub_جدول> (i); }

  منطقی is_الٹ () ثابت
  {
    واپس جھوٹ;
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

  hb_collect_glyphs_context_t::return_t collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  { واپس dispatch (c); }

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

  سانچہ <قسم_نام سیٹ_t>
  خالی collect_coverage (سیٹ_t *glyphs) ثابت
  {
    hb_collect_coverage_context_t<سیٹ_t> c (glyphs);
    dispatch (&c);
  }

  سانچہ <قسم_نام context_t>
  ساکن قسم_نام context_t::return_t dispatch_recurse_func (context_t *c, غیرعلامتی صحیح lookup_اشاریہ);

  سانچہ <قسم_نام context_t, قسم_نام ...Ts>
  قسم_نام context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  { واپس Lookup::dispatch<Sub_جدول> (c, std::forward<Ts> (ds)...); }

  منطقی subset (hb_subset_context_t *c) ثابت
  { واپس Lookup::subset<Sub_جدول> (c); }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  { واپس Lookup::sanitize<Sub_جدول> (c); }
};

}
}
}

#اگر_ختم  /* OT_LAYOUT_GPOS_POSLOOKUP_HH */
