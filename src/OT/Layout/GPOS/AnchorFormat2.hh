#ifndef OT_LAYOUT_GPOS_ANCHORFORMAT2_HH
#define OT_LAYOUT_GPOS_ANCHORFORMAT2_HH

فضاینام OT {
فضاینام Layout {
فضاینام GPOS_impl {

ساختار AnchorFormat2
{

  محافظتشده:
  HBUINT16      format;                 /* Format identifier--format = 2 */
  FWORD         xCoordinate;            /* Horizontal value--in design units */
  FWORD         yCoordinate;            /* Vertical value--in design units */
  HBUINT16      anchorPoint;            /* Index to glyph contour point */
  عمومی:
  DEFINE_SIZE_STATIC (8);

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این));
  }

  تهی get_anchor (hb_ot_apply_context_t *c, hb_codepoint_t glyph_id,
                   اعشاری *x, اعشاری *y) ثابت
  {
    hb_font_t *font = c->font;

#ifdef HB_NO_HINTING
    *x = font->em_fscale_x (xCoordinate);
    *y = font->em_fscale_y (yCoordinate);
    بازگشت;
#endif

    unsigned صحیح x_ppem = font->x_ppem;
    unsigned صحیح y_ppem = font->y_ppem;
    hb_position_t cx = 0, cy = 0;
    منطقی ret;

    ret = (x_ppem || y_ppem) &&
          font->get_glyph_contour_point_for_origin (glyph_id, anchorPoint, HB_DIRECTION_LTR, &cx, &cy);
    *x = ret && x_ppem ? cx : font->em_fscale_x (xCoordinate);
    *y = ret && y_ppem ? cy : font->em_fscale_y (yCoordinate);
  }

  AnchorFormat2* copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (این);
    return_trace (c->embed<AnchorFormat2> (این));
  }
};

}
}
}

#endif  // OT_LAYOUT_GPOS_ANCHORFORMAT2_HH
