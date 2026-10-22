#ifndef OT_LAYOUT_GSUB_ALTERNATESUBST_HH
#define OT_LAYOUT_GSUB_ALTERNATESUBST_HH

#അന്തര്ഭാവ "AlternateSubstFormat1.hh"
#അന്തര്ഭാവ "Common.hh"

നാമാകാശ OT {
നാമാകാശ Layout {
നാമാകാശ GSUB_impl {

ഘടന AlternateSubst
{
  സംരക്ഷിത:
  സംഘം {
  ഘടന { HBUINT16 v; }		format;         /* Format identifier */
  AlternateSubstFormat1_2<SmallTypes>	format1;
#ifndef HB_NO_BEYOND_64K
  AlternateSubstFormat1_2<MediumTypes>	format2;
#endif
  } u;
  പൊതു:

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) സ്ഥിരം
  {
    എങ്കിൽ (unlikely (!c->may_dispatch (ഇത്, &u.format.v))) തിരികെ c->no_dispatch_return_മൂല്യം ();
    TRACE_DISPATCH (ഇത്, u.format.v);
    തിരഞ്ഞെടുപ്പ് (u.format.v) {
    അവസ്ഥ 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
#ifndef HB_NO_BEYOND_64K
    അവസ്ഥ 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
#endif
    സ്ഥിരസ്ഥിതി:return_trace (c->default_return_മൂല്യം ());
    }
  }

  /* TODO This function is unused and should be done using iterators. While
   * at it perhaps using iterator of arrays of hb_codepoint_t instead. */
  template <typename GlyphID>
  തർക്കം serialize (hb_serialize_context_t *c,
                  hb_sorted_array_t<GlyphID> glyphs,
                  hb_array_t<സ്ഥിരം unsigned പൂർണ്ണസംഖ്യ> alternate_len_list,
                  hb_array_t<GlyphID> alternate_glyphs_list)
  {
    TRACE_SERIALIZE (ഇത്);
    എങ്കിൽ (unlikely (!c->extend_min (u.format.v))) return_trace (അസത്യം);
    unsigned പൂർണ്ണസംഖ്യ format = 1;
    u.format.v = format;
    തിരഞ്ഞെടുപ്പ് (u.format.v) {
    അവസ്ഥ 1: hb_barrier (); return_trace (u.format1.serialize (c, glyphs, alternate_len_list, alternate_glyphs_list));
    സ്ഥിരസ്ഥിതി:return_trace (അസത്യം);
    }
  }

  /* TODO subset() should choose format. */

};

}
}
}

#endif  /* OT_LAYOUT_GSUB_ALTERNATESUBST_HH */
