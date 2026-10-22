#ifndef OT_LAYOUT_GPOS_ANCHORFORMAT2_HH
#define OT_LAYOUT_GPOS_ANCHORFORMAT2_HH

নামাকাশ OT {
নামাকাশ Layout {
নামাকাশ GPOS_impl {

কাঠামো AnchorFormat2
{

  সুরক্ষিত:
  HBUINT16      format;                 /* Format identifier--format = 2 */
  FWORD         xCoordinate;            /* Horizontal value--in design units */
  FWORD         yCoordinate;            /* Vertical value--in design units */
  HBUINT16      anchorPoint;            /* Index to glyph contour point */
  সর্বজনীন:
  DEFINE_আকার_STATIC (8);

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই));
  }

  শূন্য get_anchor (hb_ot_apply_context_t *c, hb_codepoint_t glyph_id,
                   দশমিক *x, দশমিক *y) ধ্রুবক
  {
    hb_font_t *font = c->font;

#ifdef HB_NO_HINTING
    *x = font->em_fscale_x (xCoordinate);
    *y = font->em_fscale_y (yCoordinate);
    ফেরত;
#endif

    unsigned পূর্ণসংখ্যা x_ppem = font->x_ppem;
    unsigned পূর্ণসংখ্যা y_ppem = font->y_ppem;
    hb_position_t cx = 0, cy = 0;
    যৌক্তিক ret;

    ret = (x_ppem || y_ppem) &&
          font->get_glyph_contour_point_for_origin (glyph_id, anchorPoint, HB_DIRECTION_LTR, &cx, &cy);
    *x = ret && x_ppem ? cx : font->em_fscale_x (xCoordinate);
    *y = ret && y_ppem ? cy : font->em_fscale_y (yCoordinate);
  }

  AnchorFormat2* copy (hb_serialize_context_t *c) ধ্রুবক
  {
    TRACE_SERIALIZE (এই);
    return_trace (c->embed<AnchorFormat2> (এই));
  }
};

}
}
}

#endif  // OT_LAYOUT_GPOS_ANCHORFORMAT2_HH
