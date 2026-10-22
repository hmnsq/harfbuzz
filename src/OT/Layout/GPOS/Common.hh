#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OT_LAYOUT_GPOS_COMMON_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_LAYOUT_GPOS_COMMON_HH

ਨਾਮਾਕਾਸ਼ OT {
ਨਾਮਾਕਾਸ਼ Layout {
ਨਾਮਾਕਾਸ਼ GPOS_impl {

ਗਿਣਤੀ attach_type_t {
  ATTACH_TYPE_NONE      = 0X00,

  /* Each attachment should be either a mark or a cursive; can't be both. */
  ATTACH_TYPE_MARK      = 0X01,
  ATTACH_TYPE_CURSIVE   = 0X02,
};

/* buffer **position** var allocations */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ attach_chain() var.i16[0] /* glyph to which this attaches to, relative to current glyphs; negative for going back, positive for forward. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ attach_type() var.u8[2] /* attachment type */
/* Note! if attach_chain() is zero, the value of attach_type() is irrelevant. */

ਬਣਤਰ ਮੁੱਲ_ਅਧਾਰ;
ਬਣਤਰ ਮੁੱਲ_Format;

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ Iterator>
ਸਥਿਰ ਖਾਲੀ SinglePos_serialize (hb_serialize_context_t *c,
                                 ਅਚਲ ਮੁੱਲ_ਅਧਾਰ *src,
                                 ਮੁੱਲ_Format src_format,
                                 Iterator it,
                                 ਅਚਲ hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਪੂਰਨ>> *layout_variation_idx_delta_map,
                                 ਬਿਨਾਨਿਸ਼ਾਨੀ new_format);


}
}
}

#ਜੇ_ਅੰਤ  // OT_LAYOUT_GPOS_COMMON_HH
