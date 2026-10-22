#ifndef OT_LAYOUT_GSUB_LIGATURESUBST_HH
#define OT_LAYOUT_GSUB_LIGATURESUBST_HH

#شامل "Common.hh"
#شامل "LigatureSubstFormat1.hh"

فضاینام OT {
فضاینام Layout {
فضاینام GSUB_impl {

ساختار LigatureSubst
{
  محافظتشده:
  اجتماع {
  ساختار { HBUINT16 v; }		format;         /* Format identifier */
  LigatureSubstFormat1_2<SmallTypes>	format1;
#ifndef HB_NO_BEYOND_64K
  LigatureSubstFormat1_2<MediumTypes>	format2;
#endif
  } u;

  عمومی:
  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    اگر (unlikely (!c->may_dispatch (این, &u.format.v))) بازگشت c->no_dispatch_return_مقدار ();
    TRACE_DISPATCH (این, u.format.v);
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
#ifndef HB_NO_BEYOND_64K
    حالت 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
#endif
    پیشفرض:return_trace (c->default_return_مقدار ());
    }
  }

  /* TODO This function should be done using iterators. While at it perhaps
   * using iterator of arrays of hb_codepoint_t instead. */
  template <typename GlyphID>
  منطقی serialize (hb_serialize_context_t *c,
                  hb_sorted_array_t<GlyphID> first_glyphs,
                  hb_array_t<ثابت unsigned صحیح> ligature_per_first_glyph_شمار_list,
                  hb_array_t<GlyphID> ligatures_list,
                  hb_array_t<ثابت unsigned صحیح> component_شمار_list,
                  hb_array_t<GlyphID> component_list /* Starting from second for each ligature */)
  {
    TRACE_SERIALIZE (این);
    اگر (unlikely (!c->extend_min (u.format.v))) return_trace (نادرست);
    unsigned صحیح format = 1;
    u.format.v = format;
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); return_trace (u.format1.serialize (c,
                                               first_glyphs,
                                               ligature_per_first_glyph_شمار_list,
                                               ligatures_list,
                                               component_شمار_list,
                                               component_list));
    پیشفرض:return_trace (نادرست);
    }
  }

  /* TODO subset() should choose format. */

};


}
}
}

#endif  /* OT_LAYOUT_GSUB_LIGATURESUBST_HH */
