#جے_بنا_تعریف OT_LAYOUT_GPOS_SINGLEPOS_HH
#تعریف_کرو OT_LAYOUT_GPOS_SINGLEPOS_HH

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
#جے_بنا_تعریف HB_NO_BEYOND_64K
  SinglePosFormat1_3<MediumTypes> format3;
  SinglePosFormat2_4<MediumTypes> format4;
#جے_انت
  } u;

  عام:
  سانچا<قسم_ناں Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  بنانشانی get_format (Iterator glyph_val_iter_جوڑے)
  {
    hb_array_t<ثابت قدر> پہلا_val_iter = hb_دوجا (*glyph_val_iter_جوڑے);

    لكل (ثابت خودکار iter : glyph_val_iter_جوڑے)
      لكل (ثابت خودکار _ : hb_zip (iter.second, پہلا_val_iter))
        إذا (_.پہلا != _.second)
          أرجع 2;

    أرجع 1;
  }

  سانچا<قسم_ناں Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  فراغ serialize (hb_serialize_context_t *c,
                  ثابت قدر_ادھار* src,
                  قدر_Format src_format,
                  Iterator glyph_val_iter_جوڑے,
                  ثابت hb_hashmap_t<بنانشانی, hb_pair_t<بنانشانی, صحيح>> *layout_variation_idx_delta_map,
                  بنانشانی newFormat)
  {
    إذا (unlikely (!c->extend_min (u.format.v))) أرجع;
    بنانشانی format = 2;
    قدر_Format new_format;
    new_format = newFormat;

    إذا (glyph_val_iter_جوڑے)
      format = get_format (glyph_val_iter_جوڑے);

#جے_بنا_تعریف HB_NO_BEYOND_64K
    إذا (+ glyph_val_iter_جوڑے
	| hb_map_retains_چھانٹی (hb_پہلا)
	| hb_filter ([] (hb_codepoint_t gid) { أرجع gid > 0xFFFFu; }))
      format += 2;
#جے_انت

    u.format.v = format;
    بدل (u.format.v) {
    حالة 1: hb_barrier (); u.format1.serialize (c,
                                 src, src_format,
                                 glyph_val_iter_جوڑے,
                                 new_format,
                                 layout_variation_idx_delta_map);
      أرجع;
    حالة 2: hb_barrier (); u.format2.serialize (c,
                                 src, src_format,
                                 glyph_val_iter_جوڑے,
                                 new_format,
                                 layout_variation_idx_delta_map);
      أرجع;
#جے_بنا_تعریف HB_NO_BEYOND_64K
    حالة 3: u.format3.serialize (c,
                                 src, src_format,
                                 glyph_val_iter_جوڑے,
                                 new_format,
                                 layout_variation_idx_delta_map);
      أرجع;
    حالة 4: u.format4.serialize (c,
                                 src, src_format,
                                 glyph_val_iter_جوڑے,
                                 new_format,
                                 layout_variation_idx_delta_map);
      أرجع;
#جے_انت
    افتراضي:أرجع;
    }
  }

  سانچا <قسم_ناں context_t, قسم_ناں ...Ts>
  قسم_ناں context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    إذا (unlikely (!c->may_dispatch (هذا, &u.format.v))) أرجع c->no_dispatch_return_قدر ();
    TRACE_DISPATCH (هذا, u.format.v);
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    حالة 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
#جے_بنا_تعریف HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    حالة 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
#جے_انت
    افتراضي:return_trace (c->default_return_قدر ());
    }
  }
};


سانچا<قسم_ناں Iterator>
ساكن فراغ
SinglePos_serialize (hb_serialize_context_t *c,
                     ثابت قدر_ادھار *src,
                     قدر_Format src_format,
                     Iterator it,
                     ثابت hb_hashmap_t<بنانشانی, hb_pair_t<بنانشانی, صحيح>> *layout_variation_idx_delta_map,
                     بنانشانی new_format)
{ c->شروع_embed<SinglePos> ()->serialize (c, src, src_format, it, layout_variation_idx_delta_map, new_format); }


}
}
}

#جے_انت /* OT_LAYOUT_GPOS_SINGLEPOS_HH */
