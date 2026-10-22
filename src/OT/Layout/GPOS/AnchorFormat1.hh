#ifndef OT_LAYOUT_GPOS_ANCHORFORMAT1_HH
#define OT_LAYOUT_GPOS_ANCHORFORMAT1_HH

নামাকাশ OT {
নামাকাশ Layout {
নামাকাশ GPOS_impl {

কাঠামো AnchorFormat1
{
  সুরক্ষিত:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  FWORD         xCoordinate;            /* Horizontal value--in design units */
  FWORD         yCoordinate;            /* Vertical value--in design units */
  সর্বজনীন:
  DEFINE_আকার_STATIC (6);

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই));
  }

  শূন্য get_anchor (hb_ot_apply_context_t *c, hb_codepoint_t glyph_id HB_UNUSED,
                   দশমিক *x, দশমিক *y) ধ্রুবক
  {
    hb_font_t *font = c->font;
    *x = font->em_fscale_x (xCoordinate);
    *y = font->em_fscale_y (yCoordinate);
  }

  AnchorFormat1* copy (hb_serialize_context_t *c) ধ্রুবক
  {
    TRACE_SERIALIZE (এই);
    AnchorFormat1* out = c->embed<AnchorFormat1> (এই);
    যদি (!out) return_trace (out);
    out->format = 1;
    return_trace (out);
  }
};


}
}
}

#endif  // OT_LAYOUT_GPOS_ANCHORFORMAT1_HH
