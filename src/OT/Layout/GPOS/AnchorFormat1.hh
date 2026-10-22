#اگر_غیر_تعریف_شدہ OT_LAYOUT_GPOS_ANCHORFORMAT1_HH
#تعریف_کرو OT_LAYOUT_GPOS_ANCHORFORMAT1_HH

نامخلا OT {
نامخلا Layout {
نامخلا GPOS_impl {

ساخت AnchorFormat1
{
  محفوظ:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  FWORD         xCoordinate;            /* Horizontal value--in design units */
  FWORD         yCoordinate;            /* Vertical value--in design units */
  عوامی:
  DEFINE_سائز_STATIC (6);

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ));
  }

  خالی get_anchor (hb_ot_apply_context_t *c, hb_codepoint_t glyph_id HB_غیر_مستعمل,
                   اعشاری *x, اعشاری *y) ثابت
  {
    hb_font_t *font = c->font;
    *x = font->em_fscale_x (xCoordinate);
    *y = font->em_fscale_y (yCoordinate);
  }

  AnchorFormat1* copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (یہ);
    AnchorFormat1* برآمد = c->embed<AnchorFormat1> (یہ);
    اگر (!برآمد) return_trace (برآمد);
    برآمد->format = 1;
    return_trace (برآمد);
  }
};


}
}
}

#اگر_ختم  // OT_LAYOUT_GPOS_ANCHORFORMAT1_HH
