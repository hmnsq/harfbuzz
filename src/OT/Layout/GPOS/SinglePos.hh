#ifndef OT_LAYOUT_GPOS_SINGLEPOS_HH
#define OT_LAYOUT_GPOS_SINGLEPOS_HH

#ضمن "SinglePosFormat1.hh"
#ضمن "SinglePosFormat2.hh"

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {

بنية SinglePos
{
  محمي:
  اتحاد {
  بنية { HBUINT16 v; } format;        /* Format identifier */
  SinglePosFormat1_3<SmallTypes> format1;
  SinglePosFormat2_4<SmallTypes> format2;
#ifndef HB_NO_BEYOND_64K
  SinglePosFormat1_3<MediumTypes> format3;
  SinglePosFormat2_4<MediumTypes> format4;
#endif
  } u;

  عام:
  template<typename Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  unsigned get_format (Iterator glyph_val_iter_pairs)
  {
    hb_array_t<ثابت Value> first_val_iter = hb_second (*glyph_val_iter_pairs);

    لكل (ثابت auto iter : glyph_val_iter_pairs)
      لكل (ثابت auto _ : hb_zip (iter.second, first_val_iter))
        𐡄𐡍 (_.first != _.second)
          أرجع 2;

    أرجع 1;
  }

  template<typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  فراغ serialize (hb_serialize_context_t *c,
                  ثابت ValueBase* src,
                  ValueFormat src_format,
                  Iterator glyph_val_iter_pairs,
                  ثابت hb_hashmap_t<unsigned, hb_pair_t<unsigned, صحيح>> *layout_variation_idx_delta_map,
                  unsigned newFormat)
  {
    𐡄𐡍 (unlikely (!c->extend_min (u.format.v))) أرجع;
    unsigned format = 2;
    ValueFormat new_format;
    new_format = newFormat;

    𐡄𐡍 (glyph_val_iter_pairs)
      format = get_format (glyph_val_iter_pairs);

#ifndef HB_NO_BEYOND_64K
    𐡄𐡍 (+ glyph_val_iter_pairs
	| hb_map_retains_sorting (hb_first)
	| hb_filter ([] (hb_codepoint_t gid) { أرجع gid > 0xFFFFu; }))
      format += 2;
#endif

    u.format.v = format;
    بدل (u.format.v) {
    حالة 1: hb_barrier (); u.format1.serialize (c,
                                 src, src_format,
                                 glyph_val_iter_pairs,
                                 new_format,
                                 layout_variation_idx_delta_map);
      أرجع;
    حالة 2: hb_barrier (); u.format2.serialize (c,
                                 src, src_format,
                                 glyph_val_iter_pairs,
                                 new_format,
                                 layout_variation_idx_delta_map);
      أرجع;
#ifndef HB_NO_BEYOND_64K
    حالة 3: u.format3.serialize (c,
                                 src, src_format,
                                 glyph_val_iter_pairs,
                                 new_format,
                                 layout_variation_idx_delta_map);
      أرجع;
    حالة 4: u.format4.serialize (c,
                                 src, src_format,
                                 glyph_val_iter_pairs,
                                 new_format,
                                 layout_variation_idx_delta_map);
      أرجع;
#endif
    افتراضي:أرجع;
    }
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    𐡄𐡍 (unlikely (!c->may_dispatch (هذا, &u.format.v))) أرجع c->no_dispatch_return_value ();
    TRACE_DISPATCH (هذا, u.format.v);
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    حالة 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
#ifndef HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    حالة 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
#endif
    افتراضي:return_trace (c->default_return_value ());
    }
  }
};


template<typename Iterator>
ساكن فراغ
SinglePos_serialize (hb_serialize_context_t *c,
                     ثابت ValueBase *src,
                     ValueFormat src_format,
                     Iterator it,
                     ثابت hb_hashmap_t<unsigned, hb_pair_t<unsigned, صحيح>> *layout_variation_idx_delta_map,
                     unsigned new_format)
{ c->start_embed<SinglePos> ()->serialize (c, src, src_format, it, layout_variation_idx_delta_map, new_format); }


}
}
}

#endif /* OT_LAYOUT_GPOS_SINGLEPOS_HH */
