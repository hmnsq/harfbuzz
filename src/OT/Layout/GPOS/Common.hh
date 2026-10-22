#ifndef OT_LAYOUT_GPOS_COMMON_HH
#define OT_LAYOUT_GPOS_COMMON_HH

नामाकाश OT {
नामाकाश Layout {
नामाकाश GPOS_impl {

गणना attach_type_t {
  ATTACH_TYPE_NONE      = 0X00,

  /* Each attachment should be either a mark or a cursive; can't be both. */
  ATTACH_TYPE_MARK      = 0X01,
  ATTACH_TYPE_CURSIVE   = 0X02,
};

/* buffer **position** var allocations */
#define attach_chain() var.i16[0] /* glyph to which this attaches to, relative to current glyphs; negative for going back, positive for forward. */
#define attach_type() var.u8[2] /* attachment type */
/* Note! if attach_chain() is zero, the value of attach_type() is irrelevant. */

संरचना मान_Base;
संरचना मान_Format;

ढाँचा<typename Iterator>
स्थिर शून्य SinglePos_serialize (hb_serialize_context_t *c,
                                 अचर मान_Base *src,
                                 मान_Format src_format,
                                 Iterator it,
                                 अचर hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णाङ्क>> *layout_variation_idx_delta_map,
                                 अचिन्हित new_format);


}
}
}

#endif  // OT_LAYOUT_GPOS_COMMON_HH
