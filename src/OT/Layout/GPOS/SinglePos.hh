#ifndef OT_LAYOUT_GPOS_SINGLEPOS_HH
#define OT_LAYOUT_GPOS_SINGLEPOS_HH

#അന്തര്ഭാവ "SinglePosFormat1.hh"
#അന്തര്ഭാവ "SinglePosFormat2.hh"

നാമാകാശ OT {
നാമാകാശ Layout {
നാമാകാശ GPOS_impl {

ഘടന SinglePos
{
  സംരക്ഷിത:
  സംഘം {
  ഘടന { HBUINT16 v; } format;        /* Format identifier */
  SinglePosFormat1_3<SmallTypes> format1;
  SinglePosFormat2_4<SmallTypes> format2;
#ifndef HB_NO_BEYOND_64K
  SinglePosFormat1_3<MediumTypes> format3;
  SinglePosFormat2_4<MediumTypes> format4;
#endif
  } u;

  പൊതു:
  template<typename Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  unsigned get_format (Iterator glyph_val_iter_pairs)
  {
    hb_array_t<സ്ഥിരം മൂല്യം> first_val_iter = hb_second (*glyph_val_iter_pairs);

    വേണ്ടി (സ്ഥിരം auto iter : glyph_val_iter_pairs)
      വേണ്ടി (സ്ഥിരം auto _ : hb_zip (iter.second, first_val_iter))
        എങ്കിൽ (_.first != _.second)
          തിരികെ 2;

    തിരികെ 1;
  }

  template<typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  ശൂന്യം serialize (hb_serialize_context_t *c,
                  സ്ഥിരം മൂല്യം_Base* src,
                  മൂല്യം_Format src_format,
                  Iterator glyph_val_iter_pairs,
                  സ്ഥിരം hb_hashmap_t<unsigned, hb_pair_t<unsigned, പൂർണ്ണസംഖ്യ>> *layout_variation_idx_delta_map,
                  unsigned newFormat)
  {
    എങ്കിൽ (unlikely (!c->extend_min (u.format.v))) തിരികെ;
    unsigned format = 2;
    മൂല്യം_Format new_format;
    new_format = newFormat;

    എങ്കിൽ (glyph_val_iter_pairs)
      format = get_format (glyph_val_iter_pairs);

#ifndef HB_NO_BEYOND_64K
    എങ്കിൽ (+ glyph_val_iter_pairs
	| hb_map_retains_sorting (hb_first)
	| hb_filter ([] (hb_codepoint_t gid) { തിരികെ gid > 0xFFFFu; }))
      format += 2;
#endif

    u.format.v = format;
    തിരഞ്ഞെടുപ്പ് (u.format.v) {
    അവസ്ഥ 1: hb_barrier (); u.format1.serialize (c,
                                 src, src_format,
                                 glyph_val_iter_pairs,
                                 new_format,
                                 layout_variation_idx_delta_map);
      തിരികെ;
    അവസ്ഥ 2: hb_barrier (); u.format2.serialize (c,
                                 src, src_format,
                                 glyph_val_iter_pairs,
                                 new_format,
                                 layout_variation_idx_delta_map);
      തിരികെ;
#ifndef HB_NO_BEYOND_64K
    അവസ്ഥ 3: u.format3.serialize (c,
                                 src, src_format,
                                 glyph_val_iter_pairs,
                                 new_format,
                                 layout_variation_idx_delta_map);
      തിരികെ;
    അവസ്ഥ 4: u.format4.serialize (c,
                                 src, src_format,
                                 glyph_val_iter_pairs,
                                 new_format,
                                 layout_variation_idx_delta_map);
      തിരികെ;
#endif
    സ്ഥിരസ്ഥിതി:തിരികെ;
    }
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) സ്ഥിരം
  {
    എങ്കിൽ (unlikely (!c->may_dispatch (ഇത്, &u.format.v))) തിരികെ c->no_dispatch_return_മൂല്യം ();
    TRACE_DISPATCH (ഇത്, u.format.v);
    തിരഞ്ഞെടുപ്പ് (u.format.v) {
    അവസ്ഥ 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    അവസ്ഥ 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
#ifndef HB_NO_BEYOND_64K
    അവസ്ഥ 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    അവസ്ഥ 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
#endif
    സ്ഥിരസ്ഥിതി:return_trace (c->default_return_മൂല്യം ());
    }
  }
};


template<typename Iterator>
സ്ഥിതം ശൂന്യം
SinglePos_serialize (hb_serialize_context_t *c,
                     സ്ഥിരം മൂല്യം_Base *src,
                     മൂല്യം_Format src_format,
                     Iterator it,
                     സ്ഥിരം hb_hashmap_t<unsigned, hb_pair_t<unsigned, പൂർണ്ണസംഖ്യ>> *layout_variation_idx_delta_map,
                     unsigned new_format)
{ c->start_embed<SinglePos> ()->serialize (c, src, src_format, it, layout_variation_idx_delta_map, new_format); }


}
}
}

#endif /* OT_LAYOUT_GPOS_SINGLEPOS_HH */
