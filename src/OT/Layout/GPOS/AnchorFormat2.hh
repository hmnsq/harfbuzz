#ifndef OT_LAYOUT_GPOS_ANCHORFORMAT2_HH
#define OT_LAYOUT_GPOS_ANCHORFORMAT2_HH

નામાકાશ OT {
નામાકાશ Layout {
નામાકાશ GPOS_impl {

સંરચના AnchorFormat2
{

  સુરક્ષિત:
  HBUINT16      format;                 /* Format identifier--format = 2 */
  FWORD         xCoordinate;            /* Horizontal value--in design units */
  FWORD         yCoordinate;            /* Vertical value--in design units */
  HBUINT16      anchorPoint;            /* Index to glyph contour point */
  જાહેર:
  DEFINE_SIZE_STATIC (8);

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ));
  }

  શૂન્ય get_anchor (hb_ot_apply_context_t *c, hb_codepoint_t glyph_id,
                   દશાંશ *x, દશાંશ *y) અચલ
  {
    hb_font_t *font = c->font;

#ifdef HB_NO_HINTING
    *x = font->em_fscale_x (xCoordinate);
    *y = font->em_fscale_y (yCoordinate);
    પાછા;
#endif

    unsigned પૂર્ણાંક x_ppem = font->x_ppem;
    unsigned પૂર્ણાંક y_ppem = font->y_ppem;
    hb_position_t cx = 0, cy = 0;
    તાર્કિક ret;

    ret = (x_ppem || y_ppem) &&
          font->get_glyph_contour_point_for_origin (glyph_id, anchorPoint, HB_DIRECTION_LTR, &cx, &cy);
    *x = ret && x_ppem ? cx : font->em_fscale_x (xCoordinate);
    *y = ret && y_ppem ? cy : font->em_fscale_y (yCoordinate);
  }

  AnchorFormat2* copy (hb_serialize_context_t *c) અચલ
  {
    TRACE_SERIALIZE (આ);
    return_trace (c->embed<AnchorFormat2> (આ));
  }
};

}
}
}

#endif  // OT_LAYOUT_GPOS_ANCHORFORMAT2_HH
