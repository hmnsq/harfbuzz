#اگر_تعریف_نشده OT_LAYOUT_GPOS_SINGLEPOS_HH
#تعریف_کن OT_LAYOUT_GPOS_SINGLEPOS_HH

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
#اگر_تعریف_نشده HB_NO_BEYOND_64K
  SinglePosFormat1_3<MediumTypes> format3;
  SinglePosFormat2_4<MediumTypes> format4;
#پایان_اگر
  } u;

  عام:
  قالب<نام_نوع Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  بیعلامت get_format (Iterator glyph_val_iter_جفتها)
  {
    hb_array_t<ثابت مقدار> اولی_val_iter = hb_دومی (*glyph_val_iter_جفتها);

    لكل (ثابت خودکار iter : glyph_val_iter_جفتها)
      لكل (ثابت خودکار _ : hb_zip (iter.second, اولی_val_iter))
        إذا (_.اولی != _.second)
          أرجع 2;

    أرجع 1;
  }

  قالب<نام_نوع Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  فراغ serialize (hb_serialize_context_t *c,
                  ثابت مقدار_پایه* src,
                  مقدار_Format src_format,
                  Iterator glyph_val_iter_جفتها,
                  ثابت hb_hashmap_t<بیعلامت, hb_pair_t<بیعلامت, صحيح>> *layout_variation_idx_delta_map,
                  بیعلامت newFormat)
  {
    إذا (unlikely (!c->extend_min (u.format.v))) أرجع;
    بیعلامت format = 2;
    مقدار_Format new_format;
    new_format = newFormat;

    إذا (glyph_val_iter_جفتها)
      format = get_format (glyph_val_iter_جفتها);

#اگر_تعریف_نشده HB_NO_BEYOND_64K
    إذا (+ glyph_val_iter_جفتها
	| hb_map_retains_ترتیبدهی (hb_اولی)
	| hb_filter ([] (hb_codepoint_t gid) { أرجع gid > 0xFFFFu; }))
      format += 2;
#پایان_اگر

    u.format.v = format;
    بدل (u.format.v) {
    حالة 1: hb_barrier (); u.format1.serialize (c,
                                 src, src_format,
                                 glyph_val_iter_جفتها,
                                 new_format,
                                 layout_variation_idx_delta_map);
      أرجع;
    حالة 2: hb_barrier (); u.format2.serialize (c,
                                 src, src_format,
                                 glyph_val_iter_جفتها,
                                 new_format,
                                 layout_variation_idx_delta_map);
      أرجع;
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    حالة 3: u.format3.serialize (c,
                                 src, src_format,
                                 glyph_val_iter_جفتها,
                                 new_format,
                                 layout_variation_idx_delta_map);
      أرجع;
    حالة 4: u.format4.serialize (c,
                                 src, src_format,
                                 glyph_val_iter_جفتها,
                                 new_format,
                                 layout_variation_idx_delta_map);
      أرجع;
#پایان_اگر
    افتراضي:أرجع;
    }
  }

  قالب <نام_نوع context_t, نام_نوع ...Ts>
  نام_نوع context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    إذا (unlikely (!c->may_dispatch (هذا, &u.format.v))) أرجع c->no_dispatch_return_مقدار ();
    TRACE_DISPATCH (هذا, u.format.v);
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    حالة 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    حالة 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
#پایان_اگر
    افتراضي:return_trace (c->default_return_مقدار ());
    }
  }
};


قالب<نام_نوع Iterator>
ساكن فراغ
SinglePos_serialize (hb_serialize_context_t *c,
                     ثابت مقدار_پایه *src,
                     مقدار_Format src_format,
                     Iterator it,
                     ثابت hb_hashmap_t<بیعلامت, hb_pair_t<بیعلامت, صحيح>> *layout_variation_idx_delta_map,
                     بیعلامت new_format)
{ c->شروع_embed<SinglePos> ()->serialize (c, src, src_format, it, layout_variation_idx_delta_map, new_format); }


}
}
}

#پایان_اگر /* OT_LAYOUT_GPOS_SINGLEPOS_HH */
