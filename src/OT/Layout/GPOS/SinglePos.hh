#ifndef OT_LAYOUT_GPOS_SINGLEPOS_HH
#define OT_LAYOUT_GPOS_SINGLEPOS_HH

#અન્તર્ભાવ "SinglePosFormat1.hh"
#અન્તર્ભાવ "SinglePosFormat2.hh"

નામાકાશ OT {
નામાકાશ Layout {
નામાકાશ GPOS_impl {

સંરચના SinglePos
{
  સુરક્ષિત:
  સંઘ {
  સંરચના { HBUINT16 v; } format;        /* Format identifier */
  SinglePosFormat1_3<SmallTypes> format1;
  SinglePosFormat2_4<SmallTypes> format2;
#ifndef HB_NO_BEYOND_64K
  SinglePosFormat1_3<MediumTypes> format3;
  SinglePosFormat2_4<MediumTypes> format4;
#endif
  } u;

  જાહેર:
  template<typename Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  unsigned get_format (Iterator glyph_val_iter_pairs)
  {
    hb_array_t<અચલ કિંમત> first_val_iter = hb_second (*glyph_val_iter_pairs);

    માટે (અચલ auto iter : glyph_val_iter_pairs)
      માટે (અચલ auto _ : hb_zip (iter.second, first_val_iter))
        જો (_.first != _.second)
          પાછા 2;

    પાછા 1;
  }

  template<typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  શૂન્ય serialize (hb_serialize_context_t *c,
                  અચલ કિંમત_Base* src,
                  કિંમત_Format src_format,
                  Iterator glyph_val_iter_pairs,
                  અચલ hb_hashmap_t<unsigned, hb_pair_t<unsigned, પૂર્ણાંક>> *layout_variation_idx_delta_map,
                  unsigned newFormat)
  {
    જો (unlikely (!c->extend_min (u.format.v))) પાછા;
    unsigned format = 2;
    કિંમત_Format new_format;
    new_format = newFormat;

    જો (glyph_val_iter_pairs)
      format = get_format (glyph_val_iter_pairs);

#ifndef HB_NO_BEYOND_64K
    જો (+ glyph_val_iter_pairs
	| hb_map_retains_sorting (hb_first)
	| hb_filter ([] (hb_codepoint_t gid) { પાછા gid > 0xFFFFu; }))
      format += 2;
#endif

    u.format.v = format;
    પસંદગી (u.format.v) {
    સ્થિતિ 1: hb_barrier (); u.format1.serialize (c,
                                 src, src_format,
                                 glyph_val_iter_pairs,
                                 new_format,
                                 layout_variation_idx_delta_map);
      પાછા;
    સ્થિતિ 2: hb_barrier (); u.format2.serialize (c,
                                 src, src_format,
                                 glyph_val_iter_pairs,
                                 new_format,
                                 layout_variation_idx_delta_map);
      પાછા;
#ifndef HB_NO_BEYOND_64K
    સ્થિતિ 3: u.format3.serialize (c,
                                 src, src_format,
                                 glyph_val_iter_pairs,
                                 new_format,
                                 layout_variation_idx_delta_map);
      પાછા;
    સ્થિતિ 4: u.format4.serialize (c,
                                 src, src_format,
                                 glyph_val_iter_pairs,
                                 new_format,
                                 layout_variation_idx_delta_map);
      પાછા;
#endif
    મૂળભૂત:પાછા;
    }
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) અચલ
  {
    જો (unlikely (!c->may_dispatch (આ, &u.format.v))) પાછા c->no_dispatch_return_કિંમત ();
    TRACE_DISPATCH (આ, u.format.v);
    પસંદગી (u.format.v) {
    સ્થિતિ 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    સ્થિતિ 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
#ifndef HB_NO_BEYOND_64K
    સ્થિતિ 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    સ્થિતિ 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
#endif
    મૂળભૂત:return_trace (c->default_return_કિંમત ());
    }
  }
};


template<typename Iterator>
સ્થિર શૂન્ય
SinglePos_serialize (hb_serialize_context_t *c,
                     અચલ કિંમત_Base *src,
                     કિંમત_Format src_format,
                     Iterator it,
                     અચલ hb_hashmap_t<unsigned, hb_pair_t<unsigned, પૂર્ણાંક>> *layout_variation_idx_delta_map,
                     unsigned new_format)
{ c->start_embed<SinglePos> ()->serialize (c, src, src_format, it, layout_variation_idx_delta_map, new_format); }


}
}
}

#endif /* OT_LAYOUT_GPOS_SINGLEPOS_HH */
