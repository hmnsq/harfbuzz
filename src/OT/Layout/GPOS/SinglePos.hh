#ifndef OT_LAYOUT_GPOS_SINGLEPOS_HH
#define OT_LAYOUT_GPOS_SINGLEPOS_HH

#अन्तर्भाव "SinglePosFormat1.hh"
#अन्तर्भाव "SinglePosFormat2.hh"

नामाकाश OT {
नामाकाश Layout {
नामाकाश GPOS_impl {

संरचना SinglePos
{
  रक्षित:
  सङ्घ {
  संरचना { HBUINT16 v; } format;        /* Format identifier */
  SinglePosFormat1_3<SmallTypes> format1;
  SinglePosFormat2_4<SmallTypes> format2;
#ifndef HB_NO_BEYOND_64K
  SinglePosFormat1_3<MediumTypes> format3;
  SinglePosFormat2_4<MediumTypes> format4;
#endif
  } u;

  सार्वजनिक:
  ढाँचा<typename Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  अचिन्हित get_format (Iterator glyph_val_iter_pairs)
  {
    hb_array_t<अचर मान> first_val_iter = hb_second (*glyph_val_iter_pairs);

    लागि (अचर स्वतः iter : glyph_val_iter_pairs)
      लागि (अचर स्वतः _ : hb_zip (iter.second, first_val_iter))
        यदि (_.first != _.second)
          फर्काऊ 2;

    फर्काऊ 1;
  }

  ढाँचा<typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  शून्य serialize (hb_serialize_context_t *c,
                  अचर मान_Base* src,
                  मान_Format src_format,
                  Iterator glyph_val_iter_pairs,
                  अचर hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णाङ्क>> *layout_variation_idx_delta_map,
                  अचिन्हित newFormat)
  {
    यदि (unlikely (!c->extend_min (u.format.v))) फर्काऊ;
    अचिन्हित format = 2;
    मान_Format new_format;
    new_format = newFormat;

    यदि (glyph_val_iter_pairs)
      format = get_format (glyph_val_iter_pairs);

#ifndef HB_NO_BEYOND_64K
    यदि (+ glyph_val_iter_pairs
	| hb_map_retains_sorting (hb_first)
	| hb_filter ([] (hb_codepoint_t gid) { फर्काऊ gid > 0xFFFFu; }))
      format += 2;
#endif

    u.format.v = format;
    विकल्प (u.format.v) {
    पक्ष 1: hb_barrier (); u.format1.serialize (c,
                                 src, src_format,
                                 glyph_val_iter_pairs,
                                 new_format,
                                 layout_variation_idx_delta_map);
      फर्काऊ;
    पक्ष 2: hb_barrier (); u.format2.serialize (c,
                                 src, src_format,
                                 glyph_val_iter_pairs,
                                 new_format,
                                 layout_variation_idx_delta_map);
      फर्काऊ;
#ifndef HB_NO_BEYOND_64K
    पक्ष 3: u.format3.serialize (c,
                                 src, src_format,
                                 glyph_val_iter_pairs,
                                 new_format,
                                 layout_variation_idx_delta_map);
      फर्काऊ;
    पक्ष 4: u.format4.serialize (c,
                                 src, src_format,
                                 glyph_val_iter_pairs,
                                 new_format,
                                 layout_variation_idx_delta_map);
      फर्काऊ;
#endif
    स्वभाव:फर्काऊ;
    }
  }

  ढाँचा <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) अचर
  {
    यदि (unlikely (!c->may_dispatch (एतत्, &u.format.v))) फर्काऊ c->no_dispatch_return_मान ();
    TRACE_DISPATCH (एतत्, u.format.v);
    विकल्प (u.format.v) {
    पक्ष 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    पक्ष 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
#ifndef HB_NO_BEYOND_64K
    पक्ष 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    पक्ष 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
#endif
    स्वभाव:return_trace (c->default_return_मान ());
    }
  }
};


ढाँचा<typename Iterator>
स्थिर शून्य
SinglePos_serialize (hb_serialize_context_t *c,
                     अचर मान_Base *src,
                     मान_Format src_format,
                     Iterator it,
                     अचर hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णाङ्क>> *layout_variation_idx_delta_map,
                     अचिन्हित new_format)
{ c->start_embed<SinglePos> ()->serialize (c, src, src_format, it, layout_variation_idx_delta_map, new_format); }


}
}
}

#endif /* OT_LAYOUT_GPOS_SINGLEPOS_HH */
