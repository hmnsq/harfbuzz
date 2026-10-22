#ifndef OT_LAYOUT_GPOS_SINGLEPOS_HH
#define OT_LAYOUT_GPOS_SINGLEPOS_HH

#ଅନ୍ତର୍ଭାଵ "SinglePosFormat1.hh"
#ଅନ୍ତର୍ଭାଵ "SinglePosFormat2.hh"

ନାମାକାଶ OT {
ନାମାକାଶ Layout {
ନାମାକାଶ GPOS_impl {

ସଂରଚନା SinglePos
{
  ସୁରକ୍ଷିତ:
  ସଂଘ {
  ସଂରଚନା { HBUINT16 v; } format;        /* Format identifier */
  SinglePosFormat1_3<SmallTypes> format1;
  SinglePosFormat2_4<SmallTypes> format2;
#ifndef HB_NO_BEYOND_64K
  SinglePosFormat1_3<MediumTypes> format3;
  SinglePosFormat2_4<MediumTypes> format4;
#endif
  } u;

  ସର୍ବସାଧାରଣ:
  template<typename Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  unsigned get_format (Iterator glyph_val_iter_pairs)
  {
    hb_array_t<ଅଚଳ ମୂଲ୍ୟ> first_val_iter = hb_second (*glyph_val_iter_pairs);

    ପାଇଁ (ଅଚଳ auto iter : glyph_val_iter_pairs)
      ପାଇଁ (ଅଚଳ auto _ : hb_zip (iter.second, first_val_iter))
        ଯଦି (_.first != _.second)
          ଫେରନ୍ତୁ 2;

    ଫେରନ୍ତୁ 1;
  }

  template<typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  ଶୂନ୍ୟ serialize (hb_serialize_context_t *c,
                  ଅଚଳ ମୂଲ୍ୟ_Base* src,
                  ମୂଲ୍ୟ_Format src_format,
                  Iterator glyph_val_iter_pairs,
                  ଅଚଳ hb_hashmap_t<unsigned, hb_pair_t<unsigned, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>> *layout_variation_idx_delta_map,
                  unsigned newFormat)
  {
    ଯଦି (unlikely (!c->extend_min (u.format.v))) ଫେରନ୍ତୁ;
    unsigned format = 2;
    ମୂଲ୍ୟ_Format new_format;
    new_format = newFormat;

    ଯଦି (glyph_val_iter_pairs)
      format = get_format (glyph_val_iter_pairs);

#ifndef HB_NO_BEYOND_64K
    ଯଦି (+ glyph_val_iter_pairs
	| hb_map_retains_sorting (hb_first)
	| hb_filter ([] (hb_codepoint_t gid) { ଫେରନ୍ତୁ gid > 0xFFFFu; }))
      format += 2;
#endif

    u.format.v = format;
    ବାଛ (u.format.v) {
    ସ୍ଥିତି 1: hb_barrier (); u.format1.serialize (c,
                                 src, src_format,
                                 glyph_val_iter_pairs,
                                 new_format,
                                 layout_variation_idx_delta_map);
      ଫେରନ୍ତୁ;
    ସ୍ଥିତି 2: hb_barrier (); u.format2.serialize (c,
                                 src, src_format,
                                 glyph_val_iter_pairs,
                                 new_format,
                                 layout_variation_idx_delta_map);
      ଫେରନ୍ତୁ;
#ifndef HB_NO_BEYOND_64K
    ସ୍ଥିତି 3: u.format3.serialize (c,
                                 src, src_format,
                                 glyph_val_iter_pairs,
                                 new_format,
                                 layout_variation_idx_delta_map);
      ଫେରନ୍ତୁ;
    ସ୍ଥିତି 4: u.format4.serialize (c,
                                 src, src_format,
                                 glyph_val_iter_pairs,
                                 new_format,
                                 layout_variation_idx_delta_map);
      ଫେରନ୍ତୁ;
#endif
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:ଫେରନ୍ତୁ;
    }
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ଅଚଳ
  {
    ଯଦି (unlikely (!c->may_dispatch (ଏହି, &u.format.v))) ଫେରନ୍ତୁ c->no_dispatch_return_ମୂଲ୍ୟ ();
    TRACE_DISPATCH (ଏହି, u.format.v);
    ବାଛ (u.format.v) {
    ସ୍ଥିତି 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    ସ୍ଥିତି 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
#ifndef HB_NO_BEYOND_64K
    ସ୍ଥିତି 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    ସ୍ଥିତି 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
#endif
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:return_trace (c->default_return_ମୂଲ୍ୟ ());
    }
  }
};


template<typename Iterator>
ସ୍ଥିର ଶୂନ୍ୟ
SinglePos_serialize (hb_serialize_context_t *c,
                     ଅଚଳ ମୂଲ୍ୟ_Base *src,
                     ମୂଲ୍ୟ_Format src_format,
                     Iterator it,
                     ଅଚଳ hb_hashmap_t<unsigned, hb_pair_t<unsigned, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>> *layout_variation_idx_delta_map,
                     unsigned new_format)
{ c->start_embed<SinglePos> ()->serialize (c, src, src_format, it, layout_variation_idx_delta_map, new_format); }


}
}
}

#endif /* OT_LAYOUT_GPOS_SINGLEPOS_HH */
