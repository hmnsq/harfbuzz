#ifndef OT_LAYOUT_GPOS_ANCHORFORMAT1_HH
#define OT_LAYOUT_GPOS_ANCHORFORMAT1_HH

നാമാകാശ OT {
നാമാകാശ Layout {
നാമാകാശ GPOS_impl {

ഘടന AnchorFormat1
{
  സംരക്ഷിത:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  FWORD         xCoordinate;            /* Horizontal value--in design units */
  FWORD         yCoordinate;            /* Vertical value--in design units */
  പൊതു:
  DEFINE_SIZE_STATIC (6);

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്));
  }

  ശൂന്യം get_anchor (hb_ot_apply_context_t *c, hb_codepoint_t glyph_id HB_UNUSED,
                   ദശാംശം *x, ദശാംശം *y) സ്ഥിരം
  {
    hb_font_t *font = c->font;
    *x = font->em_fscale_x (xCoordinate);
    *y = font->em_fscale_y (yCoordinate);
  }

  AnchorFormat1* copy (hb_serialize_context_t *c) സ്ഥിരം
  {
    TRACE_SERIALIZE (ഇത്);
    AnchorFormat1* out = c->embed<AnchorFormat1> (ഇത്);
    എങ്കിൽ (!out) return_trace (out);
    out->format = 1;
    return_trace (out);
  }
};


}
}
}

#endif  // OT_LAYOUT_GPOS_ANCHORFORMAT1_HH
