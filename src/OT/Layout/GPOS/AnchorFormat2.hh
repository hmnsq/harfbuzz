#که_نه_تعریف_شوی OT_LAYOUT_GPOS_ANCHORFORMAT2_HH
#تعریف_کړه OT_LAYOUT_GPOS_ANCHORFORMAT2_HH

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {

بنية AnchorFormat2
{

  محمي:
  HBUINT16      format;                 /* Format identifier--format = 2 */
  FWORD         xCoordinate;            /* Horizontal value--in design units */
  FWORD         yCoordinate;            /* Vertical value--in design units */
  HBUINT16      anchor_ټکی;            /* Index to glyph contour point */
  عام:
  DEFINE_اندازه_STATIC (8);

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  فراغ get_anchor (hb_ot_apply_context_t *c, hb_codepoint_t glyph_id,
                   عائم *x, عائم *y) ثابت
  {
    hb_font_t *font = c->font;

#که_تعریف_شوی HB_NO_HINTING
    *x = font->em_fscale_x (xCoordinate);
    *y = font->em_fscale_y (yCoordinate);
    أرجع;
#د_که_پای

    بینښه صحيح x_ppem = font->x_ppem;
    بینښه صحيح y_ppem = font->y_ppem;
    hb_position_t cx = 0, cy = 0;
    منطقي ret;

    ret = (x_ppem || y_ppem) &&
          font->get_glyph_contour_ټکی_for_origin (glyph_id, anchor_ټکی, HB_DIRECTION_LTR, &cx, &cy);
    *x = ret && x_ppem ? cx : font->em_fscale_x (xCoordinate);
    *y = ret && y_ppem ? cy : font->em_fscale_y (yCoordinate);
  }

  AnchorFormat2* copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (هذا);
    return_trace (c->embed<AnchorFormat2> (هذا));
  }
};

}
}
}

#د_که_پای  // OT_LAYOUT_GPOS_ANCHORFORMAT2_HH
