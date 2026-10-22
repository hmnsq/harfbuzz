#ifndef OT_LAYOUT_GPOS_SINGLEPOS_HH
#define OT_LAYOUT_GPOS_SINGLEPOS_HH

#ಅನ್ತರ್ಭಾವ "SinglePosFormat1.hh"
#ಅನ್ತರ್ಭಾವ "SinglePosFormat2.hh"

ನಾಮಾಕಾಶ OT {
ನಾಮಾಕಾಶ Layout {
ನಾಮಾಕಾಶ GPOS_impl {

ಸಂರಚನಾ SinglePos
{
  ರಕ್ಷಿತ:
  ಸಙ್ಘ {
  ಸಂರಚನಾ { HBUINT16 v; } format;        /* Format identifier */
  SinglePosFormat1_3<SmallTypes> format1;
  SinglePosFormat2_4<SmallTypes> format2;
#ifndef HB_NO_BEYOND_64K
  SinglePosFormat1_3<MediumTypes> format3;
  SinglePosFormat2_4<MediumTypes> format4;
#endif
  } u;

  ಸಾರ್ವಜನಿಕ:
  template<typename Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  unsigned get_format (Iterator glyph_val_iter_pairs)
  {
    hb_array_t<ಧ್ರುವ ಮೌಲ್ಯ> first_val_iter = hb_second (*glyph_val_iter_pairs);

    ಪ್ರತಿ (ಧ್ರುವ auto iter : glyph_val_iter_pairs)
      ಪ್ರತಿ (ಧ್ರುವ auto _ : hb_zip (iter.second, first_val_iter))
        ಯದಿ (_.first != _.second)
          ಪ್ರತ್ಯಾಗಮ 2;

    ಪ್ರತ್ಯಾಗಮ 1;
  }

  template<typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  ಶೂನ್ಯ serialize (hb_serialize_context_t *c,
                  ಧ್ರುವ ಮೌಲ್ಯ_Base* src,
                  ಮೌಲ್ಯ_Format src_format,
                  Iterator glyph_val_iter_pairs,
                  ಧ್ರುವ hb_hashmap_t<unsigned, hb_pair_t<unsigned, ಪೂರ್ಣಾಙ್ಕ>> *layout_variation_idx_delta_map,
                  unsigned newFormat)
  {
    ಯದಿ (unlikely (!c->extend_min (u.format.v))) ಪ್ರತ್ಯಾಗಮ;
    unsigned format = 2;
    ಮೌಲ್ಯ_Format new_format;
    new_format = newFormat;

    ಯದಿ (glyph_val_iter_pairs)
      format = get_format (glyph_val_iter_pairs);

#ifndef HB_NO_BEYOND_64K
    ಯದಿ (+ glyph_val_iter_pairs
	| hb_map_retains_sorting (hb_first)
	| hb_filter ([] (hb_codepoint_t gid) { ಪ್ರತ್ಯಾಗಮ gid > 0xFFFFu; }))
      format += 2;
#endif

    u.format.v = format;
    ವಿಕಲ್ಪ (u.format.v) {
    ಪಕ್ಷ 1: hb_barrier (); u.format1.serialize (c,
                                 src, src_format,
                                 glyph_val_iter_pairs,
                                 new_format,
                                 layout_variation_idx_delta_map);
      ಪ್ರತ್ಯಾಗಮ;
    ಪಕ್ಷ 2: hb_barrier (); u.format2.serialize (c,
                                 src, src_format,
                                 glyph_val_iter_pairs,
                                 new_format,
                                 layout_variation_idx_delta_map);
      ಪ್ರತ್ಯಾಗಮ;
#ifndef HB_NO_BEYOND_64K
    ಪಕ್ಷ 3: u.format3.serialize (c,
                                 src, src_format,
                                 glyph_val_iter_pairs,
                                 new_format,
                                 layout_variation_idx_delta_map);
      ಪ್ರತ್ಯಾಗಮ;
    ಪಕ್ಷ 4: u.format4.serialize (c,
                                 src, src_format,
                                 glyph_val_iter_pairs,
                                 new_format,
                                 layout_variation_idx_delta_map);
      ಪ್ರತ್ಯಾಗಮ;
#endif
    ಸ್ವಭಾವ:ಪ್ರತ್ಯಾಗಮ;
    }
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ಧ್ರುವ
  {
    ಯದಿ (unlikely (!c->may_dispatch (ಏತತ್, &u.format.v))) ಪ್ರತ್ಯಾಗಮ c->no_dispatch_return_ಮೌಲ್ಯ ();
    TRACE_DISPATCH (ಏತತ್, u.format.v);
    ವಿಕಲ್ಪ (u.format.v) {
    ಪಕ್ಷ 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    ಪಕ್ಷ 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
#ifndef HB_NO_BEYOND_64K
    ಪಕ್ಷ 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    ಪಕ್ಷ 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
#endif
    ಸ್ವಭಾವ:return_trace (c->default_return_ಮೌಲ್ಯ ());
    }
  }
};


template<typename Iterator>
ಸ್ಥಿರ ಶೂನ್ಯ
SinglePos_serialize (hb_serialize_context_t *c,
                     ಧ್ರುವ ಮೌಲ್ಯ_Base *src,
                     ಮೌಲ್ಯ_Format src_format,
                     Iterator it,
                     ಧ್ರುವ hb_hashmap_t<unsigned, hb_pair_t<unsigned, ಪೂರ್ಣಾಙ್ಕ>> *layout_variation_idx_delta_map,
                     unsigned new_format)
{ c->start_embed<SinglePos> ()->serialize (c, src, src_format, it, layout_variation_idx_delta_map, new_format); }


}
}
}

#endif /* OT_LAYOUT_GPOS_SINGLEPOS_HH */
