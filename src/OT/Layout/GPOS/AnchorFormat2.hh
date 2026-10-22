#جيڪڏهن_وصف_نه_ٿيل OT_LAYOUT_GPOS_ANCHORFORMAT2_HH
#وصف_ڪريو OT_LAYOUT_GPOS_ANCHORFORMAT2_HH

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {

بنية AnchorFormat2
{

  محمي:
  HBUINT16      format;                 /* Format identifier--format = 2 */
  FWORD         xCoordinate;            /* Horizontal value--in design units */
  FWORD         yCoordinate;            /* Vertical value--in design units */
  HBUINT16      anchor_نقطو;            /* Index to glyph contour point */
  عام:
  DEFINE_سائيز_STATIC (8);

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  فراغ get_anchor (hb_ot_apply_context_t *c, hb_codepoint_t glyph_id,
                   عائم *x, عائم *y) ثابت
  {
    hb_font_t *font = c->font;

#جيڪڏهن_وصف_ٿيل HB_NO_HINTING
    *x = font->em_fscale_x (xCoordinate);
    *y = font->em_fscale_y (yCoordinate);
    أرجع;
#جيڪڏهن_پورو

    بنانشان صحيح x_ppem = font->x_ppem;
    بنانشان صحيح y_ppem = font->y_ppem;
    hb_position_t cx = 0, cy = 0;
    منطقي ret;

    ret = (x_ppem || y_ppem) &&
          font->get_glyph_contour_نقطو_for_origin (glyph_id, anchor_نقطو, HB_DIRECTION_LTR, &cx, &cy);
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

#جيڪڏهن_پورو  // OT_LAYOUT_GPOS_ANCHORFORMAT2_HH
