#ifndef OT_LAYOUT_GPOS_ANCHORFORMAT2_HH
#define OT_LAYOUT_GPOS_ANCHORFORMAT2_HH

ନାମାକାଶ OT {
ନାମାକାଶ Layout {
ନାମାକାଶ GPOS_impl {

ସଂରଚନା AnchorFormat2
{

  ସୁରକ୍ଷିତ:
  HBUINT16      format;                 /* Format identifier--format = 2 */
  FWORD         xCoordinate;            /* Horizontal value--in design units */
  FWORD         yCoordinate;            /* Vertical value--in design units */
  HBUINT16      anchorPoint;            /* Index to glyph contour point */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (8);

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି));
  }

  ଶୂନ୍ୟ get_anchor (hb_ot_apply_context_t *c, hb_codepoint_t glyph_id,
                   ଦଶମିକ *x, ଦଶମିକ *y) ଅଚଳ
  {
    hb_font_t *font = c->font;

#ifdef HB_NO_HINTING
    *x = font->em_fscale_x (xCoordinate);
    *y = font->em_fscale_y (yCoordinate);
    ଫେରନ୍ତୁ;
#endif

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା x_ppem = font->x_ppem;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା y_ppem = font->y_ppem;
    hb_position_t cx = 0, cy = 0;
    ତାର୍କିକ ret;

    ret = (x_ppem || y_ppem) &&
          font->get_glyph_contour_point_for_origin (glyph_id, anchorPoint, HB_DIRECTION_LTR, &cx, &cy);
    *x = ret && x_ppem ? cx : font->em_fscale_x (xCoordinate);
    *y = ret && y_ppem ? cy : font->em_fscale_y (yCoordinate);
  }

  AnchorFormat2* copy (hb_serialize_context_t *c) ଅଚଳ
  {
    TRACE_SERIALIZE (ଏହି);
    return_trace (c->embed<AnchorFormat2> (ଏହି));
  }
};

}
}
}

#endif  // OT_LAYOUT_GPOS_ANCHORFORMAT2_HH
