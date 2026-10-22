#ifndef OT_LAYOUT_GSUB_ALTERNATESUBST_HH
#define OT_LAYOUT_GSUB_ALTERNATESUBST_HH

#अन्तर्भाव "AlternateSubstFormat1.hh"
#अन्तर्भाव "Common.hh"

नामाकाश OT {
नामाकाश Layout {
नामाकाश GSUB_impl {

संरचना AlternateSubst
{
  संरक्षित:
  संघ {
  संरचना { HBUINT16 v; }		format;         /* Format identifier */
  AlternateSubstFormat1_2<SmallTypes>	format1;
#ifndef HB_NO_BEYOND_64K
  AlternateSubstFormat1_2<MediumTypes>	format2;
#endif
  } u;
  सार्वजनिक:

  ढाँचा <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) स्थिरांक
  {
    जर (unlikely (!c->may_dispatch (हे, &u.format.v))) परत c->no_dispatch_return_मूल्य ();
    TRACE_DISPATCH (हे, u.format.v);
    निवड (u.format.v) {
    स्थिती 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
#ifndef HB_NO_BEYOND_64K
    स्थिती 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
#endif
    पूर्वनिर्धारित:return_trace (c->default_return_मूल्य ());
    }
  }

  /* TODO This function is unused and should be done using iterators. While
   * at it perhaps using iterator of arrays of hb_codepoint_t instead. */
  ढाँचा <typename GlyphID>
  तार्किक serialize (hb_serialize_context_t *c,
                  hb_sorted_array_t<GlyphID> glyphs,
                  hb_array_t<स्थिरांक अचिन्हित पूर्णांक> alternate_len_list,
                  hb_array_t<GlyphID> alternate_glyphs_list)
  {
    TRACE_SERIALIZE (हे);
    जर (unlikely (!c->extend_min (u.format.v))) return_trace (असत्य);
    अचिन्हित पूर्णांक format = 1;
    u.format.v = format;
    निवड (u.format.v) {
    स्थिती 1: hb_barrier (); return_trace (u.format1.serialize (c, glyphs, alternate_len_list, alternate_glyphs_list));
    पूर्वनिर्धारित:return_trace (असत्य);
    }
  }

  /* TODO subset() should choose format. */

};

}
}
}

#endif  /* OT_LAYOUT_GSUB_ALTERNATESUBST_HH */
