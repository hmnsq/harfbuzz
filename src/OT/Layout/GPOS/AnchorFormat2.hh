#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OT_LAYOUT_GPOS_ANCHORFORMAT2_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_LAYOUT_GPOS_ANCHORFORMAT2_HH

ਨਾਮਾਕਾਸ਼ OT {
ਨਾਮਾਕਾਸ਼ Layout {
ਨਾਮਾਕਾਸ਼ GPOS_impl {

ਬਣਤਰ AnchorFormat2
{

  ਸੁਰੱਖਿਅਤ:
  HBUINT16      format;                 /* Format identifier--format = 2 */
  FWORD         xCoordinate;            /* Horizontal value--in design units */
  FWORD         yCoordinate;            /* Vertical value--in design units */
  HBUINT16      anchor_ਬਿੰਦੂ;            /* Index to glyph contour point */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (8);

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ));
  }

  ਖਾਲੀ get_anchor (hb_ot_apply_context_t *c, hb_codepoint_t glyph_id,
                   ਦਸ਼ਮਲਵ *x, ਦਸ਼ਮਲਵ *y) ਅਚਲ
  {
    hb_font_t *font = c->font;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_NO_HINTING
    *x = font->em_fscale_x (xCoordinate);
    *y = font->em_fscale_y (yCoordinate);
    ਵਾਪਸ;
#ਜੇ_ਅੰਤ

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ x_ppem = font->x_ppem;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ y_ppem = font->y_ppem;
    hb_position_t cx = 0, cy = 0;
    ਤਾਰਕਿਕ ret;

    ret = (x_ppem || y_ppem) &&
          font->get_glyph_contour_ਬਿੰਦੂ_for_origin (glyph_id, anchor_ਬਿੰਦੂ, HB_DIRECTION_LTR, &cx, &cy);
    *x = ret && x_ppem ? cx : font->em_fscale_x (xCoordinate);
    *y = ret && y_ppem ? cy : font->em_fscale_y (yCoordinate);
  }

  AnchorFormat2* copy (hb_serialize_context_t *c) ਅਚਲ
  {
    TRACE_SERIALIZE (ਇਹ);
    return_trace (c->embed<AnchorFormat2> (ਇਹ));
  }
};

}
}
}

#ਜੇ_ਅੰਤ  // OT_LAYOUT_GPOS_ANCHORFORMAT2_HH
