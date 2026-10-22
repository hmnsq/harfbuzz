#ifndef OT_LAYOUT_GPOS_ANCHORFORMAT2_HH
#define OT_LAYOUT_GPOS_ANCHORFORMAT2_HH

नामाकाश OT {
नामाकाश Layout {
नामाकाश GPOS_impl {

संरचना AnchorFormat2
{

  रक्षित:
  HBUINT16      format;                 /* Format identifier--format = 2 */
  FWORD         xCoordinate;            /* Horizontal value--in design units */
  FWORD         yCoordinate;            /* Vertical value--in design units */
  HBUINT16      anchorPoint;            /* Index to glyph contour point */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (8);

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्));
  }

  शून्य get_anchor (hb_ot_apply_context_t *c, hb_codepoint_t glyph_id,
                   भिन्न *x, भिन्न *y) ध्रुव
  {
    hb_font_t *font = c->font;

#ifdef HB_NO_HINTING
    *x = font->em_fscale_x (xCoordinate);
    *y = font->em_fscale_y (yCoordinate);
    प्रत्यागम;
#endif

    unsigned पूर्णाङ्क x_ppem = font->x_ppem;
    unsigned पूर्णाङ्क y_ppem = font->y_ppem;
    hb_position_t cx = 0, cy = 0;
    तार्किक ret;

    ret = (x_ppem || y_ppem) &&
          font->get_glyph_contour_point_for_origin (glyph_id, anchorPoint, HB_DIRECTION_LTR, &cx, &cy);
    *x = ret && x_ppem ? cx : font->em_fscale_x (xCoordinate);
    *y = ret && y_ppem ? cy : font->em_fscale_y (yCoordinate);
  }

  AnchorFormat2* copy (hb_serialize_context_t *c) ध्रुव
  {
    TRACE_SERIALIZE (एतत्);
    return_trace (c->embed<AnchorFormat2> (एतत्));
  }
};

}
}
}

#endif  // OT_LAYOUT_GPOS_ANCHORFORMAT2_HH
