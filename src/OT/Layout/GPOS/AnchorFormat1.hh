#اگر_تعریف_نشده OT_LAYOUT_GPOS_ANCHORFORMAT1_HH
#تعریف_کن OT_LAYOUT_GPOS_ANCHORFORMAT1_HH

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {

بنية AnchorFormat1
{
  محمي:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  FWORD         xCoordinate;            /* Horizontal value--in design units */
  FWORD         yCoordinate;            /* Vertical value--in design units */
  عام:
  DEFINE_اندازه_STATIC (6);

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  فراغ get_anchor (hb_ot_apply_context_t *c, hb_codepoint_t glyph_id HB_استفادهنشده,
                   عائم *x, عائم *y) ثابت
  {
    hb_font_t *font = c->font;
    *x = font->em_fscale_x (xCoordinate);
    *y = font->em_fscale_y (yCoordinate);
  }

  AnchorFormat1* copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (هذا);
    AnchorFormat1* بیرون = c->embed<AnchorFormat1> (هذا);
    إذا (!بیرون) return_trace (بیرون);
    بیرون->format = 1;
    return_trace (بیرون);
  }
};


}
}
}

#پایان_اگر  // OT_LAYOUT_GPOS_ANCHORFORMAT1_HH
