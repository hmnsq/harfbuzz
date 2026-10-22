#ifndef OT_LAYOUT_GPOS_SINGLEPOS_HH
#define OT_LAYOUT_GPOS_SINGLEPOS_HH

#अन्तर्भाव "SinglePosFormat1.hh"
#अन्तर्भाव "SinglePosFormat2.hh"

नामाकाश OT {
नामाकाश Layout {
नामाकाश GPOS_impl {

संरचना SinglePos
{
  संरक्षित:
  संघ {
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
    hb_array_t<स्थिरांक मूल्य> first_val_iter = hb_second (*glyph_val_iter_pairs);

    साठी (स्थिरांक स्वतः iter : glyph_val_iter_pairs)
      साठी (स्थिरांक स्वतः _ : hb_zip (iter.second, first_val_iter))
        जर (_.first != _.second)
          परत 2;

    परत 1;
  }

  ढाँचा<typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  शून्य serialize (hb_serialize_context_t *c,
                  स्थिरांक मूल्य_Base* src,
                  मूल्य_Format src_format,
                  Iterator glyph_val_iter_pairs,
                  स्थिरांक hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णांक>> *layout_variation_idx_delta_map,
                  अचिन्हित newFormat)
  {
    जर (unlikely (!c->extend_min (u.format.v))) परत;
    अचिन्हित format = 2;
    मूल्य_Format new_format;
    new_format = newFormat;

    जर (glyph_val_iter_pairs)
      format = get_format (glyph_val_iter_pairs);

#ifndef HB_NO_BEYOND_64K
    जर (+ glyph_val_iter_pairs
	| hb_map_retains_sorting (hb_first)
	| hb_filter ([] (hb_codepoint_t gid) { परत gid > 0xFFFFu; }))
      format += 2;
#endif

    u.format.v = format;
    निवड (u.format.v) {
    स्थिती 1: hb_barrier (); u.format1.serialize (c,
                                 src, src_format,
                                 glyph_val_iter_pairs,
                                 new_format,
                                 layout_variation_idx_delta_map);
      परत;
    स्थिती 2: hb_barrier (); u.format2.serialize (c,
                                 src, src_format,
                                 glyph_val_iter_pairs,
                                 new_format,
                                 layout_variation_idx_delta_map);
      परत;
#ifndef HB_NO_BEYOND_64K
    स्थिती 3: u.format3.serialize (c,
                                 src, src_format,
                                 glyph_val_iter_pairs,
                                 new_format,
                                 layout_variation_idx_delta_map);
      परत;
    स्थिती 4: u.format4.serialize (c,
                                 src, src_format,
                                 glyph_val_iter_pairs,
                                 new_format,
                                 layout_variation_idx_delta_map);
      परत;
#endif
    पूर्वनिर्धारित:परत;
    }
  }

  ढाँचा <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) स्थिरांक
  {
    जर (unlikely (!c->may_dispatch (हे, &u.format.v))) परत c->no_dispatch_return_मूल्य ();
    TRACE_DISPATCH (हे, u.format.v);
    निवड (u.format.v) {
    स्थिती 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    स्थिती 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
#ifndef HB_NO_BEYOND_64K
    स्थिती 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    स्थिती 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
#endif
    पूर्वनिर्धारित:return_trace (c->default_return_मूल्य ());
    }
  }
};


ढाँचा<typename Iterator>
स्थिर शून्य
SinglePos_serialize (hb_serialize_context_t *c,
                     स्थिरांक मूल्य_Base *src,
                     मूल्य_Format src_format,
                     Iterator it,
                     स्थिरांक hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णांक>> *layout_variation_idx_delta_map,
                     अचिन्हित new_format)
{ c->start_embed<SinglePos> ()->serialize (c, src, src_format, it, layout_variation_idx_delta_map, new_format); }


}
}
}

#endif /* OT_LAYOUT_GPOS_SINGLEPOS_HH */
