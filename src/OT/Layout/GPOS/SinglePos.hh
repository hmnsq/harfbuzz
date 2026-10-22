#ifndef OT_LAYOUT_GPOS_SINGLEPOS_HH
#define OT_LAYOUT_GPOS_SINGLEPOS_HH

#அந்தர்பாவ "SinglePosFormat1.hh"
#அந்தர்பாவ "SinglePosFormat2.hh"

நாமாகாஶ OT {
நாமாகாஶ Layout {
நாமாகாஶ GPOS_impl {

கட்டமைப்பு SinglePos
{
  பாதுகாக்கப்பட்ட:
  ஒன்றிப்பு {
  கட்டமைப்பு { HBUINT16 v; } format;        /* Format identifier */
  SinglePosFormat1_3<SmallTypes> format1;
  SinglePosFormat2_4<SmallTypes> format2;
#ifndef HB_NO_BEYOND_64K
  SinglePosFormat1_3<MediumTypes> format3;
  SinglePosFormat2_4<MediumTypes> format4;
#endif
  } u;

  பொது:
  template<typename Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  unsigned get_format (Iterator glyph_val_iter_pairs)
  {
    hb_array_t<மாறிலி மதிப்பு> first_val_iter = hb_second (*glyph_val_iter_pairs);

    க்கு (மாறிலி auto iter : glyph_val_iter_pairs)
      க்கு (மாறிலி auto _ : hb_zip (iter.second, first_val_iter))
        எனில் (_.first != _.second)
          திரும்பு 2;

    திரும்பு 1;
  }

  template<typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  வெறுமை serialize (hb_serialize_context_t *c,
                  மாறிலி மதிப்பு_Base* src,
                  மதிப்பு_Format src_format,
                  Iterator glyph_val_iter_pairs,
                  மாறிலி hb_hashmap_t<unsigned, hb_pair_t<unsigned, முழு>> *layout_variation_idx_delta_map,
                  unsigned newFormat)
  {
    எனில் (unlikely (!c->extend_min (u.format.v))) திரும்பு;
    unsigned format = 2;
    மதிப்பு_Format new_format;
    new_format = newFormat;

    எனில் (glyph_val_iter_pairs)
      format = get_format (glyph_val_iter_pairs);

#ifndef HB_NO_BEYOND_64K
    எனில் (+ glyph_val_iter_pairs
	| hb_map_retains_sorting (hb_first)
	| hb_filter ([] (hb_codepoint_t gid) { திரும்பு gid > 0xFFFFu; }))
      format += 2;
#endif

    u.format.v = format;
    தேர்வு (u.format.v) {
    நிலை 1: hb_barrier (); u.format1.serialize (c,
                                 src, src_format,
                                 glyph_val_iter_pairs,
                                 new_format,
                                 layout_variation_idx_delta_map);
      திரும்பு;
    நிலை 2: hb_barrier (); u.format2.serialize (c,
                                 src, src_format,
                                 glyph_val_iter_pairs,
                                 new_format,
                                 layout_variation_idx_delta_map);
      திரும்பு;
#ifndef HB_NO_BEYOND_64K
    நிலை 3: u.format3.serialize (c,
                                 src, src_format,
                                 glyph_val_iter_pairs,
                                 new_format,
                                 layout_variation_idx_delta_map);
      திரும்பு;
    நிலை 4: u.format4.serialize (c,
                                 src, src_format,
                                 glyph_val_iter_pairs,
                                 new_format,
                                 layout_variation_idx_delta_map);
      திரும்பு;
#endif
    இயல்பு:திரும்பு;
    }
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) மாறிலி
  {
    எனில் (unlikely (!c->may_dispatch (இது, &u.format.v))) திரும்பு c->no_dispatch_return_மதிப்பு ();
    TRACE_DISPATCH (இது, u.format.v);
    தேர்வு (u.format.v) {
    நிலை 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    நிலை 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
#ifndef HB_NO_BEYOND_64K
    நிலை 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    நிலை 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
#endif
    இயல்பு:return_trace (c->default_return_மதிப்பு ());
    }
  }
};


template<typename Iterator>
நிலையான வெறுமை
SinglePos_serialize (hb_serialize_context_t *c,
                     மாறிலி மதிப்பு_Base *src,
                     மதிப்பு_Format src_format,
                     Iterator it,
                     மாறிலி hb_hashmap_t<unsigned, hb_pair_t<unsigned, முழு>> *layout_variation_idx_delta_map,
                     unsigned new_format)
{ c->start_embed<SinglePos> ()->serialize (c, src, src_format, it, layout_variation_idx_delta_map, new_format); }


}
}
}

#endif /* OT_LAYOUT_GPOS_SINGLEPOS_HH */
