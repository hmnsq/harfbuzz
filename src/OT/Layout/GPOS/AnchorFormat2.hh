#ifndef OT_LAYOUT_GPOS_ANCHORFORMAT2_HH
#define OT_LAYOUT_GPOS_ANCHORFORMAT2_HH

നാമാകാശ OT {
നാമാകാശ Layout {
നാമാകാശ GPOS_impl {

ഘടന AnchorFormat2
{

  സംരക്ഷിത:
  HBUINT16      format;                 /* Format identifier--format = 2 */
  FWORD         xCoordinate;            /* Horizontal value--in design units */
  FWORD         yCoordinate;            /* Vertical value--in design units */
  HBUINT16      anchorPoint;            /* Index to glyph contour point */
  പൊതു:
  DEFINE_SIZE_STATIC (8);

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്));
  }

  ശൂന്യം get_anchor (hb_ot_apply_context_t *c, hb_codepoint_t glyph_id,
                   ദശാംശം *x, ദശാംശം *y) സ്ഥിരം
  {
    hb_font_t *font = c->font;

#ifdef HB_NO_HINTING
    *x = font->em_fscale_x (xCoordinate);
    *y = font->em_fscale_y (yCoordinate);
    തിരികെ;
#endif

    unsigned പൂർണ്ണസംഖ്യ x_ppem = font->x_ppem;
    unsigned പൂർണ്ണസംഖ്യ y_ppem = font->y_ppem;
    hb_position_t cx = 0, cy = 0;
    തർക്കം ret;

    ret = (x_ppem || y_ppem) &&
          font->get_glyph_contour_point_for_origin (glyph_id, anchorPoint, HB_DIRECTION_LTR, &cx, &cy);
    *x = ret && x_ppem ? cx : font->em_fscale_x (xCoordinate);
    *y = ret && y_ppem ? cy : font->em_fscale_y (yCoordinate);
  }

  AnchorFormat2* copy (hb_serialize_context_t *c) സ്ഥിരം
  {
    TRACE_SERIALIZE (ഇത്);
    return_trace (c->embed<AnchorFormat2> (ഇത്));
  }
};

}
}
}

#endif  // OT_LAYOUT_GPOS_ANCHORFORMAT2_HH
