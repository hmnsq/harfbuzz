#ifndef OT_LAYOUT_GPOS_ANCHORFORMAT1_HH
#define OT_LAYOUT_GPOS_ANCHORFORMAT1_HH

ನಾಮಾಕಾಶ OT {
ನಾಮಾಕಾಶ Layout {
ನಾಮಾಕಾಶ GPOS_impl {

ಸಂರಚನಾ AnchorFormat1
{
  ರಕ್ಷಿತ:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  FWORD         xCoordinate;            /* Horizontal value--in design units */
  FWORD         yCoordinate;            /* Vertical value--in design units */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (6);

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್));
  }

  ಶೂನ್ಯ get_anchor (hb_ot_apply_context_t *c, hb_codepoint_t glyph_id HB_UNUSED,
                   ಭಿನ್ನ *x, ಭಿನ್ನ *y) ಧ್ರುವ
  {
    hb_font_t *font = c->font;
    *x = font->em_fscale_x (xCoordinate);
    *y = font->em_fscale_y (yCoordinate);
  }

  AnchorFormat1* copy (hb_serialize_context_t *c) ಧ್ರುವ
  {
    TRACE_SERIALIZE (ಏತತ್);
    AnchorFormat1* out = c->embed<AnchorFormat1> (ಏತತ್);
    ಯದಿ (!out) return_trace (out);
    out->format = 1;
    return_trace (out);
  }
};


}
}
}

#endif  // OT_LAYOUT_GPOS_ANCHORFORMAT1_HH
