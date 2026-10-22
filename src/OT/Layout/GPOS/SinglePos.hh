#که_نه_تعریف_شوی OT_LAYOUT_GPOS_SINGLEPOS_HH
#تعریف_کړه OT_LAYOUT_GPOS_SINGLEPOS_HH

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
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
  SinglePosFormat1_3<MediumTypes> format3;
  SinglePosFormat2_4<MediumTypes> format4;
#د_که_پای
  } u;

  عام:
  کينډۍ<ډول_نوم Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  بینښه get_format (Iterator glyph_val_iter_جوړې)
  {
    hb_array_t<ثابت ارزښت> لومړی_val_iter = hb_دوهم (*glyph_val_iter_جوړې);

    لكل (ثابت خپلکاری iter : glyph_val_iter_جوړې)
      لكل (ثابت خپلکاری _ : hb_zip (iter.second, لومړی_val_iter))
        إذا (_.لومړی != _.second)
          أرجع 2;

    أرجع 1;
  }

  کينډۍ<ډول_نوم Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  فراغ serialize (hb_serialize_context_t *c,
                  ثابت ارزښت_بنسټ* src,
                  ارزښت_Format src_format,
                  Iterator glyph_val_iter_جوړې,
                  ثابت hb_hashmap_t<بینښه, hb_pair_t<بینښه, صحيح>> *layout_variation_idx_delta_map,
                  بینښه newFormat)
  {
    إذا (unlikely (!c->extend_min (u.format.v))) أرجع;
    بینښه format = 2;
    ارزښت_Format new_format;
    new_format = newFormat;

    إذا (glyph_val_iter_جوړې)
      format = get_format (glyph_val_iter_جوړې);

#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    إذا (+ glyph_val_iter_جوړې
	| hb_map_retains_ترتیبونه (hb_لومړی)
	| hb_filter ([] (hb_codepoint_t gid) { أرجع gid > 0xFFFFu; }))
      format += 2;
#د_که_پای

    u.format.v = format;
    بدل (u.format.v) {
    حالة 1: hb_barrier (); u.format1.serialize (c,
                                 src, src_format,
                                 glyph_val_iter_جوړې,
                                 new_format,
                                 layout_variation_idx_delta_map);
      أرجع;
    حالة 2: hb_barrier (); u.format2.serialize (c,
                                 src, src_format,
                                 glyph_val_iter_جوړې,
                                 new_format,
                                 layout_variation_idx_delta_map);
      أرجع;
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    حالة 3: u.format3.serialize (c,
                                 src, src_format,
                                 glyph_val_iter_جوړې,
                                 new_format,
                                 layout_variation_idx_delta_map);
      أرجع;
    حالة 4: u.format4.serialize (c,
                                 src, src_format,
                                 glyph_val_iter_جوړې,
                                 new_format,
                                 layout_variation_idx_delta_map);
      أرجع;
#د_که_پای
    افتراضي:أرجع;
    }
  }

  کينډۍ <ډول_نوم context_t, ډول_نوم ...Ts>
  ډول_نوم context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    إذا (unlikely (!c->may_dispatch (هذا, &u.format.v))) أرجع c->no_dispatch_return_ارزښت ();
    TRACE_DISPATCH (هذا, u.format.v);
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    حالة 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    حالة 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
#د_که_پای
    افتراضي:return_trace (c->default_return_ارزښت ());
    }
  }
};


کينډۍ<ډول_نوم Iterator>
ساكن فراغ
SinglePos_serialize (hb_serialize_context_t *c,
                     ثابت ارزښت_بنسټ *src,
                     ارزښت_Format src_format,
                     Iterator it,
                     ثابت hb_hashmap_t<بینښه, hb_pair_t<بینښه, صحيح>> *layout_variation_idx_delta_map,
                     بینښه new_format)
{ c->پیل_embed<SinglePos> ()->serialize (c, src, src_format, it, layout_variation_idx_delta_map, new_format); }


}
}
}

#د_که_پای /* OT_LAYOUT_GPOS_SINGLEPOS_HH */
