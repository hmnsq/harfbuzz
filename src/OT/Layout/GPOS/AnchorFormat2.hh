#ifndef OT_LAYOUT_GPOS_ANCHORFORMAT2_HH
#define OT_LAYOUT_GPOS_ANCHORFORMAT2_HH

ನಾಮಾಕಾಶ OT {
ನಾಮಾಕಾಶ Layout {
ನಾಮಾಕಾಶ GPOS_impl {

ಸಂರಚನಾ AnchorFormat2
{

  ರಕ್ಷಿತ:
  HBUINT16      format;                 /* Format identifier--format = 2 */
  FWORD         xCoordinate;            /* Horizontal value--in design units */
  FWORD         yCoordinate;            /* Vertical value--in design units */
  HBUINT16      anchorPoint;            /* Index to glyph contour point */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (8);

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್));
  }

  ಶೂನ್ಯ get_anchor (hb_ot_apply_context_t *c, hb_codepoint_t glyph_id,
                   ಭಿನ್ನ *x, ಭಿನ್ನ *y) ಧ್ರುವ
  {
    hb_font_t *font = c->font;

#ifdef HB_NO_HINTING
    *x = font->em_fscale_x (xCoordinate);
    *y = font->em_fscale_y (yCoordinate);
    ಪ್ರತ್ಯಾಗಮ;
#endif

    unsigned ಪೂರ್ಣಾಙ್ಕ x_ppem = font->x_ppem;
    unsigned ಪೂರ್ಣಾಙ್ಕ y_ppem = font->y_ppem;
    hb_position_t cx = 0, cy = 0;
    ತಾರ್ಕಿಕ ret;

    ret = (x_ppem || y_ppem) &&
          font->get_glyph_contour_point_for_origin (glyph_id, anchorPoint, HB_DIRECTION_LTR, &cx, &cy);
    *x = ret && x_ppem ? cx : font->em_fscale_x (xCoordinate);
    *y = ret && y_ppem ? cy : font->em_fscale_y (yCoordinate);
  }

  AnchorFormat2* copy (hb_serialize_context_t *c) ಧ್ರುವ
  {
    TRACE_SERIALIZE (ಏತತ್);
    return_trace (c->embed<AnchorFormat2> (ಏತತ್));
  }
};

}
}
}

#endif  // OT_LAYOUT_GPOS_ANCHORFORMAT2_HH
