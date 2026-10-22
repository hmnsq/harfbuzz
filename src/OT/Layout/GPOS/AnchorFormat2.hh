#నిర్వచితం_కాకపోతే OT_LAYOUT_GPOS_ANCHORFORMAT2_HH
#నిర్వచించు OT_LAYOUT_GPOS_ANCHORFORMAT2_HH

నామాకాశ OT {
నామాకాశ Layout {
నామాకాశ GPOS_impl {

నిర్మాణం AnchorFormat2
{

  రక్షిత:
  HBUINT16      format;                 /* Format identifier--format = 2 */
  FWORD         xCoordinate;            /* Horizontal value--in design units */
  FWORD         yCoordinate;            /* Vertical value--in design units */
  HBUINT16      anchor_బిందువు;            /* Index to glyph contour point */
  బహిరంగ:
  DEFINE_కొలత_STATIC (8);

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది));
  }

  శూన్య get_anchor (hb_ot_apply_context_t *c, hb_codepoint_t glyph_id,
                   దశాంశం *x, దశాంశం *y) స్థిరరాశి
  {
    hb_font_t *font = c->font;

#నిర్వచితమైతే HB_NO_HINTING
    *x = font->em_fscale_x (xCoordinate);
    *y = font->em_fscale_y (yCoordinate);
    తిరిగి;
#అయితే_ముగింపు

    గుర్తులేని పూర్ణాంకం x_ppem = font->x_ppem;
    గుర్తులేని పూర్ణాంకం y_ppem = font->y_ppem;
    hb_position_t cx = 0, cy = 0;
    తార్కిక ret;

    ret = (x_ppem || y_ppem) &&
          font->get_glyph_contour_బిందువు_for_origin (glyph_id, anchor_బిందువు, HB_DIRECTION_LTR, &cx, &cy);
    *x = ret && x_ppem ? cx : font->em_fscale_x (xCoordinate);
    *y = ret && y_ppem ? cy : font->em_fscale_y (yCoordinate);
  }

  AnchorFormat2* copy (hb_serialize_context_t *c) స్థిరరాశి
  {
    TRACE_SERIALIZE (ఇది);
    return_trace (c->embed<AnchorFormat2> (ఇది));
  }
};

}
}
}

#అయితే_ముగింపు  // OT_LAYOUT_GPOS_ANCHORFORMAT2_HH
