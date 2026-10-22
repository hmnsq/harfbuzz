#جے_بنا_تعریف OT_LAYOUT_GPOS_ANCHORFORMAT1_HH
#تعریف_کرو OT_LAYOUT_GPOS_ANCHORFORMAT1_HH

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
  DEFINE_ناپ_STATIC (6);

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  فراغ get_anchor (hb_ot_apply_context_t *c, hb_codepoint_t glyph_id HB_بن_ورتیا,
                   عائم *x, عائم *y) ثابت
  {
    hb_font_t *font = c->font;
    *x = font->em_fscale_x (xCoordinate);
    *y = font->em_fscale_y (yCoordinate);
  }

  AnchorFormat1* copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (هذا);
    AnchorFormat1* باہرلا = c->embed<AnchorFormat1> (هذا);
    إذا (!باہرلا) return_trace (باہرلا);
    باہرلا->format = 1;
    return_trace (باہرلا);
  }
};


}
}
}

#جے_انت  // OT_LAYOUT_GPOS_ANCHORFORMAT1_HH
