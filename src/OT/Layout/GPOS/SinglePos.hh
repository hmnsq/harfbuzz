#اگر_غیر_تعریف_شدہ OT_LAYOUT_GPOS_SINGLEPOS_HH
#تعریف_کرو OT_LAYOUT_GPOS_SINGLEPOS_HH

#شامل "SinglePosFormat1.hh"
#شامل "SinglePosFormat2.hh"

نامخلا OT {
نامخلا Layout {
نامخلا GPOS_impl {

ساخت SinglePos
{
  محفوظ:
  اتحاد {
  ساخت { HBUINT16 v; } format;        /* Format identifier */
  SinglePosFormat1_3<SmallTypes> format1;
  SinglePosFormat2_4<SmallTypes> format2;
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
  SinglePosFormat1_3<MediumTypes> format3;
  SinglePosFormat2_4<MediumTypes> format4;
#اگر_ختم
  } u;

  عوامی:
  سانچہ<قسم_نام Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  غیرعلامتی get_format (Iterator glyph_val_iter_جوڑے)
  {
    hb_array_t<ثابت قدر> پہلا_val_iter = hb_دوسرا (*glyph_val_iter_جوڑے);

    برائے (ثابت خودکار iter : glyph_val_iter_جوڑے)
      برائے (ثابت خودکار _ : hb_zip (iter.second, پہلا_val_iter))
        اگر (_.پہلا != _.second)
          واپس 2;

    واپس 1;
  }

  سانچہ<قسم_نام Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  خالی serialize (hb_serialize_context_t *c,
                  ثابت قدر_بنیاد* src,
                  قدر_Format src_format,
                  Iterator glyph_val_iter_جوڑے,
                  ثابت hb_hashmap_t<غیرعلامتی, hb_pair_t<غیرعلامتی, صحیح>> *layout_variation_idx_delta_map,
                  غیرعلامتی newFormat)
  {
    اگر (unlikely (!c->extend_min (u.format.v))) واپس;
    غیرعلامتی format = 2;
    قدر_Format new_format;
    new_format = newFormat;

    اگر (glyph_val_iter_جوڑے)
      format = get_format (glyph_val_iter_جوڑے);

#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    اگر (+ glyph_val_iter_جوڑے
	| hb_map_retains_ترتیب_کاری (hb_پہلا)
	| hb_filter ([] (hb_codepoint_t gid) { واپس gid > 0xFFFFu; }))
      format += 2;
#اگر_ختم

    u.format.v = format;
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); u.format1.serialize (c,
                                 src, src_format,
                                 glyph_val_iter_جوڑے,
                                 new_format,
                                 layout_variation_idx_delta_map);
      واپس;
    حالت 2: hb_barrier (); u.format2.serialize (c,
                                 src, src_format,
                                 glyph_val_iter_جوڑے,
                                 new_format,
                                 layout_variation_idx_delta_map);
      واپس;
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    حالت 3: u.format3.serialize (c,
                                 src, src_format,
                                 glyph_val_iter_جوڑے,
                                 new_format,
                                 layout_variation_idx_delta_map);
      واپس;
    حالت 4: u.format4.serialize (c,
                                 src, src_format,
                                 glyph_val_iter_جوڑے,
                                 new_format,
                                 layout_variation_idx_delta_map);
      واپس;
#اگر_ختم
    طےشدہ:واپس;
    }
  }

  سانچہ <قسم_نام context_t, قسم_نام ...Ts>
  قسم_نام context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    اگر (unlikely (!c->may_dispatch (یہ, &u.format.v))) واپس c->no_dispatch_return_قدر ();
    TRACE_DISPATCH (یہ, u.format.v);
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    حالت 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    حالت 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    حالت 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
#اگر_ختم
    طےشدہ:return_trace (c->default_return_قدر ());
    }
  }
};


سانچہ<قسم_نام Iterator>
ساکن خالی
SinglePos_serialize (hb_serialize_context_t *c,
                     ثابت قدر_بنیاد *src,
                     قدر_Format src_format,
                     Iterator it,
                     ثابت hb_hashmap_t<غیرعلامتی, hb_pair_t<غیرعلامتی, صحیح>> *layout_variation_idx_delta_map,
                     غیرعلامتی new_format)
{ c->آغاز_embed<SinglePos> ()->serialize (c, src, src_format, it, layout_variation_idx_delta_map, new_format); }


}
}
}

#اگر_ختم /* OT_LAYOUT_GPOS_SINGLEPOS_HH */
