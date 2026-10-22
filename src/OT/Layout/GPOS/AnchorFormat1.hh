#ifndef OT_LAYOUT_GPOS_ANCHORFORMAT1_HH
#define OT_LAYOUT_GPOS_ANCHORFORMAT1_HH

নামাকাশ OT {
নামাকাশ Layout {
নামাকাশ GPOS_impl {

সংরচনা AnchorFormat1
{
  রক্ষিত:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  FWORD         xCoordinate;            /* Horizontal value--in design units */
  FWORD         yCoordinate;            /* Vertical value--in design units */
  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (6);

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্));
  }

  শূন্য get_anchor (hb_ot_apply_context_t *c, hb_codepoint_t glyph_id HB_UNUSED,
                   ভিন্ন *x, ভিন্ন *y) ধ্রুৱ
  {
    hb_font_t *font = c->font;
    *x = font->em_fscale_x (xCoordinate);
    *y = font->em_fscale_y (yCoordinate);
  }

  AnchorFormat1* copy (hb_serialize_context_t *c) ধ্রুৱ
  {
    TRACE_SERIALIZE (এতত্);
    AnchorFormat1* out = c->embed<AnchorFormat1> (এতত্);
    যদি (!out) return_trace (out);
    out->format = 1;
    return_trace (out);
  }
};


}
}
}

#endif  // OT_LAYOUT_GPOS_ANCHORFORMAT1_HH
