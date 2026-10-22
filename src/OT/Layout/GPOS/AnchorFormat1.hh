#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OT_LAYOUT_GPOS_ANCHORFORMAT1_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_LAYOUT_GPOS_ANCHORFORMAT1_HH

ਨਾਮਾਕਾਸ਼ OT {
ਨਾਮਾਕਾਸ਼ Layout {
ਨਾਮਾਕਾਸ਼ GPOS_impl {

ਬਣਤਰ AnchorFormat1
{
  ਸੁਰੱਖਿਅਤ:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  FWORD         xCoordinate;            /* Horizontal value--in design units */
  FWORD         yCoordinate;            /* Vertical value--in design units */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (6);

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ));
  }

  ਖਾਲੀ get_anchor (hb_ot_apply_context_t *c, hb_codepoint_t glyph_id HB_ਅਣਵਰਤਿਆ,
                   ਦਸ਼ਮਲਵ *x, ਦਸ਼ਮਲਵ *y) ਅਚਲ
  {
    hb_font_t *font = c->font;
    *x = font->em_fscale_x (xCoordinate);
    *y = font->em_fscale_y (yCoordinate);
  }

  AnchorFormat1* copy (hb_serialize_context_t *c) ਅਚਲ
  {
    TRACE_SERIALIZE (ਇਹ);
    AnchorFormat1* ਬਾਹਰਲਾ = c->embed<AnchorFormat1> (ਇਹ);
    ਜੇ (!ਬਾਹਰਲਾ) return_trace (ਬਾਹਰਲਾ);
    ਬਾਹਰਲਾ->format = 1;
    return_trace (ਬਾਹਰਲਾ);
  }
};


}
}
}

#ਜੇ_ਅੰਤ  // OT_LAYOUT_GPOS_ANCHORFORMAT1_HH
