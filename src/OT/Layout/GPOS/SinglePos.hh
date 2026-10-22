#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OT_LAYOUT_GPOS_SINGLEPOS_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_LAYOUT_GPOS_SINGLEPOS_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "SinglePosFormat1.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "SinglePosFormat2.hh"

ਨਾਮਾਕਾਸ਼ OT {
ਨਾਮਾਕਾਸ਼ Layout {
ਨਾਮਾਕਾਸ਼ GPOS_impl {

ਬਣਤਰ SinglePos
{
  ਸੁਰੱਖਿਅਤ:
  ਸੰਘ {
  ਬਣਤਰ { HBUINT16 v; } format;        /* Format identifier */
  SinglePosFormat1_3<SmallTypes> format1;
  SinglePosFormat2_4<SmallTypes> format2;
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
  SinglePosFormat1_3<MediumTypes> format3;
  SinglePosFormat2_4<MediumTypes> format4;
#ਜੇ_ਅੰਤ
  } u;

  ਜਨਤਕ:
  ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  ਬਿਨਾਨਿਸ਼ਾਨੀ get_format (Iterator glyph_val_iter_ਜੋੜੇ)
  {
    hb_array_t<ਅਚਲ ਮੁੱਲ> ਪਹਿਲਾ_val_iter = hb_ਦੂਜਾ (*glyph_val_iter_ਜੋੜੇ);

    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ iter : glyph_val_iter_ਜੋੜੇ)
      ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ _ : hb_zip (iter.second, ਪਹਿਲਾ_val_iter))
        ਜੇ (_.ਪਹਿਲਾ != _.second)
          ਵਾਪਸ 2;

    ਵਾਪਸ 1;
  }

  ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  ਖਾਲੀ serialize (hb_serialize_context_t *c,
                  ਅਚਲ ਮੁੱਲ_ਅਧਾਰ* src,
                  ਮੁੱਲ_Format src_format,
                  Iterator glyph_val_iter_ਜੋੜੇ,
                  ਅਚਲ hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਪੂਰਨ>> *layout_variation_idx_delta_map,
                  ਬਿਨਾਨਿਸ਼ਾਨੀ newFormat)
  {
    ਜੇ (unlikely (!c->extend_min (u.format.v))) ਵਾਪਸ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ format = 2;
    ਮੁੱਲ_Format new_format;
    new_format = newFormat;

    ਜੇ (glyph_val_iter_ਜੋੜੇ)
      format = get_format (glyph_val_iter_ਜੋੜੇ);

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਜੇ (+ glyph_val_iter_ਜੋੜੇ
	| hb_map_retains_ਛਾਂਟੀ (hb_ਪਹਿਲਾ)
	| hb_filter ([] (hb_codepoint_t gid) { ਵਾਪਸ gid > 0xFFFFu; }))
      format += 2;
#ਜੇ_ਅੰਤ

    u.format.v = format;
    ਚੋਣ (u.format.v) {
    ਹਾਲਤ 1: hb_barrier (); u.format1.serialize (c,
                                 src, src_format,
                                 glyph_val_iter_ਜੋੜੇ,
                                 new_format,
                                 layout_variation_idx_delta_map);
      ਵਾਪਸ;
    ਹਾਲਤ 2: hb_barrier (); u.format2.serialize (c,
                                 src, src_format,
                                 glyph_val_iter_ਜੋੜੇ,
                                 new_format,
                                 layout_variation_idx_delta_map);
      ਵਾਪਸ;
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਹਾਲਤ 3: u.format3.serialize (c,
                                 src, src_format,
                                 glyph_val_iter_ਜੋੜੇ,
                                 new_format,
                                 layout_variation_idx_delta_map);
      ਵਾਪਸ;
    ਹਾਲਤ 4: u.format4.serialize (c,
                                 src, src_format,
                                 glyph_val_iter_ਜੋੜੇ,
                                 new_format,
                                 layout_variation_idx_delta_map);
      ਵਾਪਸ;
#ਜੇ_ਅੰਤ
    ਮੂਲ:ਵਾਪਸ;
    }
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ context_t, ਕਿਸਮ_ਨਾਂ ...Ts>
  ਕਿਸਮ_ਨਾਂ context_t::return_t dispatch (context_t *c, Ts&&... ds) ਅਚਲ
  {
    ਜੇ (unlikely (!c->may_dispatch (ਇਹ, &u.format.v))) ਵਾਪਸ c->no_dispatch_return_ਮੁੱਲ ();
    TRACE_DISPATCH (ਇਹ, u.format.v);
    ਚੋਣ (u.format.v) {
    ਹਾਲਤ 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    ਹਾਲਤ 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਹਾਲਤ 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    ਹਾਲਤ 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
#ਜੇ_ਅੰਤ
    ਮੂਲ:return_trace (c->default_return_ਮੁੱਲ ());
    }
  }
};


ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ Iterator>
ਸਥਿਰ ਖਾਲੀ
SinglePos_serialize (hb_serialize_context_t *c,
                     ਅਚਲ ਮੁੱਲ_ਅਧਾਰ *src,
                     ਮੁੱਲ_Format src_format,
                     Iterator it,
                     ਅਚਲ hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਪੂਰਨ>> *layout_variation_idx_delta_map,
                     ਬਿਨਾਨਿਸ਼ਾਨੀ new_format)
{ c->ਸ਼ੁਰੂ_embed<SinglePos> ()->serialize (c, src, src_format, it, layout_variation_idx_delta_map, new_format); }


}
}
}

#ਜੇ_ਅੰਤ /* OT_LAYOUT_GPOS_SINGLEPOS_HH */
