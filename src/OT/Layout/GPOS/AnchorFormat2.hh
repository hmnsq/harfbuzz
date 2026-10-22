#اگر_غیر_تعریف_شدہ OT_LAYOUT_GPOS_ANCHORFORMAT2_HH
#تعریف_کرو OT_LAYOUT_GPOS_ANCHORFORMAT2_HH

نامخلا OT {
نامخلا Layout {
نامخلا GPOS_impl {

ساخت AnchorFormat2
{

  محفوظ:
  HBUINT16      format;                 /* Format identifier--format = 2 */
  FWORD         xCoordinate;            /* Horizontal value--in design units */
  FWORD         yCoordinate;            /* Vertical value--in design units */
  HBUINT16      anchor_نقطہ;            /* Index to glyph contour point */
  عوامی:
  DEFINE_سائز_STATIC (8);

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ));
  }

  خالی get_anchor (hb_ot_apply_context_t *c, hb_codepoint_t glyph_id,
                   اعشاری *x, اعشاری *y) ثابت
  {
    hb_font_t *font = c->font;

#اگر_تعریف_شدہ HB_NO_HINTING
    *x = font->em_fscale_x (xCoordinate);
    *y = font->em_fscale_y (yCoordinate);
    واپس;
#اگر_ختم

    غیرعلامتی صحیح x_ppem = font->x_ppem;
    غیرعلامتی صحیح y_ppem = font->y_ppem;
    hb_position_t cx = 0, cy = 0;
    منطقی ret;

    ret = (x_ppem || y_ppem) &&
          font->get_glyph_contour_نقطہ_for_origin (glyph_id, anchor_نقطہ, HB_DIRECTION_LTR, &cx, &cy);
    *x = ret && x_ppem ? cx : font->em_fscale_x (xCoordinate);
    *y = ret && y_ppem ? cy : font->em_fscale_y (yCoordinate);
  }

  AnchorFormat2* copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (یہ);
    return_trace (c->embed<AnchorFormat2> (یہ));
  }
};

}
}
}

#اگر_ختم  // OT_LAYOUT_GPOS_ANCHORFORMAT2_HH
