#جيڪڏهن_وصف_نه_ٿيل OT_LAYOUT_GSUB_LIGATURESUBST_HH
#وصف_ڪريو OT_LAYOUT_GSUB_LIGATURESUBST_HH

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
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
  LigatureSubstFormat1_2<MediumTypes>	format2;
#جيڪڏهن_پورو
  } u;

  عام:
  سانچو <قسم_نالو context_t, قسم_نالو ...Ts>
  قسم_نالو context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    إذا (unlikely (!c->may_dispatch (هذا, &u.format.v))) أرجع c->no_dispatch_return_قدر ();
    TRACE_DISPATCH (هذا, u.format.v);
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    حالة 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
#جيڪڏهن_پورو
    افتراضي:return_trace (c->default_return_قدر ());
    }
  }

  /* TODO This function should be done using iterators. While at it perhaps
   * using iterator of arrays of hb_codepoint_t instead. */
  سانچو <قسم_نالو GlyphID>
  منطقي serialize (hb_serialize_context_t *c,
                  hb_sorted_array_t<GlyphID> پهريون_glyphs,
                  hb_array_t<ثابت بنانشان صحيح> ligature_في_پهريون_glyph_ڳڻپ_list,
                  hb_array_t<GlyphID> ligatures_list,
                  hb_array_t<ثابت بنانشان صحيح> component_ڳڻپ_list,
                  hb_array_t<GlyphID> component_list /* Starting from second for each ligature */)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!c->extend_min (u.format.v))) return_trace (خطأ);
    بنانشان صحيح format = 1;
    u.format.v = format;
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (u.format1.serialize (c,
                                               پهريون_glyphs,
                                               ligature_في_پهريون_glyph_ڳڻپ_list,
                                               ligatures_list,
                                               component_ڳڻپ_list,
                                               component_list));
    افتراضي:return_trace (خطأ);
    }
  }

  /* TODO subset() should choose format. */

};


}
}
}

#جيڪڏهن_پورو  /* OT_LAYOUT_GSUB_LIGATURESUBST_HH */
