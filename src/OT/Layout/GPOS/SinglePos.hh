#ifndef OT_LAYOUT_GPOS_SINGLEPOS_HH
#define OT_LAYOUT_GPOS_SINGLEPOS_HH

#شامل "SinglePosFormat1.hh"
#شامل "SinglePosFormat2.hh"

فضاینام OT {
فضاینام Layout {
فضاینام GPOS_impl {

ساختار SinglePos
{
  محافظتشده:
  اجتماع {
  ساختار { HBUINT16 v; } format;        /* Format identifier */
  SinglePosFormat1_3<SmallTypes> format1;
  SinglePosFormat2_4<SmallTypes> format2;
#ifndef HB_NO_BEYOND_64K
  SinglePosFormat1_3<MediumTypes> format3;
  SinglePosFormat2_4<MediumTypes> format4;
#endif
  } u;

  عمومی:
  template<typename Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  unsigned get_format (Iterator glyph_val_iter_pairs)
  {
    hb_array_t<ثابت مقدار> first_val_iter = hb_second (*glyph_val_iter_pairs);

    برای (ثابت auto iter : glyph_val_iter_pairs)
      برای (ثابت auto _ : hb_zip (iter.second, first_val_iter))
        اگر (_.first != _.second)
          بازگشت 2;

    بازگشت 1;
  }

  template<typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  تهی serialize (hb_serialize_context_t *c,
                  ثابت مقدار_Base* src,
                  مقدار_Format src_format,
                  Iterator glyph_val_iter_pairs,
                  ثابت hb_hashmap_t<unsigned, hb_pair_t<unsigned, صحیح>> *layout_variation_idx_delta_map,
                  unsigned newFormat)
  {
    اگر (unlikely (!c->extend_min (u.format.v))) بازگشت;
    unsigned format = 2;
    مقدار_Format new_format;
    new_format = newFormat;

    اگر (glyph_val_iter_pairs)
      format = get_format (glyph_val_iter_pairs);

#ifndef HB_NO_BEYOND_64K
    اگر (+ glyph_val_iter_pairs
	| hb_map_retains_sorting (hb_first)
	| hb_filter ([] (hb_codepoint_t gid) { بازگشت gid > 0xFFFFu; }))
      format += 2;
#endif

    u.format.v = format;
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); u.format1.serialize (c,
                                 src, src_format,
                                 glyph_val_iter_pairs,
                                 new_format,
                                 layout_variation_idx_delta_map);
      بازگشت;
    حالت 2: hb_barrier (); u.format2.serialize (c,
                                 src, src_format,
                                 glyph_val_iter_pairs,
                                 new_format,
                                 layout_variation_idx_delta_map);
      بازگشت;
#ifndef HB_NO_BEYOND_64K
    حالت 3: u.format3.serialize (c,
                                 src, src_format,
                                 glyph_val_iter_pairs,
                                 new_format,
                                 layout_variation_idx_delta_map);
      بازگشت;
    حالت 4: u.format4.serialize (c,
                                 src, src_format,
                                 glyph_val_iter_pairs,
                                 new_format,
                                 layout_variation_idx_delta_map);
      بازگشت;
#endif
    پیشفرض:بازگشت;
    }
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    اگر (unlikely (!c->may_dispatch (این, &u.format.v))) بازگشت c->no_dispatch_return_مقدار ();
    TRACE_DISPATCH (این, u.format.v);
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    حالت 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
#ifndef HB_NO_BEYOND_64K
    حالت 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    حالت 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
#endif
    پیشفرض:return_trace (c->default_return_مقدار ());
    }
  }
};


template<typename Iterator>
ایستا تهی
SinglePos_serialize (hb_serialize_context_t *c,
                     ثابت مقدار_Base *src,
                     مقدار_Format src_format,
                     Iterator it,
                     ثابت hb_hashmap_t<unsigned, hb_pair_t<unsigned, صحیح>> *layout_variation_idx_delta_map,
                     unsigned new_format)
{ c->start_embed<SinglePos> ()->serialize (c, src, src_format, it, layout_variation_idx_delta_map, new_format); }


}
}
}

#endif /* OT_LAYOUT_GPOS_SINGLEPOS_HH */
