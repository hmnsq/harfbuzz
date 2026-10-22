#جيڪڏهن_وصف_نه_ٿيل OT_LAYOUT_GPOS_SINGLEPOS_HH
#وصف_ڪريو OT_LAYOUT_GPOS_SINGLEPOS_HH

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
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
  SinglePosFormat1_3<MediumTypes> format3;
  SinglePosFormat2_4<MediumTypes> format4;
#جيڪڏهن_پورو
  } u;

  عام:
  سانچو<قسم_نالو Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  بنانشان get_format (Iterator glyph_val_iter_جوڙا)
  {
    hb_array_t<ثابت قدر> پهريون_val_iter = hb_ٻيو (*glyph_val_iter_جوڙا);

    لكل (ثابت خودڪار iter : glyph_val_iter_جوڙا)
      لكل (ثابت خودڪار _ : hb_zip (iter.second, پهريون_val_iter))
        إذا (_.پهريون != _.second)
          أرجع 2;

    أرجع 1;
  }

  سانچو<قسم_نالو Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  فراغ serialize (hb_serialize_context_t *c,
                  ثابت قدر_بنياد* src,
                  قدر_Format src_format,
                  Iterator glyph_val_iter_جوڙا,
                  ثابت hb_hashmap_t<بنانشان, hb_pair_t<بنانشان, صحيح>> *layout_variation_idx_delta_map,
                  بنانشان newFormat)
  {
    إذا (unlikely (!c->extend_min (u.format.v))) أرجع;
    بنانشان format = 2;
    قدر_Format new_format;
    new_format = newFormat;

    إذا (glyph_val_iter_جوڙا)
      format = get_format (glyph_val_iter_جوڙا);

#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    إذا (+ glyph_val_iter_جوڙا
	| hb_map_retains_ترتيب_ڏيڻ (hb_پهريون)
	| hb_filter ([] (hb_codepoint_t gid) { أرجع gid > 0xFFFFu; }))
      format += 2;
#جيڪڏهن_پورو

    u.format.v = format;
    بدل (u.format.v) {
    حالة 1: hb_barrier (); u.format1.serialize (c,
                                 src, src_format,
                                 glyph_val_iter_جوڙا,
                                 new_format,
                                 layout_variation_idx_delta_map);
      أرجع;
    حالة 2: hb_barrier (); u.format2.serialize (c,
                                 src, src_format,
                                 glyph_val_iter_جوڙا,
                                 new_format,
                                 layout_variation_idx_delta_map);
      أرجع;
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    حالة 3: u.format3.serialize (c,
                                 src, src_format,
                                 glyph_val_iter_جوڙا,
                                 new_format,
                                 layout_variation_idx_delta_map);
      أرجع;
    حالة 4: u.format4.serialize (c,
                                 src, src_format,
                                 glyph_val_iter_جوڙا,
                                 new_format,
                                 layout_variation_idx_delta_map);
      أرجع;
#جيڪڏهن_پورو
    افتراضي:أرجع;
    }
  }

  سانچو <قسم_نالو context_t, قسم_نالو ...Ts>
  قسم_نالو context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    إذا (unlikely (!c->may_dispatch (هذا, &u.format.v))) أرجع c->no_dispatch_return_قدر ();
    TRACE_DISPATCH (هذا, u.format.v);
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    حالة 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    حالة 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
#جيڪڏهن_پورو
    افتراضي:return_trace (c->default_return_قدر ());
    }
  }
};


سانچو<قسم_نالو Iterator>
ساكن فراغ
SinglePos_serialize (hb_serialize_context_t *c,
                     ثابت قدر_بنياد *src,
                     قدر_Format src_format,
                     Iterator it,
                     ثابت hb_hashmap_t<بنانشان, hb_pair_t<بنانشان, صحيح>> *layout_variation_idx_delta_map,
                     بنانشان new_format)
{ c->شروعات_embed<SinglePos> ()->serialize (c, src, src_format, it, layout_variation_idx_delta_map, new_format); }


}
}
}

#جيڪڏهن_پورو /* OT_LAYOUT_GPOS_SINGLEPOS_HH */
