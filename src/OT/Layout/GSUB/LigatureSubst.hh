#اگر_تعریف_نشده OT_LAYOUT_GSUB_LIGATURESUBST_HH
#تعریف_کن OT_LAYOUT_GSUB_LIGATURESUBST_HH

#ضمن "Common.hh"
#ضمن "LigatureSubstFormat1.hh"

نطاق OT {
نطاق Layout {
نطاق GSUB_impl {

بنية LigatureSubst
{
  محمي:
  اتحاد {
  بنية { HBUINT16 v; }		format;         /* Format identifier */
  LigatureSubstFormat1_2<SmallTypes>	format1;
#اگر_تعریف_نشده HB_NO_BEYOND_64K
  LigatureSubstFormat1_2<MediumTypes>	format2;
#پایان_اگر
  } u;

  عام:
  قالب <نام_نوع context_t, نام_نوع ...Ts>
  نام_نوع context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    إذا (unlikely (!c->may_dispatch (هذا, &u.format.v))) أرجع c->no_dispatch_return_مقدار ();
    TRACE_DISPATCH (هذا, u.format.v);
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    حالة 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
#پایان_اگر
    افتراضي:return_trace (c->default_return_مقدار ());
    }
  }

  /* TODO This function should be done using iterators. While at it perhaps
   * using iterator of arrays of hb_codepoint_t instead. */
  قالب <نام_نوع GlyphID>
  منطقي serialize (hb_serialize_context_t *c,
                  hb_sorted_array_t<GlyphID> اولی_glyphs,
                  hb_array_t<ثابت بیعلامت صحيح> ligature_در_هر_اولی_glyph_شمار_list,
                  hb_array_t<GlyphID> ligatures_list,
                  hb_array_t<ثابت بیعلامت صحيح> component_شمار_list,
                  hb_array_t<GlyphID> component_list /* Starting from second for each ligature */)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!c->extend_min (u.format.v))) return_trace (خطأ);
    بیعلامت صحيح format = 1;
    u.format.v = format;
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (u.format1.serialize (c,
                                               اولی_glyphs,
                                               ligature_در_هر_اولی_glyph_شمار_list,
                                               ligatures_list,
                                               component_شمار_list,
                                               component_list));
    افتراضي:return_trace (خطأ);
    }
  }

  /* TODO subset() should choose format. */

};


}
}
}

#پایان_اگر  /* OT_LAYOUT_GSUB_LIGATURESUBST_HH */
