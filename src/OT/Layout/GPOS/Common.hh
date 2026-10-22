#اگر_غیر_تعریف_شدہ OT_LAYOUT_GPOS_COMMON_HH
#تعریف_کرو OT_LAYOUT_GPOS_COMMON_HH

نامخلا OT {
نامخلا Layout {
نامخلا GPOS_impl {

شمار attach_type_t {
  ATTACH_TYPE_NONE      = 0X00,

  /* Each attachment should be either a mark or a cursive; can't be both. */
  ATTACH_TYPE_MARK      = 0X01,
  ATTACH_TYPE_CURSIVE   = 0X02,
};

/* buffer **position** var allocations */
#تعریف_کرو attach_chain() var.i16[0] /* glyph to which this attaches to, relative to current glyphs; negative for going back, positive for forward. */
#تعریف_کرو attach_type() var.u8[2] /* attachment type */
/* Note! if attach_chain() is zero, the value of attach_type() is irrelevant. */

ساخت قدر_بنیاد;
ساخت قدر_Format;

سانچہ<قسم_نام Iterator>
ساکن خالی SinglePos_serialize (hb_serialize_context_t *c,
                                 ثابت قدر_بنیاد *src,
                                 قدر_Format src_format,
                                 Iterator it,
                                 ثابت hb_hashmap_t<غیرعلامتی, hb_pair_t<غیرعلامتی, صحیح>> *layout_variation_idx_delta_map,
                                 غیرعلامتی new_format);


}
}
}

#اگر_ختم  // OT_LAYOUT_GPOS_COMMON_HH
