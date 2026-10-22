#اگر_غیر_تعریف_شدہ OT_LAYOUT_GSUB_ALTERNATESUBST_HH
#تعریف_کرو OT_LAYOUT_GSUB_ALTERNATESUBST_HH

#شامل "AlternateSubstFormat1.hh"
#شامل "Common.hh"

نامخلا OT {
نامخلا Layout {
نامخلا GSUB_impl {

ساخت AlternateSubst
{
  محفوظ:
  اتحاد {
  ساخت { HBUINT16 v; }		format;         /* Format identifier */
  AlternateSubstFormat1_2<SmallTypes>	format1;
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
  AlternateSubstFormat1_2<MediumTypes>	format2;
#اگر_ختم
  } u;
  عوامی:

  سانچہ <قسم_نام context_t, قسم_نام ...Ts>
  قسم_نام context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    اگر (unlikely (!c->may_dispatch (یہ, &u.format.v))) واپس c->no_dispatch_return_قدر ();
    TRACE_DISPATCH (یہ, u.format.v);
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    حالت 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
#اگر_ختم
    طےشدہ:return_trace (c->default_return_قدر ());
    }
  }

  /* TODO This function is unused and should be done using iterators. While
   * at it perhaps using iterator of arrays of hb_codepoint_t instead. */
  سانچہ <قسم_نام GlyphID>
  منطقی serialize (hb_serialize_context_t *c,
                  hb_sorted_array_t<GlyphID> glyphs,
                  hb_array_t<ثابت غیرعلامتی صحیح> alternate_len_list,
                  hb_array_t<GlyphID> alternate_glyphs_list)
  {
    TRACE_SERIALIZE (یہ);
    اگر (unlikely (!c->extend_min (u.format.v))) return_trace (جھوٹ);
    غیرعلامتی صحیح format = 1;
    u.format.v = format;
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); return_trace (u.format1.serialize (c, glyphs, alternate_len_list, alternate_glyphs_list));
    طےشدہ:return_trace (جھوٹ);
    }
  }

  /* TODO subset() should choose format. */

};

}
}
}

#اگر_ختم  /* OT_LAYOUT_GSUB_ALTERNATESUBST_HH */
