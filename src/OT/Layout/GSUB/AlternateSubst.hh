#که_نه_تعریف_شوی OT_LAYOUT_GSUB_ALTERNATESUBST_HH
#تعریف_کړه OT_LAYOUT_GSUB_ALTERNATESUBST_HH

#ضمن "AlternateSubstFormat1.hh"
#ضمن "Common.hh"

نطاق OT {
نطاق Layout {
نطاق GSUB_impl {

بنية AlternateSubst
{
  محمي:
  اتحاد {
  بنية { HBUINT16 v; }		format;         /* Format identifier */
  AlternateSubstFormat1_2<SmallTypes>	format1;
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
  AlternateSubstFormat1_2<MediumTypes>	format2;
#د_که_پای
  } u;
  عام:

  کينډۍ <ډول_نوم context_t, ډول_نوم ...Ts>
  ډول_نوم context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    إذا (unlikely (!c->may_dispatch (هذا, &u.format.v))) أرجع c->no_dispatch_return_ارزښت ();
    TRACE_DISPATCH (هذا, u.format.v);
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    حالة 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
#د_که_پای
    افتراضي:return_trace (c->default_return_ارزښت ());
    }
  }

  /* TODO This function is unused and should be done using iterators. While
   * at it perhaps using iterator of arrays of hb_codepoint_t instead. */
  کينډۍ <ډول_نوم GlyphID>
  منطقي serialize (hb_serialize_context_t *c,
                  hb_sorted_array_t<GlyphID> glyphs,
                  hb_array_t<ثابت بینښه صحيح> alternate_len_list,
                  hb_array_t<GlyphID> alternate_glyphs_list)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!c->extend_min (u.format.v))) return_trace (خطأ);
    بینښه صحيح format = 1;
    u.format.v = format;
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (u.format1.serialize (c, glyphs, alternate_len_list, alternate_glyphs_list));
    افتراضي:return_trace (خطأ);
    }
  }

  /* TODO subset() should choose format. */

};

}
}
}

#د_که_پای  /* OT_LAYOUT_GSUB_ALTERNATESUBST_HH */
