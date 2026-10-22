#నిర్వచితం_కాకపోతే OT_LAYOUT_GPOS_ANCHORFORMAT1_HH
#నిర్వచించు OT_LAYOUT_GPOS_ANCHORFORMAT1_HH

నామాకాశ OT {
నామాకాశ Layout {
నామాకాశ GPOS_impl {

నిర్మాణం AnchorFormat1
{
  రక్షిత:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  FWORD         xCoordinate;            /* Horizontal value--in design units */
  FWORD         yCoordinate;            /* Vertical value--in design units */
  బహిరంగ:
  DEFINE_కొలత_STATIC (6);

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది));
  }

  శూన్య get_anchor (hb_ot_apply_context_t *c, hb_codepoint_t glyph_id HB_వాడని,
                   దశాంశం *x, దశాంశం *y) స్థిరరాశి
  {
    hb_font_t *font = c->font;
    *x = font->em_fscale_x (xCoordinate);
    *y = font->em_fscale_y (yCoordinate);
  }

  AnchorFormat1* copy (hb_serialize_context_t *c) స్థిరరాశి
  {
    TRACE_SERIALIZE (ఇది);
    AnchorFormat1* బహిర్గతం = c->embed<AnchorFormat1> (ఇది);
    ఒకవేళ (!బహిర్గతం) return_trace (బహిర్గతం);
    బహిర్గతం->format = 1;
    return_trace (బహిర్గతం);
  }
};


}
}
}

#అయితే_ముగింపు  // OT_LAYOUT_GPOS_ANCHORFORMAT1_HH
