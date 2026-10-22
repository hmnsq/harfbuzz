#ifndef OT_LAYOUT_GPOS_ANCHORFORMAT1_HH
#define OT_LAYOUT_GPOS_ANCHORFORMAT1_HH

ନାମାକାଶ OT {
ନାମାକାଶ Layout {
ନାମାକାଶ GPOS_impl {

ସଂରଚନା AnchorFormat1
{
  ସୁରକ୍ଷିତ:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  FWORD         xCoordinate;            /* Horizontal value--in design units */
  FWORD         yCoordinate;            /* Vertical value--in design units */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (6);

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି));
  }

  ଶୂନ୍ୟ get_anchor (hb_ot_apply_context_t *c, hb_codepoint_t glyph_id HB_UNUSED,
                   ଦଶମିକ *x, ଦଶମିକ *y) ଅଚଳ
  {
    hb_font_t *font = c->font;
    *x = font->em_fscale_x (xCoordinate);
    *y = font->em_fscale_y (yCoordinate);
  }

  AnchorFormat1* copy (hb_serialize_context_t *c) ଅଚଳ
  {
    TRACE_SERIALIZE (ଏହି);
    AnchorFormat1* out = c->embed<AnchorFormat1> (ଏହି);
    ଯଦି (!out) return_trace (out);
    out->format = 1;
    return_trace (out);
  }
};


}
}
}

#endif  // OT_LAYOUT_GPOS_ANCHORFORMAT1_HH
