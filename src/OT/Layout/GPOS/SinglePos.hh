#ifndef OT_LAYOUT_GPOS_SINGLEPOS_HH
#define OT_LAYOUT_GPOS_SINGLEPOS_HH

#অন্তর্ভাৱ "SinglePosFormat1.hh"
#অন্তর্ভাৱ "SinglePosFormat2.hh"

নামাকাশ OT {
নামাকাশ Layout {
নামাকাশ GPOS_impl {

সংরচনা SinglePos
{
  রক্ষিত:
  সঙ্ঘ {
  সংরচনা { HBUINT16 v; } format;        /* Format identifier */
  SinglePosFormat1_3<SmallTypes> format1;
  SinglePosFormat2_4<SmallTypes> format2;
#ifndef HB_NO_BEYOND_64K
  SinglePosFormat1_3<MediumTypes> format3;
  SinglePosFormat2_4<MediumTypes> format4;
#endif
  } u;

  সার্ৱজনিক:
  template<typename Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  unsigned get_format (Iterator glyph_val_iter_pairs)
  {
    hb_array_t<ধ্রুৱ মান> first_val_iter = hb_second (*glyph_val_iter_pairs);

    বাবে (ধ্রুৱ auto iter : glyph_val_iter_pairs)
      বাবে (ধ্রুৱ auto _ : hb_zip (iter.second, first_val_iter))
        যদি (_.first != _.second)
          ঘূৰাই 2;

    ঘূৰাই 1;
  }

  template<typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  শূন্য serialize (hb_serialize_context_t *c,
                  ধ্রুৱ মান_Base* src,
                  মান_Format src_format,
                  Iterator glyph_val_iter_pairs,
                  ধ্রুৱ hb_hashmap_t<unsigned, hb_pair_t<unsigned, পূর্ণাঙ্ক>> *layout_variation_idx_delta_map,
                  unsigned newFormat)
  {
    যদি (unlikely (!c->extend_min (u.format.v))) ঘূৰাই;
    unsigned format = 2;
    মান_Format new_format;
    new_format = newFormat;

    যদি (glyph_val_iter_pairs)
      format = get_format (glyph_val_iter_pairs);

#ifndef HB_NO_BEYOND_64K
    যদি (+ glyph_val_iter_pairs
	| hb_map_retains_sorting (hb_first)
	| hb_filter ([] (hb_codepoint_t gid) { ঘূৰাই gid > 0xFFFFu; }))
      format += 2;
#endif

    u.format.v = format;
    ৱিকল্প (u.format.v) {
    পক্ষ 1: hb_barrier (); u.format1.serialize (c,
                                 src, src_format,
                                 glyph_val_iter_pairs,
                                 new_format,
                                 layout_variation_idx_delta_map);
      ঘূৰাই;
    পক্ষ 2: hb_barrier (); u.format2.serialize (c,
                                 src, src_format,
                                 glyph_val_iter_pairs,
                                 new_format,
                                 layout_variation_idx_delta_map);
      ঘূৰাই;
#ifndef HB_NO_BEYOND_64K
    পক্ষ 3: u.format3.serialize (c,
                                 src, src_format,
                                 glyph_val_iter_pairs,
                                 new_format,
                                 layout_variation_idx_delta_map);
      ঘূৰাই;
    পক্ষ 4: u.format4.serialize (c,
                                 src, src_format,
                                 glyph_val_iter_pairs,
                                 new_format,
                                 layout_variation_idx_delta_map);
      ঘূৰাই;
#endif
    স্ৱভাৱ:ঘূৰাই;
    }
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ধ্রুৱ
  {
    যদি (unlikely (!c->may_dispatch (এতত্, &u.format.v))) ঘূৰাই c->no_dispatch_return_মান ();
    TRACE_DISPATCH (এতত্, u.format.v);
    ৱিকল্প (u.format.v) {
    পক্ষ 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    পক্ষ 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
#ifndef HB_NO_BEYOND_64K
    পক্ষ 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    পক্ষ 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
#endif
    স্ৱভাৱ:return_trace (c->default_return_মান ());
    }
  }
};


template<typename Iterator>
স্থির শূন্য
SinglePos_serialize (hb_serialize_context_t *c,
                     ধ্রুৱ মান_Base *src,
                     মান_Format src_format,
                     Iterator it,
                     ধ্রুৱ hb_hashmap_t<unsigned, hb_pair_t<unsigned, পূর্ণাঙ্ক>> *layout_variation_idx_delta_map,
                     unsigned new_format)
{ c->start_embed<SinglePos> ()->serialize (c, src, src_format, it, layout_variation_idx_delta_map, new_format); }


}
}
}

#endif /* OT_LAYOUT_GPOS_SINGLEPOS_HH */
