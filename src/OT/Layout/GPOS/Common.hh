#ifndef OT_LAYOUT_GPOS_COMMON_HH
#define OT_LAYOUT_GPOS_COMMON_HH

நாமாகாஶ OT {
நாமாகாஶ Layout {
நாமாகாஶ GPOS_impl {

எண்ணிக்கை attach_type_t {
  ATTACH_TYPE_NONE      = 0X00,

  /* Each attachment should be either a mark or a cursive; can't be both. */
  ATTACH_TYPE_MARK      = 0X01,
  ATTACH_TYPE_CURSIVE   = 0X02,
};

/* buffer **position** var allocations */
#define attach_chain() var.i16[0] /* glyph to which this attaches to, relative to current glyphs; negative for going back, positive for forward. */
#define attach_type() var.u8[2] /* attachment type */
/* Note! if attach_chain() is zero, the value of attach_type() is irrelevant. */

கட்டமைப்பு மதிப்பு_Base;
கட்டமைப்பு மதிப்பு_Format;

template<typename Iterator>
நிலையான வெறுமை SinglePos_serialize (hb_serialize_context_t *c,
                                 மாறிலி மதிப்பு_Base *src,
                                 மதிப்பு_Format src_format,
                                 Iterator it,
                                 மாறிலி hb_hashmap_t<unsigned, hb_pair_t<unsigned, முழு>> *layout_variation_idx_delta_map,
                                 unsigned new_format);


}
}
}

#endif  // OT_LAYOUT_GPOS_COMMON_HH
