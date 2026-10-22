#నిర్వచితం_కాకపోతే OT_LAYOUT_GPOS_COMMON_HH
#నిర్వచించు OT_LAYOUT_GPOS_COMMON_HH

నామాకాశ OT {
నామాకాశ Layout {
నామాకాశ GPOS_impl {

గణన attach_type_t {
  ATTACH_TYPE_NONE      = 0X00,

  /* Each attachment should be either a mark or a cursive; can't be both. */
  ATTACH_TYPE_MARK      = 0X01,
  ATTACH_TYPE_CURSIVE   = 0X02,
};

/* buffer **position** var allocations */
#నిర్వచించు attach_chain() var.i16[0] /* glyph to which this attaches to, relative to current glyphs; negative for going back, positive for forward. */
#నిర్వచించు attach_type() var.u8[2] /* attachment type */
/* Note! if attach_chain() is zero, the value of attach_type() is irrelevant. */

నిర్మాణం విలువ_పునాది;
నిర్మాణం విలువ_Format;

మూస<రకం_పేరు Iterator>
స్థిర శూన్య SinglePos_serialize (hb_serialize_context_t *c,
                                 స్థిరరాశి విలువ_పునాది *src,
                                 విలువ_Format src_format,
                                 Iterator it,
                                 స్థిరరాశి hb_hashmap_t<గుర్తులేని, hb_pair_t<గుర్తులేని, పూర్ణాంకం>> *layout_variation_idx_delta_map,
                                 గుర్తులేని new_format);


}
}
}

#అయితే_ముగింపు  // OT_LAYOUT_GPOS_COMMON_HH
