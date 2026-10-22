#جے_بنا_تعریف OT_LAYOUT_GPOS_COMMON_HH
#تعریف_کرو OT_LAYOUT_GPOS_COMMON_HH

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {

تعداد attach_type_t {
  ATTACH_TYPE_NONE      = 0X00,

  /* Each attachment should be either a mark or a cursive; can't be both. */
  ATTACH_TYPE_MARK      = 0X01,
  ATTACH_TYPE_CURSIVE   = 0X02,
};

/* buffer **position** var allocations */
#تعریف_کرو attach_chain() var.i16[0] /* glyph to which this attaches to, relative to current glyphs; negative for going back, positive for forward. */
#تعریف_کرو attach_type() var.u8[2] /* attachment type */
/* Note! if attach_chain() is zero, the value of attach_type() is irrelevant. */

بنية قدر_ادھار;
بنية قدر_Format;

سانچا<قسم_ناں Iterator>
ساكن فراغ SinglePos_serialize (hb_serialize_context_t *c,
                                 ثابت قدر_ادھار *src,
                                 قدر_Format src_format,
                                 Iterator it,
                                 ثابت hb_hashmap_t<بنانشانی, hb_pair_t<بنانشانی, صحيح>> *layout_variation_idx_delta_map,
                                 بنانشانی new_format);


}
}
}

#جے_انت  // OT_LAYOUT_GPOS_COMMON_HH
