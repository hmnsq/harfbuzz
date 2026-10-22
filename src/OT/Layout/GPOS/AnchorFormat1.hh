#अगर_अघोषित OT_LAYOUT_GPOS_ANCHORFORMAT1_HH
#घोषणा OT_LAYOUT_GPOS_ANCHORFORMAT1_HH

namespace OT {
namespace Layout {
namespace GPOS_impl {

काष्ठा AnchorFormat1
{
  रक्षित:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  FWORD         xCoordinate;            /* Horizontal value--in design units */
  FWORD         yCoordinate;            /* Vertical value--in design units */
  खुला:
  DEFINE_आकार_STATIC (6);

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह));
  }

  व्योम get_anchor (hb_ot_apply_context_t *c, hb_codepoint_t glyph_id HB_UNUSED,
                   भग्न *x, भग्न *y) स्थिर
  {
    hb_font_t *font = c->font;
    *x = font->em_fscale_x (xCoordinate);
    *y = font->em_fscale_y (yCoordinate);
  }

  AnchorFormat1* copy (hb_serialize_context_t *c) स्थिर
  {
    TRACE_SERIALIZE (यह);
    AnchorFormat1* out = c->embed<AnchorFormat1> (यह);
    अगर (!out) return_trace (out);
    out->format = 1;
    return_trace (out);
  }
};


}
}
}

#पूर्ण_अगर  // OT_LAYOUT_GPOS_ANCHORFORMAT1_HH
