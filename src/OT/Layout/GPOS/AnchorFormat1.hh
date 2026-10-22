#ifndef OT_LAYOUT_GPOS_ANCHORFORMAT1_HH
#define OT_LAYOUT_GPOS_ANCHORFORMAT1_HH

नामाकाश OT {
नामाकाश Layout {
नामाकाश GPOS_impl {

संरचना AnchorFormat1
{
  रक्षित:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  FWORD         xCoordinate;            /* Horizontal value--in design units */
  FWORD         yCoordinate;            /* Vertical value--in design units */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (6);

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्));
  }

  शून्य get_anchor (hb_ot_apply_context_t *c, hb_codepoint_t glyph_id HB_UNUSED,
                   भिन्न *x, भिन्न *y) अचर
  {
    hb_font_t *font = c->font;
    *x = font->em_fscale_x (xCoordinate);
    *y = font->em_fscale_y (yCoordinate);
  }

  AnchorFormat1* copy (hb_serialize_context_t *c) अचर
  {
    TRACE_SERIALIZE (एतत्);
    AnchorFormat1* out = c->embed<AnchorFormat1> (एतत्);
    यदि (!out) return_trace (out);
    out->format = 1;
    return_trace (out);
  }
};


}
}
}

#endif  // OT_LAYOUT_GPOS_ANCHORFORMAT1_HH
